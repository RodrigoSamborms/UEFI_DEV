// Esqueleto Basico de EFI con Lectura de MCA
#include "efi.h"

// Función externa definida en nuestro msr.s
extern uint64_t ReadMsr(uint32_t MsrIndex);

// Función auxiliar para imprimir el registro
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

// El punto de entrada para la ejecución de un programa en UEFI
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    (void)ImageHandle;

    // Limpieza de pantalla
    SystemTable->ConOut->Reset(SystemTable->ConOut, 0);

    // Impresión de encabezado
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

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nPrograma finalizado. Saliendo en 5 segundos...\r\n");

    // Pausa de 5,000,000 microsegundos
    SystemTable->BootServices->Stall(5000000);

    return 0;
}