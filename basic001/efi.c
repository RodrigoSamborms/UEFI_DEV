//Esqueleto Basico de EFI
#include "efi.h" //Incluimos nuestro archivo con las deficiones de UEFI acorde a la especificación
//El punto de entrada para la ejecución de un programa en EUFI es  UEFI_MAIN
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    //Como es un es un "esqueleto" básico, evitaremos errores de compilación al no usar variables
    //Por lo que les agregaremos valores "dummies"
    (void)ImageHandle; //Convierte al tipo Void la variable ImageHandle
    (void)SystemTable; //El tipo void no tiene tamaño definido
    //Regresaremos 0 para la función en este caso
    return 0;
}