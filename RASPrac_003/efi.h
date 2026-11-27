#ifndef EFI_H
#define EFI_H

#include <stdint.h>

typedef uint8_t   BOOLEAN;
typedef uint64_t  UINTN;
typedef uint64_t  EFI_STATUS;
typedef void      VOID;
typedef VOID     *EFI_HANDLE;

#define EFI_SUCCESS 0ULL
#define EFIAPI __attribute__((ms_abi))

// Protocolo de salida de texto simple
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET)(
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    BOOLEAN ExtendedVerification
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING)(
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    char16_t *String
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_CLEAR_SCREEN)(
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
);

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET        Reset;
    EFI_TEXT_STRING       OutputString;
    EFI_TEXT_CLEAR_SCREEN ClearScreen;
};

// Servicios de arranque mínimos
typedef struct EFI_BOOT_SERVICES {
    VOID *Hdr;
    VOID *RaiseTPL;
    VOID *RestoreTPL;
    VOID *AllocatePages;
    VOID *FreePages;
    VOID *GetMemoryMap;
    VOID *AllocatePool;
    VOID *FreePool;
    VOID *CreateEvent;
    VOID *SetTimer;
    VOID *WaitForEvent;
    VOID *SignalEvent;
    VOID *CloseEvent;
    VOID *CheckEvent;
    VOID *InstallProtocolInterface;
    VOID *ReinstallProtocolInterface;
    VOID *UninstallProtocolInterface;
    VOID *HandleProtocol;
    VOID *Reserved;
    VOID *RegisterProtocolNotify;
    VOID *LocateHandle;
    VOID *LocateDevicePath;
    VOID *InstallConfigurationTable;
    VOID *ImageLoad;
    VOID *ImageStart;
    VOID *Exit;
    VOID *ImageUnload;
    VOID *ExitBootServices;
    VOID *GetNextMonotonicCount;
    VOID *Stall; // <- Necesario para pausar ejecución
} EFI_BOOT_SERVICES;

// Tabla del sistema mínima
typedef struct {
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_BOOT_SERVICES *BootServices;
} EFI_SYSTEM_TABLE;

// Punto de entrada de aplicación UEFI
typedef EFI_STATUS (EFIAPI *EFI_IMAGE_ENTRY_POINT)(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
);

#endif // EFI_H
