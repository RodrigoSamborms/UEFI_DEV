Skill: Generar y Compilar Aplicacion03

Tareas:
- `generate`: crea `efi.c`, `efi.h`, `msr.s` en la ruta destino (por defecto `./Aplicacion03`).
- `build`: compila los archivos generados usando `gcc` y el conjunto de flags UEFI proporcionado.

Uso:
```powershell
# Generar archivos en la carpeta indicada (por defecto `archivos_00`)
.
\Skills\Skill_Generar_Compilar\Skill_Generar_Compilar.ps1 -Action generate -FolderName MiCarpeta

# Compilar la aplicación desde la carpeta creada (ejecuta `& gcc @xargList` internamente)
.
\Skills\Skill_Generar_Compilar\Skill_Generar_Compilar.ps1 -Action build -FolderName MiCarpeta

# Generar y compilar
.
\Skills\Skill_Generar_Compilar\Skill_Generar_Compilar.ps1 -Action all -FolderName MiCarpeta
```

Notas:
- El script buscará `gcc` en ubicaciones comunes de MSYS2 (`C:\msys64\ucrt64\bin\gcc.exe`, etc.) si no se proporciona una ruta.
- El log de compilación se guarda en `build_out.log` dentro de la carpeta destino.
