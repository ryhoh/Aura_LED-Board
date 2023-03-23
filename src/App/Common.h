/**
 * @file Common.h
 * @author ryhoh
 * @brief Common header file for App layer.
 * 
 */

#ifndef _APP_COMMON_H_
#define _APP_COMMON_H_

// インクルード
#include <cstdint>
#ifdef ARDUINO
#include <Arduino.h>
#endif

// マクロ定義
#define m_ON (1)
#define m_OFF (0)
#define M_CLIP_MAX(x, max) ((x) > (max) ? (max) : (x))
#define M_CLIP_MIN(x, min) ((x) < (min) ? (min) : (x))

// 構造体定義
typedef struct _TransitionTable {
    void (*p_judge)(void);
    void (*p_entry)(void);
    void (*p_do)(void);
    void (*p_exit)(void);
} TransitionTable_t;  // 状態遷移テーブル



#endif  /* _APP_COMMON_H_ */
