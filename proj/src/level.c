#include "bitmap.h"
#include "level.h"
#include "tron.h"
#include "video_gr.h"
#include "rtc.h"

extern Program *Tron;
extern level_t *level_night, *level_day, *level_dawn;


//change this function to create a level accordingly with the time of the RTC

void level_create ()
{
	level_night = (level_t *) malloc (sizeof(level_t));

	level_night->upper_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Night/Upper_model_night.bmp");
	level_night->lower_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Night/Lower_model_night.bmp");
	level_night->right_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Night/Right_model_night.bmp");
	level_night->left_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Night/Left_model_night.bmp");

	level_day = (level_t *) malloc (sizeof(level_t));

	level_day->upper_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Day/Upper_model_day.bmp");
	level_day->lower_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Day/Lower_model_day.bmp");
	level_day->right_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Day/Right_model_day.bmp");
	level_day->left_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Day/Left_model_day.bmp");

	level_dawn = (level_t *) malloc (sizeof(level_t));

	level_dawn->upper_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Dawn/Upper_model_dawn.bmp");
	level_dawn->lower_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Dawn/Lower_model_dawn.bmp");
	level_dawn->right_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Dawn/Right_model_dawn.bmp");
	level_dawn->left_rectangle = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/Level/Dawn/Left_model_dawn.bmp");
}

void level_draw (short* buffer)
{


	unsigned long *rtc_time;
	unsigned short h_res, v_res;

	h_res = getHorResolution();
	v_res = getVerResolution();	


	rtc_time = gettime();

	if (rtc_time[0]>=20 || rtc_time[0]<4)
	{
		drawBitmap(level_night->upper_rectangle, 0, 0, ALIGN_LEFT, buffer);
		drawBitmap(level_night->lower_rectangle, 0, v_res -134, ALIGN_LEFT, buffer);
		drawBitmap(level_night->left_rectangle, 0, 134, ALIGN_LEFT, buffer);
		drawBitmap(level_night->right_rectangle,  h_res -112, 134, ALIGN_LEFT, buffer);
	}
	else if (rtc_time[0] >= 4 && rtc_time[0] <12)
	{
		drawBitmap(level_dawn->upper_rectangle, 0, 0, ALIGN_LEFT, buffer);
		drawBitmap(level_dawn->lower_rectangle, 0, v_res -134, ALIGN_LEFT, buffer);
		drawBitmap(level_dawn->left_rectangle, 0, 134, ALIGN_LEFT, buffer);
		drawBitmap(level_dawn->right_rectangle,  h_res -112, 134, ALIGN_LEFT, buffer);		
	}
	else if (rtc_time[0] >= 12 && rtc_time[0] <20)
	{
		drawBitmap(level_day->upper_rectangle, 0, 0, ALIGN_LEFT, buffer);
		drawBitmap(level_day->lower_rectangle, 0, v_res -134, ALIGN_LEFT, buffer);
		drawBitmap(level_day->left_rectangle, 0, 134, ALIGN_LEFT, buffer);
		drawBitmap(level_day->right_rectangle,  h_res -112, 134, ALIGN_LEFT, buffer);		
	}
}

void level_destroy ()
{

	deleteBitmap(level_night->upper_rectangle);
	deleteBitmap(level_night->lower_rectangle);
	deleteBitmap(level_night->right_rectangle);
	deleteBitmap(level_night->left_rectangle);

	free (level_night);

	deleteBitmap(level_day->upper_rectangle);
	deleteBitmap(level_day->lower_rectangle);
	deleteBitmap(level_day->right_rectangle);
	deleteBitmap(level_day->left_rectangle);
	
	free (level_day);

	deleteBitmap(level_dawn->upper_rectangle);
	deleteBitmap(level_dawn->lower_rectangle);
	deleteBitmap(level_dawn->right_rectangle);
	deleteBitmap(level_dawn->left_rectangle);
	
	free (level_dawn);
}