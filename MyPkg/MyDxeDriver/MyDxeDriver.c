#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>

EFI_GUID gMyCustomProtocolGuid = {
  0x49093234, 0xAE1E, 0x4DAE,
  { 0xAF, 0xCC, 0xBD, 0x72, 0x7C, 0x29, 0x19, 0x1C }
};

EFI_STATUS
EFIAPI
MyDxeDriverEntry (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS Status;
  EFI_HANDLE Handle = NULL;

  // Call InstallMultipleProtocolInterface() to publish protocol
  // Signature: gBS->InstallMultipleProtocolInterface(
  //  &Handle,
  //  &gMyCustomProtocolGuid, NULL // NULL interface = GUID-only marker
  //  NULL
  // );

  Status = gBS->InstallMultipleProtocolInterfaces(
    &Handle,
    &gMyCustomProtocolGuid,
    NULL,
    NULL
  );

  // DEBUG((DEBUG_INFO, "MyDxeDriver: entry point reached = %r\n", Status));
  Print(L"MyDxeDriver: entry point reached = %r\n", Status);

  return Status;
}

