//Este código muestra el como usar el protocolo de salida de texto simple en UEFI para mostrar 
//mensajes en pantalla, al esqueleto básico de un programa en UEFI, le agremos las definiciones
//para los servicios que requiramos
#include "efi.h" //Incluimos nuestro archivo de especificación para UEFI propio
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
    //Otorgamos valores simulados para la variable ImageHandle para evitar errores de compilación
    (void) ImageHandle; //Valor de simulación
    //Invocamos la funcion OutputString del protocolo ConOut
    //El prmier arguento obligatorio ('This') es el apuntador al mismo protocolo (estilo OOP)
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hola, Mundo desde UEFI!\n");
    //Creamos un bucle infinito para poder visualizar el mensaje en pantalla
    //y poder leer el mensaje antes de que la maquina se reicicie o apague
    while(1){
        __asm__ __volatile__("hlt"); //Instrucción de ensamblador para detener la CPU
    }
    return 0; 
}