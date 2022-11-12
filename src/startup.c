
#include<stdint.h>

#define SRAM_START  0x20000000U
#define SRAM_SIZE   (64U * 1024U) //128KB
#define SRAM_END    ((SRAM_START) + (SRAM_SIZE))

#define STACK_START   SRAM_END

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _la_data;

extern uint32_t _sbss;
extern uint32_t _ebss;

//prototype of main

int main(void);
void hang(){
    __asm__("b .\n\t");
};


void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);
static void SysTickISR(void);
extern int main(void);

// ISR VECTOR
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((uint32_t)STACK_START),
                                            // The initial stack pointer
    ResetISR,                               // The reset handler
    NmiSR,                                  // The NMI handler
    FaultISR,                               // The hard fault handler
    IntDefaultHandler,                      // The MPU fault handler
    IntDefaultHandler,                      // The bus fault handler
    IntDefaultHandler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,						// SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    IntDefaultHandler,                      // The PendSV handler
    SysTickISR,                             // The SysTick handler
    IntDefaultHandler,                      // IRQ general
};


void NmiSR(void)
{
	while(1);
}

void FaultISR(void)
{
	while(1);
}

void IntDefaultHandler(void)
{
	while(1);
}

void ResetISR(void)
{
	//copy .data section to SRAM
	uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
	
	uint8_t *pDst = (uint8_t*)&_sdata; //sram
	uint8_t *pSrc = (uint8_t*)&_la_data; //flash
	
	for(uint32_t i =0 ; i < size ; i++)
	{
		*pDst++ = *pSrc++;
	}
	
	//Init. the .bss section to zero in SRAM
	size = (uint32_t)&_ebss - (uint32_t)&_sbss;
	pDst = (uint8_t*)&_sbss;
	for(uint32_t i =0 ; i < size ; i++)
	{
		*pDst++ = 0;
	}
	
	main();

    //pending exit
	hang();
}

void SysTickISR(void){

}






