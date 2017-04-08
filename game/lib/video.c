#include "../include/video.h"
#include "../include/pic.h"
#include "../include/string.h"

extern uint8_t v_buffer[SCR_SIZE];

inline void clear_buffer() {
	memset(v_buffer, 0x00, SCR_SIZE);
}

inline void display_buffer() {
	// You have work to do here !!!
	//memcpy(vmem, v_buffer, SCR_SIZE);
}

inline void drawRect_buffer(const uint8_t *src, int x, int y, int w, int h){
	int i;
	int v_idx = (x + y * SCR_WIDTH) * SCR_DEPTH;
	int s_idx = 0;
	for(i = 0; i < h; ++ i) {
		memcpy(v_buffer + v_idx, src + s_idx, w * SCR_DEPTH);
		v_idx += SCR_WIDTH_SIZE;
		s_idx += w * SCR_DEPTH;
	}
}


/* something old, and would never be used
static int logo_x = 0, logo_y = 0;
static int inc_x = 1, inc_y = 1;

void flyying_logo() {
	int i;
	logo_x += inc_x;
	logo_y += inc_y;
	int idx = (logo_x * SCR_WIDTH + logo_y) * SCR_DEPTH;
	for(i = 0; i < 40; ++ i){
		memset(vmem + idx, 0x00, 40 * SCR_DEPTH);
		idx += SCR_WIDTH_SIZE;
	}
	if((inc_x > 0 && logo_x == SCR_HEIGHT - 40) || (inc_x < 0 && logo_x == 0)) inc_x = -inc_x;
	if((inc_y > 0 && logo_y == SCR_WIDTH - 40) || (inc_y < 0 && logo_y == 0)) inc_y = -inc_y;
}

void show_police() {
	int i;
	for(i = 0; i < 80; ++ i){
		memcpy(vmem + i * SCR_WIDTH_SIZE + LS_WIDTH_SIZE * 0, gImage_LowSing + i * LS_WIDTH_SIZE, LS_WIDTH_SIZE);
		memcpy(vmem + i * SCR_WIDTH_SIZE + FF_WIDTH_SIZE * 1, gImage_FeiFan_1 + i * FF_WIDTH_SIZE, FF_WIDTH_SIZE);
		memcpy(vmem + i * SCR_WIDTH_SIZE + FF_WIDTH_SIZE * 2, gImage_FeiFan_2 + i * FF_WIDTH_SIZE, FF_WIDTH_SIZE);
		memcpy(vmem + i * SCR_WIDTH_SIZE + FF_WIDTH_SIZE * 3, gImage_FeiFan_3 + i * FF_WIDTH_SIZE, FF_WIDTH_SIZE);
		memcpy(vmem + i * SCR_WIDTH_SIZE + FF_WIDTH_SIZE * 4, gImage_FeiFan_4 + i * FF_WIDTH_SIZE, FF_WIDTH_SIZE);
		memcpy(vmem + i * SCR_WIDTH_SIZE + FF_WIDTH_SIZE * 5, gImage_FeiFan_5 + i * FF_WIDTH_SIZE, FF_WIDTH_SIZE);
	}
} */
