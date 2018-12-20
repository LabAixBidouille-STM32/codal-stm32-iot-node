#include "CodalDmesg.h"
#include "codal_target_hal.h"
#include "common.h"
#include "hw.h"
#include "stm32.h"
#include "stm32l4xx_it.h"
#include "stm32l4xx_it_ble.h"

RTC_HandleTypeDef hrtc; /**< RTC handler declaration */

/**
 * @brief This function initializes the RTC IP
 * @param  None
 * @retval None
 */
void Init_RTC(void)
{
    __HAL_RCC_LSI_ENABLE(); /**< Enable the LSI clock */

    HAL_PWR_EnableBkUpAccess(); /**< Enable access to the RTC registers */

    /**
     *  Write twice the value to flush the APB-AHB bridge
     *  This bit shall be written in the register before writing the next one
     */
    HAL_PWR_EnableBkUpAccess();

    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSI); /**< Select LSI as RTC Input */

    __HAL_RCC_RTC_ENABLE(); /**< Enable RTC */

    hrtc.Instance = RTC; /**< Define instance */

    /**
     * Bypass the shadow register
     */
    HAL_RTCEx_EnableBypassShadow(&hrtc);

    /**
     * Set the Asynchronous prescaler
     */
    hrtc.Init.AsynchPrediv = CFG_RTC_ASYNCH_PRESCALER;
    hrtc.Init.SynchPrediv = CFG_RTC_SYNCH_PRESCALER;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.HourFormat =
        RTC_HOURFORMAT_24; /**< need to be initialized to not corrupt the RTC_CR register */
    hrtc.Init.OutPutPolarity =
        RTC_OUTPUT_POLARITY_HIGH; /**< need to be initialized to not corrupt the RTC_CR register */
    HAL_RTC_Init(&hrtc);

    __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc); /**< Disable Write Protection */

    LL_RTC_WAKEUP_SetClock(hrtc.Instance, CFG_RTC_WUCKSEL_DIVIDER);

    while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == 0)
        ; /**< Wait for LSI to be stable */

    return;
}

void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

void debug_facility()
{
    /**
     *********************************************************************************************************************
     * START OF SECTION - DEBUG FACILITY
     *
     * This section should be removed in production.
     * The Firmware used the RTC IP to implement the TimerServer. During development, it is common
     *to trigger the nreset to restart the application as this would be done on power ON. However,
     *in that case, the RTC domain is not reseted The following implementation resets the RTC domain
     *so that the device behaves in a similar way on nreset to power ON.
     */

    if (LL_RCC_IsActiveFlag_PINRST() != RESET)
    {
        LL_PWR_ClearFlag_SB();

        HAL_PWR_EnableBkUpAccess(); /**< Enable access to the RTC registers */

        /**
         *  Write twice the value to flush the APB-AHB bridge
         *  This bit shall be written in the register before writing the next one
         */
        HAL_PWR_EnableBkUpAccess();

        LL_RCC_ForceBackupDomainReset();
        __HAL_RCC_CLEAR_RESET_FLAGS();
        LL_RCC_ReleaseBackupDomainReset();
    }
}

void cpu_init()
{
    target_init();

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure LSE Drive Capability
     */
    HAL_PWR_EnableBkUpAccess();

    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART3 |
                                         RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_DFSDM1 |
                                         RCC_PERIPHCLK_USB;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the main internal regulator output voltage
     */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the Systick interrupt time
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick
     */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /**Enable MSI Auto calibration
     */
    HAL_RCCEx_EnableMSIPLLMode();

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    debug_facility();
    Init_RTC();
    HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  file: The file name as string.
 * @param  line: The line in file as a number.
 * @retval None
 */
void _Error_Handler(const char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

/**
 * @brief  This function handles RTC Auto wake-up interrupt request.
 * @param  None
 * @retval None
 */
void RTC_WKUP_IRQHandler(void)
{
    HW_TS_RTC_Wakeup_Handler();
}

/**
 * @brief  This function handles External line
 *         interrupt request for BlueNRG.
 * @param  None
 * @retval None
 */
void EXTI9_5_IRQHandler(void) /* BNRG_SPI_EXTI_IRQHandler */
{
    HAL_GPIO_EXTI_IRQHandler(BNRG_SPI_EXTI_PIN);
}

/**
 * @brief  This function handles DMA Rx interrupt request.
 * @param  None
 * @retval None
 */
void DMA2_Channel1_IRQHandler(void) /* BNRG_SPI_RX_DMA_IRQHandler */
{
    HW_BNRG_DMARxCb();
}

/**
 * @brief  This function handles DMA Tx interrupt request.
 * @param  None
 * @retval None
 */
void DMA2_Channel2_IRQHandler(void) /* BNRG_SPI_TX_DMA_IRQHandler */
{
    HW_BNRG_DMATxCb();
}
