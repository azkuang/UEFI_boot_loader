#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>

#define EFI_GLOBAL_VARIABLE_GUID \
  { 0x8BE4DF61, 0x93CA, 0x11D2, \
    { 0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C } }

/**
 * Read a UEFI NVRAM variable and print its value as hex bytes.
 *
 * @param[in] VariableName The variable name (wide string).
 * @param[in] VendorGuid   The Vendor GUID namespace.
 *
 * @retval EFI_SUCESS      Variable was read and printed.
 * @retval EFI_NOT_FOUND   Variable does not exist.
 * @retval other           An unexpected error occurred.
 * */
STATIC
EFI_STATUS
ReadAndPrintVariable (
  IN CHAR16 *VariableName,
  IN EFI_GUID *VendorGuid
)
{
  EFI_STATUS Status;
  UINTN DataSize;
  UINT8 *Data;
  UINT32 Attributes;
  UINTN Index;

  DataSize = 0;
  Status = gRT->GetVariable (
    VariableName,
    VendorGuid,
    &Attributes,
    &DataSize,
    NULL
  );

  if (Status != EFI_BUFFER_TOO_SMALL) {
    Print(L"GetVariable probe failed: %r\n", Status);
    return Status;
  }

  Data = AllocateZeroPool (DataSize);
  if (Data == NULL) {
    Print(L"Out of memory allocating %d bytes\n", DataSize);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gRT->GetVariable (
    VariableName,
    VendorGuid,
    &Attributes,
    &DataSize,
    Data
  );

  if (EFI_ERROR(Status)) {
    Print(L"GetVariable read failed: %r\n", Status);
    FreePool(Data);
    return(Status);
  }

  Print(L"Variable : %s\n", VariableName);
  Print(L"Size     : %d bytes\n", DataSize);
  Print(L"Attrs    : %x%08X", Attributes);

  if (Attributes & EFI_VARIABLE_NON_VOLATILE)
    Print(L" [NV]");
  if (Attributes & EFI_VARIABLE_BOOTSERVICE_ACCESS)
    Print(L" [BS]");
  if (Attributes & EFI_VARIABLE_RUNTIME_ACCESS)
    Print(L" [RT]");

  Print(L"\n");

  Print(L"Value    :\n");
  for (Index = 0; Index < DataSize; Index++) {
    if (Index % 16 == 0)
      Print(L"  %04X: ", Index);
    Print(L"%02X ", Data[Index]);
    if ((Index + 1) % 16 == 0)
      Print(L"\n");
  }
  Print(L"\n");

  FreePool(Data);
  return EFI_SUCCESS;
}


/**
* Entry point for UEFI application.
*
* @param[in] ImageHandle Handle to this loaded image.
* @param[in] SystemTable Pointer to the EFI System table.
*
* @retval EFI_SUCCESS Application completed successfully.
**/

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_GUID GlobalVarGuid = EFI_GLOBAL_VARIABLE_GUID;

  ReadAndPrintVariable(L"BootCurrent", &GlobalVarGuid);

  Print(L"\n");

  ReadAndPrintVariable(L"Timeout", &GlobalVarGuid);

  // Print() uses UCS-2 wide strings (L"...")
  Print(L"UEFI App\n");
  Print(L"UEFI Revision Table: %d.%d\n",
        SystemTable->Hdr.Revision >> 16,
        SystemTable->Hdr.Revision & 0xFFFF);

  return EFI_SUCCESS;
}
