#include "common.h"

cplx32 complex_mult(cplx16 a, cplx16 b) {
	int32 out_i = (real(a)*real(b) - imag(a)*imag(b)) >> 1;
	int32 out_q = (real(a)*imag(b) + imag(a)*real(b)) >> 1;
	cplx32 c = cplx32(out_i, out_q);
	return c;
}
