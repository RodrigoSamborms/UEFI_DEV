#ifndef UEFI_H
#define UEFI_H
#include <stdint.h>
#include <uchar.h>
//Modificador para la convención de llamadas (MS ABI en x86)
#if defined(_WIN32) || defined(_WIN64)
#define EFIAPI __attribute__((ms_abi))
#else
#define EFIAPI
#endif
//Tipos de datos básicos en UEFI
typedef unsigned long long UINTN;
typedef uint64_t EFI_STATUS;
typedef void* EFI_HANDLE;
//Estructuras incompletas (opacas) para cumplir con el registro de la tabla
typedef struct _EFI_SIMPL_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_OUPTUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct _EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;
typedef struct _EFI_BOOT_SERVICES EFI_BOOT_SERVICES;
//Definición dela tabla obligatoria de encabezado en UEFI
typedef struct {
    uint64_t Signature;     // Firma para identificar la tabla
    uint32_t Revision;      //versión de la tabla para rastreo de cambios
    uint32_t HeaderSize;    //Tamaño de la tabla en bytes para validación
    uint32_t CRC32;         //protocolo usado para verificar la intergridad de la tabla
    uint32_t Reserved;      //Reservado para usos futuros en la especificación
} EFI_TABLE_HEADER;         //Tamaño de 196 bytes
//Estructura limpia de la EFI_SYSTEM_TABLE (sin servicios detallados)
typedef struct {
    EFI_TABLE_HEADER Hdr; //Varible encabezado del tipo EFI_TABLE_HEADER
    char16_t* FirmwareVendor; //Apuntador a cadena de texto con el nombre del fabricante del firmware
    uint32_t FirmwareRevision; //Número de revisión del firmware, util para rastreo de cambios
    EFI_HANDLE  ConsoleInHandle;//Indica que código maneja la entrada del hardware de texto, como el teclado
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn; //Apuntador al código que maneja la entrada de texto
    EFI_HANDLE ConsoleOutHandle; //Indica que código maneja la salida del hardware de texto, como el monitor
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut; //Apuntador al código que maneja la salida de texto
    EFI_HANDLE StandardErrorHandle; //Indica que código maneja la salida a hardware de texto de errores, hacia un archivo
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr; //Apuntador al código que maneja la salida de texto para los errores
    EFI_RUNTIME_SERVICES* RuntimeServices; //Apuntador a la tabla de servicios de tiempo de ejecución una etapa del BIOS
    EFI_BOOT_SERVICES* BootServices; //Apuntado a la tabla de servicios de arranque una etapa del BIOS
    UINTN NumberOfTableEntries; //Contador de las entradas a la tabla de configuración dle sistema
    void* ConfigurationTable; //Apuntador a la tabla de configuración del sistema.
    //Esas son las partes requeridas (obligatorias)por la especificación de UEFI.
} EFI_SYSTEM_TABLE; //Tabla de 216 bytes

#endif // UEFI_H
