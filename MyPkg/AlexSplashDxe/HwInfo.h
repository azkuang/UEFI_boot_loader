#ifndef __HW_INFO_H__
#define __HW_INFO_H__

#include <Uefi.h>

VOID PrintSystemInfo (VOID);
VOID PrintCpuInfo    (VOID);
VOID PrintMemoryInfo (VOID);
VOID PrintPciDevices (VOID);
VOID PrintStorageDevices (VOID);

#endif
