/**
  Module 1 exercise: walk every field of EFI_SYSTEM_TABLE and print it.

  Spec references:
    - EFI_IMAGE_ENTRY_POINT      UEFI Spec 2.11 §4.1.1
    - EFI_TABLE_HEADER           UEFI Spec 2.11 §4.2.1
    - EFI_SYSTEM_TABLE           UEFI Spec 2.11 §4.3.1
    - EFI_BOOT_SERVICES layout   UEFI Spec 2.11 §4.4.1
    - EFI_RUNTIME_SERVICES layout UEFI Spec 2.11 §4.5.1
**/

#include <Uefi.h>
#include <Library/UefiLib.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  Print(L"======Hdr (EFI_TABLE_HEADER) Values=====\n");
  Print(L"Hdr Signature: %lx\n", SystemTable->Hdr.Signature);
  Print(L"Hdr Revision: %d.%d\n", SystemTable->Hdr.Revision >> 16, SystemTable->Hdr.Revision & 0xFFFF);
  Print(L"Hdr Header size: %d\n", SystemTable->Hdr.HeaderSize);
  Print(L"Hdr CRC32: %d\n", SystemTable->Hdr.CRC32);
  Print(L"Hdr Reserved: %d\n", SystemTable->Hdr.Reserved);

  Print(L"\n");

  Print(L"======EFI System Table======\n");
  Print(L"Firmware Vendor: %s\n", SystemTable->FirmwareVendor);
  Print(L"Firmware Revision: %d.%d\n", SystemTable->FirmwareRevision >> 16, SystemTable->FirmwareRevision & 0xFFFF);
  Print(L"Input Handle: %p\n", SystemTable->ConsoleInHandle);
  Print(L"Output Handle: %p\n", SystemTable->ConsoleOutHandle);
  Print(L"Error Handle: %p\n", SystemTable->StandardErrorHandle);
  Print(L"Console In: %p\n", SystemTable->ConIn);
  Print(L"Console Out: %p\n", SystemTable->ConOut);
  Print(L"Console Error: %p\n", SystemTable->StdErr);

  Print(L"\n");

  Print(L"======Boot Services======\n");
  Print(L"Boot Services Signature: %lx\n", SystemTable->BootServices->Hdr.Signature);
  Print(L"Boot Services Revision: %d.%d\n", SystemTable->BootServices->Hdr.Revision >> 16, SystemTable->BootServices->Hdr.Revision & 0xFFFF);

  Print(L"\n");

  Print(L"======Runtime Services======\n");
  Print(L"Runtime Services Signature: %lx\n", SystemTable->RuntimeServices->Hdr.Signature);
  Print(L"Runtime Services Revision: %d.%d\n", SystemTable->RuntimeServices->Hdr.Revision >> 16, SystemTable->RuntimeServices->Hdr.Revision & 0xFFFF);

  Print(L"\n");

  Print(L"======Configuration Table======\n");
  Print(L"Number of Table Entries: %d\n", SystemTable->NumberOfTableEntries);

  for (int i = 0; i < SystemTable->NumberOfTableEntries; i++) {
    Print(L"Vendor Guid: %g\n", &SystemTable->ConfigurationTable[i].VendorGuid);
    Print(L"Vendor Table: %p\n", SystemTable->ConfigurationTable[i].VendorTable);
  }

  return EFI_SUCCESS;
}
