#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

VOID
EFIAPI
MyTimerCallback (
  IN EFI_EVENT Event,
  IN VOID *Context
)
{
  Print(L"TimerEvent\n");
  UINTN *Counter = (UINTN *) Context;
  (*Counter)++;
}

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_EVENT TimerEvent;
  EFI_EVENT WaitEvent;
  UINTN Counter = 0;

  // Create Timer event
  // Set timer
  // Wait for Event
  // Close event

  gBS->CreateEvent(
    EVT_TIMER | EVT_NOTIFY_SIGNAL,
    TPL_CALLBACK,
    MyTimerCallback,
    &Counter,
    &TimerEvent
  );

  gBS->SetTimer(
    TimerEvent,
    TimerPeriodic,
    5000000ULL
  );

  gBS->CreateEvent(
    EVT_TIMER,
    0,
    NULL,
    NULL,
    &WaitEvent
  );

  gBS->SetTimer(
    WaitEvent,
    TimerRelative,
    30000000ULL
  );

  UINTN Index;
  gBS->WaitForEvent(
    1,
    &WaitEvent,
    &Index
  );

  Print(L"Timer fired %d times\n", Counter);

  gBS->SetTimer(
    TimerEvent,
    TimerCancel,
    0
  );
  gBS->CloseEvent(TimerEvent);
  gBS->SetTimer(
    WaitEvent,
    TimerCancel,
    0
  );
  gBS->CloseEvent(WaitEvent);

  return EFI_SUCCESS;
}
