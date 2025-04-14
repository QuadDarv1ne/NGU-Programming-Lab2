; -------------------------------------------------------------------------- ;
; NASM x86 assembly                                                          ;
; Вычисляет сумму и произведение цифр числа, а также модуль их разности.     ;
;                                                                            ;
; Вход:                                                                      ;
;   - Вводится натуральное число с клавиатуры                                ;
;                                                                            ;
; Выход:                                                                     ;
;   - Три числа через пробел: сумма цифр, произведение цифр, модуль разности ;
; -------------------------------------------------------------------------- ;

section .data
    prompt db "Enter number: ", 0
    input_format db "%d", 0
    output_format db "%d %d %d", 10, 0
    
section .bss
    number resd 1
    
section .text
    global main
    extern scanf, printf
    
main:
    ; Ввод числа
    push prompt
    call printf
    add esp, 4
    
    push number
    push input_format
    call scanf
    add esp, 8
    
    ; Инициализация
    mov eax, [number]  ; число
    xor ebx, ebx       ; sum = 0
    mov ecx, 1         ; product = 1
    
calculate_loop:
    test eax, eax
    jz done
    
    xor edx, edx
    mov esi, 10
    div esi            ; edx = digit, eax = num / 10
    
    add ebx, edx       ; sum += digit
    
    imul ecx, edx      ; product *= digit
    
    jmp calculate_loop
    
done:
    ; Вычисление модуля разности
    mov eax, ebx       ; eax = sum
    sub eax, ecx       ; eax = sum - product
    jns positive
    neg eax            ; если отрицательно, инвертируем
    
positive:
    ; Вывод результатов
    push eax           ; разность
    push ecx           ; произведение
    push ebx           ; сумма
    push output_format
    call printf
    add esp, 16
    
    ; Выход
    xor eax, eax
    ret