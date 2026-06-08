#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

EFI_STATUS
EFIAPI
UefiMain(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN    VariableNameSize = 1024;
  CHAR16   *VariableName = AllocateZeroPool(VariableNameSize);
  EFI_GUID VendorGuid = { 0 };
  UINT32   Attributes;
  UINTN    DataSize = 0;

  if (VariableName == NULL) {
    FreePool(VariableName);
    return EFI_OUT_OF_RESOURCES;
  }

  while (Status != EFI_NOT_FOUND) {
    Status = gRT->GetNextVariableName(
      &VariableNameSize,
      VariableName,
      &VendorGuid
    );

    if (Status == EFI_NOT_FOUND)
      break;
    if (EFI_ERROR(Status)) {
      Print(L"GetNextVariableName error: %r\n", Status);
      break;
    }

    DataSize = 0;
    Status = gRT->GetVariable(
      VariableName,
      &VendorGuid,
      &Attributes,
      &DataSize,
      NULL
    );

    if (Status != EFI_BUFFER_TOO_SMALL) {
      Print(L"DataSize allocation failed\n");
      return Status;
    }

    Print(L"\n");
    Print(L"=====NVRAM=====\n");
    Print(L"VariableNameSize: %d\n", VariableNameSize);
    Print(L"VariableName: %s\n", VariableName);
    Print(L"VendorGUID: %g\n", &VendorGuid);
    Print(L"DataSize: %d\n", DataSize);
    Print(L"Attributes: %d\n", Attributes);
  }

  FreePool(VariableName);

  return EFI_SUCCESS;
}

