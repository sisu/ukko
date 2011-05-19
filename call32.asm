text
global	makeCall

# params: funptr, nParams, params, serializer
makeFunCall:
	mov	ecx, [esp+8] # nParams
	mov	eax, [esp+16]
