EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    (void)ImageHandle;
    SystemTable->ConOut->Reset(SystemTable->ConOut, 0);

    // 1. Mensaje inicial y pausa antes de leer
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Esperando 10 segundos... INYECTA EL ERROR AHORA.\r\n");
    
    // Pausa de 10 segundos para darte tiempo de abrir el monitor
    SystemTable->BootServices->Stall(10000000); 

    // Si la interrupción no colgara el sistema, veríamos esto:
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nLectura de Registros MCA...\r\n");
    
    uint64_t mcg_cap = ReadMsr(0x179);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_CAP    : ");
    PrintHex(SystemTable->ConOut, mcg_cap);

    uint64_t mcg_status = ReadMsr(0x17A);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"IA32_MCG_STATUS : ");
    PrintHex(SystemTable->ConOut, mcg_status);

    SystemTable->BootServices->Stall(5000000);
    return 0;
}