/*
 * Esqueleto de efi.h
 */

#ifndef __EFI_H__
#define __EFI_H__

// Tipos mínimos para compilar como referencia; adapte según su entorno EDK2.
typedef unsigned long long UINT64;
typedef unsigned long UINTN;
typedef unsigned long long EFI_STATUS;

#define EFI_SUCCESS 0

// Prototipo de la entrada
EFI_STATUS EFIAPI UefiMain(void* ImageHandle, void* SystemTable);

#endif // __EFI_H__
