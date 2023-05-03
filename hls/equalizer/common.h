#include "stdint.h"
#include "ap_int.h"
#include "hls_stream.h"
#include <complex>
#define NFFT_MAX 64
#define PILOT_MAX 4
#define AVG_LEN_HALF 2
#define ATAN_LUT_SCALE 9
#define ROTATE_LUT_SCALE_SHIFT 11
#define CONS_SCALE_SHIFT 10
#define DOUBLE_PI 3217
#define PI 1608
#define PI_2 804
#define PI_4 402
#define PI_3_4 1206

typedef ap_uint<1> uint1;
typedef ap_uint<2> uint2;
typedef ap_uint<3> uint3;
typedef ap_uint<4> uint4;
typedef ap_uint<5> uint5;
typedef ap_uint<6> uint6;
typedef ap_uint<7> uint7;
typedef ap_uint<8> uint8;
typedef ap_uint<9> uint9;
typedef ap_uint<10> uint10;
typedef ap_uint<11> uint11;
typedef ap_uint<12> uint12;
typedef ap_uint<16> uint16;
typedef ap_uint<32> uint32;
typedef ap_int<2> int2;
typedef ap_int<5> int5;
typedef ap_int<6> int6;
typedef ap_int<7> int7;
typedef ap_int<9> int9;
typedef ap_int<10> int10;
typedef ap_int<12> int12;
typedef ap_int<16> int16;
typedef ap_int<17> int17;
typedef ap_int<18> int18;
typedef ap_int<24> int24;
typedef ap_int<32> int32;

typedef std::complex<int16> cplx16;
typedef std::complex<int32> cplx32;
typedef hls::stream<cplx16> stream_iq;

static uint3 nof_pilots = 4;
static uint6 pilot_loc[4] = {7, 21, 43, 57};

static uint6 ALL_64[NFFT_MAX] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};

static uint6 ACTIVE_SC_IDX_52[2][26] = {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26},
									  	  {38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63}};
static uint6 ACTIVE_SC_IDX_56[2][28] = {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28},
										 {36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63}};
static uint6 DATA_SC_IDX_48[48] = {1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,19,20,22,23,24,25,26,38,39,40,41,42,44,45,46,47,48,49,50,51,52,53,54,55,56,58,59,60,61,62,63};
static uint6 DATA_SC_IDX_52[52] = {1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,19,20,22,23,24,25,26,27,28,36,37,38,39,40,41,42,44,45,46,47,48,49,50,51,52,53,54,55,56,58,59,60,61,62,63};

static uint1 POLARITY[127] = {0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1};
static uint1 HT_POLARITY[4] = {0,0,0,1};

cplx32 complex_mult(cplx16 a, cplx16 b);
int16 phase_inst(cplx32 phase_in);
cplx16 rotate(int16 phase, cplx16 in_iq);
uint7 get_polarity(uint1 current_polarity[PILOT_MAX], uint1 ht, uint7 pol_nr);
int16 cpe_estimate(cplx16 sample_in[NFFT_MAX], cplx16 lts_pilot[PILOT_MAX], uint1 current_polarity[PILOT_MAX]);
int24 peg_estimate(cplx16 symbol_iq[NFFT_MAX], int18 sym_phase[NFFT_MAX], cplx16 lts_pilot[PILOT_MAX], uint1 current_polarity[PILOT_MAX]);
int32 lvpe_correction(int18 sym_phase[NFFT_MAX], int16 cpe, int32 acc_PEG, int24 Sxy, uint6* idx, uint7 length);
void equalizer_per_sym(cplx16 symbol_iq[NFFT_MAX], int18 sym_phase[NFFT_MAX], stream_iq &sample_out, cplx16 new_lts[NFFT_MAX], uint6* arr, uint7 length);
int32 equalizer(stream_iq &sample_in, stream_iq &sample_out, cplx16 new_lts[NFFT_MAX], uint12 nof_ofdm_sym, uint1 ht, int32 acc_PEG);

