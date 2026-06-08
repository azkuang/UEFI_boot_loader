#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Guid/EventGroup.h>
#include "HwInfo.h"

STATIC EFI_EVENT  mReadyToBootEvent;

STATIC VOID
EFIAPI
ReadyToBootCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_INPUT_KEY Key;
  UINTN         Index;

  gBS->CloseEvent (Event);

  gST->ConOut->ClearScreen (gST->ConOut);

  Print (L"======================================\n");
  Print (L"        A L E X   U E F I\n");
  Print (L"======================================\n");

  PrintSystemInfo ();
  PrintCpuInfo ();
  PrintMemoryInfo ();
  PrintPciDevices ();
  PrintStorageDevices ();

  Print (L"Press -> to continue...\n");
  Print (L"======================================\n");

  gST->ConIn->Reset (gST->ConIn, FALSE);

  // WaitForEvent requires TPL_APPLICATION; drop from TPL_CALLBACK, then restore
  gBS->RestoreTPL (TPL_APPLICATION);

  while (TRUE) {
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);
    if (!EFI_ERROR (gST->ConIn->ReadKeyStroke (gST->ConIn, &Key))) {
      if (Key.ScanCode == SCAN_RIGHT) {
        break;
      }
    }
  }

  gBS->RaiseTPL (TPL_CALLBACK);
}

EFI_STATUS
EFIAPI
AlexSplashDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return gBS->CreateEventEx (
                EVT_NOTIFY_SIGNAL,
                TPL_CALLBACK,
                ReadyToBootCallback,
                NULL,
                &gEfiEventReadyToBootGuid,
                &mReadyToBootEvent
                );
}
