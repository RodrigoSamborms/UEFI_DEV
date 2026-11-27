#include "efi.h"

// Función externa definida en msr.s
extern uint64_t ReadMsr(uint32_t MsrIndex);

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

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    (void)ImageHandle;
    SystemTable->ConOut->Reset(SystemTable->ConOut, 0);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"RASPract_004: Exploración de Bancos MCA\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"---------------------------------------\r\n");

    // Leer IA32_MCG_CAP
    uint64_t mcg_cap = ReadMsr(0x179);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_CAP    : ");
    PrintHex(SystemTable->ConOut, mcg_cap);

    // Número de bancos MCA
    uint32_t num_banks = (uint32_t)(mcg_cap & 0xFF);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nNúmero de bancos MCA: ");
    PrintHex(SystemTable->ConOut, num_banks);

    // Recorrer cada banco y leer STATUS, ADDR y MISC
    for (uint32_t i = 0; i < num_banks; i++) {
        uint32_t base = 0x400 + i * 4; // MCi base

        uint64_t mc_status = ReadMsr(base + 1); // MCi_STATUS
        uint64_t mc_addr   = ReadMsr(base + 2); // MCi_ADDR
        uint64_t mc_misc   = ReadMsr(base + 3); // MCi_MISC

        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nBanco MCA #");
        char16_t idx[3] = {u'0' + (i / 10), u'0' + (i % 10), u'\0'};
        SystemTable->ConOut->OutputString(SystemTable->ConOut, idx);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");

        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  STATUS : ");
        PrintHex(SystemTable->ConOut, mc_status);

        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  ADDR   : ");
        PrintHex(SystemTable->ConOut, mc_addr);

        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  MISC   : ");
        PrintHex(SystemTable->ConOut, mc_misc);
    }

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nPrograma finalizado. Saliendo en 5 segundos...\r\n");
    SystemTable->BootServices->Stall(5000000);

    return EFI_SUCCESS;
}
