#include <uefi.h>

// Get and display system memory map
EFI_STATUS display_memory_map(void);

// Custom memory allocation wrapper
void* allocate_memory(UINTN size);

// Memory deallocation wrapper
void free_memory(void *ptr);
