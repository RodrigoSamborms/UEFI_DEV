// efi.h
#ifndef EFI_H
#define EFI_H

#include <stdint.h>

// Definiciones básicas de tipos UEFI
typedef void* EFI_HANDLE;
typedef uint64_t          EFI_STATUS;
typedef uint16_t          CHAR16; // UEFI usa codificación UTF-16 para texto

#define EFI_SUCCESS       0

// Atributo de convención de llamadas de Microsoft (requerido por UEFI en x86_64)
#define EFIAPI __attribute__((ms_abi))

// Estructura del protocolo de salida de texto simple
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    CHAR16 *String
);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void             *Reset;
    EFI_TEXT_STRING  OutputString; // El puntero a la función que nos interesa
    // Existen más campos aquí, pero para el "Hola Mundo" nos basta con llegar a OutputString
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// Estructura de la Tabla del Sistema de UEFI (System Table)
typedef struct {
    char                             Header[24]; // Ignoramos la cabecera estándar por simplicidad
    CHAR16                           *FirmwareVendor;
    uint32_t                         FirmwareRevision;
    EFI_HANDLE                       ConsoleInHandle;
    void                             *ConIn;
    EFI_HANDLE                       ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut; // Puntero a la salida estándar (pantalla)
    // El resto de la tabla (ConErr, RuntimeServices, BootServices) no lo necesitamos para este ejemplo
} EFI_SYSTEM_TABLE;

#endif // EFI_H
