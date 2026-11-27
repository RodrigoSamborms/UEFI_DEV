.intel_syntax noprefix
.text
.global ReadMsr
.global WriteMsr

# Lectura de MSR
ReadMsr:
    rdmsr
    shl rdx, 32
    or rax, rdx
    ret

# Escritura de MSR (para simular inyección)
# RCX = índice, RAX = valor bajo, RDX = valor alto
WriteMsr:
    wrmsr
    ret
