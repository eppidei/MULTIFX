#include <MULTIFX_FX_library.h>
#include <MULTIFX_defines.h>
#include <MULTIFX_typedef.h>
#include <math.h>
#include <MULTIFX_FX_library_utils.h>
//PARAM ORDER Fc,kk,Fs
MULTIFX_API_RET moog_filter (STD_FX_LIB_CALL)
{

MULTIFX_FLOATING_T      Fc = 0 ,kk= 0;
MULTIFX_FLOATING_T      Wc= 0,Gamma= 0,alfa= 0,Gammapow= 0;
MULTIFX_FLOATING_T      Fs          = 0;
static MULTIFX_FLOATING_T      b[MOOG_ORDER] = {0,0,0,0,0};
static MULTIFX_FLOATING_T      a[MOOG_ORDER] = {0,0,0,0,0};
MULTIFX_API_RET ret = 0;
MULTIFX_UINT32_T  time_idx =0, fx_idx=0;


Fs = params[0];
for (time_idx=0;time_idx<frame_len;time_idx++)
{
    fx_idx = 0;
    Fc     = time_var_params[time_idx+fx_idx*frame_len];
    fx_idx = 1;
    kk     = time_var_params[time_idx+fx_idx*frame_len];

    Wc          = 2*PI_G*Fc;
    Gamma       = Wc/(Wc+2*Fs);
    alfa        = (2*Fs-Wc)/(2*Fs+Wc);
    Gammapow    = pow(Gamma,4);

    b[0] = Gammapow;
    b[1] = Gammapow*4;
    b[2] = Gammapow*6;
    b[3] = Gammapow*4;
    b[4] = Gammapow;

    a[0] = 1;
    a[1] = -4*(alfa-kk*Gammapow);
    a[2] = -6*(-pow(alfa,2)-kk*Gammapow);
    a[3] = -4*(pow(alfa,3)-kk*Gammapow);
    a[4] = (pow(alfa,4)+kk*Gammapow);

     ret=filter_DII_T_sample_based (in_frame[time_idx], &out_frame[time_idx], a, MOOG_ORDER, b, state);
    STRAIGHT_RETURN(ret);
}



return MULTIFX_DEFAULT_RET;

}

MULTIFX_API_RET test_tone (STD_FX_LIB_CALL)
{
    MULTIFX_FLOATING_T f_synth          = params[0];
    MULTIFX_FLOATING_T f_samp           = params[1];
    MULTIFX_FLOATING_T phase_offset     = params[2];
    MULTIFX_FLOATING_T amp              = params[3];
    MULTIFX_FLOATING_T bias = 0;
    MULTIFX_API_RET    ret              = 0;

    ret = oscillator (f_samp, f_synth, frame_len, out_frame, phase_offset,  amp,bias,state);
    STRAIGHT_RETURN(ret);

    return ret;
}
