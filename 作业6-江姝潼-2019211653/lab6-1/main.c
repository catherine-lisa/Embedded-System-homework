#define ATOMIC_BIT_IO 1

#define RCC_APB2ENR (*((volatile unsigned long*)(0x40021000 + 0x18)))

#define LED_PIN	7   //PA.7
#define GPIOA_CRL  (*((volatile unsigned long*)(0x40010800 + 0x00)))
#define GPIOA_ODR  (*((volatile unsigned long*)(0x40010800 + 0x0C)))
#define GPIOA_BSRR (*((volatile unsigned long*)(0x40010800 + 0x10)))
#define GPIOA_BRR  (*((volatile unsigned long*)(0x40010800 + 0x14)))

#define BUTTON_PIN	3	//PC.3
#define GPIOC_CRL  (*((volatile unsigned long*)(0x40011000 + 0x00)))
#define GPIOC_IDR  (*((volatile unsigned long*)(0x40011000 + 0x08)))

void Button_Init(void) {
	/* Enable GPIOC clock            */
  RCC_APB2ENR |= (1ul << 4);                
	
	/* Configure Button (PC.3) pins as input */
  GPIOC_CRL &= ~(0xFul << 12);	//Clear Bit[12..15]
  GPIOC_CRL |=  (0x4ul << 12); //Set Bit[12..15] = 0b0100
}

int Button_Read(void) {
	return ((GPIOC_IDR >> BUTTON_PIN) & 1);
}

void LED_Init(void) {
	/* Enable GPIOA clock            */
  RCC_APB2ENR |= (1ul << 2);   
	
	/* Configure LED (PA.7) pins as output */
  GPIOA_CRL &= ~(0xFul << 28); //Clear Bit[28..31]
  GPIOA_CRL |=  ( 0x1ul << 28); //Set Bit[28..31] = 0x0001
}

void LED_Write(int state) {
		//修改使得反方向跳变，将state从1改成0即可
		if(state == 1) {
			GPIOA_BSRR = (1 << LED_PIN) << 16;
		} else {
			GPIOA_BSRR = (1 << LED_PIN);				//on
		}		
}

int main(void) {
	Button_Init();
	LED_Init();
	while(1) {
		LED_Write(Button_Read());
	}
}
