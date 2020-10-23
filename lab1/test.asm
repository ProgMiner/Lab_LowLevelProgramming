%include "lib.inc"

section .data
oaoaoa: db 'Input number: ', 0
kek:    db '000000000000000000', 0

section .text

global _start

_start:
    mov rdi, oaoaoa
    call print_string
    mov rdi, kek
    mov rsi, 19
    call read_word
    call parse_int
    mov rdi, rax
    call print_int
    call print_newline

    mov rdi, rdx
    call print_uint
    call print_newline

    xor rdi, rdi
    call exit
