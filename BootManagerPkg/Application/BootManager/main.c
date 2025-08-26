#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

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

	// Clear screen and print the message
	gST->ConOut->ClearScreen(gST->ConOut);
	Print(L"Hello UEFI\n");
	Print(L"Press any key to exit...\n");

	while (1) {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
		if (Status == EFI_SUCCESS)
			break;
	}

	return EFI_SUCCESS;
}

