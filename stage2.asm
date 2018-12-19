; stage 2 of powerloader
; dogan c. karatas
; 13 dec 2018

[bits 16]
[org 0]

init:
	; set up segments (ss, ds, es, fs, gs ...)
	cli
	xor ax, ax
	mov ss, ax
	; stack top is 0xFFFF
	mov sp, 0xFFFF
	sti
	cld
	; all segments points to 0x0500, cause we loaded stage 2 into it
	mov ax, 0x0500
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
.setup:
	; setup gdt idt ldt, ...
	; enable a20
	; jump 32 bits
	; prepare for 64 bits
	; jump 64 bits
	; parse elf header, load binary.

	mov si, load_msg
	call print

	; end of execution
.finish:
	cli
	hlt


; print rutini
; si = string (zero termine edilmis)
print:
	pusha
	mov ah, 0x0E
.repeat:
	lodsb
	cmp al, 0
	je .done
	int 10h
	jmp short .repeat
.done:
	popa
	cli
ret

load_msg db "Stage 2 loaded successfully. ",13,10,0
