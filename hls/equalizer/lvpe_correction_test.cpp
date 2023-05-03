#include "common.h"
#include "stdio.h"

int main() {
	int18 sym_phase[NFFT_MAX];
	uint7 length = PILOT_MAX;
	int16 cpe = -108;
	int32 prev_PEG = -1;
	int24 Sxy = 0;
	int18 expected[4] = {-116, -130, -88, -102};
	lvpe_correction(sym_phase, cpe, prev_PEG, Sxy, pilot_loc, length);
	bool checkFailed = false;
	for (int i=0; i<length; i++) {
		if (sym_phase[pilot_loc[i]] != expected[i]) {
			printf("%d. %d is wrong, expected: %d\n", i+1, (int)sym_phase[pilot_loc[i]], (int)expected[i]);  //
			checkFailed = true;
		}
	}
	if (!checkFailed) printf("Pilot check passed!\n"); else printf("\n");

	length = 48;
	Sxy = 1526;
	int32 lvpe = lvpe_correction(sym_phase, cpe, prev_PEG, Sxy, DATA_SC_IDX_48, length);
	FILE *fp_out;
	int18 sym_phase_out;
	fp_out = fopen("/groups/ilabt-imec-be/openwifi_simulation/git_thijs/openofdminternal/hls/test/sym_phase_out.dat", "r");
	checkFailed = false;
	for (int i=0; i<length; i++) {
		fscanf(fp_out, "%d", &sym_phase_out);
		if (sym_phase[DATA_SC_IDX_48[i]] != sym_phase_out) {
			printf("%d. %d is wrong, expected: \n", i+1, (int)sym_phase[DATA_SC_IDX_48[i]], (int)sym_phase_out);
			checkFailed = true;
		}
	}
	if (!checkFailed) printf("Data check passed!\n");

	return 0;
}
