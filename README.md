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
sudo apt install build-essential gcc-multilib python3-pip

# UEFI development dependencies  
sudo apt install ovmf qemu-system-x86 dosfstools mtools uuid-dev

# Python dependencies for EDK2
pip3 install --user setuptools

# Optional: Cross-compilation tools
sudo apt install gcc-mingw-w64 binutils-mingw-w64
```

## Quick Start

### 1. Clone Repository
```bash
git clone --recursive https://github.com/yourusername/uefi-boot-manager.git
cd uefi-boot-manager
```

### 2. Initialize EDK2 Build Environment
```bash
# Initialize EDK2 submodule
git submodule update --init --recursive

# Setup EDK2 build environment
cd edk2
export EDK2_TOOLS_PATH=$PWD/BaseTools
. edksetup.sh BaseTools

# Build BaseTools (first time only)
make -C BaseTools

# Return to project root
cd ..
```

### 3. Build
```bash
cd edk2
. edksetup.sh BaseTools
build -a X64 -t GCC5 -p ../BootManagerPkg/BootManagerPkg.dsc
```

### 4. Test in QEMU
```bash
# Copy built application
cp Build/BootManager/DEBUG_GCC5/X64/BootManager.efi disk/EFI/BOOT/BOOTX64.EFI

# Run in QEMU
qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd \
                   -drive file=fat:rw:disk,format=raw \
                   -nographic
```

## Build System

### EDK2 Build Targets
```bash
# Build application (from edk2 directory)
build -a X64 -t GCC5 -p ../BootManagerPkg/BootManagerPkg.dsc

# Clean build
build -a X64 -t GCC5 -p ../BootManagerPkg/BootManagerPkg.dsc cleanall

# Debug build with symbols
build -a X64 -t GCC5 -b DEBUG -p ../BootManagerPkg/BootManagerPkg.dsc

# Release build (optimized)
build -a X64 -t GCC5 -b RELEASE -p ../BootManagerPkg/BootManagerPkg.dsc

# Verbose build output
build -a X64 -t GCC5 -p ../BootManagerPkg/BootManagerPkg.dsc -v
```

### Build Configuration Files
The EDK2 build system uses several configuration files:

#### Package Description File (.dsc)
Defines the overall build configuration, library mappings, and components to build.

#### Component Information File (.inf)  
Describes individual modules, their source files, dependencies, and build requirements.

#### Package Declaration File (.dec)
Declares the package interface, including GUIDs, protocols, and library classes.

### Compiler Configuration
```ini
# In BootManagerPkg.dsc
[BuildOptions]
  GCC:*_*_*_CC_FLAGS = -std=c99 -Wno-unused-parameter
  GCC:*_*_*_PP_FLAGS = -DEFI_DEBUG
  MSFT:*_*_*_CC_FLAGS = /wd4201 /wd4204
```

## Testing

### QEMU Virtual Testing
```bash
# Setup test environment (run once)
mkdir -p disk/EFI/BOOT

# Basic test after building
cd edk2
. edksetup.sh BaseTools
build -a X64 -t GCC5 -p ../BootManagerPkg/BootManagerPkg.dsc
cd ..
cp edk2/Build/BootManager/DEBUG_GCC5/X64/BootManager.efi disk/EFI/BOOT/BOOTX64.EFI

# Run in QEMU
qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd \
                   -drive file=fat:rw:disk,format=raw \
                   -nographic

# Test with debugging output
qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd \
                   -drive file=fat:rw:disk,format=raw \
                   -serial stdio -nographic

# Test with specific memory size
qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd \
                   -drive file=fat:rw:disk,format=raw \
                   -m 512M -nographic
```

### Real Hardware Testing
```bash
# Build release version
cd edk2
. edksetup.sh BaseTools  
build -a X64 -t GCC5 -b RELEASE -p ../BootManagerPkg/BootManagerPkg.dsc
cd ..

# Create bootable USB (replace /dev/sdX with your USB device)
sudo mkfs.fat -F32 /dev/sdX1
sudo mkdir -p /mnt/usb
sudo mount /dev/sdX1 /mnt/usb
sudo mkdir -p /mnt/usb/EFI/BOOT
sudo cp edk2/Build/BootManager/RELEASE_GCC5/X64/BootManager.efi /mnt/usb/EFI/BOOT/BOOTX64.EFI
sudo umount /mnt/usb

# Or manually copy to existing EFI system partition
sudo cp edk2/Build/BootManager/RELEASE_GCC5/X64/BootManager.efi /boot/efi/EFI/BOOT/BOOTX64.EFI
```

**⚠️ Warning**: Testing on real hardware can potentially affect your system's boot configuration. Always backup your current EFI configuration and test in a VM first.

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
1. Create new source file in `BootManagerPkg/Application/BootManager/`
2. Add corresponding header file in same directory
3. Update `BootManager.inf` [Sources] section with new files
4. Add any new library dependencies to .inf [LibraryClasses]
5. Update package .dsc if new libraries are needed
6. Test thoroughly in QEMU before real hardware

### Debugging
```bash
# Enable debug output in build
cd edk2
. edksetup.sh BaseTools
build -a X64 -t GCC5 -b DEBUG -p ../BootManagerPkg/BootManagerPkg.dsc

# Run with serial debugging
qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd \
                   -drive file=fat:rw:../disk,format=raw \
                   -serial stdio -nographic

# Use EDK2 debug output
# Add to your code:
DEBUG((EFI_D_INFO, "Debug message: %s\n", SomeVariable));
```
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
# Missing EDK2 submodule
git submodule update --init --recursive

# BaseTools not built
cd edk2
make -C BaseTools

# Environment not set up
cd edk2
. edksetup.sh BaseTools

# Python dependencies missing
pip3 install --user setuptools
```

### Runtime Issues
```bash
# Application won't start in QEMU
# Check OVMF firmware installation
sudo apt install ovmf
ls /usr/share/ovmf/

# Build output not found
# Check build completed successfully
ls edk2/Build/BootManager/DEBUG_GCC5/X64/

# File system not detected  
# Ensure disk image directory structure is correct
mkdir -p disk/EFI/BOOT
```

### Debugging Tips
1. Use EDK2's DEBUG() macro for output (`DEBUG((EFI_D_INFO, "Message\n"));`)
2. Enable serial console in QEMU for debug output
3. Check EFI_STATUS return codes from all UEFI API calls
4. Use QEMU monitor (`Ctrl+Alt+2`) for system state inspection
5. Build debug version for detailed error information

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
LinkedIn: [My LinkedIn](https://linkedin.com/in/azkuang0314)  

---
*This project demonstrates low-level systems programming skills relevant to UEFI development, embedded systems, and firmware engineering positions.*
