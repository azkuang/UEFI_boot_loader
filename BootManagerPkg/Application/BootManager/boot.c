// Enumerate boot options
EFI_STATUS enumerate_boot_options(void);

// Display boot option details
void display_boot_option(EFI_LOAD_OPTION *option);

// Launch selected boot option
EFI_STATUS launch_boot_option(UINT16 option_number);
