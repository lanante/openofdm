#include "common.h"
#include "stdio.h"

int main() {
	cplx16 sample_in[NFFT_MAX];
	cplx16 iq_sample_in;
	int16 i_in, q_in;
	FILE *fp_i;
	FILE *fp_q;
	fp_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/cpe_est_in_i.dat", "r");
	fp_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/cpe_est_in_q.dat", "r");
	for (int i=0; i<NFFT_MAX; i++) {
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		sample_in[i] = iq_sample_in;
	}
	cplx16 lts_pilot[4] = {cplx16(-182, 171), cplx16(126, 212), cplx16(104, -224), cplx16(-202, -150)};
	uint1 current_polarity[4] = {0, 1, 0, 0};

	int16 cpe = cpe_estimate(sample_in, lts_pilot, current_polarity);
	printf("CPE: %d vs expected: -54\n", (int)cpe);

	return 0;
}
