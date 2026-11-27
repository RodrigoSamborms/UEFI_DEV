#include "efi.h"

// Funciones externas en msr.s
extern uint64_t ReadMsr(uint32_t MsrIndex);
extern void WriteMsr(uint32_t MsrIndex, uint64_t ValueLow, uint64_t ValueHigh);

// Función auxiliar para imprimir hex
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

// Simulación de inyección de error recuperable
void InjectRecoverableError(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut) {
    ConOut->OutputString(ConOut, u"\r\nInyectando error recuperable...\r\n");
    // Escribir un valor simulado en MC0_STATUS (ejemplo)
    // Bits: VAL=1, UC=0 (recuperable), ADDRV=1
    uint64_t fake_status = 0x0000000000009405ULL;
    WriteMsr(0x401, (uint32_t)(fake_status & 0xFFFFFFFF), (uint32_t)(fake_status >> 32));
    ConOut->OutputString(ConOut, u"Error recuperable inyectado en MC0_STATUS.\r\n");
}

// Menú interactivo
void ShowMenu(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut) {
    ConOut->OutputString(ConOut, u"\r\n=== Menu RAS Practica ===\r\n");
    ConOut->OutputString(ConOut, u"1. Leer registros MCA\r\n");
    ConOut->OutputString(ConOut, u"2. Inyectar error recuperable\r\n");
    ConOut->OutputString(ConOut, u"3. Salir al shell\r\n");
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    (void)ImageHandle;
    SystemTable->ConOut->Reset(SystemTable->ConOut, 0);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"RASPract_Interactive: Validacion MCA\r\n");

    int option = 0;
    while (option != 3) {
        ShowMenu(SystemTable->ConOut);
        // En una práctica real, aquí se leería entrada de teclado (ConIn).
        // Para simplificar, simulamos opción 1 y 2 secuencialmente.
        if (option == 0) {
            option = 1;
        } else if (option == 1) {
            // Leer IA32_MCG_CAP
            uint64_t mcg_cap = ReadMsr(0x179);
            SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_CAP : ");
            PrintHex(SystemTable->ConOut, mcg_cap);
            option = 2;
        } else if (option == 2) {
            InjectRecoverableError(SystemTable->ConOut);
            option = 3;
        }
    }

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nPrograma finalizado. Retornando al shell...\r\n");
    SystemTable->BootServices->Stall(3000000);

    return EFI_SUCCESS;
}
