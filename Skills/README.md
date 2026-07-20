# Skills: Generador de esqueletos UEFI

Esta Skill crea esqueletos básicos de `efi.c`, `efi.h` y `asm.s` (Intel syntax) en la carpeta destino que indiques. Si la carpeta no existe, se crea.

Uso (PowerShell):

```powershell
# Desde la raíz del repositorio
.
\Skills\create_skeleton.ps1 -TargetPath "path\to\dest\MyProject" -Name MyProject
```

El script copiará las plantillas y reemplazará `{{NAME}}` por el nombre dado.

Extensiones futuras: integración con plantillas basadas en `basic*` y `Example*`.
