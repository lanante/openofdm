#include "common.h"
#include "stdio.h"

int main() {
	cplx16 iq_sample_in, iq_sample_out;
	int18 sym_phase[NFFT_MAX];
	int16 i_in, q_in;
	cplx16 symbol_iq[NFFT_MAX];
	cplx16 lts_pilot[4] = {cplx16(36, -246), cplx16(-236, -81), cplx16(60, 238), cplx16(248, -12)};
	uint1 current_polarity[4] = {0, 1, 0, 0};
	FILE (*fp_i), (*fp_q);
	fp_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/in_i.dat", "r");
	fp_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/in_q.dat", "r");
	for (int i=0; i<NFFT_MAX; i++) {
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		symbol_iq[i] = iq_sample_in;
		sym_phase[i] = (int18)-36;
	}

	int24 Sxy = peg_estimate(symbol_iq, sym_phase, lts_pilot, current_polarity);
	printf("Sxy: %d vs expected: -70\n", (int)Sxy);

	return 0;
}
