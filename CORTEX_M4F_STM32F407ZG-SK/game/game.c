#include "game.h"
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Topbarsize 20



/*
//Player1
int16_t player1X = 10;
int16_t player1Y = 10;
uint16_t player1W = 60;
uint16_t player1H = 10;
uint8_t player1IsReversed = 1;
*/
//Player2
int16_t player2X = LCD_PIXEL_WIDTH - 20;
int16_t player2Y = LCD_PIXEL_HEIGHT - 20;
uint16_t player2W = 60;
uint16_t player2H = 10;
uint8_t player2IsReversed = 0;

//Ball
uint16_t ballSize = 5;
int16_t ballX = ( LCD_PIXEL_WIDTH - 5 ) / 2;
int16_t ballY = ( LCD_PIXEL_HEIGHT - 5 ) - 30;// / 2;
int16_t ballVX = -3;
int16_t ballVY = -3;
uint8_t ballIsRun = 0;
uint8_t ballStepBack = 0;
uint16_t ballRadius = 3;//ballSize / 2;
//Bricks
uint16_t bricks[24];
uint8_t bricksinit = 1;

//Mode
uint8_t demoMode = 0;

void
BallReset()
{
	ballX = ( LCD_PIXEL_WIDTH - 5 ) / 2;
	ballY = ( LCD_PIXEL_HEIGHT - 5 ) -30;/// 2;

	ballVX = -3;
	ballVY = -3;

	ballIsRun = 1;
}

void
BricksInit()
{
	for (int16_t i = 0; i < 24;i++){
			bricks[i] = 0xffff;
	}
	LCD_SetTextColor( LCD_COLOR_WHITE );		
	for (uint16_t i = 0; i < 24;i++){
		for (uint16_t j = 0; j < 16;j++){
			LCD_DrawFullRect( i * 10, Topbarsize + j * 10, 8, 8 );
		}
	}
}

/*
void
GAME_EventHandler1()
{
	if( STM_EVAL_PBGetState( BUTTON_USER ) ){

		player1IsReversed = 0;

		while( STM_EVAL_PBGetState( BUTTON_USER ) );

		player1IsReversed = 1;
	}
}
*/
void
GAME_EventHandler2()
{
	if( IOE_TP_GetState()->TouchDetected ){

		player2IsReversed = 1;
		
		while( IOE_TP_GetState()->TouchDetected );
		player2IsReversed = 0;
		
	}
}

void
GAME_EventHandler3()
{
	if( ballIsRun == 0 ){
		BallReset();
	}
}

void
GAME_Update()
{
	//Player1
	LCD_SetTextColor( LCD_COLOR_BLACK );
//	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	LCD_DrawFullRect( player2X, player2Y, player2W, player2H );

	//Bricks init
	if (bricksinit == 1){
		BricksInit();
		bricksinit = 0;
	}
	
	if( demoMode == 0 ){
/*
		if( player1IsReversed )
			player1X -= 5;
		else
			player1X += 5;

		if( player1X <= 0 )
			player1X = 0;
		else if( player1X + player1W >= LCD_PIXEL_WIDTH )
			player1X = LCD_PIXEL_WIDTH - player1W;
*/
		//Player2
		if( player2IsReversed )
			player2X -= 5;
		else
			player2X += 5;

		if( player2X <= 0 )
			player2X = 0;
		else if( player2X + player2W >= LCD_PIXEL_WIDTH )
			player2X = LCD_PIXEL_WIDTH - player2W;

		//Ball
		if( ballIsRun == 1 ){

			LCD_SetTextColor( LCD_COLOR_BLACK );
			LCD_DrawCircle( ballX, ballY, ballRadius);

			//Ball move
			ballX += ballVX;
			ballY += ballVY;
			
			//Touch wall
			if( ballX <= 0 ){
				ballX = 0;
				ballVX *= -1;
			}
			else if( ballX + ballSize >= LCD_PIXEL_WIDTH ){
				ballX = LCD_PIXEL_WIDTH - ballSize;
				ballVX *= -1;
			}
			else if( ballY <= Topbarsize){
				ballY = Topbarsize;
				ballVY *= -1;
			}

			//PONG!
			
			if( ballY + ballSize >= player2Y ){
				if( ballX + ballSize >= player2X && ballX <= player2X + player2W ){
					if(ballX - player2X <= player2W/5){
						ballVY =-1;//3;
						ballVX =-3;//7;
					}
					if(ballX - player2X <= player2W/5 * 2){
						ballVY =-2;//3;
						ballVX =-2;//7;
					}
					if(ballX - player2X <= player2W/5 * 3){
						ballVY = 4;//3;
						ballVX = 0;//7;
					}
					if(ballX - player2X <= player2W/5 * 4){
						ballVY = -2;//3;
						ballVX = 2;//7;
					}
					if(ballX - player2X <= player2W/5 * 5){
						ballVY = -1;//3;
						ballVX = 3;//7;
					}
/*					if( ballX - ballSize <= player2Y + player2W/4 ){
						ballVY =-2;//3;
						ballVX =-2;//7;
					}
					else if( ballX >= player2Y + player2W - player2W/4 ){
						ballVY =-1;//3;
						ballVX = 3;//7;
					}
					else if( ballX + ballSize < player2Y + player2W/2 ){
						ballVY =-3;//7;
						ballVX =-1;//3;
					}
					else if( ballX > player2Y + player2W/2 ){
						ballVY =-3;//7;
						ballVX = 1;//3;
					}
					else{
						ballVY =-3;//9;
						ballVX = 0;//0;
					}*/
				}
				else
					BallReset();
			}
			int16_t xindex, yindex, xdisp, ydisp;
/*			xindex = ((ballX + ballRadius) / 10);
			yindex = ((ballY + ballRadius - Topbarsize)/ 10);
			xdisp = abs((ballX + ballRadius) - (xindex * 10 + 5));//brickSize / 2
			ydisp = abs((ballY + ballRadius) - (xindex * 10 + Topbarsize + 5));
			if (bricks[xindex] & (1 <<  yindex)){
				
			}
			
			
			if(xdisp * xdisp + ydisp * ydisp  <= hitThreshod){
				bricks[xindex] &= ~(1 << yindex);
				if(xdisp > ydisp) ballVX *= -1;
				else ballVY *= -1;
				ballStepBack = 1;
			}*/
			xindex = ((ballX) / 10);
			yindex = ((ballY - Topbarsize)/ 10);
			xdisp = abs(ballX - xindex * 10);
			ydisp = abs(ballY - Topbarsize - yindex * 10);
			if (bricks[xindex] & (1 <<  yindex)){
				bricks[xindex] &= ~(1 << yindex);
					
				if(xdisp <= ydisp){
					if(ballVX >= 0 && ballY - ballVY - Topbarsize - yindex * 10 - 10 < 0) ballVX *= -1;
					else ballVY *= -1;
				}
				else{
					if(ballVY >= 0 && ballX - ballVX - xindex * 10 - 10 < 0) ballVY *= -1;
					else ballVX *= -1;
				}
				ballStepBack = 1;
			}/*
			xindex = ((ballX + ballSize) / 10);
			yindex = ((ballY - Topbarsize)/ 10);
			if (!ballStepBack && bricks[xindex] & (1 <<  yindex)){
				bricks[xindex] &= ~(1 << yindex);
				if(abs(ballX + ballSize - xindex * 10) <= abs(ballY - (yindex * 10 + 10))){
					ballVY *= -1;
				}
				else{
					ballVX *= -1;
				}
				ballStepBack = 1;
			}
			xindex = (ballX / 10);
			yindex = ((ballY + ballSize - Topbarsize)/ 10);
			if (!ballStepBack && bricks[xindex] & (1 <<  yindex)){
				bricks[xindex] &= ~(1 << yindex);
				if(abs(ballX + ballSize - xindex * 10) <= abs(ballY + ballSize - yindex * 10)){
					ballVY *= -1;
				}
				else{
					ballVX *= -1;
				}
				ballStepBack = 1;
			}
			xindex = ((ballX + ballSize) / 10);
			yindex = ((ballY + ballSize - Topbarsize)/ 10);
			if (!ballStepBack && bricks[xindex] & (1 <<  yindex)){
				bricks[xindex] &= ~(1 << yindex);
				if(abs(ballX - (xindex * 10 + 10)) <= abs(ballY + ballSize - yindex * 10)){
					ballVY *= -1;
				}
				else{
					ballVX *= -1;
				}
				ballStepBack = 1;
			}*/
			if(ballStepBack){
				ballX -= ballVX;
				ballY -= ballVY;
				ballStepBack = 0;
			}
/*
			if( ballY <= player1Y + player1H ){
				if( ballX + ballSize >= player1X && ballX <= player1X + player1W ){
					if( ballX - ballSize <= player1Y + player1W/4 ){
						ballVY = 3;
						ballVX =-7;
					}
					else if( ballX >= player1Y + player1W - player1W/4 ){
						ballVY = 3;
						ballVX = 7;
					}
					else if( ballX + ballSize < player1Y + player1W/2 ){
						ballVY = 7;
						ballVX =-3;
					}
					else if( ballX > player1Y + player1W/2 ){
						ballVY = 7;
						ballVX = 3;
					}
					else{
						ballVY = 9;
						ballVX = 0;
					}
				}
				else
					BallReset();
			}*/
		}
	}
/*
		else{	//if demoMode == 1

			//Player1 move
			if( ballVY < 0 ){
				if( player1X + player1W/2 < ballX + ballSize/2 ){
					player1X += 8;
					player2X += 2;
				}
				else{
					player1X -= 8;
					player2X -= 2;
				}
			}

			//Player2 move
			if( ballVY > 0 ){
				if( player2X + player2W/2 < ballX + ballSize/2 ){
					player1X += 2;
					player2X += 8;
				}
				else{
					player1X -= 2;
					player2X -= 8;
				}

			}

			if( player1X <= 0 )
				player1X = 0;
			else if( player1X + player1W >= LCD_PIXEL_WIDTH )
				player1X = LCD_PIXEL_WIDTH - player1W;

			if( player2X <= 0 )
				player2X = 0;
			else if( player2X + player2W >= LCD_PIXEL_WIDTH )
				player2X = LCD_PIXEL_WIDTH - player2W;


			//Ball
			if( ballIsRun == 1 ){

				LCD_SetTextColor( LCD_COLOR_BLACK );
				LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );

				//Touch wall
				ballX += ballVX;
				if( ballX <= 0 ){
					ballX = 0;
					ballVX *= -1;
				}
				else if( ballX + ballSize >= LCD_PIXEL_WIDTH ){
					ballX = LCD_PIXEL_WIDTH - ballSize;
					ballVX *= -1;
				}

				//PONG!
				ballY += ballVY;
				if( ballY + ballSize >= player2Y ){
					if( ballX + ballSize >= player2X && ballX <= player2X + player2W ){
					if( ballX - ballSize <= player2Y + player2W/4 ){
						ballVY =-3;
						ballVX =-7;
					}
					else if( ballX >= player2Y + player2W - player2W/4 ){
						ballVY =-3;
						ballVX = 7;
					}
					else if( ballX + ballSize < player2Y + player2W/2 ){
						ballVY =-7;
						ballVX =-3;
					}
					else if( ballX > player2Y + player2W/2 ){
						ballVY =-7;
						ballVX = 3;
					}
					else{
						ballVY =-9;
						ballVX = 0;
					}
				}
				else
					BallReset();
			}

			if( ballY <= player1Y + player1H ){
				if( ballX + ballSize >= player1X && ballX <= player1X + player1W ){
					if( ballX - ballSize <= player1Y + player1W/4 ){
						ballVY = 3;
						ballVX =-7;
					}
					else if( ballX >= player1Y + player1W - player1W/4 ){
						ballVY = 3;
						ballVX = 7;
					}
					else if( ballX + ballSize < player1Y + player1W/2 ){
						ballVY = 7;
						ballVX =-3;
					}
					else if( ballX > player1Y + player1W/2 ){
						ballVY = 7;
						ballVX = 3;
					}
					else{
						ballVY = 9;
						ballVX = 0;
					}
				}
				else
					BallReset();
			}
		}
	}*/
}

	void
GAME_Render()
{
	LCD_SetTextColor( LCD_COLOR_BLACK );
	for (uint16_t i = 0; i < 24;i++){
		for (uint16_t j = 0; j < 16;j++){
			if (!(bricks[i] & (1 << j))){
				LCD_SetTextColor( LCD_COLOR_BLACK );
				LCD_DrawFullRect( i * 10, Topbarsize + j * 10, 8, 8 );
			}
			else{
				LCD_SetTextColor( LCD_COLOR_WHITE );
				LCD_DrawFullRect( i * 10, Topbarsize + j * 10, 8, 8 );
			}
		}
	}
	LCD_SetTextColor( LCD_COLOR_WHITE );
//	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	LCD_DrawFullRect( player2X, player2Y, player2W, player2H );
//	LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );
	LCD_DrawCircle( ballX, ballY, ballRadius);
	LCD_DrawLine( 0, Topbarsize - 1, LCD_PIXEL_WIDTH, LCD_DIR_HORIZONTAL );
}
