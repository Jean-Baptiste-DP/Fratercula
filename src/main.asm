BITS 64;

extern printHelloWorld

section .text
    global _start

_start:
  mov rax, 1        ; write
  mov rdi, 1        ; std output
  mov rsi, msg
  mov rdx, msglen
  syscall

  call printHelloWorld  ; call external c -> print Hello World!

  mov rax, 60       ; exit
  mov rdi, 0        ; success code
  syscall

section .rodata
  msg: db "Hello World!", 10
  msglen: equ $ - msg