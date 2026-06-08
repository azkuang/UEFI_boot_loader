# Package descriptor
# Instructs Build on what to compile

[Defines]
  PLATFORM_NAME           = MyPkg
  # Generate a unique GUID with: uuidgen
  PLATFORM_GUID           = 11223344-5566-7788-99aa-bbccddeeff00
  PLATFORM_VERSION        = 1.0
  DSC_SPECIFICATION       = 0x00010006
  OUTPUT_DIRECTORY        = Build/MyPkg
  SUPPORTED_ARCHITECTURES = X64
  BUILD_TARGETS           = DEBUG|RELEASE

[LibraryClasses]
  # Map abstract library names to concrete implementations
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  StackCheckLib|MdePkg/Library/StackCheckLib/StackCheckLib.inf
  StackCheckFailureHookLib|MdePkg/Library/StackCheckFailureHookLibNull/StackCheckFailureHookLibNull.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf

[Components]
  MyPkg/MyApp/MyApp.inf
  MyPkg/SysTableWalker/SysTableWalker.inf
  MyPkg/TimerApp/TimerApp.inf
  MyPkg/VarEnum/VarEnum.inf
  MyPkg/MyDxeDriver/MyDxeDriver.inf
  MyPkg/AlexSplashDxe/AlexSplashDxe.inf
