section .data
    kmh_to_ms: dq 0.277777778  ; 1000/3600 to convert km/h to m/s

section .text
bits 64
default rel
global calculate_acceleration

calculate_acceleration:
    push rbp
    mov rbp, rsp
    
    push rbx
    push r12
    push r13
    push r14
    
    mov r12, rcx        ; matrix pointer
    mov r13, r8         ; results pointer
    mov ecx, edx        ; number of rows (copy lower 32 bits of rdx)
    
    movsd xmm15, [rel kmh_to_ms]
    
    xor rbx, rbx        ; row counter
    mov r14, 0          ; offset counter
.loop:
    ; Load values
    movsd xmm0, [r12 + r14]      ; initial velocity
    movsd xmm1, [r12 + r14 + 8]  ; final velocity
    movsd xmm2, [r12 + r14 + 16] ; time
    
    ; Convert velocities from km/h to m/s
    mulsd xmm0, xmm15   ; v1 in m/s
    mulsd xmm1, xmm15   ; v2 in m/s
    
    ; Calculate acceleration: (v2 - v1) / t
    subsd xmm1, xmm0    ; v2 - v1
    divsd xmm1, xmm2    ; divide by time
    
    ; Convert to integer (round)
    roundsd xmm1, xmm1, 0 ; Round to nearest integer
    cvttsd2si eax, xmm1   ; Convert rounded value to integer
    
    ; Store result
    mov [r13 + rbx * 4], eax
    
    ; Increment counters and continue loop
    inc rbx
    add r14, 24         ; Move to next row (3 doubles = 24 bytes)
    cmp ebx, ecx
    jl .loop
    
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret
