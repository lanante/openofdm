#include "common.h"

uint2 ht_pol_nr = 0;

uint7 get_polarity(uint1 current_polarity[4], uint1 ht, uint7 pol_nr) {
#pragma HLS reset variable=ht_pol_nr
	if (ht) {
		uint2 first = ht_pol_nr+2;
		uint2 second = ht_pol_nr+3;
		uint2 third = ht_pol_nr;
		uint2 fourth = ht_pol_nr+1;
		current_polarity[0] = HT_POLARITY[first]^POLARITY[pol_nr];
		current_polarity[1] = HT_POLARITY[second]^POLARITY[pol_nr];
		current_polarity[2] = HT_POLARITY[third]^POLARITY[pol_nr];
		current_polarity[3] = HT_POLARITY[fourth]^POLARITY[pol_nr];
		ht_pol_nr += 1;
	} else {
		current_polarity[0] = POLARITY[pol_nr];
		current_polarity[1] = ~POLARITY[pol_nr];
		current_polarity[2] = POLARITY[pol_nr];
		current_polarity[3] = POLARITY[pol_nr];
	}
	return (pol_nr == 126 ? (uint7)0 : pol_nr+=1);
}
