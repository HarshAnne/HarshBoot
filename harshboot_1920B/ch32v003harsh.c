
/*
	C version of CH32V003 Startup .s file from WCH
	This file is public domain where possible or the following where not:
	Copyright 2023 Charles Lohr, under the MIT-x11 or NewBSD licenses, you choose.
*/

#include <ch32v003harsh.h>

#include <limits.h>
#include <stdint.h>

#define WEAK __attribute__((weak))

WEAK int errno;

int main() __attribute__((used));
void SystemInit( void ) __attribute__((used));

extern uint32_t * _sbss;
extern uint32_t * _ebss;
extern uint32_t * _data_lma;
extern uint32_t * _data_vma;
extern uint32_t * _edata;

void InterruptVector()  __attribute__((naked)) __attribute((section(".init"))) __attribute((naked));
void handle_reset( void ) __attribute__((section(".text.handle_reset")));

void InterruptVector()
{
	asm volatile( "\n\
	.align  2\n\
	.option   push;\n\
	.option   norvc;\n\
	j handle_reset\n"
	);
	asm volatile( ".option   pop;\n");
}

void handle_reset()
{
	asm volatile( "\n\
.option push\n\
.option norelax\n\
	la gp, __global_pointer$\n\
.option pop\n\
	la sp, _eusrstack\n"
#if __GNUC__ > 10
".option arch, +zicsr\n"
#endif
	// Setup the interrupt vector, processor status and INTSYSCR.

#if FUNCONF_ENABLE_HPE	// Enabled nested and hardware (HPE) stack, since it's really good on the x035.
"	li t0, 0x88\n\
	csrs mstatus, t0\n"
"	li t0, 0x0b\n\
	csrw 0x804, t0\n"
#else
"	li a0, 0x80\n\
	csrw mstatus, a0\n"
#endif
"	li a3, 0x3\n\
	la a0, InterruptVector\n\
	or a0, a0, a3\n\
	csrw mtvec, a0\n" 
	: : : "a0", "a3", "memory");

	// Careful: Use registers to prevent overwriting of self-data.
	// This clears out BSS.
asm volatile(
"	la a0, _sbss\n\
	la a1, _ebss\n\
	li a2, 0\n\
	bge a0, a1, 2f\n\
1:	sw a2, 0(a0)\n\
	addi a0, a0, 4\n\
	blt a0, a1, 1b\n\
2:"
	// This loads DATA from FLASH to RAM.
"	la a0, _data_lma\n\
	la a1, _data_vma\n\
	la a2, _edata\n\
1:	beq a1, a2, 2f\n\
	lw a3, 0(a0)\n\
	sw a3, 0(a1)\n\
	addi a0, a0, 4\n\
	addi a1, a1, 4\n\
	bne a1, a2, 1b\n\
2:\n"
: : : "a0", "a1", "a2", "a3", "memory"
);

#if defined( FUNCONF_SYSTICK_USE_HCLK ) && FUNCONF_SYSTICK_USE_HCLK
	SysTick->CTLR = 5;
#else
	SysTick->CTLR = 1;
#endif

	// set mepc to be main as the root app.
asm volatile(
"	csrw mepc, %[main]\n"
"	mret\n" : : [main]"r"(main) );
}

void SystemInit()
{
#if FUNCONF_HSE_BYPASS
	#define HSEBYP (1<<18)
#else
	#define HSEBYP 0
#endif

#if defined(FUNCONF_USE_CLK_SEC) && FUNCONF_USE_CLK_SEC
#define RCC_CSS RCC_CSSON									 	// Enable clock security system
#else
#define RCC_CSS 0
#endif

#if defined(FUNCONF_USE_PLL) && FUNCONF_USE_PLL
	#if defined(CH32V003)
		#define BASE_CFGR0 RCC_HPRE_DIV1 | RCC_PLLSRC_HSI_Mul2    // HCLK = SYSCLK = APB1 And, enable PLL
	#else
		#define BASE_CFGR0 RCC_HPRE_DIV1 | RCC_PPRE2_DIV1 | RCC_PPRE1_DIV2 | PLL_MULTIPLICATION
	#endif
#else
	#if defined(CH32V003)
		#define BASE_CFGR0 RCC_HPRE_DIV1     					  // HCLK = SYSCLK = APB1 And, no pll.
	#else
		#define BASE_CFGR0 RCC_HPRE_DIV1 | RCC_PPRE2_DIV1 | RCC_PPRE1_DIV1
	#endif
#endif

// HSI always ON - needed for the Debug subsystem
#define BASE_CTLR	(((FUNCONF_HSITRIM) << 3) | RCC_HSION | HSEBYP | RCC_CSS)
//#define BASE_CTLR	(((FUNCONF_HSITRIM) << 3) | HSEBYP | RCC_CSS)	// disable HSI in HSE modes

#if FUNCONF_SYSTEM_CORE_CLOCK > 25000000
	FLASH->ACTLR = FLASH_ACTLR_LATENCY_1;               // +1 Cycle Latency
#else
	FLASH->ACTLR = FLASH_ACTLR_LATENCY_0;               // +0 Cycle Latency
#endif

#if defined(FUNCONF_USE_HSI) && FUNCONF_USE_HSI
	#if defined(FUNCONF_USE_PLL) && FUNCONF_USE_PLL
		RCC->CFGR0 = BASE_CFGR0;
		RCC->CTLR  = BASE_CTLR | RCC_HSION | RCC_PLLON; 			// Use HSI, enable PLL.
	#else
		RCC->CFGR0 = RCC_HPRE_DIV1;                               	// PLLCLK = HCLK = SYSCLK = APB1
		RCC->CTLR  = BASE_CTLR | RCC_HSION;     					// Use HSI, Only.
	#endif
#endif

	RCC->INTR  = 0x009F0000;                               // Clear PLL, CSSC, HSE, HSI and LSI ready flags.

#if defined(FUNCONF_USE_PLL) && FUNCONF_USE_PLL
	while((RCC->CTLR & RCC_PLLRDY) == 0);                       	// Wait till PLL is ready
	uint32_t tmp32 = RCC->CFGR0 & ~(0x03);							// clr the SW
	RCC->CFGR0 = tmp32 | RCC_SW_PLL;                       			// Select PLL as system clock source
	while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08); 	// Wait till PLL is used as system clock source
#endif
}
