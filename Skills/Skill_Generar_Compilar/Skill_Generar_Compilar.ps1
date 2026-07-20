param(
    [Parameter(Mandatory=$true)]
    [ValidateSet('generate','build','all')]
    [string]$Action,

    [Parameter(Mandatory=$false)]
    [string]$FolderName = 'archivos_00',

    [Parameter(Mandatory=$false)]
    [string]$Compiler = 'gcc'
)

$ErrorActionPreference = 'Stop'

function Resolve-Gcc {
    param([string]$Preferred = 'gcc')
    if ($Preferred -ne 'gcc' -and (Test-Path $Preferred)) { return $Preferred }
    $candidates = @(
        'C:\msys64\ucrt64\bin\gcc.exe',
        'C:\msys64\mingw64\bin\gcc.exe',
        'C:\msys64\usr\bin\gcc.exe'
    )
    foreach ($c in $candidates) { if (Test-Path $c) { return $c } }
    return $Preferred
}

function New-SkillFiles {
    param([string]$Dst)
    if (-not (Test-Path $Dst)) { New-Item -ItemType Directory -Path $Dst -Force | Out-Null }

    $efiC = @'
// Esqueleto Basico de EFI con Lectura de MCA
#include "efi.h"

// Función externa definida en nuestro msr.s
extern uint64_t ReadMsr(uint32_t MsrIndex);

// Función auxiliar para imprimir el registro
void PrintHex(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut, uint64_t value) {
    char16_t buffer[21];
    char16_t hexChars[] = u"0123456789ABCDEF";
    
    buffer[0] = u'0'; 
    buffer[1] = u'x'; 
    buffer[18] = u'\r'; 
    buffer[19] = u'\n'; 
    buffer[20] = u'\0';
    
    for (int i = 15; i >= 0; i--) {
        buffer[2 + i] = hexChars[value & 0xF];
        value >>= 4;
    }
    
    ConOut->OutputString(ConOut, buffer);
}

// El punto de entrada para la ejecución de un programa en UEFI
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    (void)ImageHandle;

    // Limpieza de pantalla
    SystemTable->ConOut->Reset(SystemTable->ConOut, 0);

    // Impresión de encabezado
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Lectura de Registros MCA (Intel SDM Vol 3)\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"------------------------------------------\r\n");

    // Leer IA32_MCG_CAP (MSR 0x179)
    uint64_t mcg_cap = ReadMsr(0x179);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_CAP    : ");
    PrintHex(SystemTable->ConOut, mcg_cap);

    // Leer IA32_MCG_STATUS (MSR 0x17A)
    uint64_t mcg_status = ReadMsr(0x17A);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_STATUS : ");
    PrintHex(SystemTable->ConOut, mcg_status);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nPrograma finalizado. Saliendo en 5 segundos...\r\n");

    // Pausa de 5,000,000 microsegundos
    SystemTable->BootServices->Stall(5000000);

    return 0;
}
'@

    $efiH = @'
#ifndef UEFI_H
#define UEFI_H
#include <stdint.h>
#include <uchar.h>

// Modificador para la convención de llamadas (MS ABI en x86)
#if defined(_WIN32) || defined(_WIN64)
#define EFIAPI __attribute__((ms_abi))
#else
#define EFIAPI
#endif

// Tipos de datos básicos en UEFI
typedef unsigned long long UINTN;
typedef uint64_t EFI_STATUS;
typedef void* EFI_HANDLE;

// Estructura expandida para salida de texto
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_STATUS (EFIAPI *Reset)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*, uint8_t);
    EFI_STATUS (EFIAPI *OutputString)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*, char16_t*);
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct _EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;

// Definición de la tabla obligatoria de encabezado en UEFI
typedef struct {
    uint64_t Signature;     // Firma para identificar la tabla
    uint32_t Revision;      // versión de la tabla para rastreo de cambios
    uint32_t HeaderSize;    // Tamaño de la tabla en bytes para validación
    uint32_t CRC32;         // protocolo usado para verificar la intergridad de la tabla
    uint32_t Reserved;      // Reservado para usos futuros en la especificación
} EFI_TABLE_HEADER;         // Tamaño de 24 bytes

// Expandimos los servicios de arranque saltando los punteros previos a Stall
typedef struct _EFI_BOOT_SERVICES {
    EFI_TABLE_HEADER Hdr;
    void* ReservedPointers[28]; // Salto de los 28 servicios previos según la spec UEFI
    EFI_STATUS (EFIAPI *Stall)(UINTN Microseconds);
} EFI_BOOT_SERVICES;

// Estructura limpia de la EFI_SYSTEM_TABLE
typedef struct {
    EFI_TABLE_HEADER Hdr; 
    char16_t* FirmwareVendor; 
    uint32_t FirmwareRevision; 
    EFI_HANDLE ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn; 
    EFI_HANDLE ConsoleOutHandle; 
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut; 
    EFI_HANDLE StandardErrorHandle; 
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr; 
    EFI_RUNTIME_SERVICES* RuntimeServices; 
    EFI_BOOT_SERVICES* BootServices; 
    UINTN NumberOfTableEntries; 
    void* ConfigurationTable; 
} EFI_SYSTEM_TABLE;

#endif // UEFI_H
'@

    $msrS = @'
.intel_syntax noprefix
.text
.global ReadMsr

ReadMsr:
    # RCX contiene el primer argumento (MsrIndex)
    # rdmsr lee el MSR especificado en ECX
    rdmsr
    
    # rdmsr guarda el resultado dividido en dos registros de 32 bits:
    # EDX (parte alta) y EAX (parte baja).
    # Necesitamos empaquetarlo en RAX para devolver un UINT64.
    shl rdx, 32
    or rax, rdx
    
    ret
'@

    Set-Content -Path (Join-Path $Dst 'efi.c') -Value $efiC -Encoding UTF8
    Set-Content -Path (Join-Path $Dst 'efi.h') -Value $efiH -Encoding UTF8
    Set-Content -Path (Join-Path $Dst 'msr.s') -Value $msrS -Encoding UTF8
    Write-Host "Generados: $Dst\efi.c, $Dst\efi.h, $Dst\msr.s"
}

function Build-SkillApp {
    param([string]$Dst)
    Push-Location $Dst
    try {
        # Use exactly the requested xargList and invoke `& gcc @xargList` so it can be copy-pasted
        $xargList = @(
            'efi.c',
            'msr.s',  # Incluimos el archivo que contiene la rutina ReadMsr
            '-o', 'Esqueleto00.EFI',
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

        Write-Host "Ejecutando: & $Compiler $($xargList -join ' ')"
        # Direct invocation as requested
        & $Compiler @xargList 2>&1 | Tee-Object build_out.log
        $exit = $LASTEXITCODE
        if ($exit -ne 0) { throw "gcc finalizó con código $exit" }
        if (Test-Path 'Esqueleto00.EFI') { Write-Host 'Compilación exitosa: Esqueleto00.EFI' }
    }
    finally { Pop-Location }
}

$root = (Get-Location).Path
$dstPath = Join-Path $root $FolderName

switch ($Action) {
    'generate' { New-SkillFiles -Dst $dstPath }
    'build'    { Build-SkillApp -Dst $dstPath }
    'all'      { New-SkillFiles -Dst $dstPath; Build-SkillApp -Dst $dstPath }
}
