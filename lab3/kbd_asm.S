#include "i8042.h"

.global _kbd_int_handler_ass
.text 

_kbd_int_handler_ass:

	xor %eax, %eax				//initializes register to 0

	inb $STAT_REG

	test $BIT(0), %al

	loopz _kbd_int_handler_ass

	inb $OUT_BUF

	ret


	.end
