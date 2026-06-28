// efi.h
#ifndef EFI_H
#define EFI_H

#include <stdint.h>

typedef void* EFI_HANDLE;
typedef uint64_t          EFI_STATUS;
typedef uint16_t          CHAR16;

#define EFI_SUCCESS       0

#define EFIAPI __attribute__((ms_abi))

// --- PROTOCOLO DE ENTRADA (TECLADO) ---
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct {
    uint16_t ScanCode;
    CHAR16   UnicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS (EFIAPI *EFI_INPUT_RESET)(
    struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
    uint8_t ExtendedVerification
);

typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY)(
    struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
    EFI_INPUT_KEY *Key
);

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_INPUT_RESET    Reset;
    EFI_INPUT_READ_KEY ReadKeyStroke; // Función para leer la tecla presionada
    void               *WaitForKey;   // Evento para esperar entrada (no requerido para este ejemplo)
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

// --- PROTOCOLO DE SALIDA (PANTALLA) ---
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    CHAR16 *String
);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void             *Reset;
    EFI_TEXT_STRING  OutputString;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// --- TABLA DEL SISTEMA ---
typedef struct {
    char                            Header[24];
    CHAR16                          *FirmwareVendor;
    uint32_t                        FirmwareRevision;
    EFI_HANDLE                      ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;             // Ahora usaremos este puntero
    EFI_HANDLE                      ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
} EFI_SYSTEM_TABLE;

#endif // EFI_H
