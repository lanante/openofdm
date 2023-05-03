#include "common.h"

void equalizer_per_sym(cplx16 symbol_iq[NFFT_MAX], int18 sym_phase[NFFT_MAX], stream_iq &sample_out, cplx16 new_lts[NFFT_MAX], uint6* arr, uint7 length) {
#pragma HLS INLINE
	cplx16 rot_iq[NFFT_MAX];
	cplx16 new_lts_conj;
	cplx32 prod_iq;
	int24 mag_sq;
	int16 norm_i, norm_q;
	int32 prod_i_scaled, prod_q_scaled;
	eq_per_sym: for (uint7 i=0; i<length; i++) {
#pragma HLS PIPELINE
		rot_iq[*(arr + i)] = rotate(sym_phase[*(arr + i)], symbol_iq[*(arr + i)]);

		new_lts_conj = cplx16(real(new_lts[*(arr + i)]), -imag(new_lts[*(arr + i)]));

		prod_iq = complex_mult(rot_iq[*(arr + i)], new_lts_conj);

		prod_i_scaled = real(prod_iq) << (CONS_SCALE_SHIFT+1);
		prod_q_scaled = imag(prod_iq) << (CONS_SCALE_SHIFT+1);
		mag_sq = real(complex_mult(new_lts[*(arr + i)], new_lts_conj));

		norm_i = prod_i_scaled/mag_sq;
		norm_q = prod_q_scaled/mag_sq;

		sample_out.write(cplx16(norm_i, norm_q));
	}
}
