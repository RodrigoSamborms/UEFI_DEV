#include "efi.h"

// Función externa definida en msr.s
extern uint64_t ReadMsr(uint32_t MsrIndex);

// Función auxiliar para imprimir en hexadecimal
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

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Lectura de Registros MCA (Intel SDM Vol 3)\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"------------------------------------------\r\n");

    // Leer IA32_MCG_CAP (MSR 0x179)
    uint64_t mcg_cap = ReadMsr(0x179);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_CAP    : ");
    PrintHex(SystemTable->ConOut, mcg_cap);

    // Leer IA32_MCG_STATUS (MSR 0x17A)
    uint64_t mcg_status = ReadMsr(0x17A);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_STATUS : ");
    PrintHex(SystemTable->ConOut, mcg_status);

    // Número de bancos MCA soportados (bits 0–7 de MCG_CAP)
    uint32_t num_banks = (uint32_t)(mcg_cap & 0xFF);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nBancos MCA detectados: ");
    PrintHex(SystemTable->ConOut, num_banks);

    // Recorrer cada banco y leer IA32_MC{i}_STATUS
    for (uint32_t i = 0; i < num_banks; i++) {
        uint32_t msr_index = 0x401 + i * 4; // Base de MC0_STATUS
        uint64_t mc_status = ReadMsr(msr_index);

        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MC");
        // Imprimir índice del banco
        char16_t idx[6];
        idx[0] = u'0' + (i / 10);
        idx[1] = u'0' + (i % 10);
        idx[2] = u'_';
        idx[3] = u'S';
        idx[4] = u'T';
        idx[5] = u'\0';
        SystemTable->ConOut->OutputString(SystemTable->ConOut, idx);

        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"ATUS : ");
        PrintHex(SystemTable->ConOut, mc_status);
    }

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nPrograma finalizado. Saliendo en 5 segundos...\r\n");
    SystemTable->BootServices->Stall(5000000);

    return EFI_SUCCESS;
}
