#include"../include/memlayout.h"
#include"../include/x86.h"
#include"../include/pmap.h"
#include"../include/string.h"
#include"../include/stdio.h"
#include"../include/mmu.h"
#include "../include/device/video.h"
#include "../include/device/video_mode.h"
#include "../include/logo.h"

enum {
	// Kernel error codes -- keep in sync with list in lib/printfmt.c.
	E_UNSPECIFIED	= 1,	// Unspecified or unknown problem
	E_BAD_ENV	= 2,	// Environment doesn't exist or otherwise
				// cannot be used in requested action
	E_INVAL		= 3,	// Invalid parameter
	E_NO_MEM	= 4,	// Request failed due to memory shortage
	E_NO_FREE_ENV	= 5,	// Attempt to create a new environment beyond
				// the maximum allowed
	E_FAULT		= 6,	// Memory fault
	E_NO_SYS	= 7,	// Unimplemented system call

	MAXERROR
};
struct PageInfo pages[npages];		// Physical page state array
struct PageInfo *page_free_list;	// Free list of physical pages

extern pde_t entry_pgdir[NR_PDE];
bool empty_list(struct PageInfo*pt)
{
	if(!pt) return true; else return false;
}
void page_init(void)
{
	unsigned long i;
	for (i = npages - 1; i > 0x150; i--)
	{
		pages[i].pp_ref = 0;
		pages[i].pp_link = page_free_list;
		page_free_list = &pages[i];
	}
	return;

}

struct PageInfo *page_alloc(int alloc_flags)
{
	if (page_free_list == NULL) return NULL;
	struct PageInfo* pt = page_free_list;
	page_free_list = page_free_list->pp_link;  
	if (alloc_flags & ALLOC_ZERO) memset(page2kva(pt),0,PGSIZE);
	return pt;
}

void page_free(struct PageInfo *pp)
{
	pp->pp_link = page_free_list;
	page_free_list = pp;
	return;
}

void page_decref(struct PageInfo* pp)
{
	if (--pp->pp_ref == 0) page_free(pp);
}

pte_t *pgdir_walk(pde_t *pgdir, const void *va, int create)
{
	struct PageInfo *newpage;
	physaddr_t pa;
	pte_t *result;
	if (!pgdir[PDX(va)])
	{
		if (create == false) return NULL;
		else 
		if (!(newpage = page_alloc(ALLOC_ZERO))) return NULL;
		else
		{
			newpage->pp_ref++;
			pa = page2pa(newpage);
			pgdir[PDX(va)] = pa | PTE_P | PTE_W | PTE_U;
			result = page2kva(newpage);
		}
	}
	else
	{
		result = page2kva(pa2page(pgdir[PDX(va)]));
	}
	return &result[PTX(va)];
}

static void boot_map_region(pde_t *pgdir, uint32_t va, unsigned long size,uint32_t pa, int perm)
{
	pte_t *p;
	int i;
	for (i = 0;i < size / PGSIZE;i++,va += PGSIZE)
	{
		p = pgdir_walk(pgdir,(void*)va,1);
		*p = (pa + PGSIZE * i) | perm | PTE_P;
	}
}

//
// Map the physical page 'pp' at virtual address 'va'.
// The permissions (the low 12 bits) of the page table entry
// should be set to 'perm|PTE_P'.
//
// Requirements
//   - If there is already a page mapped at 'va', it should be page_remove()d.
//   - If necessary, on demand, a page table should be allocated and inserted
//     into 'pgdir'.
//   - pp->pp_ref should be incremented if the insertion succeeds.
//   - The TLB must be invalidated if a page was formerly present at 'va'.
//
// Corner-case hint: Make sure to consider what happens when the same
// pp is re-inserted at the same virtual address in the same pgdir.
// However, try not to distinguish this case in your code, as this
// frequently leads to subtle bugs; there's an elegant way to handle
// everything in one code path.
//
// RETURNS:
//   0 on success
//   -E_NO_MEM, if page table couldn't be allocated
//
// Hint: The TA solution is implemented using pgdir_walk, page_remove,
// and page2pa.
//
int page_insert(pde_t *pgdir, struct PageInfo *pp, void *va, int perm)
{
	// Fill this function in
	//pte_t *pte=pgdir_walk(pgdir,va,1);
	struct PageInfo* pg = page_lookup(pgdir,va,NULL);
	if (pg == pp)
	{
		pte_t *pte = pgdir_walk(pgdir,va,1);
		(*pte) = page2pa(pp) | perm | PTE_P;
		return 0;
	}
	else 
	if(pg)
	{
		page_remove(pgdir,va);	
	}
	pte_t *pte = pgdir_walk(pgdir,va,1);
	if (pte==NULL) return -E_NO_MEM;
	else
	{
		(*pte) = page2pa(pp) | perm | PTE_P;
		pp->pp_ref++;
	}
	return 0;
}

//
// Return the page mapped at virtual address 'va'.
// If pte_store is not zero, then we store in it the address
// of the pte for this page.  This is used by page_remove and
// can be used to verify page permissions for syscall arguments,
// but should not be used by most callers.
//
// Return NULL if there is no page mapped at va.
//
// Hint: the TA solution uses pgdir_walk and pa2page.
//
struct PageInfo *page_lookup(pde_t *pgdir, void *va, pte_t **pte_store)
{
	// Fill this function in
	struct PageInfo *result;
	pte_t *pte = pgdir_walk(pgdir,va,0);
	if (!pte) return NULL;
	if (pte_store) *pte_store = pte;
	if (!(*pte)) return NULL;
	result = pa2page(*pte);
	return result;
}

//
// Unmaps the physical page at virtual address 'va'.
// If there is no physical page at that address, silently does nothing.
//
// Details:
//   - The ref count on the physical page should decrement.
//   - The physical page should be freed if the refcount reaches 0.
//   - The pg table entry corresponding to 'va' should be set to 0.
//     (if such a PTE exists)
//   - The TLB must be invalidated if you remove an entry from
//     the page table.
//
// Hint: The TA solution is implemented using page_lookup,
// 	tlb_invalidate, and page_decref.
//
void page_remove(pde_t *pgdir, void *va)
{
	// Fill this function in
	pte_t* pte = NULL;
	struct PageInfo *page = page_lookup(pgdir,&va,&pte);
	if (page->pp_ref) page_decref(page);
	(*pte) = 0;
	tlb_invalidate(pgdir,va);
}

//
// Invalidate a TLB entry, but only if the page tables being
// edited are the ones currently in use by the processor.
//
void tlb_invalidate(pde_t *pgdir, void *va)
{
	// Flush the entry only if we're modifying the current address space.
	// For now, there is only one address space, so always invalidate.
	invlpg(va);
}

void init_mem()
{
	//tm create a mappting
	//uint32_t VMEM_ADDR = 0xfd000000;
	boot_map_region(entry_pgdir,KERNBASE,npages * PGSIZE,0,PTE_W);
	boot_map_region(entry_pgdir,0xa0000,320 * 200,0xa0000,PTE_W | PTE_U);
	//memcpy((uint32_t)VMEM_ADDR,gImage_PARK,800 * 600 * 3);
	return ;
}
