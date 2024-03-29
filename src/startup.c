#include "../includes/_common.h"
#include "../testing/tests.h"
#include "../pios-kernel/kernel/kernel.h"
#include "../pios-kernel/kernel/exceptions.h"
#include <stdint.h>

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

EventHandle test_completed_event;

/* Forward declaration of the default fault handlers. */
void Default_Handler            (void) __attribute__((weak));
extern void Reset_Handler       (void) __attribute__((weak));
void HardFault_Handler          (void) __attribute__((weak));

/* Cortex-M4 Processor Exceptions */
extern void NMI_Handler         (void) __attribute__((weak, alias("Default_Handler")));
extern void MemManage_Handler   (void) __attribute__((weak, alias("Default_Handler")));
extern void BusFault_Handler    (void) __attribute__((weak, alias("Default_Handler")));
extern void UsageFault_Handler  (void) __attribute__((weak, alias("Default_Handler")));
extern void SVC_Handler         (void) __attribute__((weak, alias("Default_Handler")));
extern void DebugMon_Handler    (void) __attribute__((weak, alias("Default_Handler")));
extern void PendSV_Handler      (void) __attribute__((weak, alias("Default_Handler")));
extern void SysTick_Handler     (void) __attribute__((weak,alias("Default_Handler")));
extern void IRQGeneralISR      (void) __attribute__((weak,alias("Default_Handler")));

// ISR VECTOR
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((uint32_t)STACK_START),
                                            // The initial stack pointer
    Reset_Handler,                         /* The reset handler         */
    NMI_Handler,                           /* The NMI handler           */
    HardFault_Handler,                     /* The hard fault handler    */
    MemManage_Handler,                     /* The MPU fault handler     */
    BusFault_Handler,                      /* The bus fault handler     */
    UsageFault_Handler,                    /* The usage fault handler   */
    0,                                     /* Reserved                  */
    0,                                     /* Reserved                  */
    0,                                     /* Reserved                  */
    0,                                     /* Reserved                  */
    SVC_Handler,                             /* SVCall handler            */
    DebugMon_Handler,                      /* Debug monitor handler     */
    0,                                     /* Reserved                  */
    PendSV_Handler,                        /* The PendSV handler        */
    SysTick_Handler,                       /* The SysTick handler       */    
    IRQGeneralISR,                         /* IRQ general               */
};

void Default_Handler()
{
	while(1);
}

void HardFault_Handler()
{
    serial_print("HardFault, baby!\n");
	while(1);
}

void Reset_Handler()
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

    // setup SysTick
    SysTick_init(120000); // +- 1ms
    SysTick_enable();
    
    // go to user mode
    __asm__("MRS R0, CONTROL\n\t");
    __asm__("MOV R0, #1\n\t");
    __asm__("MSR CONTROL, R0\n\t");

	kernel_init();

    // The 'test completed' event is created
    test_completed_event = NEW_EVENT(bool);

    create_task((void(*)(void*)) tests_runner, (void*)0, 0, NULL);
    start_scheduler();
    while(1);
}