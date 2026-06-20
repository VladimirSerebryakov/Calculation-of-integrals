section .data
    const035 dq 0.35
    const095 dq 0.95
    const27 dq 2.7
    const3 dq 3.0
    const1 dq 1.0
    const2 dq 2.0

section .text
global f1
global f2
global f3
f1: ;0.35x^2 - 0.95x + 2.7
    push ebp
    mov ebp, esp

    fld qword [ebp + 8]
    fld qword [ebp + 8]
    fmulp
    fld qword [const035]
    fmulp
    ;st0 = 0.35*x^2

    fld qword [ebp + 8]
    fld qword [const095]
    ; st0 = 0.95
    ; st1 = x
    ; st2 = 0.35*x^2
    fmulp
    ; st0 = 0.95x
    ; sp1 = 0.35x^2
    fsubp
    ; sp0 = 0.35x^2 - 0.95x
    fld qword[const27]
    faddp

    mov esp, ebp
    pop ebp
    ret


f2: ;3x + 1
    push ebp
    mov ebp, esp

    fld qword [const3]
    fld qword [ebp + 8]
    fmulp
    fld qword [const1]
    faddp

    mov esp, ebp
    pop ebp
    ret


f3: ; 1/(x + 2)
    push ebp
    mov ebp, esp

    fld qword [const1]
    fld qword [ebp + 8]
    fld qword [const2]
    ;st0 = 2
    ;st1 = x
    ;st2 = 1
    faddp
    ;st0 = 2 + x
    ;st1 = 1
    fdivp

    mov esp, ebp
    pop ebp
    ret
