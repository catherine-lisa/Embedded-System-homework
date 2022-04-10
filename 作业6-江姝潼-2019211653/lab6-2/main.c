#include "stm32f10x.h"

#define ATOMIC_BIT_IO 1

#define LED_PIN	7	//PA.7

#define BUTTON_PIN 3	//PC.3

void Button_Init(void) {
	/* Enable GPIOC clock            */
  RCC->APB2ENR |= (1ul << 4);                
	
	/* Configure Button (PC.13) pins as input */
  GPIOC->CRL &= ~(0xFul << (4*(BUTTON_PIN)));	//Clear Bit[15..12]
  GPIOC->CRL |=  (0x4ul << (4*(BUTTON_PIN))); //Set Bit[15..12] = 0b0100
}

int Button_Read(void) {
	return ((GPIOC->IDR >> BUTTON_PIN) & 1);
}

void LED_Init(void) {
	/* Enable GPIOA clock            */
  RCC->APB2ENR |= (1ul << 2);   
	
	/* Configure LED (PA.5) pins as output */
  GPIOA->CRL &= ~(0xFul << (4*LED_PIN)); //Clear Bit[31..26]
  GPIOA->CRL |=  ( 0x1ul << (4*LED_PIN)); //Set Bit[31..26] = 0b0001
}

void LED_Write(int state) {
		if(state == 1) {
			GPIOA->BSRR = (1 << LED_PIN) << 16;	//off
		} else {
			GPIOA->BSRR = (1 << LED_PIN);				//on
		}		
}

int main(void) {
	Button_Init();
	LED_Init();
	while(1) {
		LED_Write(Button_Read());
	}
}
