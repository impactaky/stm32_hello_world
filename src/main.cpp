/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/

extern "C"{ 
#include "main.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

//Library config for this project!!!!!!!!!!!
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

//SD
#include "stm324xg_eval.h"
} 

#include <cstdio>
#include <array>
// #include <vector>

/** @addtogroup STM32F4-Discovery_Demo 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TESTRESULT_ADDRESS         0x080FFFFC
#define ALLTEST_PASS               0x00000000
#define ALLTEST_FAIL               0x55555555

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

/* Private function prototypes -----------------------------------------------*/

/*---------------------------------------------------------*/
/* Work Area                                               */
/*---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

// SD
extern "C" { 
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
}

void wait_init () {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 84-1;
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
  TIM_ARRPreloadConfig(TIM2, DISABLE); 
  TIM_GenerateEvent(TIM2, TIM_EventSource_CC2);
}

void wait_ms (unsigned int time) {
	unsigned int i;
  TIM2->CNT = 0;
  TIM2->ARR = 84000-1;
  TIM_Cmd(TIM2, ENABLE);
  TIM_ClearFlag(TIM2, TIM_IT_Update);
  for (i = 0; i < time; ++i) {
    while (TIM_GetFlagStatus(TIM2, TIM_IT_Update) == RESET);
    TIM_ClearFlag(TIM2, TIM_IT_Update);
  }
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

  /* Initialize LEDs and User_Button on STM32F4-Discovery --------------------*/

  USBD_Init(&USB_OTG_dev,     
            USB_OTG_FS_CORE_ID, 
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);
  
  // us wait
  if (SysTick_Config(SystemCoreClock / 100))
  { 
    /* Capture error */ 
    while (1) VCP_put_char('e');
  }
	NVIC_SetPriority(SysTick_IRQn, 0x0);	

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  wait_init();

  wait_ms(5000);

  // Interrupt Config
  NVIC_Configuration();

	printf("Hello world!\n\r");
  wait_ms(5000);

  return 0;
}


extern "C"{

/**
  * @brief  Configures SDIO IRQ channel.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
}

/**
  * @brief  This function handles the test program fail.
  * @param  None
  * @retval None
  */
void Fail_Handler(void)
{
  /* Erase last sector */ 
  FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
  /* Write FAIL code at last word in the flash memory */
  FLASH_ProgramWord(TESTRESULT_ADDRESS, ALLTEST_FAIL);
  
  while(1)
  {
    /* Toggle Red LED */
    /* STM_EVAL_LEDToggle(LED5); */
    wait_ms(5);
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  uint8_t itoa_line[6];
  VCP_send_str((uint8_t *)"Error in ");
  // VCP_send_str(file);
  VCP_send_str((uint8_t *)" at ");
  for(int digit = 4; digit>=0; --digit) {
    itoa_line[digit] = line%10+'0';
    line /= 10;
  }
  VCP_send_str(itoa_line);
  VCP_send_str((uint8_t *)"\n\r");
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

}
