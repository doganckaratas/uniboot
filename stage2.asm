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
	; boot sector loads at 7c00, length 200
	; after this stage 2 placed at 7f00
	; all segments points to 7f00, cause we loaded stage 2 into it
	mov ax, 0x7f00
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
.start:
	; setup gdt idt ldt, ...
	; enable a20
	; jump 32 bits
	; prepare for 64 bits
	; jump 64 bits
	; parse elf header, load binary.

	push dx
	mov dx, 0x03F8
	call serial_init
	mov al, 13
	call serial_send_char
	mov al, 10
	call serial_send_char
	mov si, load_msg
	call serial_send_str
	pop dx

	mov si, load_msg
	call print
	mov si, msg2
	call print
	mov si, msg3
	call print
	mov si, msg4
	call print


	; end of execution
.finish:
	cli
	hlt
	jmp $ ; eger HLT state'i NMI interrupt ile sonlanirsa infinite loop'a gir


%include "serial.asm"
%include "video.asm"

load_msg db "Stage 2 loaded successfully. ",13,10,0
msg2 db "From now on, I can load my kernel into memory and load it. ",13,10,0
msg3 db "There is no 512 byte boundary limit, this binary has no limits, ",13,10,0
msg4 db "We are in 16 Bit Real Mode,",13,10,0
