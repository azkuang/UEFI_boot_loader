# UEFI Boot Manager

A minimal UEFI boot manager application demonstrating low-level systems programming concepts for firmware development. This project showcases UEFI protocols, memory management, file system operations, and boot option handling in a bare-metal environment.

## Features

- **Interactive Console Interface**: Colored text output with keyboard navigation
- **File System Browser**: Enumerate and browse files across multiple file systems
- **Memory Management**: Display system memory map and custom allocation routines
- **Boot Option Manager**: List, display, and launch available boot options
- **Cross-Platform**: Supports x86-64 UEFI systems (physical and virtual)

## Screenshots

## Architecture

### Project Structure
```
uefi-boot-manager/
├── src/
│   ├── main.c              # Entry point and main menu loop
│   ├── console.c           # Console I/O and menu systems
│   ├── filesystem.c        # File system enumeration and browsing
│   ├── memory.c            # Memory map display and allocation
│   ├── boot.c              # Boot option management
│   └── utils.c             # Utility functions and helpers
├── include/
│   ├── common.h            # Common definitions and macros
│   ├── console.h           # Console function prototypes
│   ├── filesystem.h        # File system operation prototypes
│   ├── memory.h            # Memory management prototypes
│   └── boot.h              # Boot option handling prototypes
├── build/                  # Build output directory
├── Makefile               # Build configuration
├── link.ld                # UEFI linker script
├── .gitignore             # Git ignore rules
├── .gitmodules            # EDK2 submodule configuration
└── README.md              # This file
```

### Core Components

#### Console Management (`console.c`)
- UEFI Simple Text Input/Output protocol handling
- Colored text output with RGB support
- Keyboard input processing with special key handling
- Menu system with arrow key navigation

#### File System Interface (`filesystem.c`)
- Simple File System Protocol enumeration
- Directory traversal and file information display
- Multi-file system support (FAT32, NTFS detection)
- File attribute and timestamp handling

#### Memory Management (`memory.c`)
- System memory map retrieval and parsing
- Memory type classification (Conventional, Reserved, ACPI, etc.)
- Custom memory allocation wrappers
- Memory usage statistics and visualization

#### Boot Option Manager (`boot.c`)
- Boot option enumeration from NVRAM
- Boot variable parsing and display
- Boot option execution with error handling
- Support for various boot device types

## Prerequisites

### Development Environment
- **Compiler**: GCC 9+ or Clang 10+ with UEFI target support
- **Build Tools**: GNU Make 4.0+
- **Architecture**: x86_64 host system
- **OS**: Linux (Ubuntu 20.04+ recommended)

### Testing Environment
- **Emulator**: QEMU 6.0+ with KVM support
- **Firmware**: OVMF (Open Virtual Machine Firmware)
- **Disk Tools**: `dosfstools` for FAT32 image creation

### Installation (Ubuntu/Debian)
```bash
# Development tools
sudo apt update
sudo apt install build-essential gcc-multilib

# UEFI development dependencies  
sudo apt install gnu-efi ovmf qemu-system-x86 dosfstools mtools

# Optional: Cross-compilation tools
sudo apt install gcc-mingw-w64 binutils-mingw-w64
```

## Quick Start

### 1. Clone Repository
```bash
git clone --recursive https://github.com/yourusername/uefi-boot-manager.git
cd uefi-boot-manager
```

### 2. Initialize Submodules (if not cloned recursively)
```bash
git submodule update --init --recursive
```

### 3. Build
```bash
make clean && make
```

### 4. Test in QEMU
```bash
make run
```

## Build System

### Makefile Targets
```bash
make                 # Build bootmanager.efi
make clean          # Remove build artifacts
make distclean      # Clean including dependencies
make run            # Build and run in QEMU
make debug          # Run with GDB debugging enabled
make disk           # Create bootable disk image
make install        # Install to USB device (requires DEVICE=/dev/sdX)
```

### Build Configuration
The build system supports several configuration options:

```bash
# Debug build with symbols
make DEBUG=1

# Optimize for size
make OPTIMIZE=size

# Cross-compile for different architectures
make ARCH=ia32      # 32-bit x86
make ARCH=aarch64   # 64-bit ARM
```

### Compiler Flags
```makefile
CFLAGS = -std=c99 -fno-stack-protector -fpie -fshort-wchar
CFLAGS += -mno-red-zone -mno-sse -mno-mmx
CFLAGS += -Wall -Wextra -Werror -pedantic
CFLAGS += -DGNU_EFI_USE_MS_ABI
```

## Testing

### QEMU Virtual Testing
```bash
# Basic test (automatically runs after build)
make run

# Test with debugging output
make run DEBUG=1

# Test with serial console
make run SERIAL=1

# Test with specific memory size
make run MEMORY=512M
```

### Real Hardware Testing
```bash
# Create bootable USB (replace /dev/sdX with your USB device)
make install DEVICE=/dev/sdX

# Or manually copy to existing EFI system partition
sudo cp build/bootmanager.efi /boot/efi/EFI/BOOT/BOOTX64.EFI
```

**⚠️ Warning**: Testing on real hardware can potentially affect your system's boot configuration. Always backup your current EFI configuration and test in a VM first.

### Test Coverage
The project includes several test scenarios:
- Empty file systems
- Large directory structures
- Various memory configurations
- Multiple boot options
- Error condition handling

## Usage

### Main Menu Navigation
- **Arrow Keys**: Navigate menu options
- **Enter**: Select current option
- **Escape**: Return to previous menu or exit
- **F1**: Help/About information

### File System Browser
- Browse available file systems
- Navigate directories with arrow keys
- View file details (size, attributes, timestamps)
- Support for various file system types

### Memory Manager
- Display comprehensive memory map
- Show memory type distribution
- View available vs. used memory
- Test custom allocation routines

### Boot Option Manager
- List all configured boot options
- Display boot option details and paths
- Launch selected boot options
- Handle boot failures gracefully

## Development

### Adding New Features
1. Create new source file in `src/`
2. Add corresponding header in `include/`
3. Update `Makefile` with new object file
4. Update `common.h` with any shared definitions
5. Test thoroughly in QEMU before real hardware

### Debugging
```bash
# Enable debug output
make DEBUG=1

# Run with GDB support
make debug
# In another terminal:
gdb build/bootmanager.efi
(gdb) target remote localhost:1234
```

### Code Style Guidelines
- Follow C99 standard
- Use UEFI naming conventions (EFI_STATUS, etc.)
- Include comprehensive error checking
- Document all public functions
- Maximum 80 characters per line

## Architecture Details

### UEFI Integration
- Uses EDK2 headers for UEFI protocol definitions
- Implements proper UEFI application entry points
- Follows UEFI specification 2.9 requirements
- Compatible with Secure Boot (when properly signed)

### Memory Model
- Uses UEFI Boot Services memory allocation
- Respects UEFI memory map and types
- Implements custom allocator for demonstration
- Proper cleanup on application exit

### Protocol Usage
- Simple Text Input/Output for console operations
- Simple File System Protocol for file access
- Loaded Image Protocol for application info
- Device Path Protocol for boot device identification

## Troubleshooting

### Common Build Issues
```bash
# Missing EDK2 headers
git submodule update --init --recursive

# Compiler not found
sudo apt install gcc-multilib

# Linker errors
make clean && make
```

### Runtime Issues
```bash
# Application won't start in QEMU
# Check OVMF firmware installation
sudo apt install ovmf

# File system not detected  
# Ensure disk image is properly formatted as FAT32

# Memory allocation failures
# Check QEMU memory configuration (-m 512M)
```

### Debugging Tips
1. Use serial console output for debugging (`make run SERIAL=1`)
2. Add debug prints to trace execution flow
3. Check UEFI return codes for all API calls
4. Use QEMU monitor for system state inspection

## Contributing

### Development Setup
1. Fork the repository
2. Create a feature branch
3. Make changes following coding standards
4. Test thoroughly in QEMU and real hardware (if possible)
5. Submit pull request with detailed description

### Bug Reports
Please include:
- Build environment details (OS, compiler version)
- Complete error messages
- Steps to reproduce
- Expected vs. actual behavior

## License

This project is released under the MIT License. See `LICENSE` file for details.

## Acknowledgments

- **Tianocore EDK2**: UEFI headers and specifications
- **GNU-EFI**: Alternative UEFI development framework
- **OVMF**: Open source UEFI firmware for virtualization
- **QEMU**: Essential for UEFI development and testing

## References

### UEFI Specifications
- [UEFI Specification 2.9](https://uefi.org/specifications)
- [Platform Initialization Specification](https://uefi.org/specifications)

### Development Resources
- [EDK2 Documentation](https://github.com/tianocore/edk2)
- [GNU-EFI Project](https://sourceforge.net/projects/gnu-efi/)
- [OVMF Documentation](https://github.com/tianocore/tianocore.github.io/wiki/OVMF)

### Learning Materials
- "Beyond BIOS: Developing with the Unified Extensible Firmware Interface" by Vincent Zimmer
- Intel UEFI Development Training Materials
- ARM UEFI Development Guide

## Author

Alex Kuang 
Email: alexzkuang0314@gmail.com  
LinkedIn: [LinkedIn](https://linkedin.com/in/azkuang0314)  

