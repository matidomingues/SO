bits 32

global inb
global outb
global inw
global outw

section .text

; unsigned inb(unsigned ioaddr);
inb:
	push edx
	mov dx, [esp + 8]
	xor eax, eax
	in al, dx
	pop edx
	ret

; void outb(unsigned ioaddr, unsigned data);
outb:
	push edx
	mov dx, [esp + 8]
	mov eax, [esp + 12]
	out dx, al
	pop edx
	ret

; unsigned inw(unsigned ioaddr);
inw:
	push ebp
	mov ebp, esp
	push dx

	mov eax, 0
	mov dx, [ebp+8]
	in ax, dx

	pop dx
	leave
	ret

; void outw(unsigned ioaddr, unsigned data;
outw:
	push ebp
	mov ebp, esp
	pusha

	mov dx, [ebp+8]
	mov ax, [ebp+12]
	out dx, ax

	popa
	leave
	ret
