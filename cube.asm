	bits 16
	org 100h

start:
		finit
		call	set_videomode
		jmp	.main_loop

	.main_loop:
		call	move_line

		mov	ax,	word [vertices2d + point2d.x]
		mov	bx,	word [vertices2d + point2d.y]
		mov	cx,	word [vertices2d + point2d.size + point2d.x]
		mov	dx,	word [vertices2d + point2d.size + point2d.y]
		mov	di,	0011b
		call	draw_line
		
		mov	di,	0
		call	draw_line

		jmp .main_loop

	.exit:
		ret
		%include	"bresenham.inc"

move_line:
		fild	word [vertices2d + point2d.x]
		fiadd	word [tmp]
		fistp	word [vertices2d + point2d.x]
		fild	word [vertices2d + point2d.size + point2d.x]
		fiadd	word [tmp]
		fistp	word [vertices2d + point2d.size + point2d.x]
		ret

data:
		struc	point
			.x	resq 1
			.y	resq 1
			.z	resq 1
			.size:
		endstruc

		struc	point2d
			.x	resw 1
			.y	resw 1
			.size:
		endstruc

		struc	edge
			.p1	resb 1
			.p2	resb 1
			.size:
		endstruc

		vertices:
			dq	0.0,0.0,0.0 ; 0
			dq	0.0,0.0,0.0 ; 1
			dq	0.0,0.0,0.0 ; 2
			dq	0.0,0.0,0.0 ; 3
			dq	0.0,0.0,0.0 ; 4
			dq	0.0,0.0,0.0 ; 5
			dq	0.0,0.0,0.0 ; 6
			dq	0.0,0.0,0.0 ; 7

		vertices2d:
			dw	10,10 ; 0
			dw	30,100 ; 1

		tmp	dw	1

