#ifndef FILESYSTEM_H
#define FILESYSTEM_H

// Enumerate all available file systems
EFI_STATUS enumerate_filesystems(void);

// Browse files in a directory
EFI_STATUS browse_directory(EFI_FILE_PROTOCOL *root);

// Display file information
void display_file_info(EFI_FILE_INFO *file_info);

#endif
