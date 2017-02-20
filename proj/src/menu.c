#include "menu.h"
#include <minix/syslib.h> //where NULL is declared
#include "vars.h"
#include "video_gr.h"
#include "tron.h"
#include "mouse.h"

extern Program *Tron;
extern menu_t *menu;
extern mouse_t* mouse_info;
extern Program *Tron;

void start_menu ()
{
	unsigned short h_res, v_res;

	h_res = getHorResolution();
	v_res = getVerResolution();	

	switch (Tron->State)
	{
		case Menu_video:
			menu_create();

			button_create ("Local Multiplayer", loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Menu/Buttons/local.bmp"),
					loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Menu/Buttons/local2.bmp"), h_res/2, 100);
			button_create ("Singleplayer", loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Menu/Buttons/singleplayer.bmp"),
					loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Menu/Buttons/singleplayer2.bmp"), h_res/2, 200);
			button_create ("Instructions", loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Menu/Buttons/instructions.bmp"),
					loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Menu/Buttons/instructions2.bmp"), h_res/2, 300);
			button_create ("Exit", loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Menu/Buttons/exit2.bmp"),
					loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Menu/Buttons/exit.bmp"), h_res/2, 400);
			menu->buttons[menu->selection]->selected = 1;
			break;
		case Multiplayer_local:
			break;
		default:
			break;
	}
}

void menu_create ()
{
	menu = (menu_t *) malloc (sizeof(menu));
	menu->n = 0;
	menu->buttons = NULL;
	menu->selection = 0;
	menu->background = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Menu/Menu_background.bmp");
}

void button_create (char *button_name, Bitmap *bmp_normal, Bitmap *bmp_selected, int x, int y)
{
	
	button *b = (button *) malloc(sizeof(button));
	b->name = button_name;
	b->image[0] = bmp_normal;
	b->image[1] = bmp_selected;
	b->x_pos = x;
	b->y_pos = y;
	b->selected = 0;

	menu->n++;
	menu->buttons = (button **) realloc (menu->buttons, sizeof(button *)*menu->n);
	menu->buttons[(menu->n - 1)] = b;
}


void menu_change_selection (Direction direction)
{
	menu->buttons[menu->selection]->selected = 0;
	if ((menu->selection == (menu->n-1)) && (direction == Down))
		{
			menu->selection = 0;
		}
	else if ((menu->selection == 0) && (direction == Up))
		{
			menu->selection = (menu->n)-1;
		}
	else if (direction == Up)
		{	
			menu->selection--;
		}
	else if (direction == Down)
		{
			menu->selection++;
		}
	menu->buttons[menu->selection]->selected = 1;
}


void menu_make_selection ()
{
	if (strncmp(menu->buttons[menu->selection]->name, "Local Multiplayer", strlen("Local Multiplayer")) == 0)
	{
		tron_change_state (Multiplayer_local);
		return;		
	}

	if (strncmp(menu->buttons[menu->selection]->name, "Exit", strlen("Exit")) == 0)
	{	
		vg_exit ();
		tron_delete_state ();
		Tron->quit = 1;
		return;		
	}

	if (strncmp(menu->buttons[menu->selection]->name, "Singleplayer", strlen("Singleplayer")) == 0)
	{
		tron_change_state (Singleplayer);
		return;
	}	

	if (strncmp(menu->buttons[menu->selection]->name, "Instructions", strlen("Instructions")) == 0)
	{
		tron_change_state (Instructions);
		return;
	}


}

void button_draw(Bitmap *image, short* buffer, int x, int y)
{
	drawBitmap(image, x, y, ALIGN_CENTER, buffer);
}

void menu_draw (short* buffer)
{
	int i;

	drawBitmap(menu->background, 0, 0, ALIGN_LEFT, buffer);

	for (i=0; i<menu->n; i++)
	{
		button_draw (menu->buttons[i]->image[menu->buttons[i]->selected], buffer, menu->buttons[i]->x_pos, menu->buttons[i]->y_pos);
	}
	
	
}

void menu_destroy ()
{
	int i, j;

	if (menu == NULL)
		return;

	deleteBitmap(menu->background);

	for (i=0; i<menu->n; i++)
	{
		for (j=0; j<2; j++)
		{
			deleteBitmap(menu->buttons[i]->image[j]);
		}
		free (menu->buttons[i]);
	}

	free (menu);
}



