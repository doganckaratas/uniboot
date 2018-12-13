; stage 2 of powerloader
; dogan c. karatas
; 13 dec 2018

bits 16
cli
mov ax, 0
mov ss, ax
mov sp, 0FFFFh
sti
cld
mov ax, 2000h
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax

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
hlt

hw db "Stage 2 loaded successfully. ",13,10,0
