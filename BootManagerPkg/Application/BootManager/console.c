#include "Uefi/UefiBaseType.h"
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

// Initialize console protocols
EFI_STATUS console_init(void) {
	// Clear the screen
	gST->ConOut->ClearScreen(gST->ConOut);
	return EFI_SUCCESS;
}

// Print colored text to console
void console_print_colored(CHAR16 *text, UINTN foreground, UINTN background) {
	gST->ConOut->SetAttribute(gST->ConOut, foreground);
	Print(text);
}

// Display main menu and handle input
UINTN display_main_menu(void);

// Handle input after displaying
void display_console_print(EFI_STATUS Status, EFI_INPUT_KEY *Key) {
	while (1) {
		Status = gST->ConIn->ReadKeyStroke(gST->ConIn, Key);
		if (Status == EFI_SUCCESS)
			break;
	}

	return;
}

// Clear screen and reset cursor
void console_clear(void) {
	gST->ConOut->ClearScreen(gST->ConOut);
}
