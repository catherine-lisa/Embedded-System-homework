#include "stm32f10x.h"

#define LED_PIN	GPIO_Pin_5	//PA.5
#define BUTTON_PIN	GPIO_Pin_13	//PC.13
#define BUTTON_EXTI_LINE  EXTI_Line13  //PC.13

void EXTI15_10_IRQHandler(){
	if(EXTI_GetITStatus(BUTTON_EXTI_LINE) != RESET){
		EXTI_ClearITPendingBit(BUTTON_EXTI_LINE);
		uint8_t state = GPIO_ReadOutputDataBit(GPIOA, LED_PIN);
		GPIO_WriteBit(GPIOA, LED_PIN, !state);
	}
}

void EXTI_Line_Init(void){
		EXTI_InitTypeDef exti;
		exti.EXTI_Line = BUTTON_EXTI_LINE;
		exti.EXTI_Trigger = EXTI_Trigger_Falling;
		exti.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_Init(&exti);	
}

void Button_Init(void) {
	/* Enable GPIOC clock            */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);                
	
	/* Configure Button (PC.13) pins as input */
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = BUTTON_PIN;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &gpio);
}

void LED_Init(void) {
	/* Enable GPIOA clock            */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   
	
	/* Configure LED (PA.5) pins as output */
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = LED_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &gpio);
}

int main(void) {
	Button_Init();
	LED_Init();
	EXTI_Line_Init();
	while(1) {
		EXTI15_10_IRQHandler();
	}
}
