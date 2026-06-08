# Alex UEFI — Custom UEFI Firmware Modules

A collection of UEFI firmware modules built with [EDK2](https://github.com/tianocore/edk2),
targeting the OVMF virtual platform and tested in QEMU. The main project is **AlexSplashDxe**,
a DXE driver baked into the firmware image that displays a hardware info splash screen on
every boot before the OS loads.

---

## AlexSplashDxe — Boot Splash Driver

A UEFI DXE driver that automatically runs at boot and displays a branded splash screen
with live hardware information read directly from firmware protocols — no OS, no drivers,
no userspace.

### What it displays

```
======================================
        A L E X   U E F I
======================================
BIOS     : EDK II  2.10
System   : QEMU Standard PC (Q35 + ICH9)

CPU      : Intel(R) Xeon(R) Gold ... @ 2500 MHz  [4 cores]

Memory   :
  Slot 0 : 2048 MB  DDR4  @ 2666 MHz
  Slot 1 : (empty)

PCI Devices:
  00:00.0  8086:1237  Bridge
  00:01.3  8086:7113  Bridge
  00:02.0  1234:1111  Display Controller

Storage:
  Disk 0  : 20480 MB  (fixed)

Press -> to continue...
======================================
```

### How it works

- **Module type:** DXE_DRIVER — loads during the UEFI Driver Execution Environment phase
- **Trigger:** Registers a `gEfiEventReadyToBootGuid` signal event at TPL_CALLBACK;
  the callback fires synchronously right before BDS hands off to the boot image
- **Hardware info sources:**
  - CPU, memory, BIOS, system info — SMBIOS types 0, 1, 4, 17 via `EFI_SMBIOS_PROTOCOL`
  - PCI devices — bus:device.function + VID:DID + class via `EFI_PCI_IO_PROTOCOL`
  - Storage — whole-disk block devices via `EFI_BLOCK_IO_PROTOCOL`
- **Input handling:** Drops TPL to `TPL_APPLICATION` before `WaitForEvent` (required by
  the UEFI spec), waits for right arrow key only, then raises TPL back to `TPL_CALLBACK`
  before returning

### Files

```
MyPkg/AlexSplashDxe/
  AlexSplashDxe.inf   — module manifest: GUID, entry point, dependencies
  AlexSplashDxe.c     — entry point, ReadyToBoot event registration, display logic
  HwInfo.c            — hardware enumeration (SMBIOS, PCI IO, Block IO)
  HwInfo.h            — function declarations
```

---

## Other Modules

| Module | Type | Description |
|--------|------|-------------|
| `MyApp` | UEFI_APPLICATION | Reads and hex-dumps UEFI NVRAM variables (BootCurrent, Timeout) |
| `MyDxeDriver` | DXE_DRIVER | Minimal DXE driver skeleton |
| `SysTableWalker` | UEFI_APPLICATION | Walks and prints the EFI System Table structure |
| `TimerApp` | UEFI_APPLICATION | Demonstrates UEFI timer events |
| `VarEnum` | UEFI_APPLICATION | Enumerates all NVRAM variables with attributes |

---

## Build Instructions

This package is an overlay for EDK2 — clone EDK2 first, then drop this repo in.

### Prerequisites
- Linux build environment
- EDK2 build dependencies: `build-essential nasm iasl python3 uuid-dev`
- GCC 5+

### Steps

```bash
# 1. Clone EDK2
git clone https://github.com/tianocore/edk2.git
cd edk2
git submodule update --init

# 2. Clone this repo into the edk2 tree
git clone https://github.com/azkuang0314/Alex_UEFI MyPkg

# 3. Apply the OVMF integration patch (wires AlexSplashDxe into the firmware image)
git apply MyPkg/ovmf_integration.patch

# 4. Set up the build environment
source edksetup.sh

# 5. Build OVMF with the splash driver included
build -p OvmfPkg/OvmfPkgX64.dsc -a X64 -t GCC5
```

Output: `Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd`

### Test in QEMU

```bash
cp Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd /tmp/ovmf_test.fd

qemu-system-x86_64 \
  -drive if=pflash,format=raw,file=/tmp/ovmf_test.fd \
  -m 2G -nographic -net none
```

The splash screen appears before the UEFI Shell. Press the right arrow key to continue.
Press `Ctrl+A` then `X` to exit QEMU.

> **Note:** Copy OVMF.fd before running — QEMU writes back to the file (NVRAM variables)
> and will corrupt your build output if you point it at the original.

---

## Key Concepts Demonstrated

- **DXE driver architecture** — module lifecycle, entry point, depex
- **UEFI event system** — `CreateEventEx`, signal events, ReadyToBoot group
- **TPL management** — raising/lowering Task Priority Level within a callback to satisfy
  `WaitForEvent`'s TPL_APPLICATION requirement while preserving synchronous dispatch
- **SMBIOS table parsing** — walking typed records and extracting null-terminated string tables
- **PCI enumeration** — `LocateHandleBuffer`, config space reads via `EFI_PCI_IO_PROTOCOL`
- **Block device enumeration** — filtering whole disks from partitions via `EFI_BLOCK_IO_PROTOCOL`
- **EDK2 build system** — INF module manifests, DSC platform descriptors, FDF firmware image layout
- **OVMF platform integration** — adding a custom module to an existing firmware platform

---

## Author

Alex Kuang  
Email: <alexzkuang0314@gmail.com>  
LinkedIn: [linkedin.com/in/azkuang0314](https://linkedin.com/in/azkuang0314)

---

*This project demonstrates low-level systems programming skills relevant to UEFI firmware
development, embedded systems, and firmware engineering positions.*
