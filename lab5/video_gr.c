#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "test5.h"
#include "video_gr.h"
#include "vbe.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */
static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned vram_size;

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

  if(video_mem_local == MAP_FAILED)
     panic("couldn't map video memory");

  return video_mem_local;
}

int vg_draw_pixel (unsigned int x, unsigned int y, unsigned long color, char *buffer)
{
  if ((x<h_res)&&(y<v_res))
  {
    *(buffer + (y*h_res) + x) = color;
    return OK;
  }
  return ERROR;
}

int vg_erase_xpm (unsigned short xi, unsigned short yi, char *map, int width, int height)
{
  int i, j, k=0;
  for (j = yi; j<(yi+height); j++)
  {
    for (i = xi; i<(xi+height); i++)
    {
      if (vg_draw_pixel((unsigned int) i, (unsigned int) j, 0, video_mem) != OK)
      {
        printf ("\tvg_erase_xpm(): vg_draw_pixel() failed\n");
        return ERROR;
      }
    }
  }
  return OK;
}

int vg_draw_xpm (unsigned short xi, unsigned short yi, char *map, int width, int height)
{
  int i, j, k=0;
  char *xpm_buffer = calloc(vram_size, 1);
  for (j = yi; j<(yi+height); j++)
  {
    for (i = xi; i<(xi+height); i++)
    {
      if (vg_draw_pixel((unsigned int) i, (unsigned int) j, map[k++], xpm_buffer) != OK)
      {
        printf ("\tvg_draw_xpm(): vg_draw_pixel() failed\n");
        return ERROR;
      }
    }
  }
  memcpy(video_mem, xpm_buffer, vram_size);
  free(xpm_buffer);
  return OK;
}




int vg_exit() {
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
