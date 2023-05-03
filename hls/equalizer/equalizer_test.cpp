#include "common.h"

int main() {
	stream_iq sample_in, sample_out;
	cplx16 iq_sample_in, iq_sample_out;
	cplx16 new_lts[NFFT_MAX];
	int16 i_in, q_in;
	FILE (*fp_i), (*fp_q), (*fp_lts_i), (*fp_lts_q);
	uint12 nof_ofdm_sym;
	int32 acc_PEG;
	bool checkFailed;
	char format[] = "/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/%s/%s.dat";
	char filename[sizeof(format)+100];
	char folder[30];

	uint1 phy_type = 1;

	if (phy_type == 0) {
		sprintf(folder, "NonHT_packet");
		nof_ofdm_sym = 1;
	} else {
		sprintf(folder, "HT_packet");
		nof_ofdm_sym = 44;
	}

	sprintf(filename, format, folder, "new_lts_i");
	fp_lts_i = fopen(filename, "r");
	sprintf(filename, format, folder, "new_lts_q");
	fp_lts_q = fopen(filename, "r");
	for (int i=0; i<NFFT_MAX; i++) {
		fscanf(fp_lts_i, "%d", &i_in);
		fscanf(fp_lts_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		new_lts[i] = iq_sample_in;
	}

	if (phy_type == 1) {
		nof_ofdm_sym = 3;
	}
	sprintf(filename, format, folder, "in_i");
	fp_i = fopen(filename, "r");
	sprintf(filename, format, folder, "in_q");
	fp_q = fopen(filename, "r");
	for (int i=0; i<nof_ofdm_sym*NFFT_MAX; i++) {
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		sample_in.write(iq_sample_in);
	}

	acc_PEG = 0;
	acc_PEG = equalizer(sample_in, sample_out, new_lts, nof_ofdm_sym, 0, acc_PEG);

	sprintf(filename, format, folder, "out_i");
	fp_i = fopen(filename, "r");
	sprintf(filename, format, folder, "out_q");
	fp_q = fopen(filename, "r");
	checkFailed = false;
	for (int j=0; j<nof_ofdm_sym*48; j++) {
		iq_sample_out = sample_out.read();
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		if (i_in != real(iq_sample_out) || q_in != imag(iq_sample_out)) {
			printf("%d. %d %d is wrong! Expected: %d %d\n", j+1, (int)real(iq_sample_out), (int)imag(iq_sample_out), (int)i_in, (int)q_in);
			checkFailed = true;
		}
	}
	if (!checkFailed) printf("Legacy check passed!\n");

	if (phy_type) {
		sprintf(filename, format, folder, "ht_new_lts_i");
		fp_lts_i = fopen(filename, "r");
		sprintf(filename, format, folder, "ht_new_lts_q");
		fp_lts_q = fopen(filename, "r");
		for (int i=0; i<NFFT_MAX; i++) {
			fscanf(fp_lts_i, "%d", &i_in);
			fscanf(fp_lts_q, "%d", &q_in);
			iq_sample_in = cplx16(i_in, q_in);
			new_lts[i] = iq_sample_in;
		}

		nof_ofdm_sym = 44;
		sprintf(filename, format, folder, "ht_in_i");
		fp_i = fopen(filename, "r");
		sprintf(filename, format, folder, "ht_in_q");
		fp_q = fopen(filename, "r");
		for (int i=0; i<nof_ofdm_sym*NFFT_MAX; i++) {
			fscanf(fp_i, "%d", &i_in);
			fscanf(fp_q, "%d", &q_in);
			iq_sample_in = cplx16(i_in, q_in);
			sample_in.write(iq_sample_in);
		}

		equalizer(sample_in, sample_out, new_lts, nof_ofdm_sym, phy_type, acc_PEG);

		sprintf(filename, format, folder, "ht_out_i");
		fp_i = fopen(filename, "r");
		sprintf(filename, format, folder, "ht_out_q");
		fp_q = fopen(filename, "r");
		checkFailed = false;
		for (int j=0; j<nof_ofdm_sym*52; j++) {
			iq_sample_out = sample_out.read();
			fscanf(fp_i, "%d", &i_in);
			fscanf(fp_q, "%d", &q_in);
			if (i_in != real(iq_sample_out) || q_in != imag(iq_sample_out)) {
				printf("%d. %d %d is wrong! Expected: %d %d\n", j+1, (int)real(iq_sample_out), (int)imag(iq_sample_out), (int)i_in, (int)q_in);
				checkFailed = true;
			}
		}
		if (!checkFailed) printf("HT check passed!\n");
	}

	return 0;
}
