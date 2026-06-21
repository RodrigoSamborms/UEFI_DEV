/*Este archivo de especificación para UEFI, toma el esqueleto básico y habilita la función para
Mostrar Texto En Pantalla, por lo que definiremos el protocolo de salida de texto simple*/
#ifndef EFI_H
#define EFI_H
#include <stdint.h> //Libreria para tipos de datos enteros con tamaños específicos
#include <uchar.h> //Libreria para tipos de datos de caracteres Unicode
//Tipos de datos básicos para UEFI
typedef unsigned long long UINT;
typedef uint64_t EFI_STATUS; //la variable EFI_STATUS mide 64 bits y guarda el estado de las operaciones en UEIF
typedef void* EFI_HANDLE; //Variable apuntador para código que maneja controladores y protocolos en UEFI de tamaño no indicado
//Modificador para la convención de llamada de UEFI, asegura que el compilador se adhiera al estandar
#if defined(_WIN32) || defined (_WIN64)
#define EFIAPI                          //define las llamadas a la convencion MS ABI
#else
#define EFIAPI __attribute__((ms_abi))
#endif
//Estructuras Incompletas para los serivicios que no usaremso aún
typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct _EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;
typedef struct _EFI_BOOT_SERVICES EFI_BOOT_SERVICES;
//---- DEFINICION DEL PROTOCOLO DE SALIDA DE TEXTO SIMPLE ----
//Defineción de los tipos de datos para las funciones del protocolo de salida que usaremsos en esta ocasión
typedef struct  _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
//La función OUTPUT_STRING requiere dos apuntadores uno al protocolo de salida de texto simple
//Y otro a l acadena de texto en UTF-16 unicode
typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING) ( //definicion de un apuntador a una función con el alias EFI_TEXT_STRING
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,  //Llamada a la funcion al estilo de C++, a un objeto de la misma clase como
    char16_t                        *String //listas enlazadas, y tiene un paraemtro String que guarda el dato (atributo)
);
//Definición de la estructura del protocolo de salida de texto simple
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void* Reset;    //No usaremos el protocolo de reinicio, pero debemos definirlo y usaremos void* para indicar
                    //que no tiene un tamaño específico asignado.
    EFI_TEXT_STRING OutputString; //La funcion que necesitamos outputstring apunta a la definición de la función
    void* TestString;           //Los siguientes parametros pueden explicarse por su nombre si mantenemos
    void* QueryMode;            //en mente que estamos escribiendo texto en la pantalla
    void* SetMode;
    void* SetAttribute;
    void* ClearScreen;
    void* SetCursorPosition;
    void* EnableCursor;
    void* Mode; 
};
//Estructura de encabezado Standard
typedef struct {
    uint64_t Signature;
    uint32_t Revision;
    uint32_t HeaderSize;
    uint32_t CRC32;
    uint32_t Reserverd;
} EFI_TABLE_HEADER;
//Tabla del sistema (System Table) con el protocolo mapeado
//En UEFI, la tabla de sistema es el punto de entrada para acceder a los serivicios
//Mediante los protocolos definidos previamente
//cada entrada en la tabla indica quien se encargara de manejar las funciones que se ofreceran
//En este caso, solamente exite la funcion CONOUT la función CONIN esta simplemente indicada mas arrbia
//ya que solamente definimos el protocolo de salida de texto simple para esta ocasión
typedef struct{
    EFI_TABLE_HEADER Hdr;   //Variable del tipo de datos EFI_TABLE_HEADER
                            //estructura que fue definida previamente y que mide 192 bits de tamaño
    char16_t *FirmwareVendor;
    uint32_t FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    EFI_HANDLE ConsoloeOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
    EFI_RUNTIME_SERVICES *RuntimeServices;
    EFI_BOOT_SERVICES *BootServices;
    UINT NumberOfTableEntries;
    void* ConfigurationTable;
} EFI_SYSTEM_TABLE;
#endif //EFI_H termina