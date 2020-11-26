; vim: syntax=nasm

%define c_1_1 0.393
%define c_1_2 0.769
%define c_1_3 0.189
%define c_2_1 0.349
%define c_2_2 0.686
%define c_2_3 0.168
%define c_3_1 0.272
%define c_3_2 0.543
%define c_3_3 0.131

global do_sepia

section .rodata

align 16
c_r: dd c_1_1, c_2_1, c_3_1, c_1_1, c_2_1, c_3_1, c_1_1, c_2_1, c_3_1, c_1_1, c_2_1, c_3_1
c_g: dd c_1_2, c_2_2, c_3_2, c_1_2, c_2_2, c_3_2, c_1_2, c_2_2, c_3_2, c_1_2, c_2_2, c_3_2
c_b: dd c_1_3, c_2_3, c_3_3, c_1_3, c_2_3, c_3_3, c_1_3, c_2_3, c_3_3, c_1_3, c_2_3, c_3_3

section .text

; process 4 pixels with sepia filter via SSE
; rdi - red    12-dim vector
; rsi - green  12-dim vector
; rdx - blue   12-dim vector
; rcx - result 12-dim vector
do_sepia:
    lea r9, [rel c_r]
    lea r10, [rel c_g]
    lea r11, [rel c_b]

    mov r8, 3
.loop:
    cvtdq2ps xmm0, [rdi]
    cvtdq2ps xmm1, [rsi]
    cvtdq2ps xmm2, [rdx]

    mulps xmm0, [r9]
    mulps xmm1, [r10]
    mulps xmm2, [r11]

    addps xmm0, xmm1
    addps xmm0, xmm2
    cvtps2dq xmm0, xmm0
    movdqa [rcx], xmm0

    add rdi, 16
    add rsi, 16
    add rdx, 16
    add rcx, 16
    add r9, 16
    add r10, 16
    add r11, 16

    dec r8
    test r8, r8
    jnz .loop

    ret
