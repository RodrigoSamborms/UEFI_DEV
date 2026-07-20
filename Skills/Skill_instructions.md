# Skill: Generador de esqueletos UEFI

Objetivo:

- Generar archivos `efi.c`, `efi.h` y `asm.s` con estructura mínima para proyectos UEFI.

Detalles:

- `efi.c` incluye una entrada `UefiMain` (esqueleto) y ejemplos de llamadas a servicios UEFI (comentadas para adaptación).
- `efi.h` declara prototipos y dependencias mínimas.
- `asm.s` usa notación Intel y contiene una función de ejemplo que realiza manipulaciones simples de registros (`mov`, `xor`, `push/pop`) y devuelve un valor en `eax`.

Notas de seguridad:

- No se ejecutan instrucciones privilegiadas en el esqueleto; cuando extienda la Skill, tenga cuidado con accesos a CR/MSR.
