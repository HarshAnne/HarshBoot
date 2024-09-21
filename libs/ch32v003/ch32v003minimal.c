#include <ch32v003harsh.h>

// #include <stdio.h>
// #include <string.h>
// #include <stdarg.h>
#include <limits.h>
#include <stdint.h>

#define WEAK __attribute__((weak))

WEAK int errno;

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

WEAK size_t strlen(const char *s) { const char *a = s;for (; *s; s++);return s-a; }
WEAK void *memset(void *dest, int c, size_t n) { unsigned char *s = dest; for (; n; n--, s++) *s = c; return dest; }

WEAK void *memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;
	for (; n; n--) *d++ = *s++;
	return dest;
}

WEAK int memcmp(const void *vl, const void *vr, size_t n)
{
	const unsigned char *l=vl, *r=vr;
	for (; n && *l == *r; n--, l++, r++);
	return n ? *l-*r : 0;
}

WEAK int puts(const char *s)
{
	int sl = strlen( s );
	_write(0, s, sl );
	_write(0, "\n", 1 );
	return sl + 1;
}

/*
	C version of CH32V003 Startup .s file from WCH
	This file is public domain where possible or the following where not:
	Copyright 2023 Charles Lohr, under the MIT-x11 or NewBSD licenses, you choose.
*/

// #ifdef CPLUSPLUS
// // Method to call the C++ constructors
// void __libc_init_array(void);
// #endif

int main() __attribute__((used));
void SystemInit( void ) __attribute__((used));

extern uint32_t * _sbss;
extern uint32_t * _ebss;
extern uint32_t * _data_lma;
extern uint32_t * _data_vma;
extern uint32_t * _edata;


// If you don't override a specific handler, it will just spin forever.
void DefaultIRQHandler( void )
{
	// Infinite Loop
#if defined( DEBUG )
	printf( "DefaultIRQHandler MSTATUS:%08x MTVAL:%08x MCAUSE:%08x MEPC:%08x\n", (int)__get_MSTATUS(), (int)__get_MTVAL(), (int)__get_MCAUSE(), (int)__get_MEPC() );
#endif
	asm volatile( "1: j 1b" );
}

// This makes it so that all of the interrupt handlers just alias to
// DefaultIRQHandler unless they are individually overridden.

#if defined(FUNCONF_USE_CLK_SEC) && FUNCONF_USE_CLK_SEC
/**
 * @brief 	Non Maskabke Interrupt handler
 * 			Invoked when the Clock Security system
 * 			detects the failure of the HSE oscilator.
 * 			The sys clock is switched to HSI.
 * 			Clears the CSSF flag in RCC->INTR
 */
void NMI_RCC_CSS_IRQHandler( void )
{
	RCC->INTR |= RCC_CSSC;	// clear the clock security int flag
}

void NMI_Handler( void ) 				 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("NMI_RCC_CSS_IRQHandler"))) __attribute__((used));
#else
void NMI_Handler( void )                 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif
void HardFault_Handler( void )           __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#if defined(CH32V20x) || defined(CH32V30x) || defined(CH32X03x)
void Ecall_M_Mode_Handler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void Ecall_U_Mode_Handler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void Break_Point_Handler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif // defined(CH32V20x) || defined(CH32V30x)
void SysTick_Handler( void )             __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void SW_Handler( void )                  __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void WWDG_IRQHandler( void )             __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void PVD_IRQHandler( void )              __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#if defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
void TAMPER_IRQHandler( void )			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void RTC_IRQHandler( void )				 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif // defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
void FLASH_IRQHandler( void )            __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void RCC_IRQHandler( void )              __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#if defined(CH32V003) || defined(CH32X03x)
void EXTI7_0_IRQHandler( void )          __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void AWU_IRQHandler( void )              __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#elif defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
void EXTI0_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void EXTI1_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void EXTI2_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void EXTI3_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void EXTI4_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif
void DMA1_Channel1_IRQHandler( void )    __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA1_Channel2_IRQHandler( void )    __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA1_Channel3_IRQHandler( void )    __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA1_Channel4_IRQHandler( void )    __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA1_Channel5_IRQHandler( void )    __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA1_Channel6_IRQHandler( void )    __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA1_Channel7_IRQHandler( void )    __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#if defined( CH32V003 ) || defined(CH32X03x)
void ADC1_IRQHandler( void )             __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#elif defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
void ADC1_2_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif
#if defined(CH32V20x) || defined(CH32V30x)
void USB_HP_CAN1_TX_IRQHandler( void ) 	 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USB_LP_CAN1_RX0_IRQHandler( void )  __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void CAN1_RX1_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void CAN1_SCE_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif // defined(CH32V20x) || defined(CH32V30x)
#if defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
void EXTI9_5_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif // defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
void TIM1_BRK_IRQHandler( void )         __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM1_UP_IRQHandler( void )          __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM1_TRG_COM_IRQHandler( void )     __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM1_CC_IRQHandler( void )          __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM2_IRQHandler( void )             __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#if defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
void TIM3_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM4_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif // defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
void I2C1_EV_IRQHandler( void )          __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void I2C1_ER_IRQHandler( void )          __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#if defined( CH32V003 ) || defined( CH32X03x )
void USART1_IRQHandler( void )           __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void SPI1_IRQHandler( void )             __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#elif defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
void I2C2_EV_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void I2C2_ER_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void SPI1_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void SPI2_IRQHandler( void )			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USART1_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USART2_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USART3_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void EXTI15_10_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void RTCAlarm_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USBWakeUp_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif
#if defined(CH32V10x) || defined(CH32V20x)
void USBHD_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif // defined(CH32V10x) || defined(CH32V20x)
#if defined(CH32V20x) || defined(CH32V30x)
void USBHDWakeUp_IRQHandler( void ) 	 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void ETH_IRQHandler( void ) 			 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void ETHWakeUp_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void OSC32KCal_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void OSCWakeUp_IRQHandler( void ) 		 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA1_Channel8_IRQHandler( void ) 	 __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
// This appears to be masked to USBHD
void TIM8_BRK_IRQHandler( void ) 		__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM8_UP_IRQHandler( void ) 		__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM8_TRG_COM_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM8_CC_IRQHandler( void )			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void RNG_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void FSMC_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void SDIO_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM5_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void SPI3_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void UART4_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void UART5_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM6_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM7_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel1_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel2_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel3_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel4_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel5_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void OTG_FS_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USBHSWakeup_IRQHandler( void )		__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USBHS_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DVP_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void UART6_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void UART7_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void UART8_IRQHandler( void ) 			__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM9_BRK_IRQHandler( void ) 		__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM9_UP_IRQHandler( void ) 		__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM9_TRG_COM_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM9_CC_IRQHandler( void ) 		__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM10_BRK_IRQHandler( void ) 		__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM10_UP_IRQHandler( void ) 		__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM10_TRG_COM_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM10_CC_IRQHandler( void ) 		__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel6_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel7_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel8_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel9_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel10_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA2_Channel11_IRQHandler( void ) 	__attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif

#if defined( CH32X03x)
void USART2_IRQHandler( void )        __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void EXTI15_8_IRQHandler( void )      __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void EXTI25_16_IRQHandler( void )     __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USART3_IRQHandler( void ) 		  __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USART4_IRQHandler( void ) 		  __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void DMA1_Channel8_IRQHandler( void ) __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USBFS_IRQHandler( void )         __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USBFS_WakeUp_IRQHandler( void )  __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void PIOC_IRQHandler( void )          __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void OPA_IRQHandler( void )           __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USBPD_IRQHandler( void )         __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void USBPD_WKUP_IRQHandler( void )    __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM2_CC_IRQHandler( void )       __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM2_TRG_IRQHandler( void )      __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM2_BRK_IRQHandler( void )      __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
void TIM3_IRQHandler( void )          __attribute__((section(".text.vector_handler"))) __attribute((weak,alias("DefaultIRQHandler"))) __attribute__((used));
#endif

void InterruptVector()         __attribute__((naked)) __attribute((section(".init"))) __attribute((weak,alias("InterruptVectorDefault"))) __attribute((naked));
void InterruptVectorDefault()  __attribute__((naked)) __attribute((section(".init"))) __attribute((naked));
void handle_reset( void ) __attribute__((section(".text.handle_reset")));

#if defined( CH32V003 ) || defined( CH32X03x )

void InterruptVectorDefault()
{
	asm volatile( "\n\
	.align  2\n\
	.option   push;\n\
	.option   norvc;\n\
	j handle_reset\n"
#if !defined(FUNCONF_TINYVECTOR)
"	.word   0\n\
	.word   NMI_Handler               /* NMI Handler */                    \n\
	.word   HardFault_Handler         /* Hard Fault Handler */             \n\
	.word   0\n"
#if defined(CH32X03x)
"	.word   Ecall_M_Mode_Handler       /* Ecall M Mode */ \n\
	.word   0 \n\
	.word   0 \n\
	.word   Ecall_U_Mode_Handler       /* Ecall U Mode */ \n\
	.word   Break_Point_Handler        /* Break Point */ \n\
"
#else
"	.word   0\n\
	.word   0\n\
	.word   0\n\
	.word   0\n\
	.word   0\n"
#endif
"	.word   0\n\
	.word   0\n\
	.word   SysTick_Handler           /* SysTick Handler */                \n\
	.word   0\n\
	.word   SW_Handler                /* SW Handler */                     \n\
	.word   0\n\
	/* External Interrupts */                                              \n\
	.word   WWDG_IRQHandler           /* Window Watchdog */                \n\
	.word   PVD_IRQHandler            /* PVD through EXTI Line detect */   \n\
	.word   FLASH_IRQHandler          /* Flash */                          \n\
	.word   RCC_IRQHandler            /* RCC */                            \n\
	.word   EXTI7_0_IRQHandler        /* EXTI Line 7..0 */                 \n\
	.word   AWU_IRQHandler            /* AWU */                            \n\
	.word   DMA1_Channel1_IRQHandler  /* DMA1 Channel 1 */                 \n\
	.word   DMA1_Channel2_IRQHandler  /* DMA1 Channel 2 */                 \n\
	.word   DMA1_Channel3_IRQHandler  /* DMA1 Channel 3 */                 \n\
	.word   DMA1_Channel4_IRQHandler  /* DMA1 Channel 4 */                 \n\
	.word   DMA1_Channel5_IRQHandler  /* DMA1 Channel 5 */                 \n\
	.word   DMA1_Channel6_IRQHandler  /* DMA1 Channel 6 */                 \n\
	.word   DMA1_Channel7_IRQHandler  /* DMA1 Channel 7 */                 \n\
	.word   ADC1_IRQHandler           /* ADC1 */                           \n\
	.word   I2C1_EV_IRQHandler        /* I2C1 Event */                     \n\
	.word   I2C1_ER_IRQHandler        /* I2C1 Error */                     \n\
	.word   USART1_IRQHandler         /* USART1 */                         \n\
	.word   SPI1_IRQHandler           /* SPI1 */                           \n\
	.word   TIM1_BRK_IRQHandler       /* TIM1 Break */                     \n\
	.word   TIM1_UP_IRQHandler        /* TIM1 Update */                    \n\
	.word   TIM1_TRG_COM_IRQHandler   /* TIM1 Trigger and Commutation */   \n\
	.word   TIM1_CC_IRQHandler        /* TIM1 Capture Compare */           \n\
	.word   TIM2_IRQHandler           /* TIM2 */                           \n"
#if defined( CH32X03x )
"	.word	USART2_IRQHandler         /* UART2 Interrupt                          */ \n\
	.word	EXTI15_8_IRQHandler       /* External Line[8:15] Interrupt            */ \n\
	.word	EXTI25_16_IRQHandler      /* External Line[25:16] Interrupt           */ \n\
	.word	USART3_IRQHandler         /* UART2 Interrupt                          */ \n\
	.word	USART4_IRQHandler         /* UART2 Interrupt                          */ \n\
	.word	DMA1_Channel8_IRQHandler  /* DMA1 Channel 8 global Interrupt          */ \n\
	.word	USBFS_IRQHandler          /* USB Full-Speed Interrupt                 */ \n\
	.word	USBFS_WakeUp_IRQHandler   /* USB Full-Speed Wake-Up Interrupt         */ \n\
	.word	PIOC_IRQHandler           /* Programmable IO Controller Interrupt     */ \n\
	.word	OPA_IRQHandler            /* Op Amp Interrupt                         */ \n\
	.word	USBPD_IRQHandler          /* USB Power Delivery Interrupt             */ \n\
	.word	USBPD_WKUP_IRQHandler     /* USB Power Delivery Wake-Up Interrupt     */ \n\
	.word	TIM2_CC_IRQHandler        /* Timer 2 Compare Global Interrupt         */ \n\
	.word	TIM2_TRG_IRQHandler       /* Timer 2 Trigger Global Interrupt         */ \n\
	.word	TIM2_BRK_IRQHandler       /* Timer 2 Brk Global Interrupt             */ \n\
	.word	TIM3_IRQHandler           /* Timer 3 Global Interrupt                 */"
#endif
#endif
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
#ifdef CPLUSPLUS
	// Call __libc_init_array function
"	call %0 \n\t"
: : "i" (__libc_init_array) 
: "a0", "a1", "a2", "a3", "a4", "a5", "t0", "t1", "t2", "memory"
#else
: : : "a0", "a1", "a2", "a3", "memory"
#endif
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

#elif defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)

void InterruptVectorDefault()
{
	asm volatile( "\n\
	.align	1 \n\
	.option norvc; \n\
	j handle_reset \n"
#if !defined(FUNCONF_TINYVECTOR) || !FUNCONF_TINYVECTOR
"	.word   0 \n\
	.word   NMI_Handler                /* NMI */ \n\
	.word   HardFault_Handler          /* Hard Fault */ \n\
	.word   0 \n"
#if !defined(CH32V10x)
"	.word   Ecall_M_Mode_Handler       /* Ecall M Mode */ \n\
	.word   0 \n\
	.word   0 \n\
	.word   Ecall_U_Mode_Handler       /* Ecall U Mode */ \n\
	.word   Break_Point_Handler        /* Break Point */ \n\
	.word   0 \n\
	.word   0 \n"
#else
"	.word   0 \n\
	.word   0 \n\
	.word   0 \n\
	.word   0 \n\
	.word   0 \n\
	.word   0 \n\
	.word   0 \n"
#endif
"	.word   SysTick_Handler            /* SysTick = 12 */ \n\
	.word   0 \n\
	.word   SW_Handler                 /* SW = 14 */ \n\
	.word   0 \n\
	/* External Interrupts */ \n\
	.word   WWDG_IRQHandler            /* 16: Window Watchdog */ \n\
	.word   PVD_IRQHandler             /* 17: PVD through EXTI Line detect */ \n\
	.word   TAMPER_IRQHandler          /* 18: TAMPER */ \n\
	.word   RTC_IRQHandler             /* 19: RTC */ \n\
	.word   FLASH_IRQHandler           /* 20: Flash */ \n\
	.word   RCC_IRQHandler             /* 21: RCC */ \n\
	.word   EXTI0_IRQHandler           /* 22: EXTI Line 0 */ \n\
	.word   EXTI1_IRQHandler           /* 23: EXTI Line 1 */ \n\
	.word   EXTI2_IRQHandler           /* 24: EXTI Line 2 */ \n\
	.word   EXTI3_IRQHandler           /* 25: EXTI Line 3 */ \n\
	.word   EXTI4_IRQHandler           /* 26: EXTI Line 4 */ \n\
	.word   DMA1_Channel1_IRQHandler   /* 27: DMA1 Channel 1 */ \n\
	.word   DMA1_Channel2_IRQHandler   /* 28: DMA1 Channel 2 */ \n\
	.word   DMA1_Channel3_IRQHandler   /* 29: DMA1 Channel 3 */ \n\
	.word   DMA1_Channel4_IRQHandler   /* 30: DMA1 Channel 4 */ \n\
	.word   DMA1_Channel5_IRQHandler   /* 31: DMA1 Channel 5 */ \n\
	.word   DMA1_Channel6_IRQHandler   /* 32: DMA1 Channel 6 */ \n\
	.word   DMA1_Channel7_IRQHandler   /* 33: DMA1 Channel 7 */ \n\
	.word   ADC1_2_IRQHandler          /* 34: ADC1_2 */ \n"
#if defined(CH32V20x) || defined(CH32V30x)
"	.word   USB_HP_CAN1_TX_IRQHandler  /* 35: USB HP and CAN1 TX */ \n\
	.word   USB_LP_CAN1_RX0_IRQHandler /* 36: USB LP and CAN1RX0 */ \n\
	.word   CAN1_RX1_IRQHandler        /* 37: CAN1 RX1 */ \n\
	.word   CAN1_SCE_IRQHandler        /* 38: CAN1 SCE */ \n"
#else
"	.word   0 \n\
	.word   0 \n\
	.word   0 \n\
	.word   0 \n"
#endif
"	.word   EXTI9_5_IRQHandler         /* 39: EXTI Line 9..5 */ \n\
	.word   TIM1_BRK_IRQHandler        /* 40: TIM1 Break */ \n\
	.word   TIM1_UP_IRQHandler         /* 41: TIM1 Update */ \n\
	.word   TIM1_TRG_COM_IRQHandler    /* 42: TIM1 Trigger and Commutation */ \n\
	.word   TIM1_CC_IRQHandler         /* 43: TIM1 Capture Compare */ \n\
	.word   TIM2_IRQHandler            /* 44: TIM2 */ \n\
	.word   TIM3_IRQHandler            /* 45: TIM3 */ \n\
	.word   TIM4_IRQHandler            /* 46: TIM4 */ \n\
	.word   I2C1_EV_IRQHandler         /* 47: I2C1 Event */ \n\
	.word   I2C1_ER_IRQHandler         /* 48: I2C1 Error */ \n\
	.word   I2C2_EV_IRQHandler         /* 49: I2C2 Event */ \n\
	.word   I2C2_ER_IRQHandler         /* 50: I2C2 Error */ \n\
	.word   SPI1_IRQHandler            /* 51: SPI1 */ \n\
	.word   SPI2_IRQHandler            /* 52: SPI2 */ \n\
	.word   USART1_IRQHandler          /* 53: USART1 */ \n\
	.word   USART2_IRQHandler          /* 54: USART2 */ \n\
	.word   USART3_IRQHandler          /* 55: USART3 */ \n\
	.word   EXTI15_10_IRQHandler       /* 56: EXTI Line 15..10 */ \n\
	.word   RTCAlarm_IRQHandler        /* 57: RTC Alarm through EXTI Line */ \n"
#if defined(CH32V10x) || defined(CH32V20x) || defined(CH32V30x)
"	.word   USBWakeUp_IRQHandler       /* 58: USB Wake up from suspend */ \n"
#if defined(CH32V20x) || defined(CH32V30x)

#if defined(CH32V30x)
"   .word   TIM8_BRK_IRQHandler        /* 59: TIM8 Break == masked to USBHD? */ \n"
#else
"	.word   USBHD_IRQHandler           /* 59: USBHD Break */ \n"
#endif
"   .word   TIM8_UP_IRQHandler         /* 60: TIM8 Update */ \n\
    .word   TIM8_TRG_COM_IRQHandler    /* 61: TIM8 Trigger and Commutation */ \n\
    .word   TIM8_CC_IRQHandler         /* 62: TIM8 Capture Compare */ \n\
    .word   RNG_IRQHandler             /* 63: RNG */ \n\
    .word   FSMC_IRQHandler            /* 64: FSMC */ \n\
    .word   SDIO_IRQHandler            /* 65: SDIO */ \n\
    .word   TIM5_IRQHandler            /* 66: TIM5 */ \n\
    .word   SPI3_IRQHandler            /* 67: SPI3 */ \n\
    .word   UART4_IRQHandler           /* 68: UART4 */ \n\
    .word   UART5_IRQHandler           /* 59: UART5 */ \n\
    .word   TIM6_IRQHandler            /* 70: TIM6 */ \n\
    .word   TIM7_IRQHandler            /* 71: TIM7 */ \n\
    .word   DMA2_Channel1_IRQHandler   /* 72: DMA2 Channel 1 */ \n\
    .word   DMA2_Channel2_IRQHandler   /* 73: DMA2 Channel 2 */ \n\
    .word   DMA2_Channel3_IRQHandler   /* 74: DMA2 Channel 3 */ \n\
    .word   DMA2_Channel4_IRQHandler   /* 75: DMA2 Channel 4 */ \n\
    .word   DMA2_Channel5_IRQHandler   /* 76: DMA2 Channel 5 */ \n\
	.word   ETH_IRQHandler             /* 77: ETH global */ \n\
	.word   ETHWakeUp_IRQHandler       /* 78: ETH Wake up */ \n\
    .word   0                          /* 79: CAN2_TX */ \n\
    .word   0                          /* 80: CAN2_RX0 */ \n\
    .word   0                          /* 81: CAN2_RX1 */ \n\
    .word   0                          /* 82: CAN2_SCE */ \n\
    .word   OTG_FS_IRQHandler          /* 83: OTGFS */ \n"
#if defined(CH32V30x)
"   .word   USBHSWakeup_IRQHandler     /* 84: USBHsWakeUp */ \n\
    .word   USBHS_IRQHandler           /* 85: USBHS */ \n\
    .word   DVP_IRQHandler             /* 86: DVP */ \n"
#else
"   .word   0                          /* 84: USBHsWakeUp */ \n\
    .word   0                          /* 85: USBHS */ \n\
    .word   0                          /* 86: DVP */ \n"
#endif
"   .word   UART6_IRQHandler           /* 87: UART6 */ \n\
    .word   UART7_IRQHandler           /* 88: UART7 */ \n\
    .word   UART8_IRQHandler           /* 89: UART8 */ \n\
    .word   TIM9_BRK_IRQHandler        /* 90: TIM9 Break */ \n\
    .word   TIM9_UP_IRQHandler         /* 91: TIM9 Update */ \n\
    .word   TIM9_TRG_COM_IRQHandler    /* 92: TIM9 Trigger and Commutation */ \n\
    .word   TIM9_CC_IRQHandler         /* 93: TIM9 Capture Compare */ \n\
    .word   TIM10_BRK_IRQHandler       /* 94: TIM10 Break */ \n\
    .word   TIM10_UP_IRQHandler        /* 95: TIM10 Update */ \n\
    .word   TIM10_TRG_COM_IRQHandler   /* 96: TIM10 Trigger and Commutation */ \n\
    .word   TIM10_CC_IRQHandler        /* 97: TIM10 Capture Compare */ \n\
    .word   DMA2_Channel6_IRQHandler   /* 98: DMA2 Channel 6 */ \n\
    .word   DMA2_Channel7_IRQHandler   /* 99: DMA2 Channel 7 */ \n\
    .word   DMA2_Channel8_IRQHandler   /* 100: DMA2 Channel 8 */ \n\
    .word   DMA2_Channel9_IRQHandler   /* 101: DMA2 Channel 9 */ \n\
    .word   DMA2_Channel10_IRQHandler  /* 102: DMA2 Channel 10 */ \n\
    .word   DMA2_Channel11_IRQHandler  /* 103: DMA2 Channel 11 */ \n"
#endif
#endif

#endif // !defined(FUNCONF_TINYVECTOR) || !FUNCONF_TINYVECTOR
"	.option rvc; \n");

}

void handle_reset( void )
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
			);

	// Careful: Use registers to prevent overwriting of self-data.
	// This clears out BSS.
	asm volatile(
"	la a0, _sbss\n\
	la a1, _ebss\n\
	bgeu a0, a1, 2f\n\
1:	sw zero, 0(a0)\n\
	addi a0, a0, 4\n\
	bltu a0, a1, 1b\n\
2:"
	// This loads DATA from FLASH to RAM.
"	la a0, _data_lma\n\
	la a1, _data_vma\n\
	la a2, _edata\n\
	beq a1, a2, 2f\n\
1:	lw t0, 0(a0)\n\
	sw t0, 0(a1)\n\
	addi a0, a0, 4\n\
	addi a1, a1, 4\n\
	bltu a1, a2, 1b\n\
2:\n"
#ifdef CPLUSPLUS
	// Call __libc_init_array function
"	call %0 \n\t"
: : "i" (__libc_init_array)
#else
: :
#endif
: "a0", "a1", "a2", "a3", "memory"
);

	// Setup the interrupt vector, processor status and INTSYSCR.
	asm volatile(
"	li t0, 0x1f\n\
	csrw 0xbc0, t0\n"

#if defined(CH32V30x) && !defined( DISABLED_FLOAT )
"	li t0, 0x6088\n\
	csrs mstatus, t0\n"
#else
"	li t0, 0x88\n\
	csrs mstatus, t0\n"
#endif

#if FUNCONF_ENABLE_HPE	// Enabled nested and hardware (HPE) stack, since it's really good on the x035.
"	li t0, 0x0b\n\
	csrw 0x804, t0\n"
#endif
"	la t0, InterruptVector\n\
	ori t0, t0, 3\n\
	csrw mtvec, t0\n"
	: : "InterruptVector" (InterruptVector) : "t0", "memory"
	);

#if defined( FUNCONF_SYSTICK_USE_HCLK ) && FUNCONF_SYSTICK_USE_HCLK && !defined(CH32V10x)
	SysTick->CTLR = 5;
#else
	SysTick->CTLR = 1;
#endif

	// set mepc to be main as the root app.
	asm volatile(
"	csrw mepc, %[main]\n"
"	mret\n" : : [main]"r"(main) );
}

#endif

#if defined( FUNCONF_USE_UARTPRINTF ) && FUNCONF_USE_UARTPRINTF
void SetupUART( int uartBRR )
{
#ifdef CH32V003
	// Enable GPIOD and UART.
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1;

	// Push-Pull, 10MHz Output, GPIO D5, with AutoFunction
	GPIOD->CFGLR &= ~(0xf<<(4*5));
	GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*5);
#elif defined(CH32X03x)
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1;

	// Push-Pull, 10MHz Output, GPIO A9, with AutoFunction
	GPIOB->CFGHR &= ~(0xf<<(4*2));
	GPIOB->CFGHR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*2);
#else
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1;

	// Push-Pull, 10MHz Output, GPIO A9, with AutoFunction
	GPIOA->CFGHR &= ~(0xf<<(4*1));
	GPIOA->CFGHR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*1);
#endif

	// 115200, 8n1.  Note if you don't specify a mode, UART remains off even when UE_Set.
	USART1->CTLR1 = USART_WordLength_8b | USART_Parity_No | USART_Mode_Tx;
	USART1->CTLR2 = USART_StopBits_1;
	USART1->CTLR3 = USART_HardwareFlowControl_None;

	USART1->BRR = uartBRR;
	USART1->CTLR1 |= CTLR1_UE_Set;
}

// For debug writing to the UART.
WEAK int _write(int fd, const char *buf, int size)
{
	for(int i = 0; i < size; i++){
	    while( !(USART1->STATR & USART_FLAG_TC));
	    USART1->DATAR = *buf++;
	}
	return size;
}

// single char to UART
WEAK int putchar(int c)
{
	while( !(USART1->STATR & USART_FLAG_TC));
	USART1->DATAR = (const char)c;
	return 1;
}
#endif

// #if defined( FUNCONF_USE_DEBUGPRINTF ) && FUNCONF_USE_DEBUGPRINTF


// void handle_debug_input( int numbytes, uint8_t * data ) __attribute__((weak));
// void handle_debug_input( int numbytes, uint8_t * data ) { (void)numbytes; (void)data; }

// static void internal_handle_input( volatile uint32_t * dmdata0 )
// {
// 	uint32_t dmd0 = *dmdata0;
// 	int bytes = (dmd0 & 0x3f) - 4;
// 	if( bytes > 0 )
// 	{
// 		handle_debug_input( bytes, ((uint8_t*)dmdata0) + 1 );
// 	}
// }


// void poll_input()
// {
// 	volatile uint32_t * dmdata0 = (volatile uint32_t *)DMDATA0;
//  	if( ((*dmdata0) & 0x80) == 0 )
// 	{
// 		internal_handle_input( dmdata0 );
// 		// Should be 0x80 or so, but for some reason there's a bug that retriggers.
// 		*dmdata0 = 0x00;
// 	}
// }


// //           MSB .... LSB
// // DMDATA0: char3 char2 char1 [status word]
// // where [status word] is:
// //   b7 = is a "printf" waiting?
// //   b0..b3 = # of bytes in printf (+4).  (5 or higher indicates a print of some kind)
// //     note: if b7 is 0 in reply, but b0..b3 have >=4 then we received data from host.
// // declare as weak to allow overriding.
// WEAK int _write(int fd, const char *buf, int size)
// {
// 	(void)fd;

// 	char buffer[4] = { 0 };
// 	int place = 0;
// 	uint32_t lastdmd;
// 	uint32_t timeout = FUNCONF_DEBUGPRINTF_TIMEOUT; // Give up after ~40ms

// 	if( size == 0 )
// 	{
// 		lastdmd = (*DMDATA0);
// 		if( lastdmd && !(lastdmd&0x80) ) internal_handle_input( (uint32_t*)DMDATA0 );
// 	}
// 	while( place < size )
// 	{
// 		int tosend = size - place;
// 		if( tosend > 7 ) tosend = 7;

// 		while( ( lastdmd = (*DMDATA0) ) & 0x80 )
// 			if( timeout-- == 0 ) return place;

// 		if( lastdmd ) internal_handle_input( (uint32_t*)DMDATA0 );

// 		timeout = FUNCONF_DEBUGPRINTF_TIMEOUT;

// 		int t = 3;
// 		while( t < tosend )
// 		{
// 			buffer[t-3] = buf[t+place];
// 			t++;
// 		}
// 		*DMDATA1 = *(uint32_t*)&(buffer[0]);
// 		t = 0;
// 		while( t < tosend && t < 3 )
// 		{
// 			buffer[t+1] = buf[t+place];
// 			t++;
// 		}
// 		buffer[0] = 0x80 | (tosend + 4);
// 		*DMDATA0 = *(uint32_t*)&(buffer[0]);

// 		//buf += tosend;
// 		place += tosend;
// 	}
// 	return size;
// }

// // single to debug intf
// WEAK int putchar(int c)
// {
// 	int timeout = FUNCONF_DEBUGPRINTF_TIMEOUT;
// 	uint32_t lastdmd = 0;

// 	while( ( lastdmd = (*DMDATA0) ) & 0x80 )
// 		if( timeout-- == 0 ) return 0;

// 	// Simply seeking input.
// 	if( lastdmd ) internal_handle_input( (uint32_t*)DMDATA0 );
// 	*DMDATA0 = 0x85 | ((const char)c<<8);
// 	return 1;
// }

// void SetupDebugPrintf()
// {
// 	// Clear out the sending flag.
// 	*DMDATA1 = 0x0;
// 	*DMDATA0 = 0x80;
// }

// void WaitForDebuggerToAttach()
// {
// 	while( ((*DMDATA0) & 0x80) );
// }

// #endif

// #if (defined( FUNCONF_USE_DEBUGPRINTF ) && !FUNCONF_USE_DEBUGPRINTF) && \
//     (defined( FUNCONF_USE_UARTPRINTF ) && !FUNCONF_USE_UARTPRINTF) && \
//     (defined( FUNCONF_NULL_PRINTF ) && FUNCONF_NULL_PRINTF)

// WEAK int _write(int fd, const char *buf, int size)
// {
// 	return size;
// }

// // single to debug intf
// WEAK int putchar(int c)
// {
// 	return 1;
// }
// #endif

void DelaySysTick( uint32_t n )
{
#ifdef CH32V003
	uint32_t targend = SysTick->CNT + n;
	while( ((int32_t)( SysTick->CNT - targend )) < 0 );
#elif defined(CH32V20x) || defined(CH32V30x)
	uint64_t targend = SysTick->CNT + n;
	while( ((int64_t)( SysTick->CNT - targend )) < 0 );
#elif defined(CH32V10x) || defined(CH32X03x)
	uint32_t targend = SysTick->CNTL + n;
	while( ((int32_t)( SysTick->CNTL - targend )) < 0 );
#else
	#error DelaySysTick not defined.
#endif
}

void SystemInit()
{
#if defined(CH32V30x) && defined(TARGET_MCU_MEMORY_SPLIT)
	FLASH->OBR = TARGET_MCU_MEMORY_SPLIT<<8;
#endif

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
	#if defined(CH32V003) || defined(CH32X03x)
		#define BASE_CFGR0 RCC_HPRE_DIV1     					  // HCLK = SYSCLK = APB1 And, no pll.
	#else
		#define BASE_CFGR0 RCC_HPRE_DIV1 | RCC_PPRE2_DIV1 | RCC_PPRE1_DIV1
	#endif
#endif

// HSI always ON - needed for the Debug subsystem
#define BASE_CTLR	(((FUNCONF_HSITRIM) << 3) | RCC_HSION | HSEBYP | RCC_CSS)
//#define BASE_CTLR	(((FUNCONF_HSITRIM) << 3) | HSEBYP | RCC_CSS)	// disable HSI in HSE modes

	// CH32V003 flash latency
#if defined(CH32X03x)
	FLASH->ACTLR = FLASH_ACTLR_LATENCY_2;                   // +2 Cycle Latency (Recommended per TRM)
#elif defined(CH32V003)
	#if FUNCONF_SYSTEM_CORE_CLOCK > 25000000
		FLASH->ACTLR = FLASH_ACTLR_LATENCY_1;               // +1 Cycle Latency
	#else
		FLASH->ACTLR = FLASH_ACTLR_LATENCY_0;               // +0 Cycle Latency
	#endif
#endif

#if defined(FUNCONF_USE_HSI) && FUNCONF_USE_HSI
	#if defined(CH32V30x) || defined(CH32V20x) || defined(CH32V10x)
		EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;
	#endif
	#if defined(FUNCONF_USE_PLL) && FUNCONF_USE_PLL
		RCC->CFGR0 = BASE_CFGR0;
		RCC->CTLR  = BASE_CTLR | RCC_HSION | RCC_PLLON; 			// Use HSI, enable PLL.
	#else
		RCC->CFGR0 = RCC_HPRE_DIV1;                               	// PLLCLK = HCLK = SYSCLK = APB1
		RCC->CTLR  = BASE_CTLR | RCC_HSION;     					// Use HSI, Only.
	#endif

#elif defined(FUNCONF_USE_HSE) && FUNCONF_USE_HSE

	#if defined(CH32V003)
		RCC->CTLR = BASE_CTLR | RCC_HSION | RCC_HSEON ;       		  // Keep HSI on while turning on HSE
	#else
		RCC->CTLR = RCC_HSEON;							  			  // Only turn on HSE.
	#endif

	// Values lifted from the EVT.  There is little to no documentation on what this does.
	while(!(RCC->CTLR&RCC_HSERDY));

	#if defined(CH32V003)
		RCC->CFGR0 = RCC_PLLSRC_HSE_Mul2 | RCC_SW_HSE;
	#else
		RCC->CFGR0 = BASE_CFGR0 | RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE;
	#endif

	#if defined(FUNCONF_USE_PLL) && FUNCONF_USE_PLL
		RCC->CTLR  = BASE_CTLR | RCC_HSEON | RCC_PLLON;            // Turn off HSI.
	#else
		RCC->CTLR  = RCC_HSEON | HSEBYP;                           // Turn off PLL and HSI.
	#endif
#endif

	// CH32V10x flash prefetch buffer
#if defined(CH32V10x)
	// Enable Prefetch Buffer
	FLASH->ACTLR |= FLASH_ACTLR_PRFTBE;
#endif

	// CH32V10x flash latency
#if defined(CH32V10x)
	#if defined(FUNCONF_USE_HSE) && FUNCONF_USE_HSE
		#if !defined(FUNCONF_USE_PLL) || !FUNCONF_USE_PLL
			FLASH->ACTLR = FLASH_ACTLR_LATENCY_0;           // +0 Cycle Latency
		#else
			#if FUNCONF_SYSTEM_CORE_CLOCK < 56000000
				FLASH->ACTLR = FLASH_ACTLR_LATENCY_1;       // +1 Cycle Latency
			#else
				FLASH->ACTLR = FLASH_ACTLR_LATENCY_2;       // +2 Cycle Latency
			#endif
		#endif
	#else
		FLASH->ACTLR = FLASH_ACTLR_LATENCY_1;       		// +1 Cycle Latency
	#endif
#endif

	RCC->INTR  = 0x009F0000;                               // Clear PLL, CSSC, HSE, HSI and LSI ready flags.

#if defined(FUNCONF_USE_PLL) && FUNCONF_USE_PLL
	while((RCC->CTLR & RCC_PLLRDY) == 0);                       	// Wait till PLL is ready
	uint32_t tmp32 = RCC->CFGR0 & ~(0x03);							// clr the SW
	RCC->CFGR0 = tmp32 | RCC_SW_PLL;                       			// Select PLL as system clock source
	while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08); 	// Wait till PLL is used as system clock source
#endif

#if defined( FUNCONF_USE_UARTPRINTF ) && FUNCONF_USE_UARTPRINTF
	SetupUART( UART_BRR );
#endif
#if defined( FUNCONF_USE_DEBUGPRINTF ) && FUNCONF_USE_DEBUGPRINTF
	SetupDebugPrintf();
#endif
}
