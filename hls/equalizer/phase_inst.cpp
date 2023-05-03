#include "atan_lut.h"
#include "hls_math.h"

int16 phase_inst(cplx32 phase_in) {
#pragma HLS INLINE
	int32 in_i = real(phase_in);
	int32 in_q = imag(phase_in);
	int32 abs_i = hls::abs(in_i);
	int32 abs_q = hls::abs(in_q);
	uint32 min, max;
	uint3 quadrant = 0;

	quadrant |= (uint3)(((in_i < 0)?1:0) << 2);
	quadrant |= (uint3)(((in_q < 0)?1:0) << 1);
	quadrant |= (uint3)((abs_i<abs_q)?1:0);

	if (abs_i >= abs_q) {
		max = abs_i;
		min = abs_q;
	} else {
		max = abs_q;
		min = abs_i;
	}

	uint32 dividend = (max > 4194304) ? min : (min << 9);
	uint32 divisor = (max > 4194304) ? (max >> 9) : max;

	uint10 quotient = (dividend/divisor).range(9,0);
	uint9 atan_addr = (quotient > 511) ? (uint9)511 : (uint9)quotient;

	uint9 _phase = atan_lut[atan_addr];
	int16 phase_out;
	switch (quadrant) {
		case 0:
			phase_out = _phase;
			break;
		case 1:
			phase_out = PI_2 - _phase;
			break;
		case 2:
			phase_out = -_phase;
			break;
		case 3:
			phase_out = _phase - PI_2;
			break;
		case 4:
			phase_out = PI - _phase;
			break;
		case 5:
			phase_out = PI_2 + _phase;
			break;
		case 6:
			phase_out = _phase - PI;
			break;
		case 7:
			phase_out = -PI_2 - _phase;
			break;
		default:
			phase_out = _phase;
			break;
	}

	return phase_out;
}
