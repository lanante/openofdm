#include "common.h"
#include "stdio.h"

int main() {
	uint1 current_polarity[4];
	uint1 ht = 0;
	uint7 pol_nr = 0;

	for (int i=0; i<8; i++) {
		if (i == 3) ht = 1;
		pol_nr = get_polarity(current_polarity, ht, pol_nr);
		printf("%d %d %d %d\n", (int)current_polarity[0], (int)current_polarity[1], (int)current_polarity[2], (int)current_polarity[3]);
	}

	return 0;
}
