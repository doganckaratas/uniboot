
; serial stuff
; init serial:
; DX = port
; DX 0x3F8 -> COM1
; DX 0x2F8 -> COM2
; DX 0x3E8 -> COM3
; DX 0X2E8 -> COM4
serial_init:
	pusha
	xor ax,ax
	mov al, 0x00
	inc dx
	out dx, al ; dx + 1: disabled ints
	mov al, 0x80
	add dx, 2
	out dx, al ; dx + 3:
	mov al, 0x01
	sub dx, 3
	out dx, al ; dx + 0: 115200 baud
	xor ax,ax
	inc dx
	out dx, al ; dx + 1
	mov al, 0x03
	add dx, 2
	out dx, al ; dx + 3: 8N1
	mov al, 0xC7
	dec dx
	out dx, al ; dx + 2: FIFO enable
	mov al, 0x0B
	add dx, 2
	out dx, al ; dx + 4: enable ints, rts/dsr
	xor ax,ax
	popa
	ret

; send char
; DX = port
; AL = char
serial_send_char:
	pusha
	call serial_wait_send_avail
	out dx, al
	popa
	ret

; serial send availability check
; DX = port
; AL = 0 if available
serial_wait_send_avail:
	pusha
	add dx, 5
.wait_send:
	in al, dx ; get avail bit
	cmp al, 0
	jz .wait_send
	popa
	ret

; serial send zero terminated string
; DX = port
; SI = string
serial_send_str:
	pusha
.next:
	lodsb
	cmp al, 0
	je .done
	call serial_send_char
	jmp short .next

.done:
	popa
	cli
	ret
