#include "common.h"

int32 equalizer(stream_iq &sample_in, stream_iq &sample_out, cplx16 new_lts[NFFT_MAX], uint12 nof_ofdm_sym, uint1 ht, int32 acc_PEG) {
#pragma HLS INTERFACE mode=bram port=new_lts latency=2 storage_type=ram_1p
	cplx16 symbol_iq[NFFT_MAX];
	uint1 current_polarity[4];
	int16 cpe;
	int24 Sxy;
	int18 sym_phase[NFFT_MAX];
	cplx16 lts_pilot[4];
	uint6* arr;
	uint7 length, pol_nr;
	if (ht) {
		pol_nr = 3;
		arr = DATA_SC_IDX_52;
		length = 52;
	} else {
		pol_nr = 0;
		arr = DATA_SC_IDX_48;
		length = 48;
	}

	lts_pilot: for (uint4 j=0; j<PILOT_MAX; j++) {
		lts_pilot[j] = new_lts[pilot_loc[j]];
	}

	equalize: for (uint12 i=0; i<nof_ofdm_sym; i++) {
#pragma HLS allocation function instances=lvpe_correction limit=1
		symbol_iq: for (uint7 l=0; l<NFFT_MAX; l++) {
#pragma HLS PIPELINE
			symbol_iq[l] = sample_in.read();
		}
		pol_nr = get_polarity(current_polarity, ht, pol_nr);

		cpe = cpe_estimate(symbol_iq, lts_pilot, current_polarity);

		lvpe_correction(sym_phase, cpe, acc_PEG, 0, pilot_loc, 4);

		Sxy = peg_estimate(symbol_iq, sym_phase, lts_pilot, current_polarity);

		acc_PEG = lvpe_correction(sym_phase, cpe, acc_PEG, Sxy, arr, length);

		equalizer_per_sym(symbol_iq, sym_phase, sample_out, new_lts, arr, length);
	}

	return acc_PEG;
}
