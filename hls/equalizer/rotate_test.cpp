#include "common.h"
#include "stdio.h"

int main() {
	cplx32 iq_sample_in;
	iq_sample_in = cplx32(-35, -100);
	cplx16 rot_out = rotate(-795, iq_sample_in);
	printf("rot_i: %d vs expected: -51\n", (int)real(rot_out));
	printf("rot_q: %d vs expected: 16\n", (int)imag(rot_out));
	return 0;
}
