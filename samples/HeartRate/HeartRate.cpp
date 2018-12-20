/* Includes ------------------------------------------------------------------*/
#include "HeartRate.h"
#include "common.h"
#include "hw.h"

#include "hr.h"
#include "hrs_app.h"
#include "scheduler.h"
#include "stm32l4xx_it_ble.h"
#include "tl_ble_hci.h"
#include "tl_ble_reassembly.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/**
 * In order to support the Standby Mode, we need to store some variables in a retention memory.
 * In the STM32L4, the SRAM2 is used for that purpose
 * The section below identified the variables to be stored in the retention memory
 * This section is mapped in the SRAM2 in the scatter file.
 * All variables which content shall be retained while in standby mode shall be located in that
 * section. The same section could be defined in other files to store variables. It is as well
 * possible to define other sections and map them in the SRAM2 in the scatter file.
 */
/**
 * START of Section APP_CONTEXT
 */

/**
 * END of Section APP_CONTEXT
 */

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
void HeartRate_main(codal::STM32IotNode &iotNode)
{

    /**
     * END OF SECTION - DEBUG FACILITY
     *********************************************************************************************************************
     */

    /**
     * It shall be registered before calling HR_Init()
     */
    SCH_RegTask(CFG_IdleTask_MeasReq, HRSAPP_Measurement);
    SCH_RegTask(CFG_IdleTask_HciAsynchEvt, TL_BLE_HCI_UserEvtProc);
    SCH_RegTask(CFG_IdleTask_TlEvt, TL_BLE_R_EvtProc);
    SCH_RegTask(CFG_IdleTask_ConnMgr, HR_AdvUpdate);

    /**
     * BLE application start
     */
    HR_Init(HR_Full);

    /* Start the main processes */
    while (1)
    {
        SCH_Run();
    } /* end while(1) */
}

/* ********************  Private functions ***********************************/

/* ******************************************************************************/
/* ******  Exported functions to OVERLOAD __weak defined in /Common modules *****/
/* ******************************************************************************/

/**
 * @brief  This function implement the application action when a EXTI IRQ is raised
 *         It OVERLOADS the __WEAK function defined in stm32l4xx_hal_gpio.h/c
 * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case BNRG_SPI_EXTI_PIN:
        HW_BNRG_SpiIrqCb();
        break;

    default:
        break;
    }

    return;
}

/**
 * @brief  This function OVERLOADEDs the one defined as __WEAK in the LPM (LowPowerManager)
 *         The application should implement dedicated code before getting out from Stop Mode.
 *         This is where the application should reconfigure the clock tree when needed
 * @param  None
 * @retval None
 */
void LPM_ExitStopMode(void)
{
#if (HSI_WITH_PLL == 1)
    __HAL_RCC_PLL_ENABLE(); /**< Enable PLL */
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
        ;                                    /**< Wait till PLL is ready */
    LL_RCC_SetSysClkSource(RCC_CFGR_SW_PLL); /**< Select PLL as system clock source */
    while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_CFGR_SWS_PLL)
        ; /**< Wait till PLL is used as system clock source */
#endif

    return;
}

/**
 * @brief  This function OVERLOADEDs the one defined as __WEAK in the LPM (LowPowerManager)
 *         It sets the IO configuration to be applied when in standby mode
 * @param  None
 * @retval None
 */
void LPM_EnterOffMode(void)
{
    LL_PWR_EnableWakeUpPin(BNRG_SPI_IRQ_WAKEUP_PIN);
    LL_PWR_EnablePUPDCfg();

    return;
}

/**
 * @brief  This function OVERLOADEDs the one defined as __WEAK in the LPM (LowPowerManager)
 *         The application should implement dedicated code before getting out from Off mode
 * @param  None
 * @retval None
 */
void LPM_ExitOffMode(void)
{
    LL_PWR_DisablePUPDCfg();
    LL_PWR_DisableWakeUpPin(BNRG_SPI_IRQ_WAKEUP_PIN);
    BNRG_SPI_CLEAR_WAKEUP_FLAG();

    return;
}

/**
 * @brief This function OVERLOADEDs the one defined as __WEAK in the SCH (scheduler)
 *        The application should enter low power mode
 * @param  None
 * @retval None
 */
void SCH_Idle(void)
{
    return;
}

/**
 * @brief This function OVERLOADEDs the one defined as __WEAK in the TL (transport layer), because
 *        only the application knows which TASKs are currently running and should be paused/resumed.
 *        The applicaiton shall not send a new command when TL the status is set to
 * TL_BLE_HCI_CmdBusy. At least, TL_BLE_HCI_UserEvtProc shall not be executed while the TL remains
 * busy
 * @param status
 * @retval None
 */
void TL_BLE_HCI_StatusNot(TL_BLE_HCI_CmdStatus_t status)
{
    switch (status)
    {
    case TL_BLE_HCI_CmdBusy:
        SCH_PauseTask(CFG_IdleTask_ConnMgr);
        SCH_PauseTask(CFG_IdleTask_MeasReq);
        SCH_PauseTask(CFG_IdleTask_HciAsynchEvt);
        break;

    case TL_BLE_HCI_CmdAvailable:
        SCH_ResumeTask(CFG_IdleTask_ConnMgr);
        SCH_ResumeTask(CFG_IdleTask_MeasReq);
        SCH_ResumeTask(CFG_IdleTask_HciAsynchEvt);
        break;

    default:
        break;
    }
    return;
}

/**
 * @brief  This function OVERLOADEDs the one defined as __WEAK in the TS (TimeServer)
 *         This API is running in the RTC Wakeup interrupt context.
 *         The application may implement an Operating System to change the context priority where
 * the timer callback may be handled. This API provides the module ID to identify which module is
 * concerned and to allow sending the information to the correct task
 *
 * @param  TimerProcessID: The TimerProcessId associated with the timer when it has been created
 * @param  TimerID: The TimerID of the expired timer
 * @param  pTimerCallBack: The Callback associated with the timer when it has been created
 * @retval None
 */

void HW_TS_RTC_Int_AppNot(uint32_t eTimerProcessID, uint8_t ubTimerID,
                          HW_TS_pTimerCb_t pfTimerCallBack)
{
    switch (eTimerProcessID)
    {
    case CFG_TimProcID_isr:
        /**
         * This is expected to be called in the Timer Interrupt Handler to limit latency
         */
        pfTimerCallBack();
        break;

    default:
        pfTimerCallBack();
        break;
    }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
