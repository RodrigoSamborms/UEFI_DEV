param(
    [Parameter(Mandatory=$false)]
    [string]$Source = 'efi.c',

    [Parameter(Mandatory=$false)]
    [string]$Output = 'Hello.EFI',

    [Parameter(Mandatory=$false)]
    [string]$Compiler = 'gcc',

    [Parameter(Mandatory=$false)]
    [string]$VhdPath = '.\UEFI_VFat32.vhd',

    [Parameter(Mandatory=$false)]
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

# Resolve sources: include asm/assembly files found alongside the provided Source
function Resolve-Sources {
    param([string]$Src)

    $resolved = @()

    # If Source is a folder, look inside for efi.c and assembly files
    if (Test-Path $Src -PathType Container) {
        $dir = Resolve-Path $Src
        if (Test-Path (Join-Path $dir 'efi.c')) { $resolved += (Join-Path $dir 'efi.c') }
        $asmFiles = Get-ChildItem -Path $dir -Include *.s,*.asm -File -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Name
        foreach ($a in $asmFiles) { $resolved += (Join-Path $dir $a) }
        return $resolved
    }

    # If Source is a file or filename, resolve its directory and include nearby assembly files
    if (Test-Path $Src -PathType Leaf) {
        $fileFull = Resolve-Path $Src
        $resolved += $fileFull
        $dir = Split-Path $fileFull
        $asmFiles = Get-ChildItem -Path $dir -Include *.s,*.asm -File -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Name
        foreach ($a in $asmFiles) {
            $asmFull = Join-Path $dir $a
            if ($asmFull -ne $fileFull) { $resolved += $asmFull }
        }
        return $resolved
    }

    # If not found, treat Source as a filename in current directory
    if (Test-Path (Join-Path (Get-Location) $Src)) {
        return Resolve-Sources (Join-Path (Get-Location) $Src)
    }

    throw "No se pudo resolver Source: $Src"
}

try {
    $sources = Resolve-Sources $Source
} catch {
    Write-Error $_.Exception.Message
    exit 1
}

# Build compile arguments
$compileArgs = @()
$compileArgs += $sources
$compileArgs += '-o'; $compileArgs += $Output
$compileArgs += '-nostdlib'
$compileArgs += '-ffreestanding'
$compileArgs += '-fshort-wchar'
$compileArgs += '-mno-red-zone'
$compileArgs += '-Wall'
$compileArgs += '-Wextra'
$compileArgs += '-Wpedantic'
$compileArgs += '-std=c17'
$compileArgs += '-Wl,--subsystem,10'
$compileArgs += '-Wl,-e,efi_main'
$compileArgs += '-Wl,--image-base,0'

& $Compiler @compileArgs

if ($DismountVhd) {
    Ensure-VhdDetached -ImagePath $VhdPath
}

if ($RunQemu) {
    Ensure-VhdDetached -ImagePath $VhdPath
    & $QemuPath -bios .\OVMF.fd -net none -hda $VhdPath
}