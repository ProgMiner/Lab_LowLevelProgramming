; vim: syntax=nasm

%include "colon.inc"
%include "colon.inc"

section .data
%include "words.inc"

value_strconst: db "Value: ", 0
too_long_key_strconst: db "Too long key!", 0x0a, 0
key_not_found_strconst: db "Key not found.", 0x0a, 0

section .text

global _start

extern read_word
extern find_word
extern print_string
extern print_newline
extern print_error
extern exit

_start:
    sub rsp, 256
    mov rdi, rsp
    mov rsi, 256
    call read_word

    test rax, rax
    je .too_long_error

    mov rdi, rax
    mov rsi, dict
    call find_word

    test rax, rax
    je .not_found_error

    mov rdi, value_strconst
    call print_string

    mov rdi, [rax + 8]
    call print_string
    call print_newline

    xor rdi, rdi
    call exit
    ret

.too_long_error:
    mov rdi, too_long_key_strconst
    call print_error

    mov rdi, 1
    call exit


.not_found_error:
    mov rdi, key_not_found_strconst
    call print_error

    mov rdi, 2
    call exit
