/*Archivo que implementa los sevicios de UEFI de ingresar texto simple
y de salida de texto simple */
#include "efi.h"
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
    (void)ImageHandle; //Creamos una entrada "simulada" para evitar errores de compilación
    //Envia a pantalla la petición de ingresar el nombre de ususario
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Ingresa tu nombre de usuario: ");
    char16_t nombre[100]; //Buffer para almacenar el nombre de usuario ingresado
    UINTN indice=0; //la deficinicion de UINTN viene de efi.h, es un tipo de dato entero sin signo de 64 bits
    char16_t ecoStr[2]= {0, 0}; //Buffer para mostrar los caracteres ingresados en pantalla
                                //Se prevee la combinación de valores {tecla, '\0'} la tecla y el caracter nulo para formar una cadena de texto de un solo caracter
    EFI_INPUT_KEY tecla; //Variable que almacena la tecla presionada
    //Bucle para leer el nombre de usuario caracter por caracter hasta encontrar Enter
    while (indice < 99) { //El arreglo tiene 100 espacios desde 0 a 99
        //Llamamos a la función ReadKeyStroke si no hay techa presionada, devuel ve EFI_NOT_READY
        EFI_STATUS status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &tecla);
        if (status == EFI_SUCCESS){
            //Si presionamos Enter, salimos del bucle while antes de llenar el buffer
            if (tecla.UnicodeChar == '\r') {
                break;
            }
            //Si se teclea un caracter valido (Evitamos teclas especiales sin respesentación imprimible)
            if (tecla.UnicodeChar != 0){
                nombre[indice] = tecla.UnicodeChar; //Guardamos el caracter en el buffer
                indice++; //Movemos el indice a la siguiente casilla
                //Hacemo eco del caracter para que se muestre en pantalla
                ecoStr[0] = tecla.UnicodeChar;
                SystemTable->ConOut->OutputString(SystemTable->ConOut, ecoStr);
            }
        }
    }
    //Añadimos el terminador nulo de cadena al final del buffer del nombre capturado
    nombre[indice] = L'\0';
    //Imprimios el salto de línea antes del saludo final
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");
    //Mostramos el saludo personalizado incluye el nombre del usuario
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hola ");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, nombre);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");
    //Colocamos un While infinito para poder visualizar el mensaje
    while(1){
        __asm__ __volatile__("hlt"); //Instrucción de ensamblador para detener la CPU
        //la instrucción __asm__ puede generar error en el editor pero no en el compilador
    }
    return EFI_SUCCESS; //Usaremos la variable de estado de la ejecución del proceso UEFI
}