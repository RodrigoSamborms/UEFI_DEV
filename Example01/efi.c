// main.c
#include "efi.h"

EFI_STATUS EFIAPI EfiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    // Evitamos advertencia de variable no usada
    (void)ImageHandle;

    // Invocamos OutputString a través del protocolo ConOut de la System Table
    // El prefijo L indica una cadena de caracteres anchos (UTF-16)
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hola Mundo desde el firmware UEFI!\r\n");

    // Retornamos éxito para mantener el control o regresar a la shell de UEFI
    return EFI_SUCCESS;
}
