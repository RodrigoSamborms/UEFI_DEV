// main.c o EFI.c
#include "efi.h"

// Función auxiliar para leer una cadena de caracteres desde el teclado de forma síncrona
void LeerLinea(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *Buffer, uint32_t MaxLongitud) {
    EFI_INPUT_KEY Key;
    uint32_t Indice = 0;
    CHAR16 EchoStr[2] = {0, 0}; // Buffer temporal para hacer eco de un solo carácter

    while (Indice < (MaxLongitud - 1)) {
        // Bloquear el flujo esperando a que se presione una tecla
        // ReadKeyStroke devuelve un estado diferente a EFI_SUCCESS si no hay tecla en el buffer
        while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) != EFI_SUCCESS) {
            // Espera activa (polling) de bajo nivel
        }

        // Si se presiona Enter (\r o \n dependiendo del firmware), terminamos la lectura
        if (Key.UnicodeChar == L'\r' || Key.UnicodeChar == L'\n') {
            break;
        }

        // Manejo básico de Backspace (Retroceso) para permitir corregir
        if (Key.UnicodeChar == L'\b') {
            if (Indice > 0) {
                Indice--;
                SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\b"); // Retrocede el cursor
            }
            continue;
        }

        // Si es un carácter imprimible válido, lo guardamos y lo mostramos en pantalla
        if (Key.UnicodeChar != 0) {
            Buffer[Indice++] = Key.UnicodeChar;
            EchoStr[0] = Key.UnicodeChar;
            SystemTable->ConOut->OutputString(SystemTable->ConOut, EchoStr);
        }
    }

    // Aseguramos el terminador nulo de la cadena de texto UTF-16
    Buffer[Indice] = L'\0';
}

EFI_STATUS EFIAPI EfiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    (void)ImageHandle;
    
    // Buffer para almacenar el nombre introducido por el usuario
    CHAR16 NombreUsuario[50];

    // 1. Preguntar el nombre
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Cual es tu nombre?\r\n-> ");

    // 2. Capturar la entrada del teclado
    LeerLinea(SystemTable, NombreUsuario, 50);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n"); // Salto de línea estético tras el Enter

    // 3. Imprimir el saludo personalizado estructurando la salida
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hola ");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, NombreUsuario);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L" buen dia\r\n");

    return EFI_SUCCESS;
}
