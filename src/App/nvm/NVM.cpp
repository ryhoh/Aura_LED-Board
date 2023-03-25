/**
 * @file NVM.c
 * @author ryhoh
 * @brief Non-Volatile Memory manager in App layer.
 * 
 */

// includes
#include "NVM.h"

// variables
static String gsstr_NVM_SSID;  // SSID
static String gsstr_NVM_PASSWD;  // PASSWD
static String gsstr_NVM_hostname;  // ネットワーク上での名前
static uint8_t gsu8_NVM_variant_idx;  // バリアントインデックス

// prototypes
static void NVM_WriteString(uint32_t begin_addr, uint32_t end_addr, String str);
static String NVM_ReadString(uint32_t begin_addr, uint32_t end_addr);

// functions
void NVM_Init(void) {
  // EEPROMの初期化
  call_nvm_init(m_NVM_SIZE);

  // EEPROMのデータをRAMに展開
  gsstr_NVM_SSID = NVM_ReadString(m_NVM_ADDR_SSID, m_NVM_ADDR_SSID+31);
  gsstr_NVM_PASSWD = NVM_ReadString(m_NVM_ADDR_PASSWD, m_NVM_ADDR_PASSWD+31);
  gsstr_NVM_hostname = NVM_ReadString(m_NVM_ADDR_HOST_NAME, m_NVM_ADDR_HOST_NAME+31);

  char u8_buffer[1] = { 0 }; 
  call_nvm_read(m_NVM_ADDR_VARIANT_IDX, u8_buffer, 1);
	gsu8_NVM_variant_idx = u8_buffer[0];
  
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
  const int32_t i32_written_num = sprintf(u8_buffer, "%s", str.substring(0, (u32_clipped_end_addr - begin_addr - 1)).c_str());
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

String Get_NVM_SSID() {
	return gsstr_NVM_SSID;
}

void Set_NVM_SSID(String str_SSID) {
	gsstr_NVM_SSID = str_SSID;
	NVM_WriteString(m_NVM_ADDR_SSID, m_NVM_ADDR_SSID+0x1F, gsstr_NVM_SSID);
}

String Get_NVM_PASSWD() {
	return gsstr_NVM_PASSWD;
}

void Set_NVM_PASSWD(String str_PASSWD) {
	gsstr_NVM_PASSWD = str_PASSWD;
	NVM_WriteString(m_NVM_ADDR_PASSWD, m_NVM_ADDR_PASSWD+0x1F, gsstr_NVM_PASSWD);
}

String Get_NVM_HostName() {
	return gsstr_NVM_hostname;
}

void Set_NVM_HostName(String str_hostname) {
	gsstr_NVM_hostname = str_hostname;
	NVM_WriteString(m_NVM_ADDR_HOST_NAME, m_NVM_ADDR_HOST_NAME+0x1F, gsstr_NVM_hostname);
}

uint8_t Get_NVM_Variant_idx() {
	return gsu8_NVM_variant_idx;
}
