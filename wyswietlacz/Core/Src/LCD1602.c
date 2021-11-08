/*
 * LCD1602.c
 *
 *  Created on: 21-Jan-2020
 *      Author: Controllerstech
 */

#include <LCD1602.h>
#include "stm32f3xx_hal.h"
#include "main.h"

#define ON 1
#define OFF 0

#define timer htim16


extern TIM_HandleTypeDef timer;

void delay (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&timer, 0);
	while (__HAL_TIM_GET_COUNTER(&timer) < us)
	{

	}
}


void send_to_lcd (char data, int rs)
{
	HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, rs);

	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, ((data>>0)&0x01));
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, ((data>>1)&0x01));
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, ((data>>2)&0x01));
	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, ((data>>3)&0x01));


	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, ON);
	delay (100);
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, OFF);
	delay (100);
}

void lcd_send_cmd (char cmd)
{
    char datatosend;


    datatosend = ((cmd>>4)&0x0f);
    send_to_lcd(datatosend,0);


    datatosend = ((cmd)&0x0f);
	send_to_lcd(datatosend, 0);
}

void lcd_send_data (char data)
{
	char datatosend;

	datatosend = ((data>>4)&0x0f);
	send_to_lcd(datatosend, 1);


	datatosend = ((data)&0x0f);
	send_to_lcd(datatosend, 1);
}

void lcd_clear (void)
{
	lcd_send_cmd(0x01);
	delay(2000);
}

void lcd_cursor(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}


void lcd_Init (void)
{

	lcd_send_cmd (0x28);
		HAL_Delay(1);
		lcd_send_cmd (0x08);
		HAL_Delay(1);
		lcd_send_cmd (0x01);
		HAL_Delay(1);
		HAL_Delay(1);
		lcd_send_cmd (0x06);
		HAL_Delay(1);
		lcd_send_cmd (0x0C);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}
