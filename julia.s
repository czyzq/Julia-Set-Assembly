section .data

;c_x:	dd	-1.125	;rozne ksztalty dla zbioru julii
;c_y:	dd	0.25

;c_x:	dd	-0.123	
;c_y:	dd	0.745

;c_x:	dd	-0.390541
;c_y:	dd	-0.586788

c_x:	dd	-0.10
c_y:	dd	0.65

r:	dd	4.0

section	.text
global	julia

julia:
	; dane są szczytywane wg kolejności do rejestrow:
	; edi wskaznik na tablice pixelii
	; esi rozmiar wyswietlacza
	; xmm0 p_x		punkt p
	; xmm1 p_y
	; xmm2 przesuniecie
	
	movss	xmm8, [r]	;laduje "promien" do sprawdzania warunku
	movss	xmm9, xmm0	;przechowuje pierwszego x'a
	movss	xmm10, [c_x]	;c_x i c_y odpowiedzialne za ksztalt zbioru julii
	movss	xmm11, [c_y]
	mov	r8d,	0x000000

	mov	edx, esi	;licznik zewnetrznej petli
loop1:
	mov	ecx, esi	;licznik wewnetrznj petli
loop2:
	movss	xmm3,xmm0	;bierzacy x
	movss	xmm4,xmm1	;bierzacy y
	mov	r9d, 256	;ustawiam liczbe iteracji		
licz:
	movss	xmm5, xmm3	; xmm5 = x^2
	mulss	xmm5, xmm5
	movss	xmm6, xmm4	; xmm6 = y^2
	mulss	xmm6, xmm6
	
	mulss	xmm4, xmm3	;y=2*x*y+cx
	addss	xmm4, xmm4
	addss	xmm4, xmm11

	movss xmm3, xmm5	; x=x^2-y^2+cx
	subss xmm3, xmm6
	addss xmm3, xmm10

	movss	xmm7, xmm5	; xmm7 = x^2 + y^2 
	addss	xmm7, xmm6

	ucomiss	xmm7, xmm8	; if x^2 + y^2 > 4
	jnb	no_color	; tak, czyli nie kolorujemy

	dec	r9d
	jnz	licz
	
	mov	r10d, 0x9acd32	; ustawiam dany kolor

no_color:
	cmovnz	r10d, r8d	; ustawia kolor czarny

end_loop2:
	mov	[rdi], r10d	;laduje piksel pokolorowany na wybrany kolor
 
	add	rdi, 4		;przerzucam na kolejny pixel w wierszu
	addss	xmm0, xmm2	
	loop	loop2		

end_loop1:
	addss	xmm1, xmm2	;przerzucam na pixel w kolejnym wierszu
	movss	xmm0, xmm9	
	dec	edx
	jnz	loop1
end:
	ret

