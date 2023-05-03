#include "common.h"

int main() {
	cplx16 iq_sample_in, iq_sample_out;
	stream_iq sample_out;
	int18 sym_phase[NFFT_MAX];
	int18 sym_phase_in;
	int16 i_in, q_in;
	cplx16 symbol_iq[NFFT_MAX], new_lts[NFFT_MAX];
	FILE (*fp_i), (*fp_q), (*fp_lts_i), (*fp_lts_q), (*fp_sym_phase);
	fp_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/in_i.dat", "r");
	fp_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/in_q.dat", "r");
	fp_lts_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/new_lts_i.dat", "r");
	fp_lts_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/new_lts_q.dat", "r");
	fp_sym_phase = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/sym_phase_in.dat", "r");
	for (int i=0; i<NFFT_MAX; i++) {
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		symbol_iq[i] = iq_sample_in;
		fscanf(fp_lts_i, "%d", &i_in);
		fscanf(fp_lts_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		new_lts[i] = iq_sample_in;
		fscanf(fp_sym_phase, "%d", &sym_phase_in);
		sym_phase[i] = sym_phase_in;
	}

	uint7 length = 48;
	uint6* arr = DATA_SC_IDX_48;
	equalizer_per_sym(symbol_iq, sym_phase, sample_out, new_lts, arr, length);
	fp_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/out_i.dat", "r");
	fp_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/out_q.dat", "r");
	bool checkFailed = false;
	for (int j=0; j<length; j++) {
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		iq_sample_out = sample_out.read();
		if (i_in != real(iq_sample_out) || q_in != imag(iq_sample_out)) {
			printf("%d. %d %d is wrong! Expected: %d %d\n", j+1, (int)real(iq_sample_out), (int)imag(iq_sample_out), (int)i_in, (int)q_in);
			checkFailed = true;
		}
	}
	if (!checkFailed) printf("Legacy check passed!\n");

	// HT part
	fp_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/ht_in_i.dat", "r");
	fp_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/ht_in_q.dat", "r");
	fp_lts_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/ht_new_lts_i.dat", "r");
	fp_lts_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/ht_new_lts_q.dat", "r");
	fp_sym_phase = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/ht_sym_phase_in.dat", "r");
	for (int i=0; i<NFFT_MAX; i++) {
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		symbol_iq[i] = iq_sample_in;
		fscanf(fp_lts_i, "%d", &i_in);
		fscanf(fp_lts_q, "%d", &q_in);
		iq_sample_in = cplx16(i_in, q_in);
		new_lts[i] = iq_sample_in;
		fscanf(fp_sym_phase, "%d", &sym_phase_in);
		sym_phase[i] = sym_phase_in;
	}
	length = 52;
	arr = DATA_SC_IDX_52;
	equalizer_per_sym(symbol_iq, sym_phase, sample_out, new_lts, arr, length);
	fp_i = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/ht_out_i.dat", "r");
	fp_q = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/HT_packet/ht_out_q.dat", "r");
	checkFailed = false;
	for (int k=0; k<length; k++) {
		fscanf(fp_i, "%d", &i_in);
		fscanf(fp_q, "%d", &q_in);
		iq_sample_out = sample_out.read();
		if (i_in != real(iq_sample_out) || q_in != imag(iq_sample_out)) {
			printf("%d. %d %d is wrong! Expected: %d %d\n", k+1, (int)real(iq_sample_out), (int)imag(iq_sample_out), (int)i_in, (int)q_in);
			checkFailed = true;
		}
	}
	if (!checkFailed) printf("HT check passed!\n");

	return 0;
}
