#include "rot_lut.h"

cplx16 rotate(int16 phase, cplx16 in_iq) {
	uint3 quadrant;
	uint9 actual_phase;
	uint16 phase_abs = phase < 0 ? (uint16)-phase : (uint16)phase;
	if (phase_abs <= PI_4) {
		quadrant = (phase < 0) ? 4 : 0;
		actual_phase = phase_abs;
	} else if (phase_abs <= PI_2) {
		quadrant = (phase < 0) ? 5 : 1;
		actual_phase = PI_2 - phase_abs;
	} else if (phase_abs <= PI_3_4) {
		quadrant = (phase < 0) ? 6 : 2;
		actual_phase = phase_abs - PI_2;
	} else {
		quadrant = (phase < 0) ? 7 : 3;
		actual_phase = PI - phase_abs;
	}

	cplx16 rot_iq;
	switch (quadrant) {
		case 0:
			rot_iq = cplx16(rotate_I[actual_phase], rotate_Q[actual_phase]);
			break;
		case 1:
			rot_iq = cplx16(rotate_Q[actual_phase], rotate_I[actual_phase]);
			break;
		case 2:
			rot_iq = cplx16(-rotate_Q[actual_phase], rotate_I[actual_phase]);
			break;
		case 3:
			rot_iq = cplx16(-rotate_I[actual_phase], rotate_Q[actual_phase]);
			break;
		case 4:
			rot_iq = cplx16(rotate_I[actual_phase], -rotate_Q[actual_phase]);
			break;
		case 5:
			rot_iq = cplx16(rotate_Q[actual_phase], -rotate_I[actual_phase]);
			break;
		case 6:
			rot_iq = cplx16(-rotate_Q[actual_phase], -rotate_I[actual_phase]);
			break;
		case 7:
			rot_iq = cplx16(-rotate_I[actual_phase], -rotate_Q[actual_phase]);
			break;
		default:
			rot_iq = cplx16(rotate_I[actual_phase], rotate_Q[actual_phase]);
			break;
	}
	cplx32 mult_out = complex_mult(in_iq, rot_iq);
	cplx16 out_iq = cplx16((int16)(real(mult_out) >> ROTATE_LUT_SCALE_SHIFT), (int16)(imag(mult_out) >> ROTATE_LUT_SCALE_SHIFT));
	return out_iq;
}
