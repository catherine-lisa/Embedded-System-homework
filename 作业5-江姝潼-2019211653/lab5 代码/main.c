#include <stdio.h>
#include <stdint.h>

struct cortex_cm_exception_frame
{
    // software saved
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    // hardware saved
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
};

void core_dump(const struct cortex_cm_exception_frame *frame)
{
    printf("exception frame at %p\n", frame);
    // TODO:print all registers
		printf("r0  0x");
    printf("%08x  ", frame->r0);
    printf("r1  0x");
    printf("%08x  ", frame->r1);
    printf("r2  0x");
    printf("%08x  ", frame->r2);
    printf("r3  0x");
    printf("%08x  ", frame->r3);
    printf("r4  0x");
    printf("%08x\n", frame->r4);
    
    printf("r5  0x");
    printf("%08x  ", frame->r5);
    printf("r6  0x");
    printf("%08x  ", frame->r6);
    printf("r7  0x");
    printf("%08x  ", frame->r7);
    printf("r8  0x");
    printf("%08x  ", frame->r8);
    printf("r9  0x");
    printf("%08x\n", frame->r9);
    
    printf("r10 0x");
    printf("%08x  ", frame->r10);
    printf("r11 0x");
    printf("%08x  ", frame->r11);
    printf("r12 0x");
    printf("%08x\n", frame->r12);

    printf("lr  0x");
    printf("%08x  ", frame->lr);
    printf("pc  0x");
    printf("%08x  ", frame->pc);
    printf("psr 0x");
    printf("%08x\n", frame->psr);
}

__asm void HardFault_Handler(void)
{
    // TODO: save r4-r11, then call core_dump
		PUSH {R11}
		PUSH {R10}
    PUSH {R9}
		PUSH {R8}
		PUSH {R7}
		PUSH {R6}
		PUSH {R5}
		PUSH {R4}
		//pass the parameter of Pointer sp to function 'core_dump'
    MOV R0, SP
    BL core_dump
}

static void trigger_unaligned_mem_access(void)
{
    volatile uint32_t *ccr = (void *)0xE000ED14;
    *ccr |= (1 << 3); // enable trap on unaligned word or halfword accesses
    __asm("isb");
    int a = 1;
    int *pint = (int *)(((char *)&a) + 1);
    *pint = 2;
    printf("trap UNALIGN_TRP");
}

int main(void)
{
    trigger_unaligned_mem_access();
    while (1);
}