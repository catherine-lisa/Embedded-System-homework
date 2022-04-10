#include <stdio.h>

#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons

#include "stm32f10x.h"                  // Device header

volatile uint32_t msTicks;              // counts 1ms timeTicks

//update Systick count
void SysTick_Handler(void){
	msTicks++;
}

void Delay(uint32_t dlyTicks){
	uint32_t curTicks;
	curTicks = msTicks;
	while((msTicks-curTicks) < dlyTicks){
`		__NOP();
	}
}

#define DELAY_TICKS 10
void low_to_high(){
	LED_Off(0);
	Delay(DELAY_TICKS);
	LED_On(0);
	Delay(DELAY_TICKS);
}

void high_to_low(){
	LED_On(0);
	Delay(DELAY_TICKS);
	LED_Off(0);
	Delay(DELAY_TICKS);
}


void manchester_encode(char ch){
	printf("%c:", ch);
	for(int i=7; i >=0; i--){
		int bit = (ch>>i) &0x01; 
		if(bit==0){      //0:low-high
			low_to_high();
			printf("_|-");
		}
		else{            //1:high-low
			high_to_low();
			printf("-|_");
		}
	}
}

void differential_manchester_encode(char ch){
	int state;
	printf("%c:", ch);
	// first confirm the inital state depending on the first bit
	int bit = (ch>>7) &0x01; 
	if(bit==0){
		low_to_high();
		state=1;
		printf("_|-");
	}
	else{
		high_to_low();
		state = 0;
		printf("-|_");
	}
	//then encode the rest of the bits
	for(int i=6; i >=0; i--){
		bit = (ch>>i) &0x01; 
		if(bit==0){      // current bit is 0
			if(state==0){
				high_to_low();
				printf("|-|_");
			}
			else{
				low_to_high();
				printf("|_|-");
			}
		}
		else{            // current bit is 1
			if(state==0){
				low_to_high();
				state = 1;
				printf("_|-");
			}
			else{
				high_to_low();
				state = 0;
				printf("-|_");
			}
		}
	}
}


void str_encode(char *str){
	while(*str){
		//manchester_encode(*str++);
		differential_manchester_encode(*str++);
		printf("\n");
	}
}

int main(){
	SystemCoreClockUpdate();
	LED_Initialize();
	SysTick_Config(SystemCoreClock / 1000);
	while(1){
		str_encode("Hello, World!\r\n");
	}
}
