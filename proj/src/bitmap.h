#ifndef __BITMAP_H
#define __BITMAP_H

#include <minix/syslib.h>
#include <minix/com.h>
#include <minix/drivers.h>
#include <stdio.h>

/** @defgroup Bitmap Bitmap
 * @{
 * Functions for manipulating bitmaps
 */

typedef enum {
    ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
} Alignment;

typedef struct {
    unsigned short type; // specifies the file type
    unsigned int size; // specifies the size in bytes of the bitmap file
    unsigned int reserved; // reserved; must be 0
    unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

typedef struct {
    unsigned int size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels
    int height; // specifies height in pixels
    unsigned short planes; // specifies the number of color planes, must be 1
    unsigned short bits; // specifies the number of bit per pixel
    unsigned int compression; // specifies the type of compression
    unsigned int imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    unsigned int nColors; // number of colors used by the bitmap
    unsigned int importantColors; // number of colors that are important
} BitmapInfoHeader;

/// Represents a Bitmap
typedef struct {
    BitmapInfoHeader bitmapInfoHeader;
    short *bitmapData;
} Bitmap;

/**
 * @brief Loads a bmp image
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position
 *
 * @param bmp Bitmap* to be drawn
 * @param x x coord
 * @param y y coord
 * @param alignment image alignment
 * @param buffer buffer to draw in
 */
void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment, short *buffer);

/**
 * @brief Takes a value between 0 and 255 for red green and blue, and turns into a value we can use for color
 *
 * @param r red
 * @param g green
 * @param b blue
 * @return corresponding value for color
 */

short rgb (unsigned char r, unsigned char g, unsigned char b);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position
 *
 * @param bmp Bitmap* to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 * @param buffer buffer to draw in
 */

void drawBitmap_greenscreen(Bitmap* bmp, int x, int y, Alignment alignment, short *buffer);

/**
 * @brief Destroys the given bitmap, freeing all resources used by it.
 *
 * @param bmp Bitmap* to be destroyed
 */
void deleteBitmap(Bitmap* bmp);

/**@}*/

#endif
