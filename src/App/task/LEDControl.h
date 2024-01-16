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
#include "App/accelsensor/Direction.h"

// 構造体定義
typedef struct _DisplayInfo {
    uint32_t u32_offset_from_left;
    uint8_t u8_is_updated;
    uint8_t u8_dummy[3];
    String str_to_display;
} DisplayInfo_t;

// マクロ定義
#define m_LED_TASK_CALL_ITVL (16)  // [ms,1]
#define m_LED_TASK_10s (10000)  // [ms,1]
#define m_LED_TASK_SCROLL_ITVL (50)  // [ms/pixel,1] 1pixel=50ms, 1s=20pixels
#define m_LED_TASK_SCROLL_CLOCK (m_LED_TASK_SCROLL_ITVL / m_LED_TASK_CALL_ITVL)  // [1/step,1]
#define m_LED_TASK_CONNECTING_MSG ("Connecting to ")

// プロトタイプ宣言
void LED_Task_Init(void);
void LED_Task_FirstTimeToRunningState(void);
void LED_Task_Main(void);
void LED_PrimeTask_OutputMain(void);

// インタフェース
uint8_t GET_LED_Task_Setup_Done(void);
DisplayInfo_t *GET_LED_Task_DisplayInfoClock(void);
DisplayInfo_t *GET_LED_Task_DisplayInfoDate(void);
DisplayInfo_t *GET_LED_Task_DisplayInfoMsg(void);

#endif  /* _LEDTASK_H_ */
