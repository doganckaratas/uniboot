BITS 16
buffer equ 24576

stage2:
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
	mov si, file_found
	call print_string
	hlt

reboot:
	mov ax, 0
	int 16h				; Wait for keystroke
	mov ax, 0
	int 19h				; Reboot the system


print_string:				; Output string in SI to screen
	pusha

	mov ah, 0Eh			; int 10h teletype function

.repeat:
	lodsb				; Get char from string
	cmp al, 0
	je .done			; If char is zero, end of string
	int 10h				; Otherwise, print it
	jmp short .repeat

.done:
	popa
	ret

file_found	db "Stage 2 Loaded to disk...", 0

