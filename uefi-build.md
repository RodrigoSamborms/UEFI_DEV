# Compilar y ejecutar aplicaciones UEFI en QEMU

Esta guía describe cómo compilar una aplicación UEFI x86-64 con GCC y ejecutarla en QEMU con firmware OVMF. Los comandos principales son para PowerShell en Windows; al final se incluye una alternativa para Linux.

## Requisitos

- GCC para x86-64 con soporte para generar ejecutables PE/COFF (por ejemplo, MinGW-w64), disponible como `gcc` en `PATH`.
- QEMU x86-64, disponible como `qemu-system-x86_64` en `PATH`.
- Firmware OVMF, por ejemplo el archivo `OVMF.fd` incluido en la raíz de este repositorio.
- Una imagen de disco FAT32 accesible desde QEMU, por ejemplo `UEFI_VFat32.vhd`.
- PowerShell en Windows. En Linux, usa un firmware OVMF y una imagen FAT adecuados para esa plataforma.

Comprueba que las herramientas estén disponibles:

```powershell
gcc --version
qemu-system-x86_64 --version
```

## Compilar una aplicación

Abre PowerShell en la carpeta del proyecto. Para compilar `efi.c` junto con la rutina en ensamblador `msr.s` y producir `Esqueleto00.EFI`:

```powershell
$xargList = @(
    'efi.c',
    'msr.s',
    '-o', 'Esqueleto00.EFI',
    '-nostdlib',
    '-ffreestanding',
    '-fshort-wchar',
    '-mno-red-zone',
    '-Wall',
    '-Wextra',
    '-Wpedantic',
    '-std=c11',
    '-Wl,--subsystem,10',
    '-Wl,-e,efi_main',
    '-Wl,--image-base,0'
)
& gcc @xargList
if ($LASTEXITCODE -ne 0) { throw "La compilación falló (código $LASTEXITCODE)." }
```

Ejecuta el comando desde la carpeta que contiene ambos archivos. Si los fuentes están en otra carpeta, cambia las rutas; por ejemplo, para `RASPRact_005`:

```powershell
$xargList[0] = '.\RASPRact_005\efi.c'
$xargList[1] = '.\RASPRact_005\msr.s'
$xargList[3] = '.\RASPRact_005\Esqueleto00.EFI'
& gcc @xargList
if ($LASTEXITCODE -ne 0) { throw "La compilación falló (código $LASTEXITCODE)." }
```

El fuente C puede incluir otros archivos del proyecto, como `efi.h`; GCC los encontrará si están en la misma carpeta que `efi.c`.

También existe un script para compilar la aplicación raíz `efi.c` como `Hello.EFI`:

```powershell
.\build-uefi.ps1
```

El script usa `-std=c17`; usa el comando directo anterior si necesitas reproducir exactamente las opciones `-std=c11` o compilar otra aplicación.

## Preparar el disco virtual

La aplicación debe estar en una ubicación desde la cual OVMF pueda iniciarla. Para arranque automático, coloca el archivo compilado en la imagen FAT32 con esta ruta estándar:

```text
\EFI\BOOT\BOOTX64.EFI
```

Puedes usar el nombre `BOOTX64.EFI` al copiarlo a esa ruta. Si la imagen no tiene un cargador de arranque configurado, abre el UEFI Shell en QEMU, localiza el sistema de archivos (por ejemplo `FS0:`) y ejecuta la aplicación manualmente:

```text
FS0:
\EFI\BOOT\BOOTX64.EFI
```

La imagen debe estar desmontada en Windows antes de abrirla en QEMU si Windows la tiene adjunta. El script `build-uefi.ps1` admite `-DismountVhd` para desmontar la imagen predeterminada `.\UEFI_VFat32.vhd`.

## Ejecutar en QEMU

Desde PowerShell, ajusta las rutas a OVMF y al disco virtual:

```powershell
$ovmf = (Resolve-Path '.\OVMF.fd').Path
$disk = (Resolve-Path '.\UEFI_VFat32.vhd').Path
qemu-system-x86_64 -machine q35 -m 256M -bios $ovmf -net none -drive "format=raw,file=$disk"
```

Con las rutas absolutas indicadas, ejecuta en PowerShell:

```powershell
qemu-system-x86_64 -machine q35 -m 256M -bios "C:\Users\sambo\Documents\UEFI_Dev\OVMF.fd" -net none -drive "format=raw,file=C:\Users\sambo\Documents\UEFI_Dev\Practica_01\UEFI_VFat32.vhd"
```

Para usar el monitor de QEMU en la misma terminal y dejar la máquina pausada antes de iniciar la ejecución:

```powershell
$ovmf = (Resolve-Path '.\OVMF.fd').Path
$disk = (Resolve-Path '.\UEFI_VFat32.vhd').Path
qemu-system-x86_64 -machine q35 -m 256M -bios $ovmf -net none -drive "format=raw,file=$disk" -monitor stdio -S
```

`-S` pausa las CPU al arrancar. En el prompt `(qemu)`, escribe `c` y presiona Enter para reanudar. Si prefieres usar la ventana gráfica, omite `-monitor stdio -S`; `-monitor stdio` usa la terminal como monitor y permite introducir comandos ahí.

Puedes añadir `-nographic` para ejecutar sin la ventana gráfica. En ese modo la salida de video no está disponible; úsalo si tu firmware y aplicación muestran la consola por serie.

### Ejecutar en Linux

Usa las rutas de firmware y disco disponibles en tu sistema:

```sh
qemu-system-x86_64 \
  -machine q35 \
  -m 256M \
  -bios /ruta/a/OVMF.fd \
  -net none \
  -drive format=raw,file=/ruta/a/UEFI_VFat32.vhd
```

Para controlar el monitor desde la terminal y comenzar pausado, añade `-monitor stdio -S`.

Si tu distribución instala OVMF como imágenes separadas de código y variables, localízalas primero:

```sh
find /usr/share -type f \( -name 'OVMF_CODE*.fd' -o -name 'OVMF_VARS*.fd' \) 2>/dev/null
```

Usa una copia escribible de la imagen de variables para conservar la configuración UEFI entre ejecuciones. Ajusta las rutas según el resultado de la búsqueda:

```sh
cp /usr/share/OVMF/OVMF_VARS.fd "$HOME/OVMF_VARS.fd"
qemu-system-x86_64 \
  -machine q35 \
  -m 256M \
  -net none \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
  -drive if=pflash,format=raw,file="$HOME/OVMF_VARS.fd" \
  -drive format=raw,file=/ruta/a/UEFI_VFat32.vhd
```

## Inyectar un estado MCE desde el monitor

Este procedimiento sirve para probar una aplicación que lea el registro `IA32_MCG_STATUS` (`0x17A`) mediante `ReadMsr(0x17A)`. La aplicación debe mostrar ese registro después de reanudar la máquina; una aplicación que lea otros MSR no mostrará necesariamente el valor inyectado aquí.

1. Inicia QEMU con `-monitor stdio -S` como se indica arriba. El prompt `(qemu)` aparecerá en la terminal y la CPU permanecerá pausada.
2. Introduce el comando MCE:

   ```text
   (qemu) mce 0 1 0x2000000000000000 0x5 0x0 0x0
   ```

   La sintaxis es `mce <cpu> <bank> <status> <mcg_status> <addr> <misc>`. En el ejemplo se selecciona la CPU 0 y el banco 1; `0x5` es el valor solicitado para `mcg_status`, y `addr` y `misc` son cero.
3. Reanuda QEMU:

   ```text
   (qemu) c
   ```

4. Observa la salida de la aplicación en la ventana de QEMU. El resultado depende de que la aplicación lea e imprima `IA32_MCG_STATUS` y de que la versión de QEMU admita el comando `mce`.

También puedes probar este valor alternativo para `status`:

```text
(qemu) mce 0 1 0xA000000000000000 0x5 0x0 0x0
```

Si QEMU muestra que `mce` no es un comando reconocido, consulta `(qemu) help` y la documentación de la versión instalada: la disponibilidad del comando puede variar entre compilaciones. Si iniciaste con la ventana gráfica y un monitor compatible, `Ctrl+Alt+2` cambia al monitor y `Ctrl+Alt+1` vuelve a la pantalla de video.



```text
qemu-system-x86_64 -machine q35 -m 256M -bios "C:\Users\sambo\Documents\UEFI_Dev\OVMF.fd" -net none -drive "format=raw,file=C:\Users\sambo\Documents\UEFI_Dev\Practica_01\UEFI_VFat32.vhd"
```