#include "stm32f10x.h"

#define LED_PIN	GPIO_Pin_7	//PA.7

#define BUTTON_PIN GPIO_Pin_3	//PC.3

void Button_Init(void) {
	/* Enable GPIOC clock            */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);                
	
	/* Configure Button (PC.3) pins as input */
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = BUTTON_PIN;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &gpio);
}

int Button_Read(void) {
	return GPIO_ReadInputDataBit(GPIOC, BUTTON_PIN);
}

void LED_Init(void) {
	/* Enable GPIOA clock            */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   
	
	/* Configure LED (PA.7) pins as output */
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = LED_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &gpio);
}

void LED_Write(int state) {
		if(state == 1) {
			GPIO_ResetBits(GPIOA, LED_PIN);	;	//off
		} else {
			GPIO_SetBits(GPIOA, LED_PIN);	;	  //on
		}
}

int main(void) {
	Button_Init();
	LED_Init();
	while(1) {
		LED_Write(Button_Read());
	}
}
