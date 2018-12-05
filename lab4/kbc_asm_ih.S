#include <machine/asm.h>
#include "i8042.h"

.intel_syntax
.global _C_LABEL(kbc_asm_ih)
.global _C_LABEL(scancode)

.data
_C_LABEL(scancode):
	.byte 0

.text
_C_LABEL(kbc_asm_ih):
	mov ecx, TRIES_TIL_TIMEOUT  # ecx = 5
ciclo:							# runs the same loop as kbc_ih does
	in al, KBC_STAT_REG			# reads status reg to eax

	mov dl, KBC_STATREG_PAR		
	or dl, KBC_STATREG_TO		# edx <- PARITY_ERROR | TIMEOUT_ERROR
	test al, dl					# checks if there is error
	jnz error					# ends if there is 

	test al, KBC_STATREG_OBF	# checks if OUT_BUF is full
	jnz read					
	loop ciclo					# loops while empty
	jmp end
read:
	in al, KBC_OUT_BUF			# Else, reads OUT_BUF to eax and stores it
	mov _C_LABEL(scancode), al	
	jmp end
error:
	in al, KBC_OUT_BUF			# if there's an error, discards out_buf

end: ret
