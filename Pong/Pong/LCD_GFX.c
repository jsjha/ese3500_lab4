/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */ 

#include "LCD_GFX.h"
#include "ST7735.h"
#include <string.h>

/******************************************************************************
* Local Functions
******************************************************************************/



/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
* @brief		Convert RGB888 value to RGB565 16-bit color data
* @note
*****************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255));
}

/**************************************************************************//**
* @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
* @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
* @note
*****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
	LCD_setAddr(x,y,x,y);
	SPI_ControllerTx_16bit(color);
}

/**************************************************************************//**
* @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
* @brief		Draw a character starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor){
	uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
	int i, j;
	if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
		for(i=0;i<5;i++){
			uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
			for(j=0;j<8;j++) {
				if ((pixels>>j)&(1==1)){
					LCD_drawPixel(x+i,y+j,fColor);
				}
				else {
					LCD_drawPixel(x+i,y+j,bColor);
				}
			}
		}
	}
}


/******************************************************************************
* LAB 4 TO DO. COMPLETE THE FUNCTIONS BELOW.
* You are free to create and add any additional files, libraries, and/or
*  helper function. All code must be authentically yours.
******************************************************************************/

/**************************************************************************//**
* @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored circle of set radius at coordinates
* @note
*****************************************************************************/
void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
{
	// Works assuming you are not drawing a circle where the radius is larger than the x or y coordinate.
	LCD_drawBlock(x0-(radius/2), y0-radius, x0+(radius/2), y0+radius, color);
	LCD_drawBlock(x0-radius, y0-(radius/2), x0+radius, y0+(radius/2), color);
	LCD_drawBlock(x0-(13*radius/16), y0-(13*radius/16), x0+(13*radius/16), y0+(13*radius/16), color);
}


/**************************************************************************//**
* @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
* @brief		Draw a line from and to a point with a color
* @note
*****************************************************************************/
void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
{
	
	// This is Bresenham's Algorithm for a line
	
	short dy, dx, incrE, incrNE, d,x,y;
	
	// Width
	dx = x1 - x0;
	
	// Length
	dy = y1 - y0;
	
	// This distance tells us if the "midpoint" of a pixel is above or below the line
	d = 2 * dy - dx;
	
	// If above, inrement d by this value
	incrE = 2*dy;
	
	// If below, increment d by this value
	incrNE = 2*(dy - dx);
	x = x0;
	y = y0;

	LCD_drawPixel(x,y,c);

	while(x < x1) {
		  
		if (d <= 0) {
			d += incrE;
			x++;
		}
		else {
			d += incrNE;
			x++;
			y++;
		}
		
		LCD_drawPixel(x,y,c);
	  }	
	
}



/**************************************************************************//**
* @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw a colored block at coordinates
* @note
*****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
	int x_small = 0;
	int y_small = 0;
	int x_large = 0;
	int y_large = 0;
	
	if (x1-x0 > 0) {
		x_small = x0;
		x_large = x1;
	} else {
		x_small = x1;
		x_large = x0;
	}
	
	if (y1-10 > 0) {
		y_small = y0;
		y_large = y1;
	} else {
		y_small = y1;
		y_large = y0;
	}
	
	LCD_setAddr(x_small,y_small,x_large,y_large);

	for(int i=x_small;i<=x_large;i++){
		for(int j=y_small;j<=y_large;j++){
			if (j< i) {
				SPI_ControllerTx_16bit(color);
			} else {
				SPI_ControllerTx_16bit(RED);
			}
		}
	}
}


/**************************************************************************//**
* @fn			void LCD_setScreen(uint16_t color)
* @brief		Draw the entire screen to a color
* @note
*****************************************************************************/
void LCD_setScreen(uint16_t color) 
{
	LCD_setAddr(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
	
	for(int i=0;i<LCD_WIDTH;i++){
		for(int j=0;j<LCD_HEIGHT;j++){
			SPI_ControllerTx_16bit(color);
		}
	}
}

/**************************************************************************//**
* @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
* @brief		Draw a string starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
{
	if ((LCD_WIDTH-x*strlen(str)>7)&&(LCD_HEIGHT-y>7)){
	
		for (int i = 0; i < strlen(str); i++) {
			LCD_drawChar(x + 5*i, y, str[i], fg, bg);
		}
	
	}
	
}