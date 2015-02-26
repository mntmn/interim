.data	32
fmt:
.c	"nfibs(%d) = %d\n"

.code
	jmpi main

	name rfibs
rfibs:
	prolog
	arg $in
	getarg %v0 $in		/* V0 = N */

	blti_u out %v0 2
	subi %v1 %v0 1		/* V1 = N-1 */
	subi %v2 %v0 2		/* V1 = N-2 */
	prepare
		pushargr %v1
	finishi rfibs
	retval %v1		/* V1 = rfibs(N-1) */
	prepare
		pushargr %v2
	finishi rfibs
	retval %v2		/* V2 = rfibs(N-2) */
	addi %v1 %v1 1
	addr %r0 %v1 %v2
	retr %r0
out:
	reti 1
	epilog

	name main
main:
	prolog
	prepare
		pushargi 32
	finishi rfibs
	retval %v0
	prepare
		pushargi fmt
		ellipsis
		pushargi 32
		pushargr %v0
	finishi @printf
	ret
	epilog
