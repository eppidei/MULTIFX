#ifndef _FX_LIBRARY_UTILS_H
#define  _FX_LIBRARY_UTILS_H

#include <MULTIFX_typedef.h>

MULTIFX_API_RET filter_DII_T (MULTIFX_FLOATING_T *in, MULTIFX_FLOATING_T *out, MULTIFX_FLOATING_T *a, MULTIFX_UINT32_T order, MULTIFX_FLOATING_T *b, MULTIFX_FLOATING_T *state,MULTIFX_UINT32_T len_frame);
//MULTIFX_API_RET filter_time_varying_DII_T (MULTIFX_FLOATING_T *in, MULTIFX_FLOATING_T *out, MULTIFX_FLOATING_T *a, MULTIFX_UINT32_T order, MULTIFX_FLOATING_T *b, MULTIFX_FLOATING_T *state,MULTIFX_UINT32_T len_frame);
MULTIFX_API_RET oscillator (MULTIFX_FLOATING_T f_samp, MULTIFX_FLOATING_T f_synth, MULTIFX_UINT32_T len_frame, MULTIFX_FLOATING_T *out_frame, MULTIFX_FLOATING_T phase_offset, MULTIFX_FLOATING_T amp,MULTIFX_FLOATING_T* old_phase);
MULTIFX_API_RET filter_DII_T_sample_based (MULTIFX_FLOATING_T *in, MULTIFX_FLOATING_T *out, MULTIFX_FLOATING_T *a, MULTIFX_UINT32_T order, MULTIFX_FLOATING_T *b, MULTIFX_FLOATING_T *state);


#endif
