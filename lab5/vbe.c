#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "i8042.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

void vbe_print(vbe_mode_info_t *vmi_p){
	printf("X Resolution: %d\nY Resolution: %d\nBits per Pixel: %d\nVRAM Physical Adress: %x\n",
			vmi_p->XResolution, 
			vmi_p->YResolution, 
			vmi_p->BitsPerPixel, 
			vmi_p->PhysBasePtr);
}


int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	phys_bytes buf;
	struct reg86u r;
	mmap_t map;


	if (lm_init() == NULL)
	{
		printf ("\tvbe_get_mode_info(): lm_init() failed\n");
		return ERROR;
	}

	if(lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL)
	{
		printf("\tvbe_get_mode_info(): lm_alloc() failed\n");
		return ERROR;
	}

	r.u.b.intno = 0x10; /* BIOS video services */

  	r.u.w.ax = VBE_MODE_INFO;
	r.u.w.es = PB2BASE(map.phys);
	r.u.w.di = PB2OFF(map.phys);
	r.u.w.cx = mode;

	if( sys_int86(&r) != OK){
		printf("\tvbe_get_mode_info(): sys_int86 failed\n");
		return ERROR;
	}


	*vmi_p = *(vbe_mode_info_t*)map.virtual;

	lm_free(&map);
  
    return OK;
}

void* vbe_get_controller_info(vbe_controller_info_t* vbe_info){
	struct reg86u r;
	mmap_t map;

	void* aux = lm_init();

	if (aux == NULL)
	{
		printf ("\tvbe_get_controller_info(): lm_init() failed\n");
		return NULL;
	}

	if(lm_alloc(sizeof(vbe_controller_info_t), &map) == NULL)
	{
		printf("\tvbe_get_controller_info(): lm_alloc() failed\n");
		return NULL;
	}

	vbe_info->VbeSignature[0] = 'V';
	vbe_info->VbeSignature[1] = 'B';
	vbe_info->VbeSignature[2] = 'E';
	vbe_info->VbeSignature[3] = '2';

	r.u.b.intno = 0x10;

	r.u.w.ax = VBE_CTRL_INFO;				/*Get Controller Info Function*/
	r.u.w.es = PB2BASE(map.phys);
	r.u.w.di = PB2OFF(map.phys);

	if( sys_int86(&r) != OK){
			printf("\tvbe_get_controller_info(): sys_int86 failed\n");
			return NULL;
	}

	*vbe_info = *(vbe_controller_info_t*)map.virtual;

	lm_free(&map);

	return aux;
}
