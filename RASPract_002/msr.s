.intel_syntax noprefix
.text
.global ReadMsr

ReadMsr:
    # RCX contiene el argumento (MsrIndex)
    rdmsr
    # EDX:EAX -> resultado de 64 bits
    shl rdx, 32
    or rax, rdx
    ret
