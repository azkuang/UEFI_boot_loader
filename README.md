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
