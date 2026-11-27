.intel_syntax noprefix
.text
.global ReadMsr
.global WriteMsr
.global CpuidAsm

# Lectura de MSR
ReadMsr:
    rdmsr
    shl rdx, 32
    or rax, rdx
    ret

# Escritura de MSR
WriteMsr:
    wrmsr
    ret

# CPUID: EAX = código, devuelve en RAX,RBX,RCX,RDX
CpuidAsm:
    mov eax, edi
    cpuid
    mov [rsi], eax
    mov [rdx], ebx
    mov [rcx], ecx
    mov [r8], edx
    ret
