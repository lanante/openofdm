#include "common.h"

static int6 pilot_idx[4] = {7, 21, -21, -7};

int24 peg_estimate(cplx16 symbol_iq[NFFT_MAX], int18 sym_phase[NFFT_MAX], cplx16 lts_pilot[PILOT_MAX], uint1 current_polarity[PILOT_MAX]) {
#pragma HLS INLINE
	cplx16 pilot_rot_iq[PILOT_MAX];
	int16 input_i, input_q;
	cplx32 mult_out;
	int16 pilot_iq_phase;
	int24 Sxy = 0;
	lvpe_est: for (uint3 i=0; i<nof_pilots; i++) {
#pragma HLS PIPELINE
		pilot_rot_iq[i] = rotate(sym_phase[pilot_loc[i]], symbol_iq[pilot_loc[i]]);

		if (current_polarity[i] == 0) {
			input_i = real(pilot_rot_iq[i]);
			input_q = -imag(pilot_rot_iq[i]);
		} else {
			input_i = -real(pilot_rot_iq[i]);
			input_q = imag(pilot_rot_iq[i]);
		}
		mult_out = complex_mult(cplx16(input_i, input_q), lts_pilot[i]);
		pilot_iq_phase = phase_inst(mult_out);

		Sxy += pilot_idx[i]*pilot_iq_phase;
	}
	return Sxy;
}
