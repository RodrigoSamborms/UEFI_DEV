; asm.s - Plantilla en notación Intel
; Función de ejemplo: get_sample_registers
; Devuelve un valor en eax

global get_sample_registers
section .text
get_sample_registers:
    ; preserva registros usados
    push rbp
    mov rbp, rsp

    ; ejemplo de manipulaciones de registros (Intel syntax)
    mov rax, 0x1234
    xor rbx, rbx
    mov rbx, rax
    add rbx, 0x10

    ; devolver en rax (convención x86_64) o eax según objetivo
    mov rax, rbx

    ; restaurar y regresar
    pop rbp
    ret

; Nota: adapte nombres y convenciones si compila para 32-bit (use eax/ebx, push ebp, etc.)
