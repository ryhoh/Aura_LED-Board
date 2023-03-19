/**
 * @file NetworkTask.h
 * @author ryhoh
 * @brief Network task header file.
 * 
 */

#ifndef _NETWORKTASK_H_
#define _NETWORKTASK_H_


// インクルード
#include "App/Common.h"
#include "App/variant/Variant.h"
#include "App/task/SystemControl.h"
#include "App/task/LEDTask.h"

// マクロ定義
#define m_NETWORK_TASK_WEEKDAY_STR_SIZE (22)  // 曜日文字列サイズ： 7曜日 * 3文字 + 1終端文字
#define m_NETWORK_TASK_TIME_STR_SIZE (9)  // 時刻文字列サイズ
#define m_NETWORK_TASK_DATE_STR_SIZE (16)  // 日付文字列サイズ
#define m_NETWORK_TASK_TIME_LEFT_OFFSET (9)
#define m_NETWORK_TASK_DATE_LEFT_OFFSET (1)

// プロトタイプ宣言
void Network_Task_Main(void);
static void Network_Task_SubTaskClock(void);
static void Network_Task_SubTaskDate(void);
static void Network_Task_SubTaskMsg(void);
uint8_t GET_NetWork_Task_NetworkSetupState(void);
String GET_Network_WiFi_SSID(void);
// String GET_Network_Task_LastMessage(void);

#endif  /* _NETWORKTASK_H_ */
