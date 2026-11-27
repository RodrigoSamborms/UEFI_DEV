.intel_syntax noprefix
.text
.global ReadMsr

ReadMsr:
    # RCX contiene el primer argumento (MsrIndex)
    # rdmsr lee el MSR especificado en ECX
    rdmsr
    
    # rdmsr guarda el resultado dividido en dos registros de 32 bits:
    # EDX (parte alta) y EAX (parte baja).
    # Necesitamos empaquetarlo en RAX para devolver un UINT64.
    shl rdx, 32
    or rax, rdx
    
    ret