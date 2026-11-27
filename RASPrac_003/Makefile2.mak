#uso:
#make #compilar
#make run #ejcutar
#make clean #limpiar binarios

# Makefile para RASPract_002
# Compila efi.c + msr.s en un binario EFI y ejecuta QEMU

TARGET   = RASPract_002.EFI
SRC      = efi.c msr.s
CC       = gcc
CFLAGS   = -nostdlib -ffreestanding -fshort-wchar -mno-red-zone \
           -Wall -Wextra -Wpedantic -std=c11
LDFLAGS  = -Wl,--subsystem,10 -Wl,-e,efi_main -Wl,--image-base,0

# Rutas de firmware y disco virtual (ajusta según tu entorno)
OVMF     = /mnt/c/Users/sambo/Documents/Programacion/GitHub/UEFI_DEV/OVMF.fd
VHD      = /mnt/c/Users/sambo/Documents/UEFI_Dev/Practica_01/UEFI_VFat32.vhd

all: $(TARGET)

$(TARGET): $(SRC)
    $(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: $(TARGET)
    qemu-system-x86_64 -bios $(OVMF) -net none -hda $(VHD)

clean:
    rm -f $(TARGET) *.o
