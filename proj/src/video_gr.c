#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>


#include "video_gr.h"
#include "vbe.h"
#include "i8042.h"
#include "vars.h"



/* Private global variables */

extern unsigned long vram_size;
extern short *double_buffer;

static char *video_mem;		/* Process address to which VRAM is mapped */
static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void *vg_init(unsigned short mode)
{
  struct reg86u reg86;
  int r;
  struct mem_range mr;
  unsigned int vram_base;
  vbe_mode_info_t vmi_p;
  void *video_mem_local;         /* frame-buffer VM address */ 


  reg86.u.b.intno = 0x10; /* BIOS video services */
  reg86.u.w.ax = SET_VBE_MODE;
  reg86.u.w.bx = BIT(14)|mode;

  if(sys_int86(&reg86) != OK ) 
  {
    printf("\tvg_init(): sys_int86() failed \n");
    return NULL;
  }

  if(vbe_get_mode_info(mode, &vmi_p) != OK)
  {
    printf ("\tvg_init(): Error getting mode info\n");
    return NULL;
  }

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;

  vram_base = vmi_p.PhysBasePtr;  /* VRAM's physical addresss */
  vram_size = (h_res*v_res*bits_per_pixel)/8;  /* VRAM's size, but you can use the frame-buffer size, instead */
  //Porque é que dividmos por 8 ?

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;  
  mr.mr_limit = mr.mr_base + vram_size;  

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
     panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  video_mem_local = video_mem;

  double_buffer = calloc(vram_size, 1);

  if(video_mem_local == MAP_FAILED)
     panic("couldn't map video memory");

  return video_mem_local;
}

int vg_draw_pixel (unsigned int x, unsigned int y, short color, short *buffer)
{
	if ((x>=0)&&(x<h_res)&&(y>=0)&&(y<v_res))
	{
	    *(buffer + (y*h_res + x)) = color;
    	return OK;
	}
	return ERROR;
}

int vg_exit() {

  free (double_buffer);
  
  struct reg86u reg86;
  reg86.u.b.intno = 0x10; /* BIOS video services */
  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return ERROR;
  } else
      return OK;
}


void* getGraphicsBuffer(){
	return video_mem;//screen_buffer;
}

unsigned getHorResolution(){
	return h_res;
}

unsigned getVerResolution(){
	return v_res;
}


