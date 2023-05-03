#include "stdint.h"
#include "ap_int.h"
#include "hls_stream.h"
#include <complex>
#define NFFT_MAX 64
#define AVG_LEN_HALF 2

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
typedef ap_int<12> int12;
typedef ap_int<16> int16;
typedef ap_int<17> int17;
typedef ap_int<18> int18;
typedef ap_int<24> int24;
typedef ap_int<32> int32;

typedef std::complex<int16> cplx16;
typedef std::complex<int32> cplx32;
typedef hls::stream<cplx16> stream_iq;

static uint6 ACTIVE_SC_IDX_52[2][26] = {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26},
									  	  {38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63}};
static uint6 ACTIVE_SC_IDX_56[2][28] = {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28},
										 {36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63}};

static int2 LTS_REF[NFFT_MAX] = 	{1, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, -1, -1, -1, -1, 1, 1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1};
static int2 HT_LTS_REF[NFFT_MAX] = 	{1, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, -1, -1, -1, -1, 1, 1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1};

void ch_gain_cal(stream_iq &lts_iq_fd, cplx16 new_lts[NFFT_MAX], uint1 smooth, uint1 ltf_type);
void moving_avg(cplx16 iq_samples_in[NFFT_MAX], cplx16 iq_samples_out[NFFT_MAX], uint5 len, uint6* arr);
void smoothing(cplx16 iq_samples_in[NFFT_MAX], cplx16 iq_samples_out[NFFT_MAX], uint6 ru_width);
