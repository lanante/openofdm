#include "common.h"
#include "stdio.h"

int main() {
	cplx32 iq_sample_in;
	iq_sample_in = cplx32(-35, -100);
	int16 phase_out = phase_inst(iq_sample_in);
	printf("phase_out: %d vs expected: %d\n");
	return 0;
}
