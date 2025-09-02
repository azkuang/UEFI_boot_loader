#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

// Initialize console protocols
EFI_STATUS console_init(void) {
	
}

// Print colored text to console
void console_print_colored(CHAR16 *text, UINTN foreground, UINTN background);

// Display main menu and handle input
UINTN display_main_menu(void);

// Clear screen and reset cursor
void console_clear(void);
