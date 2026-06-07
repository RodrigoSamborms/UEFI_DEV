param(
    [string]$Source = 'efi.c',
    [string]$Output = 'Hello.EFI',
    [string]$Compiler = 'gcc',
    [string]$VhdPath = '.\UEFI_VFat32.vhd',
    [string]$QemuPath = 'qemu-system-x86_64',
    [switch]$RunQemu,
    [switch]$DismountVhd
)

$ErrorActionPreference = 'Stop'

function Ensure-VhdDetached {
    param([string]$ImagePath)

    $diskImage = Get-DiskImage -ImagePath $ImagePath -ErrorAction SilentlyContinue
    if ($null -ne $diskImage -and $diskImage.Attached) {
        Dismount-DiskImage -ImagePath $ImagePath
    }
}

$compileArgs = @(
    $Source,
    '-o', $Output,
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

& $Compiler @compileArgs

if ($DismountVhd) {
    Ensure-VhdDetached -ImagePath $VhdPath
}

if ($RunQemu) {
    Ensure-VhdDetached -ImagePath $VhdPath
    & $QemuPath -bios .\OVMF.fd -net none -hda $VhdPath
}