#include "common.h"

int16 cpe_estimate(cplx16 sample_in[NFFT_MAX], cplx16 lts_pilot[4], uint1 current_polarity[4]) {
	int16 input_i, input_q;
	int16 pilot_i, pilot_q;
	int32 pilot_sum_i = 0, pilot_sum_q = 0;
	cplx32 mult_out;
	cpe_est: for (uint3 i=0; i<nof_pilots; i++) {
#pragma HLS PIPELINE
		if (current_polarity[i] == 0) {
			input_i = real(sample_in[pilot_loc[i]]);
			input_q = -imag(sample_in[pilot_loc[i]]);
		} else {
			input_i = -real(sample_in[pilot_loc[i]]);
		    input_q = imag(sample_in[pilot_loc[i]]);
		}
		mult_out = complex_mult(cplx16(input_i, input_q), lts_pilot[i]);
		pilot_sum_i += real(mult_out);
		pilot_sum_q += imag(mult_out);
	}

	int16 cpe = phase_inst(cplx32(pilot_sum_i, pilot_sum_q));
	return cpe;
}
