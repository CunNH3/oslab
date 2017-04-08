#include "../include/video.h"
#include "../include/pic.h"
#include "../include/string.h"
#include "../include/syscall.h"

extern uint8_t v_buffer[SCR_SIZE];

inline void clear_buffer()
{
	memset(v_buffer, 0x00, SCR_SIZE);
}

inline void display_buffer()
{
	put_video(v_buffer);
}

inline void drawRect_buffer(const uint8_t *src, int x, int y, int w, int h)
{
	int i;
	int v_idx = (x + y * SCR_WIDTH) * SCR_DEPTH;
	int s_idx = 0;
	for(i = 0;i < h;i++)
	{
		memcpy(v_buffer + v_idx, src + s_idx, w * SCR_DEPTH);
		v_idx += SCR_WIDTH_SIZE;
		s_idx += w * SCR_DEPTH;
	}
}



