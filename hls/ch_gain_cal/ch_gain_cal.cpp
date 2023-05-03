#include "ch_gain_cal.h"

void ch_gain_cal(stream_iq &lts_iq_fd, cplx16 new_lts[NFFT_MAX], uint1 smooth, uint1 ltf_type) {
#pragma HLS INTERFACE mode=bram port=new_lts
	cplx16 iq_sample;
	cplx16 first_lts[NFFT_MAX];
	cplx16 avg_lts[NFFT_MAX];
	if (ltf_type == 0) {
		l_lts: for (int i=0; i<2*NFFT_MAX; i++) {
#pragma HLS PIPELINE
			if (i<2*NFFT_MAX) {
				iq_sample = lts_iq_fd.read();
			}
			if (i<NFFT_MAX) {
				first_lts[i] = iq_sample;
			} else if (i<2*NFFT_MAX) {
				avg_lts[i-NFFT_MAX] = cplx16(LTS_REF[i-NFFT_MAX]*(((real(first_lts[i-NFFT_MAX])>>1) + (real(iq_sample)>>1))),
									LTS_REF[i-NFFT_MAX]*(((imag(first_lts[i-NFFT_MAX])>>1) + (imag(iq_sample)>>1))));
			}
		}
	} else {
		ht_lts: for (int i=0; i<NFFT_MAX; i++) {
			iq_sample = lts_iq_fd.read();
			avg_lts[i] = cplx16(HT_LTS_REF[i]*real(iq_sample), HT_LTS_REF[i]*imag(iq_sample));
		}
	}
	if (smooth) {
		uint6 ru_width = ltf_type ? 56 : 52;
		smoothing(avg_lts, first_lts, ru_width);
		new_lts_smooth_out: for (int i=0; i<NFFT_MAX; i++) {
			new_lts[i] = first_lts[i];
		}
	} else {
		new_lts_out: for (int i=0; i<NFFT_MAX; i++) {
			new_lts[i] = avg_lts[i];
		}
	}
}

