/*Archivo que agrega el protocolo de salida de texto simple en UEFI
Junto con el protocolo de entrada de texto simple*/
#ifndef EFI_H
#define EFI_H
#include <stdint.h>
#include <uchar.h>
//Definición del modificador para las llamadas a MS ABI en x86
#if defined (_WIN32) || defined (_WIN64)
#define EFIAPI
#else
#define EFIAPI __attribute__((ms_abi))
#endif
//Definición de los tipos de datos básicos de UEFI
typedef unsigned long long UINTN;
typedef uint64_t EFI_STATUS;
typedef void* EFI_HANDLE;
//Códigos de Error y Exito comunes en UEFI
#define EFI_SUCCESS 0
#define EFI_NOT_READY 0x8000000000000006ULL
//Estructuras opacas/incompletas para los servicios que no usaremos aún
typedef struct _EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;
typedef struct _EFI_BOOT_SERVICES EFI_BOOT_SERVICES;
//---- PROTOCOLO DE ENTRADA DE TEXTO (TECLADO) ----
typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
//Estructura para captar la tecla presionada en el teclado, difierenciando si es una tecla normal o una especial
typedef struct {
    uint16_t ScanCode; //Variable que obtiene el código la tecla presionada para flechas y teclas especiales
    char16_t UnicodeChar; //El cáracter ejecutable (ej. 'A', 'b', '\n', etc.)
} EFI_INPUT_KEY; //EFI_INPUT_KEY es la definición de la estructura que será un nuevo tipo de dato
//Función que registra la tecla presionada en el teclado
typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY)(
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,  //Apuntador a la estructura al estilo de C++ en OOP
    EFI_INPUT_KEY *Key //Apuntador a la estructura que almacena la tecla presionada
);
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    void* Reset; //Funcion para reinciar el bufffer de entrada del teclado
    EFI_INPUT_READ_KEY ReadKeyStroke; //Función que lee la tecla presionada
    void* WaitForKey; //Variable que espera captar cuando se presiona la siguiente tecla
};
//---- PROTOCOLO DE SALIDA (PANTALLA/MONITOR) ----
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
//Definicón del la función que maneja al protocolo de salida de texto simple
typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING)(
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, //Apuntador a la estructura al estilo de C++ en OOP
    char16_t *String    //Variable que almacena el mensaje a mostrar en pantalla (atributo de la clase)
);
//Estructura que define las tabla con las variables de entrada que espera el protocolo de salida de texto simple
//solo agregamos las minimas que usaremos
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void* Reset;
    EFI_TEXT_STRING OutputString; //Función que muestra el texto en pantalla
    void* TestString;
    void* QueryMode;
    void* SetMode;
    void* SetAttribute;
    void* ClearScreen;
    void* SetCursorPosition;
    void* EnableCursor;
    void* Mode;
};
//---- TABLA DEL SISTEMA UEFI ----
typedef struct {
    uint64_t Signature;
    uint32_t Revision;
    uint32_t HeaderSize;
    uint32_t CRC32;
    uint32_t Reserved;
} EFI_TABLE_HEADER;//Este es el punto de entrada de los servicios y protocolos que ofrece UEFI
typedef struct {
    EFI_TABLE_HEADER Hdr; //Encabezado de la tabla del sistema UEFI
    char16_t *FrimwareVendor; //Variable que almacena el nombre del fabricante del Firmware
    uint32_t FirmwareRevision; //Variable que guarda la revisión del Firmware para rastreos posteriores
    EFI_HANDLE ConsoleInHandle; //Variable que indica que código maneja la entrada de texto (teclado)
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn; //Variable que apunta al código que conecta al hardware del teclado
    EFI_HANDLE ConsoleOutHandle; //Variable que indica que condigo maneja la salida de texto (monitor)
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut; //Variable que apunta al código que conecta al hardware del monitor
    EFI_HANDLE StandardErrorHandle; //Variable que indica que código maneja la salida de error (archivo)
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr; //Variable que apunta al código que envia los errores a un archivo
    EFI_RUNTIME_SERVICES *RuntimeServices; //Variable que apunta a los servicios de tiempo de ejecución (no implementada)
    EFI_BOOT_SERVICES *BootServices; //Variable que apunta a los servicios de arranque (no implementada)
    UINTN NubmerOfTableEntries; //Variable que guarda el número de entradas en la tabla del sistema UEFI
    void* ConfigurationTable; //Variable que apunta a la tabla de configuración (no implementada)
} EFI_SYSTEM_TABLE;
#endif //Termina el archivo EFI_H