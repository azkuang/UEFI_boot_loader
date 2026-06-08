#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/PciIo.h>
#include <Protocol/BlockIo.h>
#include "HwInfo.h"

STATIC CHAR8 *
SmbiosGetString (
  IN SMBIOS_STRUCTURE *Hdr,
  IN UINT8             Index
  )
{
  CHAR8 *Str;

  if (Index == 0) {
    return "N/A";
  }
  Str = (CHAR8 *)Hdr + Hdr->Length;
  while (--Index > 0) {
    while (*Str != '\0') {
      Str++;
    }
    Str++;
    if (*Str == '\0') {
      return "N/A";
    }
  }
  return Str;
}

VOID
PrintSystemInfo (VOID)
{
  EFI_STATUS               Status;
  EFI_SMBIOS_PROTOCOL     *Smbios;
  EFI_SMBIOS_HANDLE        Handle;
  EFI_SMBIOS_TABLE_HEADER *Record;
  SMBIOS_TABLE_TYPE0      *Type0;
  SMBIOS_TABLE_TYPE1      *Type1;
  EFI_SMBIOS_TYPE          Type;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **)&Smbios);
  if (EFI_ERROR (Status)) {
    Print (L"BIOS     : [unavailable]\n");
    Print (L"System   : [unavailable]\n\n");
    return;
  }

  Type   = SMBIOS_TYPE_BIOS_INFORMATION;
  Handle = SMBIOS_HANDLE_PI_RESERVED;
  Status = Smbios->GetNext (Smbios, &Handle, &Type, &Record, NULL);
  if (!EFI_ERROR (Status)) {
    Type0 = (SMBIOS_TABLE_TYPE0 *)Record;
    Print (L"BIOS     : %a  %a\n",
           SmbiosGetString (&Type0->Hdr, Type0->Vendor),
           SmbiosGetString (&Type0->Hdr, Type0->BiosVersion));
  } else {
    Print (L"BIOS     : N/A\n");
  }

  Type   = SMBIOS_TYPE_SYSTEM_INFORMATION;
  Handle = SMBIOS_HANDLE_PI_RESERVED;
  Status = Smbios->GetNext (Smbios, &Handle, &Type, &Record, NULL);
  if (!EFI_ERROR (Status)) {
    Type1 = (SMBIOS_TABLE_TYPE1 *)Record;
    Print (L"System   : %a  %a\n",
           SmbiosGetString (&Type1->Hdr, Type1->Manufacturer),
           SmbiosGetString (&Type1->Hdr, Type1->ProductName));
  } else {
    Print (L"System   : N/A\n");
  }

  Print (L"\n");
}

VOID
PrintCpuInfo (VOID)
{
  EFI_STATUS               Status;
  EFI_SMBIOS_PROTOCOL     *Smbios;
  EFI_SMBIOS_HANDLE        Handle;
  EFI_SMBIOS_TABLE_HEADER *Record;
  SMBIOS_TABLE_TYPE4      *Type4;
  EFI_SMBIOS_TYPE          Type;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **)&Smbios);
  if (EFI_ERROR (Status)) {
    Print (L"CPU      : [unavailable]\n\n");
    return;
  }

  Type   = SMBIOS_TYPE_PROCESSOR_INFORMATION;
  Handle = SMBIOS_HANDLE_PI_RESERVED;
  Status = Smbios->GetNext (Smbios, &Handle, &Type, &Record, NULL);
  if (!EFI_ERROR (Status)) {
    Type4 = (SMBIOS_TABLE_TYPE4 *)Record;
    Print (L"CPU      : %a @ %u MHz  [%u cores]\n",
           SmbiosGetString (&Type4->Hdr, Type4->ProcessorVersion),
           (UINT32)Type4->MaxSpeed,
           (UINT32)Type4->CoreCount);
  } else {
    Print (L"CPU      : N/A\n");
  }

  Print (L"\n");
}

STATIC CHAR16 *
MemTypeToString (
  IN UINT32  Type
  )
{
  switch (Type) {
    case 0x03: return L"DRAM";
    case 0x0F: return L"SDRAM";
    case 0x12: return L"DDR";
    case 0x13: return L"DDR2";
    case 0x18: return L"DDR3";
    case 0x1A: return L"DDR4";
    case 0x1E: return L"DDR5";
    default:   return L"Unknown";
  }
}

VOID
PrintMemoryInfo (VOID)
{
  EFI_STATUS               Status;
  EFI_SMBIOS_PROTOCOL     *Smbios;
  EFI_SMBIOS_HANDLE        Handle;
  EFI_SMBIOS_TABLE_HEADER *Record;
  SMBIOS_TABLE_TYPE17     *Type17;
  EFI_SMBIOS_TYPE          Type;
  UINTN                    SlotIndex;
  UINT16                   SizeMB;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **)&Smbios);
  if (EFI_ERROR (Status)) {
    Print (L"Memory   : [unavailable]\n\n");
    return;
  }

  Print (L"Memory   :\n");
  SlotIndex = 0;

  Type   = SMBIOS_TYPE_MEMORY_DEVICE;
  Handle = SMBIOS_HANDLE_PI_RESERVED;
  while (TRUE) {
    Status = Smbios->GetNext (Smbios, &Handle, &Type, &Record, NULL);
    if (EFI_ERROR (Status)) {
      break;
    }
    Type17 = (SMBIOS_TABLE_TYPE17 *)Record;

    if (Type17->Size == 0) {
      Print (L"  Slot %-2u : (empty)\n", SlotIndex);
    } else {
      SizeMB = (Type17->Size & 0x8000)
                 ? (Type17->Size & 0x7FFF) / 1024
                 : (Type17->Size & 0x7FFF);
      Print (L"  Slot %-2u : %5u MB  %s  @ %u MHz\n",
             SlotIndex,
             (UINT32)SizeMB,
             MemTypeToString ((UINT32)Type17->MemoryType),
             (UINT32)Type17->Speed);
    }
    SlotIndex++;
  }

  if (SlotIndex == 0) {
    Print (L"  (no DIMM records found)\n");
  }

  Print (L"\n");
}

STATIC CHAR16 *
PciClassToString (
  IN UINT8  Class
  )
{
  switch (Class) {
    case 0x00: return L"Unclassified";
    case 0x01: return L"Storage Controller";
    case 0x02: return L"Network Controller";
    case 0x03: return L"Display Controller";
    case 0x04: return L"Multimedia Controller";
    case 0x05: return L"Memory Controller";
    case 0x06: return L"Bridge";
    case 0x07: return L"Communication Controller";
    case 0x08: return L"System Peripheral";
    case 0x09: return L"Input Device Controller";
    case 0x0C: return L"Serial Bus Controller";
    case 0x0D: return L"Wireless Controller";
    default:   return L"Unknown";
  }
}

VOID
PrintPciDevices (VOID)
{
  EFI_STATUS           Status;
  EFI_HANDLE          *Handles;
  UINTN                HandleCount;
  UINTN                Index;
  EFI_PCI_IO_PROTOCOL *PciIo;
  UINTN                Seg, Bus, Dev, Func;
  UINT32               IdReg;
  UINT8                BaseClass;

  Print (L"PCI Devices:\n");

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR (Status)) {
    Print (L"  [unavailable]\n\n");
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    Handles[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    PciIo->GetLocation (PciIo, &Seg, &Bus, &Dev, &Func);

    Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, 0, 1, &IdReg);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 11, 1, &BaseClass);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Print (L"  %02x:%02x.%x  %04x:%04x  %s\n",
           (UINT32)Bus, (UINT32)Dev, (UINT32)Func,
           IdReg & 0xFFFF,
           (IdReg >> 16) & 0xFFFF,
           PciClassToString (BaseClass));
  }

  FreePool (Handles);
  Print (L"\n");
}

VOID
PrintStorageDevices (VOID)
{
  EFI_STATUS              Status;
  EFI_HANDLE             *Handles;
  UINTN                   HandleCount;
  UINTN                   Index;
  EFI_BLOCK_IO_PROTOCOL  *BlockIo;
  UINTN                   DiskIndex;
  UINT64                  TotalBytes;
  UINT64                  SizeMB;

  Print (L"Storage:\n");

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiBlockIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR (Status)) {
    Print (L"  [unavailable]\n\n");
    return;
  }

  DiskIndex = 0;
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    Handles[Index],
                    &gEfiBlockIoProtocolGuid,
                    (VOID **)&BlockIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }
    if (!BlockIo->Media->MediaPresent) {
      continue;
    }
    if (BlockIo->Media->LogicalPartition) {
      continue;
    }

    TotalBytes = MultU64x32 (
                   BlockIo->Media->LastBlock + 1,
                   BlockIo->Media->BlockSize
                   );
    SizeMB = RShiftU64 (TotalBytes, 20);

    Print (L"  Disk %-2u : %6Lu MB  (%s)\n",
           DiskIndex++,
           SizeMB,
           BlockIo->Media->RemovableMedia ? L"removable" : L"fixed");
  }

  if (DiskIndex == 0) {
    Print (L"  (none found)\n");
  }

  FreePool (Handles);
  Print (L"\n");
}
