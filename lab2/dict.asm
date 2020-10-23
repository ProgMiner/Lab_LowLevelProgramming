; vim: syntax=nasm

global find_word

extern string_equals

; Find word in dict
; rdi - key, rsi - dict
; rax - entry address
find_word:
.loop:                  ; do {
    push rdi
    push rsi
    add rsi, 16
    call string_equals  ;     rax = string_equals(key, dict.key);
    pop rsi
    pop rdi

    test rax, 1         ;     if (rax & 1 != 0)
    jnz .succ           ;         goto .succ;

    mov rsi, [rsi]      ;     dict = dict.prev;
    test rsi, rsi       ; } while (dict != 0);
    jnz .loop

    xor rax, rax        ; return 0;
    ret

.succ:
    mov rax, rsi        ; return dict;
    ret
