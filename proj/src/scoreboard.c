#include "scoreboard.h"
#include "bitmap.h"
#include "players.h"
#include "tron.h"


extern Program *Tron;
extern scoreboard_t *scoreboard;
extern player *player_1, *player_2;

void scoreboard_create ()
{
	scoreboard = (scoreboard_t *) malloc (sizeof(scoreboard_t));

	scoreboard->numbers[0] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/0.bmp");
	scoreboard->numbers[1] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/1.bmp");
	scoreboard->numbers[2] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/2.bmp");
	scoreboard->numbers[3] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/3.bmp");
	scoreboard->numbers[4] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/4.bmp");
	scoreboard->numbers[5] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/5.bmp");
	scoreboard->numbers[6] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/6.bmp");
	scoreboard->numbers[7] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/7.bmp");
	scoreboard->numbers[8] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/8.bmp");
	scoreboard->numbers[9] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/9.bmp");

	switch (Tron->State)
	{
		case Menu_video:
			break;
		case Multiplayer_local:
			scoreboard->board = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/Scoreboard.bmp");
			break;
		case Singleplayer:
			scoreboard->board = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Scoreboard/Scoreboard_singleplayer.bmp");
			break;
		case Instructions:
			break;
		default: 
			break;
	}
}

void scoreboard_draw (short *buffer)
{
	drawBitmap(scoreboard->board, 92, 0, ALIGN_LEFT, buffer);

	drawBitmap(scoreboard->numbers[player_1->wins], 392, 20, ALIGN_LEFT, buffer);
	drawBitmap(scoreboard->numbers[player_2->wins], 562, 20, ALIGN_LEFT, buffer);
}


void scoreboard_destroy ()
{
	int i;

	for (i=0; i<10; i++)
	{
		deleteBitmap(scoreboard->numbers[i]);
	}

	deleteBitmap (scoreboard->board);

	free (scoreboard);
}