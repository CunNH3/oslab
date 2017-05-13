#include "../include/common.h"
#include "../include/string.h"
#include "../include/memory.h"
#include "../include/mmu.h"
#include "../include/device/video.h"
#include "../include/logo.h"

static PTE uptable[NR_PTE] align_to_page;

PDE* get_kpdir();

void init_vmem()
{
	uint32_t VMEM_ADDR = (uint32_t)vmem;
	printk("vmem_addr = 0x%08x\n",vmem);
	PDE *pdir = (PDE*)( (uint32_t)get_kpdir() + 4 * ((VMEM_ADDR >> 22) & 0x3ff) );
	PTE *ptable = (PTE*)( (uint32_t)uptable + 4 * ((VMEM_ADDR >> 12) & 0x3ff) );

	pdir->val = PTE_ADDR(va_to_pa(uptable)) | PTE_P | PTE_W | PTE_U;

	uint32_t pframe_addr = VMEM_ADDR;
	for (; pframe_addr < VMEM_ADDR + SCR_SIZE; pframe_addr += PGSIZE)
	{
		ptable->val = PTE_ADDR(pframe_addr) | PTE_P | PTE_W | PTE_U;
		ptable ++;
	}

	memcpy(vmem,gImage_PARK,SCR_SIZE);
}
