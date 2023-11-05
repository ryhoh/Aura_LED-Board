#ifndef _APP_TYPE_H_
#define _APP_TYPE_H_

// インクルード
#include "App/Common.h"

// コンポーネントを跨ぐ型定義
typedef struct _Network_Config {
    String str_ssid;
    String str_passwd;
    String str_hostname;
} Network_Config_t;

#endif  /* _APP_TYPE_H_ */