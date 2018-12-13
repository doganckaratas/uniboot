; stage 2 of powerloader
; dogan c. karatas
; 13 dec 2018

bits 16
cli
xor ax, ax
mov ss, ax
; stack top is FFFF
mov sp, 0FFFFh
sti
cld
; all segments points to 0500h, cause we loaded stage 2 into it
mov ax, 0500h
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax

; setup gdt idt ldt, ...
; enable a20
; jump 32 bits
; prepare for 64 bits
; jump 64 bits
; parse elf header, load binary.

mov si, hw
pusha
mov ah, 0Eh			; int 10h teletype function
repeat:
lodsb				; Get char from string
cmp al, 0
je done			; If char is zero, end of string
int 10h				; Otherwise, print it
jmp short repeat
done:
popa
hlt

hw db "Stage 2 loaded successfully. ",13,10,0
