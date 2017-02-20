#include "test5.h"

/** Creates a new sprite with pixmap "pic", with specified
* position (within the screen limits) and speed;
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/
Sprite *create_sprite(char *pic[]) 
{
//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
	if( sp == NULL )
		return NULL;
	// read the sprite pixmap
	sp->map = (char *) read_xpm(pic, &(sp->width), &(sp->height));
	if( sp->map == NULL ) 
	{
		free(sp);
		return NULL;
	}
	return sp;
}

void destroy_sprite(Sprite *sp) {
	if( sp == NULL )
		return;
	free(sp->map);
	free(sp);
	sp = NULL; // hopeless: pointer is passed by value
				// should do this @ the caller
}

void draw_sprite(Sprite *sp) 
{
	vg_draw_xpm((unsigned int) sp->x, (unsigned int) sp->y, sp->map, sp->width, sp->height);
}

void erase_sprite(Sprite *sp)
{
	vg_erase_xpm((unsigned int) sp->x, (unsigned int) sp->y, sp->map, sp->width, sp->height);
}