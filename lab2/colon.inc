; vim: syntax=nasm

%ifndef _colon_inc
%define _colon_inc

%define colon_prev 0

%macro colon 2
%2:
    dq colon_prev
    dq %%value
    db %1, 0
%%value:
%define colon_prev %2
%endmacro

%endif
