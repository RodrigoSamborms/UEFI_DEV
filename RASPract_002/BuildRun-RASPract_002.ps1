#copia el archivo en la carpeta del proyecto y ejecuta ahi el script
#.\BuildRun-RASPract_002.ps1
#nota los archivos deben ser efi.c efi.h msr.s
# Ruta base de la práctica
$basePath = "C:\Users\sambo\Documents\UEFI_Dev\RASPract_002"

# Archivos fuente
$efiFile = Join-Path $basePath "efi.c"
$hdrFile = Join-Path $basePath "efi.h"
$asmFile = Join-Path $basePath "msr.s"

# Nombre del binario EFI
$outFile = Join-Path $basePath "RASPract_002.EFI"

# Comando de compilación con GCC
$xargList = @(
    $efiFile,
    $asmFile,
    '-o', $outFile,
    '-nostdlib',
    '-ffreestanding',
    '-fshort-wchar',
    '-mno-red-zone',
    '-Wall',
    '-Wextra',
    '-Wpedantic',
    '-std=c11',
    '-Wl,--subsystem,10',
    '-Wl,-e,efi_main',
    '-Wl,--image-base,0'
)

Write-Host "🔨 Compilando fuentes en $basePath..."
& gcc @xargList

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Compilación exitosa: $outFile"
} else {
    Write-Host "❌ Error en compilación"
    exit 1
}

# Ejecutar en QEMU
$ovmfPath = "C:\Users\sambo\Documents\Programacion\GitHub\UEFI_DEV\OVMF.fd"
$vhdPath  = "C:\Users\sambo\Documents\UEFI_Dev\Practica_01\UEFI_VFat32.vhd"

Write-Host "🚀 Lanzando QEMU con OVMF y disco virtual..."
qemu-system-x86_64 `
    -bios $ovmfPath `
    -net none `
    -hda $vhdPath
