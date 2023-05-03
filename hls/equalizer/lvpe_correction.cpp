#include "common.h"

int32 lvpe_correction(int18 sym_phase[NFFT_MAX], int16 cpe, int32 acc_PEG, int24 Sxy, uint6* idx, uint7 length) {
	int32 lvpe_store;
	int32 peg_sym_scale;
	static uint10 Sx2 = 980;
	int7 used_idx;
	lvpe_corr: for (uint7 i=0; i<length; i++) {
#pragma HLS PIPELINE
		if (*(idx+i)+1 <= 33) used_idx = *(idx+i)+1;
		else used_idx = *(idx+i)-63;

		lvpe_store = used_idx*Sxy/Sx2;
		peg_sym_scale = used_idx*acc_PEG;
		int18 phase_err = (int18)cpe + (int18)lvpe_store + (int18)peg_sym_scale;
		if (phase_err > PI) {
			sym_phase[*(idx+i)] = phase_err - DOUBLE_PI;
		} else if (phase_err < -PI) {
			sym_phase[*(idx+i)] = phase_err + DOUBLE_PI;
		} else {
			sym_phase[*(idx+i)] = phase_err;
		}
	}

	return acc_PEG + (int32)(((Sxy << 5)/Sx2) >> 5); 
}
