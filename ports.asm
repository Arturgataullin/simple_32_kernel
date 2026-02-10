global read_port
global write_port 

read_port:
	mov edx, [esp + 4]	; port
	in al, dx
	ret

write_port:
	mov edx, [esp + 4]	; port
	mov al, [esp + 8]	; data
	out dx, al
	ret 
