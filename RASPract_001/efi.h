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