#ifndef _NVMMOCK_H_
#define _NVMMOCK_H_

#include <stdint.h>

#define MOCK_EEPROM_SIZE (0x61)

void NVM_Mock_Read(int32_t begin_addr, char buf[], uint32_t size);
void NVM_Mock_Write(int32_t begin_addr, const char buf[], uint32_t size);

#endif  /* _NVMMOCK_H_ */
