// Esqueleto Basico de EFI con Lectura de MCA (Ensamblador Inline en sintaxis Intel)
#include "efi.h"

// Función externa definida en nuestro msr.s (la mantenemos como respaldo o referencia)
extern uint64_t ReadMsr(uint32_t MsrIndex);

// NUEVA FUNCIÓN: Lee el MSR usando ensamblador de línea con sintaxis Intel
uint64_t ReadMsrInlineIntel(uint32_t msr_index) {
    uint32_t low, high;

    // Bloque inline asm: pasamos a Intel, ejecutamos rdmsr y regresamos a AT&T
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "rdmsr\n\t"
        ".att_syntax\n\t"
        : "=a" (low), "=d" (high)  // Salida: EAX mapeado a 'low', EDX mapeado a 'high'
        : "c" (msr_index)           // Entrada: 'msr_index' colocado directamente en ECX
    );

    // Recomponemos los dos registros de 32 bits a un entero de 64 bits
    return ((uint64_t)high << 32) | low;
}

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
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Lectura de Registros MCA (Intel SDM Vol 3 - Inline Intel)\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"----------------------------------------------------------\r\n");

    // 1. Leer IA32_MCG_CAP (MSR 0x179) usando la función inline Intel
    uint64_t mcg_cap = ReadMsrInlineIntel(0x179);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_CAP    : ");
    PrintHex(SystemTable->ConOut, mcg_cap);

    // 2. Leer IA32_MCG_STATUS (MSR 0x17A) usando la función inline Intel
    uint64_t mcg_status = ReadMsrInlineIntel(0x17A);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_STATUS : ");
    PrintHex(SystemTable->ConOut, mcg_status);

    // Adicional: Leer el registro de control del primer banco de MCA (IA32_MC0_CTL - MSR 0x400)
    uint64_t mc0_ctl = ReadMsrInlineIntel(0x400);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MC0_CTL    : ");
    PrintHex(SystemTable->ConOut, mc0_ctl);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nPrograma finalizado. Saliendo en 5 segundos...\r\n");

    // Pausa de 5,000,000 microsegundos
    SystemTable->BootServices->Stall(5000000);

    return 0;
}