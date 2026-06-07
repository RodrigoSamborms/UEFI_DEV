# UEFI Build Guide

This workspace includes `build-uefi.ps1`, a PowerShell script that compiles `efi.c` into an EFI application and can optionally dismount the VHD and launch QEMU.

## What the script does

- Compiles `efi.c` into `Hello.EFI`.
- Uses MinGW64 / GCC with UEFI-friendly flags.
- Optionally dismounts `UEFI_VFat32.vhd` if it is attached in Windows.
- Optionally starts QEMU with OVMF after compilation.

## Requirements

- MinGW64 GCC in `PATH`.
- `OVMF.fd` in the workspace folder.
- `UEFI_VFat32.vhd` in the workspace folder.
- QEMU installed as `qemu-system-x86_64` in `PATH`.
- Windows `Storage` module available for `Get-DiskImage` and `Dismount-DiskImage`.

## Build command used by the script

The script effectively runs a direct compile and link step like this:

```powershell
& gcc @(
    'efi.c',
    '-o', 'Hello.EFI',
    '-nostdlib',
    '-ffreestanding',
    '-fshort-wchar',
    '-mno-red-zone',
    '-Wall',
    '-Wextra',
    '-Wpedantic',
    '-std=c17',
    '-Wl,--subsystem,10',
    '-Wl,-e,efi_main',
    '-Wl,--image-base,0'
)
```

## Script parameters

- `-Source`: source file to compile. Default: `efi.c`
- `-Output`: EFI output file. Default: `Hello.EFI`
- `-Compiler`: compiler command. Default: `gcc`
- `-VhdPath`: virtual disk path. Default: `.\UEFI_VFat32.vhd`
- `-QemuPath`: QEMU executable. Default: `qemu-system-x86_64`
- `-DismountVhd`: dismount the VHD before finishing.
- `-RunQemu`: dismount the VHD if attached, then start QEMU.

## Usage examples

Compile only:

```powershell
.\build-uefi.ps1
```

Compile and dismount the VHD if it is attached:

```powershell
.\build-uefi.ps1 -DismountVhd
```

Compile, dismount the VHD, and run QEMU:

```powershell
.\build-uefi.ps1 -RunQemu
```

Use a different source or output name:

```powershell
.\build-uefi.ps1 -Source .\efi.c -Output .\MyApp.EFI
```

## QEMU boot command

If you want to start QEMU manually, use:

```powershell
qemu-system-x86_64 -bios .\OVMF.fd -net none -hda .\UEFI_VFat32.vhd
```

## Notes

- The script checks whether the VHD is attached and only dismounts it if needed.
- The EFI entry point used by this codebase is `efi_main`.
- The generated file can be copied into the FAT32 image and booted in QEMU with OVMF.
# UEFI Build Guide

This workspace includes `build-uefi.ps1`, a PowerShell script that compiles `efi.c` into an EFI application and can optionally dismount the VHD and launch QEMU.

## What the script does

- Compiles `efi.c` into `Hello.EFI`.
- Uses MinGW64 / GCC with UEFI-friendly flags.
- Optionally dismounts `UEFI_VFat32.vhd` if it is attached in Windows.
- Optionally starts QEMU with OVMF after compilation.

## Requirements

- MinGW64 GCC in `PATH`.
- `OVMF.fd` in the workspace folder.
- `UEFI_VFat32.vhd` in the workspace folder.
- QEMU installed as `qemu-system-x86_64` in `PATH`.
- Windows `Storage` module available for `Get-DiskImage` and `Dismount-DiskImage`.

## Build command used by the script

The script effectively runs a direct compile and link step like this:

```powershell
& gcc @(
    'efi.c',
    '-o', 'Hello.EFI',
    '-nostdlib',
    '-ffreestanding',
    '-fshort-wchar',
    '-mno-red-zone',
    '-Wall',
    '-Wextra',
    '-Wpedantic',
    '-std=c17',
    '-Wl,--subsystem,10',
    '-Wl,-e,efi_main',
    '-Wl,--image-base,0'
)
```

## Script parameters

- `-Source`: source file to compile. Default: `efi.c`
- `-Output`: EFI output file. Default: `Hello.EFI`
- `-Compiler`: compiler command. Default: `gcc`
- `-VhdPath`: virtual disk path. Default: `.UEFI_VFat32.vhd`
- `-QemuPath`: QEMU executable. Default: `qemu-system-x86_64`
- `-DismountVhd`: dismount the VHD before finishing.
- `-RunQemu`: dismount the VHD if attached, then start QEMU.

## Usage examples

Compile only:

```powershell
.\build-uefi.ps1
```

Compile and dismount the VHD if it is attached:

```powershell
.\build-uefi.ps1 -DismountVhd
```

Compile, dismount the VHD, and run QEMU:

```powershell
.\build-uefi.ps1 -RunQemu
```

Use a different source or output name:

```powershell
.\build-uefi.ps1 -Source .\efi.c -Output .\MyApp.EFI
```

## QEMU boot command

If you want to start QEMU manually, use:

```powershell
qemu-system-x86_64 -bios .\OVMF.fd -net none -hda .\UEFI_VFat32.vhd
```

## Notes

- The script checks whether the VHD is attached and only dismounts it if needed.
- The EFI entry point used by this codebase is `efi_main`.
- The generated file can be copied into the FAT32 image and booted in QEMU with OVMF.
