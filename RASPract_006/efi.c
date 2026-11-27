#include "efi.h"

// Funciones externas
extern uint64_t ReadMsr(uint32_t MsrIndex);
extern void WriteMsr(uint32_t MsrIndex, uint64_t ValueLow, uint64_t ValueHigh);
extern void CpuidAsm(uint32_t code, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

void PrintHex(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut, uint64_t value) {
    char16_t buffer[21];
    char16_t hexChars[] = u"0123456789ABCDEF";
    buffer[0] = u'0'; buffer[1] = u'x';
    buffer[18] = u'\r'; buffer[19] = u'\n'; buffer[20] = u'\0';
    for (int i = 15; i >= 0; i--) {
        buffer[2 + i] = hexChars[value & 0xF];
        value >>= 4;
    }
    ConOut->OutputString(ConOut, buffer);
}

void PrintDec(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut, uint32_t value) {
    char16_t buffer[16];
    int i = 14;
    buffer[15] = u'\0';
    do {
        buffer[i--] = u'0' + (value % 10);
        value /= 10;
    } while (value > 0);
    ConOut->OutputString(ConOut, &buffer[i+1]);
    ConOut->OutputString(ConOut, u"\r\n");
}

void ShowCpuidMca(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut) {
    uint32_t a,b,c,d;
    CpuidAsm(1, &a, &b, &c, &d);

    ConOut->OutputString(ConOut, u"\r\n=== CPUID Función 1 ===\r\n");
    ConOut->OutputString(ConOut, u"EAX (Versión/Familia/Modelo): ");
    PrintHex(ConOut, a);

    ConOut->OutputString(ConOut, u"EDX Flags: ");
    PrintHex(ConOut, d);

    // Bit 7: MCE
    ConOut->OutputString(ConOut, (d & (1<<7)) ? u"Bit7 MCE=1 (Machine Check Exception soportada)\r\n"
                                              : u"Bit7 MCE=0 (No soportada)\r\n");

    // Bit 14: MCA
    ConOut->OutputString(ConOut, (d & (1<<14)) ? u"Bit14 MCA=1 (Machine Check Architecture soportada)\r\n"
                                               : u"Bit14 MCA=0 (No soportada)\r\n");

    if (d & (1<<14)) {
        uint64_t mcg_cap = ReadMsr(0x179);
        ConOut->OutputString(ConOut, u"IA32_MCG_CAP: ");
        PrintHex(ConOut, mcg_cap);

        uint32_t num_banks = (uint32_t)(mcg_cap & 0xFF);
        ConOut->OutputString(ConOut, u"Número de bancos MCA: ");
        PrintDec(ConOut, num_banks);
    }
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    (void)ImageHandle;
    SystemTable->ConOut->Reset(SystemTable->ConOut, 0);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"RASPract_CPUID_MCA: Exploración de capacidades\r\n");

    ShowCpuidMca(SystemTable->ConOut);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nPrograma finalizado. Retornando al shell...\r\n");
    SystemTable->BootServices->Stall(3000000);

    return EFI_SUCCESS;
}
