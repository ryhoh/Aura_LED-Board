/**
 * @file NVM.c
 * @author ryhoh
 * @brief Non-Volatile Memory manager in App layer.
 * 
 */

// includes
#include "NVM.h"

#define NVM_DEBUG (1)

// variables
static uint8_t gsu8_NVM_variant_idx;  // バリアントインデックス(NVM読み出し値)
Network_Config_t gsst_NVM_NetworkConfig;

// prototypes
static void NVM_WriteString(uint32_t begin_addr, uint32_t end_addr, String str);
static String NVM_ReadString(uint32_t begin_addr, uint32_t end_addr);

// functions
void NVM_Init(void) {
  // EEPROMの初期化
  call_nvm_init(m_NVM_SIZE);

  #if NVM_DEBUG 
  #include "App/nvm/NVM_DebugCode.h"
  #else

  // EEPROMのデータをRAMに展開
  gsst_NVM_NetworkConfig.str_ssid = NVM_ReadString(m_NVM_ADDR_SSID, m_NVM_ADDR_SSID+31);
  gsst_NVM_NetworkConfig.str_passwd = NVM_ReadString(m_NVM_ADDR_PASSWD, m_NVM_ADDR_PASSWD+31);
  gsst_NVM_NetworkConfig.str_hostname = NVM_ReadString(m_NVM_ADDR_HOST_NAME, m_NVM_ADDR_HOST_NAME+31);
  char u8_buffer[1] = { 0 }; 
  call_nvm_read(m_NVM_ADDR_VARIANT_IDX, u8_buffer, 1);
	gsu8_NVM_variant_idx = u8_buffer[0];

  #endif  /* NVM_DEBUG */
}

static void NVM_WriteString(uint32_t begin_addr, uint32_t end_addr, String str) {
  uint32_t u32_clipped_end_addr = end_addr;
	char u8_buffer[m_NVM_BUFFER_SIZE] = { 0 };

  if (u32_clipped_end_addr < begin_addr) {
    return;
  }
  if (u32_clipped_end_addr > m_NVM_SIZE) {
    u32_clipped_end_addr = m_NVM_SIZE;
  }
  if (u32_clipped_end_addr - begin_addr > m_NVM_BUFFER_SIZE) {
    // 運用上 m_NVM_BUFFER_SIZE 以上の文字列は扱わない
    u32_clipped_end_addr = begin_addr + m_NVM_BUFFER_SIZE;
  }

  // 終端-開始位置-(ヌル文字)をバッファにコピー
  const uint32_t u32_write_string_len = u32_clipped_end_addr - begin_addr - 1;
  const int32_t i32_written_num = snprintf(u8_buffer, u32_write_string_len, "%s", str.substring(0, u32_write_string_len).c_str());
	call_nvm_write(begin_addr, u8_buffer, i32_written_num+1);
	call_nvm_commit();
}

static String NVM_ReadString(uint32_t begin_addr, uint32_t end_addr) {
  uint32_t u32_clipped_end_addr = end_addr;
  char u8_buffer[m_NVM_BUFFER_SIZE + 1] = { 0 };  // 最後にヌル文字を入れるために+1
  
  if (u32_clipped_end_addr < begin_addr) {
    return String("");
  }
  if (u32_clipped_end_addr > m_NVM_SIZE) {
    u32_clipped_end_addr = m_NVM_SIZE;
  }
  if (u32_clipped_end_addr - begin_addr > m_NVM_BUFFER_SIZE) {
    // 運用上 m_NVM_BUFFER_SIZE 以上の文字列は扱わない
    u32_clipped_end_addr = begin_addr + m_NVM_BUFFER_SIZE;
  }

  call_nvm_read(begin_addr, u8_buffer, u32_clipped_end_addr - begin_addr);
  return String(u8_buffer);
}

void NVM_Main(void) {
  /* NVM無効化のため書き込み処理停止 */
  #if 0
  const Network_Config_t cst_NVM_NetworkConfig = Get_NetworkAP_Network_Config();

  // NVMのデータとRAMのデータを比較して、変更があればNVMに書き込む
  if (gsst_NVM_NetworkConfig.str_ssid != cst_NVM_NetworkConfig.str_ssid) {
    NVM_WriteString(m_NVM_ADDR_SSID, m_NVM_ADDR_SSID+0x1F, cst_NVM_NetworkConfig.str_ssid);
    gsst_NVM_NetworkConfig.str_ssid = cst_NVM_NetworkConfig.str_ssid;
  }
  if (gsst_NVM_NetworkConfig.str_passwd != cst_NVM_NetworkConfig.str_passwd) {
    NVM_WriteString(m_NVM_ADDR_PASSWD, m_NVM_ADDR_PASSWD+0x1F, cst_NVM_NetworkConfig.str_passwd);
    gsst_NVM_NetworkConfig.str_passwd = cst_NVM_NetworkConfig.str_passwd;
  }
  if (gsst_NVM_NetworkConfig.str_hostname != cst_NVM_NetworkConfig.str_hostname) {
    NVM_WriteString(m_NVM_ADDR_HOST_NAME, m_NVM_ADDR_HOST_NAME+0x1F, cst_NVM_NetworkConfig.str_hostname);
    gsst_NVM_NetworkConfig.str_hostname = cst_NVM_NetworkConfig.str_hostname;
  }
  #endif
}

Network_Config_t Get_NVM_Network_Config() {
  return gsst_NVM_NetworkConfig;
}

uint8_t Get_NVM_Variant_idx() {
	return gsu8_NVM_variant_idx;
}
