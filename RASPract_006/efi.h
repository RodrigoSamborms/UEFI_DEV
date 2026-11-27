#ifndef EFI_H
#define EFI_H

#include <stdint.h>

typedef uint64_t EFI_STATUS;
typedef void* EFI_HANDLE;
#define EFI_SUCCESS 0ULL
#define EFIAPI __attribute__((ms_abi))

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*, uint8_t);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*, char16_t*);

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET Reset;
    EFI_TEXT_STRING OutputString;
};

typedef struct EFI_BOOT_SERVICES {
    void *Hdr;
    void *Reserved[27];
    void *Stall;
} EFI_BOOT_SERVICES;

typedef struct {
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_BOOT_SERVICES *BootServices;
} EFI_SYSTEM_TABLE;

#endif
