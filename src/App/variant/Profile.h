/**
 * @file Profile.h
 * @author ryhoh
 * @brief Profile configuration for system.
 * 
 */

#ifndef _APP_PROFILE_H_
#define _APP_PROFILE_H_

// インクルード
#include "App/Common.h"

/* ---------------- マクロ定義 ---------------- */
/*                    ++------------- Product Name: "Luminous Board"     */
/*                    || +----------- Major Version Decimal              */
/*                    || | +--------- Minor Version Decimal              */
/*                    || | | +------- Patch Version Decimal              */
/*                    || | | |+------ Series Code: "Advanced"            */
#define m_SW_VERSION "LBv2.4.0A"  /* Version ID                         */

/* ---------------- コンパイルスイッチ ---------------- */
#define m_SW_DEBUG (0)  /* デバッグモード (1: ON, 0: OFF) ※リリース時は0にすること */

/* ---------------- システム用定数 ---------------- */
#define m_PROFILE_MAX_DESIGNED_PANEL_NUM (12)  // 設計上の限界パネル数

#endif  /* _APP_PROFILE_H_ */
