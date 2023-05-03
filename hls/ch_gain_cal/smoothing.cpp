#include "ch_gain_cal.h"

void moving_avg(cplx16 iq_samples_in[NFFT_MAX], cplx16 iq_samples_out[NFFT_MAX], uint5 len, uint6 (*arr)) {
	cplx16 iq_sample_in, iq_sample_out;
	int18 sum_fd_ch_init_i = 0, sum_fd_ch_init_q = 0;
	int16 prev_i = 0, prev_q = 0, new_input_i = 0, new_input_q = 0;
	cplx16 iq_sample_i, iq_sample_o;
	uint8 i=0, j=0, k=0;

 	int16 window_i[AVG_LEN_HALF*2+1] = {}, window_q[AVG_LEN_HALF*2+1] = {};
	clear: for (k=0; k<AVG_LEN_HALF*2+1; k++) {
		window_i[k] = 0;
		window_q[k] = 0;
	}
#pragma HLS array_partition complete variable=window_i
#pragma HLS array_partition complete variable=window_q

	outer: for (i=0; i<len+2*AVG_LEN_HALF; i++) {
		if (i < AVG_LEN_HALF) {
			iq_sample_in = iq_samples_in[*(arr)];
		} else if (i >= AVG_LEN_HALF && i < len+AVG_LEN_HALF) {
			iq_sample_in = iq_samples_in[*(arr + i-AVG_LEN_HALF)];
		} else {
			iq_sample_in = iq_samples_in[*(arr + len-1)];
		}

		new_input_i = real(iq_sample_in);
		new_input_q = imag(iq_sample_in);

		prev_i = window_i[0];
		prev_q = window_q[0];
		shift_reg: for (j=0; j<2*AVG_LEN_HALF; j++) {
			window_i[j] = window_i[j+1];
			window_q[j] = window_q[j+1];
		}
		window_i[2*AVG_LEN_HALF] = new_input_i;
		window_q[2*AVG_LEN_HALF] = new_input_q;

		sum_fd_ch_init_i = sum_fd_ch_init_i - prev_i + new_input_i;
		sum_fd_ch_init_q = sum_fd_ch_init_q - prev_q + new_input_q;

		if (i>=2*AVG_LEN_HALF) {
			iq_sample_out = cplx16((int16)sum_fd_ch_init_i/(2*AVG_LEN_HALF+1), (int16)sum_fd_ch_init_q/(2*AVG_LEN_HALF+1));
			iq_samples_out[*(arr + i-2*AVG_LEN_HALF)] = iq_sample_out;
		}
	}
}


void smoothing(cplx16 iq_samples_in[NFFT_MAX], cplx16 iq_samples_out[NFFT_MAX], uint6 ru_width) {
#pragma HLS allocation function instances=moving_avg limit=1
	uint5 len = ru_width >> 1;
	uint6 (*arr1), (*arr2);
	if (ru_width == 56) {
		arr1 = ACTIVE_SC_IDX_56[0];
		arr2 = ACTIVE_SC_IDX_56[1];
	} else {
		arr1 = ACTIVE_SC_IDX_52[0];
		arr2 = ACTIVE_SC_IDX_52[1];
	}
	moving_avg(iq_samples_in, iq_samples_out, len, arr1);
	moving_avg(iq_samples_in, iq_samples_out, len, arr2);
}
