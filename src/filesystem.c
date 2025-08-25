#include <uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/Uefilib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/SimpleTextOut.h>
#include <Library/FileHandleLib.h>

// Enumerate all available file systems
EFI_STATUS enumerate_filesystems(void);

// Browse files in a directory
EFI_STATUS browse_directory(EFI_FILE_PROTOCOL *root);

// Display file information
void display_file_info(EFI_FILE_INFO *file_info);
