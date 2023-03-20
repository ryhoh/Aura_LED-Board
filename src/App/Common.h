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



#endif  /* _APP_COMMON_H_ */
