#ifndef CONSOLE_H
#define CONSOLE_H

// Initialize console protocols
EFI_STATUS console_init(void);

// Print colored text to console
void console_print_colored(CHAR16 *text, UINTN foreground, UINTN background);

// Display main menu and handle input
UINTN display_main_menu(void);

// Display until input
void display_console_print(EFI_STATUS Status, EFI_INPUT_KEY *Key);

// Clear screen and reset cursor
void console_clear(void);

#endif
