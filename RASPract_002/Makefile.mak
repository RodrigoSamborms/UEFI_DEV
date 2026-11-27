#Uso en la carpeta con los archivos efi.c efi.h msr.h, lanza el comando:
#make
#Esto crea el archivo RASPract_002.EFI
#make clean
#el comando anterior limpia los archivos binarios
#Ejecutar simulacion con:
#qemu-system-x86_64 \
#    -bios /mnt/c/Users/sambo/Documents/Programacion/GitHub/UEFI_DEV/OVMF.fd \
#    -net none \
#    -hda /mnt/c/Users/sambo/Documents/UEFI_Dev/Practica_01/UEFI_VFat32.vhd
#

# Makefile para RASPract_002
# Compila efi.c + msr.s en un binario EFI

TARGET = RASPract_002.EFI
SRC    = efi.c msr.s
CC     = gcc
CFLAGS = -nostdlib -ffreestanding -fshort-wchar -mno-red-zone \
         -Wall -Wextra -Wpedantic -std=c11
LDFLAGS = -Wl,--subsystem,10 -Wl,-e,efi_main -Wl,--image-base,0

all: $(TARGET)

$(TARGET): $(SRC)
    $(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
    rm -f $(TARGET) *.o
