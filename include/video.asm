
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
