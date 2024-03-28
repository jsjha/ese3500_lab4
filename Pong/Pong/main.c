#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define F_CPU 16000000UL
#include <util/delay.h>


#include "ST7735.h"
#include "LCD_GFX.h"


#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128
#define BALL_RADIUS 5
#define PADDLE_WIDTH 8
#define PADDLE_HEIGHT 20
#define BACKGROUND_COLOR BLACK

#define F_CPU                16000000UL   // 16MHz clock
#define USART_BAUD_RATE      9600
#define USART_BAUD_PRESCALER (((F_CPU / (USART_BAUD_RATE * 16UL))) - 1)


volatile int ballX = SCREEN_WIDTH / 2;
volatile int ballY = SCREEN_WIDTH / 2;

volatile int ballVelX = -5;
volatile int ballVelY = -3;

volatile int paddleX1 = 0;
volatile int paddleY1 = 53;

volatile int paddleVelY1 = 5;

volatile int paddleX2 = 151;
volatile int paddleY2 = 53;

volatile int paddleVelY2 = 5;

volatile int score1 = 0;
volatile int score2 = 0;

void Initialize() {
	
	
	DDRD |= (1<<DDD3);
	DDRD |= (1<<DDD4);
	
	DDRD &= ~(1 << DDD0);
	DDRD &= ~(1 << DDD1);
	
	// Clear power reduction bit for ADC
	PRR0 &= ~(1 << PRADC);
	
	// Select Vref = AVcc
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
	
	// Select Channel ADC0 (pin C0)
	ADMUX &= ~(1 << MUX0);
	ADMUX &= ~(1 << MUX1);
	ADMUX &= ~(1 << MUX2);
	ADMUX &= ~(1 << MUX3);
	
	DDRC &= ~(1<<DDC0);

	
	ADCSRA |= (1 << ADATE);   // Autotriggering of ADC

	// Free running mode ADTS[2:0] = 000
	ADCSRB &= ~(1 << ADTS0);
	ADCSRB &= ~(1 << ADTS1);
	ADCSRB &= ~(1 << ADTS2);
	
	// Disable digital input buffer on ADC pin
	DIDR0 |= (1 << ADC0D);

	// Enable ADC
	ADCSRA |= (1 << ADEN);
	
	// Enable ADC interrupt
	ADCSRA |= (1<<ADIE);

	// Start conversion
	ADCSRA |= (1 << ADSC);
}


void updateBallPosition() {
	// Clear the old ball position
	LCD_drawCircle(ballX, ballY, BALL_RADIUS, BACKGROUND_COLOR);
	
	// Update ball position based on velocity
	ballX += ballVelX;
	ballY += ballVelY;
	
	// Replace ball if screen hit vertical edges
	if (ballX <= BALL_RADIUS) {
		ballX = 79;
		ballY = 63;
		LCD_drawCircle(ballX, ballY, BALL_RADIUS, RED);
		ballVelX = rand() % 10 + 1;
		ballVelY = rand() % 10 + 1;
		if (ballVelX <= 5) {
			ballVelX = ballVelX * -1;
			} else {
			ballVelX = ballVelX - 5;
		}
		if (ballVelY <= 5) {
			ballVelY = ballVelY * -1;
			} else {
			ballVelY = ballVelY - 5;
		}
		score1 += 1;
		PORTD |= (1 << PORTD3);
		PORTD |= (1 << PORTD4);
		_delay_ms(50);
		PORTD &= ~(1 << PORTD3);
		PORTD &= ~(1 << PORTD4);
		
	}
	
	if (ballX >= SCREEN_WIDTH - BALL_RADIUS) {
		ballX = 79;
		ballY = 63;
		LCD_drawCircle(ballX, ballY, BALL_RADIUS, RED);
		ballVelX = rand() % 10 + 2;
		ballVelY = rand() % 10 + 2;
		if (ballVelX <= 6) {
			ballVelX = ballVelX * -1;
			} else {
			ballVelX = ballVelX - 5;
		}
		if (ballVelY <= 6) {
			ballVelY = ballVelY * -1;
			} else {
			ballVelY = ballVelY - 5;
		}
		score2 += 1;
		PORTD |= (1 << PORTD3);
		PORTD |= (1 << PORTD4);
		_delay_ms(50);
		PORTD &= ~(1 << PORTD3);
		PORTD &= ~(1 << PORTD4);
		
	}
	
	// Reverse direction if ball hits horizontal edges
	if (ballY <= BALL_RADIUS + 6|| ballY >= SCREEN_HEIGHT - BALL_RADIUS - 6 - 1) {
		ballVelY = -ballVelY;
		//ballVelX = rand() % 11 - 10;
		//ballVelY = rand() % 11 - 10;
		//if (ballVelX == 0) {
		//ballVelX += 1;
		//}
		//if (ballVelY == 0) {
		//ballVelY += 1;
		//}
		PORTD |= (1 << PORTD3);
		_delay_ms(50);
		PORTD &= ~(1 << PORTD3);
	}
	
	// Reverse direction if ball hits paddle1 edges
	if (ballX <= paddleX1 + PADDLE_WIDTH + BALL_RADIUS + 5 && ballY >= paddleY1 && ballY <= paddleY1 + PADDLE_HEIGHT) {
		ballVelX = -ballVelX;
		ballVelY = -ballVelY;
		ballVelX = rand() % 5 + 2;
		ballVelY = rand() % 11 - 10;
		if (ballVelY == 0) {
			ballVelY += 1;
			
		}
		PORTD |= (1 << PORTD3);
		_delay_ms(50);
		PORTD &= ~(1 << PORTD3);
		
	}
	
	// Reverse direction if ball hits paddle2 edges
	if (ballX >= paddleX2 - PADDLE_WIDTH - BALL_RADIUS + 5 && ballY >= paddleY2 && ballY <= paddleY2 + PADDLE_HEIGHT) {
		ballVelX = -ballVelX;
		ballVelY = -ballVelY;
		ballVelX = -1 * (rand() % 5 + 1) - 2;
		ballVelY = rand() % 11 - 10;
		if (ballVelY == 0) {
			ballVelY += 1;
			
		}
		PORTD |= (1 << PORTD3);
		_delay_ms(50);
		PORTD &= ~(1 << PORTD3);
		
	}
	
	// Draw the ball at the new position
	LCD_drawCircle(ballX, ballY, BALL_RADIUS, RED);
}

void updatePaddlePosition1() {
	// Clear the old paddle position
	//LCD_drawBlock(paddleX1, paddleY1, paddleX1 + PADDLE_WIDTH, paddleY1 + PADDLE_HEIGHT, BACKGROUND_COLOR);
	//
	//paddleY1 += paddleVelY1;
	//
	//// Ensure paddle stays within screen boundaries
	//if (paddleY1 < 0) {
	//paddleY1 = 0;
	//paddleVelY1 = -paddleVelY1;
	//} else if (paddleY1 > SCREEN_HEIGHT - PADDLE_HEIGHT) {
	//paddleY1 = SCREEN_HEIGHT - PADDLE_HEIGHT;
	//paddleVelY1 = -paddleVelY1;
	//}
	//
	//LCD_drawBlock(paddleX1, paddleY1, paddleX1 + PADDLE_WIDTH, paddleY1 + PADDLE_HEIGHT, WHITE);
	LCD_drawBlock(paddleX1, paddleY1, paddleX1 + PADDLE_WIDTH, paddleY1 + PADDLE_HEIGHT, BACKGROUND_COLOR);
	
	if ((PIND & (1 << PIND0))) {
		if (paddleY1 - 5>= 0) {
			paddleY1 -= 5;
		}
		} else if (PIND & (1 << PIND1)) {
		if (paddleY1 + PADDLE_HEIGHT <= 127) {
			paddleY1 += 5;
		}
	}
	
	if (paddleY1 < 0) {
		paddleY1 = 0;
		} else if (paddleY1 > SCREEN_HEIGHT - PADDLE_HEIGHT) {
		paddleY1 = SCREEN_HEIGHT - PADDLE_HEIGHT - 1;
	}
	
	
	
	LCD_drawBlock(paddleX1, paddleY1, paddleX1 + PADDLE_WIDTH, paddleY1 + PADDLE_HEIGHT, WHITE);
	
	
}

void updatePaddlePosition2() {
	
	//// Clear the old paddle position
	//LCD_drawBlock(paddleX2, paddleY2, paddleX2 + PADDLE_WIDTH, paddleY2 + PADDLE_HEIGHT, BACKGROUND_COLOR);
	//
	////if (ADC >= 550) {
	////
	////// Ensure paddle stays within screen boundaries
	////if (paddleY2 <= SCREEN_HEIGHT - PADDLE_HEIGHT) {
	////paddleY2 -= paddleVelY2;
	////}
	////
	////} else {
	////
	////if (paddleY2 > PADDLE_HEIGHT / 2) {
	////paddleY2 += paddleVelY2;
	////}
	////
	////}
	//
	//LCD_drawBlock(paddleX2, paddleY2, paddleX2 + PADDLE_WIDTH, paddleY2 + PADDLE_HEIGHT, WHITE);
	
	
	//uint16_t joystickValue = ADC;
	//
	//
	//int16_t mappedValue = (joystickValue * (SCREEN_HEIGHT - PADDLE_HEIGHT)) / 1023;
	//
	//paddleY2 = mappedValue;
	
	// LCD_drawBlock(paddleX2, paddleY2, paddleX2 + PADDLE_WIDTH, paddleY2 + PADDLE_HEIGHT, BACKGROUND_COLOR);
	
	// LCD_drawBlock(paddleX2, paddleY2, paddleX2 + PADDLE_WIDTH, paddleY2 + PADDLE_HEIGHT, WHITE);
	
	LCD_drawBlock(paddleX2, paddleY2, paddleX2 + PADDLE_WIDTH, paddleY2 + PADDLE_HEIGHT, BACKGROUND_COLOR);
	
	if (ADC >= 550) {
		if (paddleY2 - 5>= 0) {
			paddleY2 -= 5;
		}
		} else if (ADC <= 490) {
		if (paddleY2 + PADDLE_HEIGHT <= 127) {
			paddleY2 += 5;
		}
	}
	
	if (paddleY1 < 0) {
		paddleY2 = 0;
		} else if (paddleY1 > SCREEN_HEIGHT - PADDLE_HEIGHT) {
		paddleY2 = SCREEN_HEIGHT - PADDLE_HEIGHT - 1;
	}
	
	
	
	LCD_drawBlock(paddleX2, paddleY2, paddleX2 + PADDLE_WIDTH, paddleY2 + PADDLE_HEIGHT, WHITE);

}


void updateScoreboard1() {
	char str[5];
	sprintf(str, "%d", score1);
	LCD_drawString(129, 7, str, WHITE, BLACK);
}

void updateScoreboard2() {
	char str[5];
	sprintf(str, "%d", score2);
	LCD_drawString(20, 7, str, WHITE, BLACK);
}


int main() {
	Initialize();
	lcd_init();
	
	// Set up screen
	LCD_setScreen(BACKGROUND_COLOR);
	
	
	// main loop
	while (1) {
		
		
		// update ball position
		updateBallPosition();
		
		updatePaddlePosition1();
		
		updatePaddlePosition2();
		
		updateScoreboard1();
		
		updateScoreboard2();
		
		_delay_ms(50);
	}
	
	return 0;
}
