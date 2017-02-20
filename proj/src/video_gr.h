#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

unsigned long vram_size;
short *double_buffer;


/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);


int vg_draw_pixel (unsigned int x, unsigned int y, short color, short *buffer);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
 * @brief returns pointer to start of video_mem
 *
 * @return pointer to start of video_mem
 */

void* getGraphicsBuffer();

/**
 * @brief returns horizontal resolution
 *
 * @return horizontal resolution
 */

unsigned getHorResolution();

/**
 * @brief returns vertical resolution
 *
 * @return vertical resolution
 */

unsigned getVerResolution();
 
#endif /* __VIDEO_GR_H */
