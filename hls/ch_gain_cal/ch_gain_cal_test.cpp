#include "ch_gain_cal.h"
#include "stdio.h"

int main() {
	stream_iq lts_iq_fd;
	cplx16 new_lts[NFFT_MAX];
	cplx16 iq_sample_in, iq_sample_out;
	int16 i_in, q_in;
	uint1 smooth = 1;
	int i;
	FILE *fp_i;
	FILE *fp_q;

	// NON-HT
	fp_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/lts_iq_fd_i.dat", "r");
	fp_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/lts_iq_fd_q.dat", "r");
	for (i=0; i<2*NFFT_MAX; i++) {
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		lts_iq_fd.write(iq_sample_in);
	}
	ch_gain_cal(lts_iq_fd, new_lts, smooth, 0);
	for (i=0; i<NFFT_MAX; i++) {
		printf("%d. I: %d  ", i+1, (int)real(new_lts[i]));
		printf("Q: %d\n", (int)imag(new_lts[i]));
	}

	// HT
	fp_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/ht_lts_iq_fd_i.dat", "r");
	fp_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/ht_lts_iq_fd_q.dat", "r");
	for (i=0; i<NFFT_MAX; i++) {
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		lts_iq_fd.write(iq_sample_in);
	}
	ch_gain_cal(lts_iq_fd, new_lts, smooth, 1);
	for (i=0; i<NFFT_MAX; i++) {
		iq_sample_out = new_lts[i];
		printf("%d. I: %d  ", i+1, (int)real(iq_sample_out));
		printf("Q: %d\n", (int)imag(iq_sample_out));
	}

	return 0;
}
