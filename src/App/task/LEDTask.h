/**
 * @file LEDTask.h
 * @author ryhoh
 * @brief LED制御タスク
 * 
 */

#ifndef _LEDTASK_H_
#define _LEDTASK_H_

// インクルード
#include "MW/MW_pub.h"
#include "App/Common.h"
#include "App/led/AsciiMatrix.h"
#include "App/led/JISMatrix.h"
#include "App/led/Scroll.h"
#include "App/task/SystemControl.h"
#include "App/variant/Variant.h"
#include "App/variant/Profile.h"

// マクロ定義
#define m_LED_TASK_CALL_ITVL (16)  // [ms,1]
#define m_LED_TASK_10s (10000)  // [ms,1]
#define m_LED_TASK_SCROLL_ITVL (80)  // [ms/step,1]
#define m_LED_TASK_SCROLL_CLOCK (m_LED_TASK_SCROLL_ITVL / m_LED_TASK_CALL_ITVL)  // [1/step,1]
#define m_LED_TASK_CONNECTING_MSG ("Connecting to ")

// プロトタイプ宣言
void LED_Main_Task(void);
static void LED_Task_InSetupState(void);
static void LED_Task_RunningState(void);
MatrixLED *LED_Task_GetMatrixLEDs_clock(void);
MatrixLED *LED_Task_GetMatrixLEDs_date(void);
MatrixLED *LED_Task_GetMatrixLEDs_msg(void);

#endif  /* _LEDTASK_H_ */
