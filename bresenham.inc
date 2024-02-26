set_videomode:
        mov ax, 0a000h
        mov es, ax
        mov ah, 0h
        mov al, 13h
        int 10h
        ret

; ax = x0
; bx = y0
; cx = x1
; dx = y1     
draw_line:
        mov word [.x0], ax
        mov word [.y0], bx
        mov word [.x1], cx
        mov word [.y1], dx
        sub cx, ax
        sub dx, bx
        mov word [.dx], cx
        mov word [.dy], dx        
        cmp cx, 0
        jl .dx_neg
        jmp .dx_pos
    
    .dx_pos:
        cmp dx, 0
        jge .q0
        jmp .q3
    
    .dx_neg:
        cmp dx, 0
        jge .q1
        jmp .q2
        
    .q0:         
        mov ax, word [.dx]
        mov bx, word [.dy]
        mov word [draw_line_q.dx], ax
        mov word [draw_line_q.dy], bx
        mov ax, word [.x0]
        mov bx, word [.y0]
        mov cx, word [.x1]
        mov dx, word [.y1]
        mov si, 0
        jmp .continue
    .q1:           
        mov ax, word [.dy]
        mov bx, word [.dx]
        neg bx
        mov word [draw_line_q.dx], ax
        mov word [draw_line_q.dy], bx
        mov ax, word [.y0]
        mov bx, word [.x0]
        neg bx
        mov cx, word [.y1]
        mov dx, word [.x1]
        neg dx
        mov si, 1
        jmp .continue
    .q2:
        mov ax, word [.dx]
        neg ax
        mov bx, word [.dy]
        neg bx
        mov word [draw_line_q.dx], ax
        mov word [draw_line_q.dy], bx
        mov ax, word [.x0]
        neg ax
        mov bx, word [.y0]
        neg bx
        mov cx, word [.x1]
        neg cx
        mov dx, word [.y1]
        neg dx
        mov si, 2
        jmp .continue
    .q3:
        mov ax, word [.dy]
        neg ax
        mov bx, word [.dx]
        mov word [draw_line_q.dx], ax
        mov word [draw_line_q.dy], bx
        mov ax, word [.y0]
        neg ax
        mov bx, word [.x0]
        mov cx, word [.y1]
        neg cx
        mov dx, word [.x1]
        mov si, 3
        jmp .continue
 
    .continue:    
        call draw_line_q
        ret   
.x0 dw 0
.y0 dw 0
.x1 dw 0
.y1 dw 0
.dx dw 0
.dy dw 0

; ax = x0
; bx = y0
; cx = x1
; dx = y1
; si = quadrant (0..3)
draw_line_q:
        add si, si
        push cx
        push dx
        mov cx, word [.dx]
        mov dx, word [.dy]
        cmp cx, dx
        jge .not_swap
    .swap:                 
        pop dx
        pop cx
        xchg ax, bx
        xchg cx, dx
        inc si
        jmp .continue
    .not_swap:        
        pop dx
        pop cx
    .continue:     
        call draw_line_o
        ret             
.dx dw 0
.dy dw 0
        
; ax = x0
; bx = y0
; cx = x1
; dx = y1
; si = octant (0..7)
; di = color
draw_line_o:         
        mov word [.x2], cx
        sub cx, ax
        sub dx, bx
        add dx, dx
        mov word [.2dy], dx
        sub dx, cx ; dx = d = 2dy - dx
        add cx, cx
        mov word [.2dx], cx
        mov cx, ax ; x
        mov ax, di ; color
    .next_point:
        call pset_octant
        cmp dx, 0
        jle .dle
    .dg:       
		add dx, word [.2dy]
		sub dx, word [.2dx]
		inc bx
		jmp .continue
	.dle:        
	    add dx, word [.2dy]
    .continue:
		inc cx
		cmp cx, word [.x2]
		jbe .next_point
        ret
        .x2 dw 0
        .2dx dw 0
        .2dy dw 0

; al = color index
; bx = row
; cx = col
; si = octant
pset_octant:
		push bx
		push cx
		cmp si, 0
		jz .octant_0
		cmp si, 1
		jz .octant_1
		cmp si, 2
		jz .octant_2
		cmp si, 3
		jz .octant_3
		cmp si, 4
		jz .octant_4
		cmp si, 5
		jz .octant_5
		cmp si, 6
		jz .octant_6
		cmp si, 7
		jz .octant_7
	.octant_0:
		; do nothing
		jmp .continue
	.octant_1:
		xchg bx, cx
		jmp .continue
	.octant_2:
		neg bx
		xchg bx, cx
		jmp .continue
	.octant_3:
		neg cx
		jmp .continue
	.octant_4:
		neg cx
		neg bx
		jmp .continue
	.octant_5:
		neg cx
		neg bx
		xchg bx, cx
		jmp .continue
	.octant_6:
		neg cx
		xchg bx, cx
		jmp .continue
	.octant_7:
		neg bx
	.continue:
		call pset
		pop cx
		pop bx
		ret
		
; al = color index
; bx = row
; cx = col
pset:
	pusha
	xor dx, dx
	push ax
	mov ax, 320
	mul bx
	add ax, cx
	mov bx, ax
	pop ax
	mov byte [es:bx], al
	popa
	ret
