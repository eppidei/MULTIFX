#include <MULTIFX_FX_library.h>
#include <MULTIFX_defines.h>
#include <MULTIFX_typedef.h>
#include <math.h>
#include <MULTIFX_FX_library_utils.h>
//PARAM ORDER Fc,kk,Fs
MULTIFX_API_RET moog_filter (STD_FX_LIB_CALL)
{
MULTIFX_FLOATING_T      Fc          = params[0];
MULTIFX_FLOATING_T      kk          = params[1];
MULTIFX_FLOATING_T      Fs          = params[2];
MULTIFX_FLOATING_T      Wc          = 2*PI_G*Fc;
MULTIFX_FLOATING_T      Gamma       = Wc/(Wc+2*Fs);
MULTIFX_FLOATING_T      alfa        = (2*Fs-Wc)/(2*Fs+Wc);
MULTIFX_FLOATING_T      Gammapow    = pow(Gamma,4);
MULTIFX_FLOATING_T      b[MOOG_ORDER]        =  {Gammapow *1,Gammapow *4,Gammapow *6,Gammapow *4,Gammapow *1};
MULTIFX_FLOATING_T      a[MOOG_ORDER]        =  {1,-4*(alfa-kk*Gammapow),-6*(-pow(alfa,2)-kk*Gammapow),-4*(pow(alfa,3)-kk*Gammapow),(pow(alfa,4)+kk*Gammapow)};
//static MULTIFX_FLOATING_T state[MOOG_ORDER];
MULTIFX_API_RET ret = 0;

 ret=filter_DII_T (in_frame, out_frame, a, MOOG_ORDER, b, state,frame_len);
 STRAIGHT_RETURN(ret);

return MULTIFX_DEFAULT_RET;

}

MULTIFX_API_RET test_tone (STD_FX_LIB_CALL)
{
    MULTIFX_FLOATING_T f_synth          = params[0];
    MULTIFX_FLOATING_T f_samp           = params[1];
    MULTIFX_FLOATING_T phase_offset     = params[2];
    MULTIFX_FLOATING_T amp              = params[3];
    MULTIFX_API_RET    ret              = 0;

    ret = oscillator (f_samp, f_synth, frame_len, out_frame, phase_offset,  amp,state);
    STRAIGHT_RETURN(ret);

    return ret;
}
