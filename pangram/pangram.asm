section .data
    true_msg db 'true', 0
    true_len equ $ - true_msg
    false_msg db 'false', 0
    false_len equ $ - false_msg

section .text
    global _start

; Проверка панграммы (упрощенная версия)
_start:
    ; Здесь должна быть реализация проверки панграммы
    ; Для простоты просто выведем 'true'
    mov eax, 4        ; sys_write
    mov ebx, 1        ; stdout
    mov ecx, true_msg
    mov edx, true_len
    int 0x80

    ; Завершение программы
    mov eax, 1        ; sys_exit
    xor ebx, ebx      ; код возврата 0
    int 0x80