#include <MULTIFX_FX_library_utils.h>
#include <math.h>

MULTIFX_API_RET filter_DII_T (MULTIFX_FLOATING_T *in, MULTIFX_FLOATING_T *out, MULTIFX_FLOATING_T *a, MULTIFX_UINT32_T order, MULTIFX_FLOATING_T *b, MULTIFX_FLOATING_T *state,MULTIFX_UINT32_T len_frame)
{
    MULTIFX_UINT32_T i=0,k=0;

    for (k=0;k<len_frame;k++)
    {


    out[k] = b[0]*(in[k])+state[0];

        for (i=0;i<(order-1);i++)
        {
            state[i]=b[i+1]*(in[k])+state[i+1]-a[i+1]*(out[k]);
        }

    }

    return MULTIFX_DEFAULT_RET;

}

MULTIFX_API_RET filter_DII_T_sample_based (MULTIFX_FLOATING_T in, MULTIFX_FLOATING_T *out, MULTIFX_FLOATING_T *a, MULTIFX_UINT32_T order, MULTIFX_FLOATING_T *b, MULTIFX_FLOATING_T *state)
{
    MULTIFX_UINT32_T i=0;


    *out = b[0]*(in)+state[0];

        for (i=0;i<(order-1);i++)
        {
            state[i]=b[i+1]*(in)+state[i+1]-a[i+1]*(*out);
        }



    return MULTIFX_DEFAULT_RET;

}


//MULTIFX_API_RET filter_time_varying_DII_T (MULTIFX_FLOATING_T *in, MULTIFX_FLOATING_T *out, MULTIFX_FLOATING_T *a, MULTIFX_UINT32_T order, MULTIFX_FLOATING_T *b, MULTIFX_FLOATING_T *state,MULTIFX_UINT32_T len_frame)
//{
//    MULTIFX_UINT32_T i=0,k=0,idx=0;
//
//    for (k=0;k<len_frame;k++)
//    {
//        idx = 0 + k*order;
//
//    out[k] = b[idx]*(in[k])+state[0];
//
//        for (i=0;i<(order-1);i++)
//        {
//            idx = (i+1) + k*order;
//            state[i]=b[idx]*(in[k])+state[i+1]-a[idx]*(out[k]);
//        }
//
//    }
//
//    return MULTIFX_DEFAULT_RET;
//
//}

MULTIFX_API_RET oscillator (MULTIFX_FLOATING_T f_samp, MULTIFX_FLOATING_T f_synth, MULTIFX_UINT32_T len_frame, MULTIFX_FLOATING_T *out_frame, MULTIFX_FLOATING_T phase_offset, MULTIFX_FLOATING_T amp,MULTIFX_FLOATING_T* old_phase)
{

    //static MULTIFX_FLOATING_T old_phase ;
    MULTIFX_UINT32_T i = 0;
    MULTIFX_FLOATING_T normalized_phase = 0.0;

    for (i=0;i<len_frame;i++)
    {
        normalized_phase = fmod(f_synth/f_samp*(i+1),1)+ *old_phase;
        out_frame[i]= amp*sin(2 * PI_G * normalized_phase + phase_offset);
    }

    *old_phase = normalized_phase ;


    return MULTIFX_DEFAULT_RET;


}


