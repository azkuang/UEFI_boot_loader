#include <Uefi.h>
#include "Uefi/UefiBaseType.h"
#include "Uefi/UefiSpec.h"
#include "console.h"

EFI_STATUS
EFIAPI
UefiMain
(
		IN EFI_HANDLE ImageHandle, 
		IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;

	Status = console_init();

	if (Status != EFI_SUCCESS) {
		return EFI_ERROR(Status);
	}

	console_print_colored(L"UEFI", EFI_YELLOW, EFI_GREEN);
	display_console_print(Status, &Key);

	return EFI_SUCCESS;
}

