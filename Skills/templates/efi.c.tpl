/*
 * Esqueleto de efi.c
 * Reemplace {{NAME}} por el nombre del proyecto cuando corresponda.
 */

#include "efi.h"

// Entrada principal UEFI (EDK2 style)
// Ajuste la firma si utiliza otro entorno de construcción.
EFI_STATUS
EFIAPI
UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    // Inicialización mínima
    // TODO: Llamar a servicios de SystemTable->BootServices o RuntimeServices según sea necesario

    return EFI_SUCCESS;
}
