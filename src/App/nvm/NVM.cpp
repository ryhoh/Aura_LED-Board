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
static void NVM_WriteString(uint8_t u8_addr, String str);

// functions
void NVM_Init(void) {
  // EEPROMの初期化
  call_nvm_init(m_NVM_SIZE);

	// EEPROMのデータをRAMに展開
	char u8_buffer[m_NVM_BUFFER_SIZE] = { 0 }; 
	call_nvm_read(m_NVM_ADDR_SSID, u8_buffer, m_NVM_BUFFER_SIZE);
	gsstr_NVM_SSID = String(u8_buffer);
	call_nvm_read(m_NVM_ADDR_PASSWD, u8_buffer, m_NVM_BUFFER_SIZE);
	gsstr_NVM_PASSWD = String(u8_buffer);
	call_nvm_read(m_NVM_ADDR_HOST_NAME, u8_buffer, m_NVM_BUFFER_SIZE);
	gsstr_NVM_hostname = String(u8_buffer);
	call_nvm_read(m_NVM_ADDR_VARIANT_IDX, u8_buffer, m_NVM_BUFFER_SIZE);
	gsu8_NVM_variant_idx = u8_buffer[0];
}

static void NVM_WriteString(uint8_t u8_addr, String str) {
	char u8_buffer[m_NVM_BUFFER_SIZE] = { 0 };
	str.toCharArray(u8_buffer, m_NVM_BUFFER_SIZE);
	call_nvm_write(u8_addr, u8_buffer, m_NVM_BUFFER_SIZE);
	call_nvm_commit();
}

String Get_NVM_SSID() {
	return gsstr_NVM_SSID;
}

void Set_NVM_SSID(String str_SSID) {
	gsstr_NVM_SSID = str_SSID;
	NVM_WriteString(m_NVM_ADDR_SSID, gsstr_NVM_SSID);
}

String Get_NVM_PASSWD() {
	return gsstr_NVM_PASSWD;
}

void Set_NVM_PASSWD(String str_PASSWD) {
	gsstr_NVM_PASSWD = str_PASSWD;
	NVM_WriteString(m_NVM_ADDR_PASSWD, gsstr_NVM_PASSWD);
}

String Get_NVM_HostName() {
	return gsstr_NVM_hostname;
}

void Set_NVM_HostName(String str_hostname) {
	gsstr_NVM_hostname = str_hostname;
	NVM_WriteString(m_NVM_ADDR_HOST_NAME, gsstr_NVM_hostname);
}

uint8_t Get_NVM_Variant_idx() {
	return gsu8_NVM_variant_idx;
}
