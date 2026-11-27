.intel_syntax noprefix
.text
.global ReadMsr

ReadMsr:
    rdmsr
    shl rdx, 32
    or rax, rdx
    ret
