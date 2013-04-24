#include <MULTIFX_OSCILLATOR_class.h>
#include <MULTIFX_typedef.h>
#include <stdlib.h>


struct OSCILLATOR_S {
MULTIFX_FLOATING_T phase_offset;
MULTIFX_FLOATING_T bias;
MULTIFX_FLOATING_T amp;
MULTIFX_FLOATING_T sample_rate;
MULTIFX_FLOATING_T freq;
MULTIFX_FLOATING_T *tv_parameter;
MULTIFX_FLOATING_T *oscillator_state;
MULTIFX_API_RET (*var_param_func)(MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_UINT32_T,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T*);//(MULTIFX_FLOATING_T f_samp, MULTIFX_FLOATING_T f_synth, MULTIFX_UINT32_T len_frame, MULTIFX_FLOATING_T *out_frame, MULTIFX_FLOATING_T phase_offset, MULTIFX_FLOATING_T amp,MULTIFX_FLOATING_T* old_phase)
};

OSCILLATOR_T* OSC_init()
{
    MULTIFX_UINT32_T state_len = 1;

     OSCILLATOR_T* pthis = calloc(1,sizeof(OSCILLATOR_T));

     if (pthis!=NULL)
     {
         pthis->oscillator_state = calloc(state_len,sizeof(MULTIFX_FLOATING_T));
     }


    return pthis;

}

MULTIFX_API_RET OSC_configure (OSCILLATOR_T *p_osc,MULTIFX_FLOATING_T low_limit, MULTIFX_FLOATING_T high_limit,MULTIFX_FLOATING_T sample_rate,MULTIFX_FLOATING_T freq ,MULTIFX_FLOATING_T phase_off)
{
    p_osc->sample_rate  =sample_rate;
    p_osc->freq         = freq;
    p_osc->phase_offset =phase_off;
    p_osc->bias         = (low_limit+high_limit)/2;
    p_osc->amp          =  (high_limit-low_limit)/2;

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET OSC_set_implementation (OSCILLATOR_T *p_osc, MULTIFX_INT32_T (*pfunc)(MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_UINT32_T,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T*))
{
    p_osc->var_param_func = pfunc;

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET OSC_trigger(OSCILLATOR_T *p_osc,MULTIFX_FLOATING_T* tv_parameters,MULTIFX_UINT32_T param_idx,MULTIFX_UINT32_T len_frame)
{

    MULTIFX_API_RET ret = 0;

    p_osc->tv_parameter = &tv_parameters[param_idx*len_frame];//indirizza il parametro all'interno buffer parametri tempo varianti

    ret = (p_osc->var_param_func)(p_osc->sample_rate,p_osc->freq,len_frame,p_osc->tv_parameter,p_osc->phase_offset, p_osc->amp,p_osc->bias,p_osc->oscillator_state);

    return ret;
}

MULTIFX_API_RET OSC_release (OSCILLATOR_T* p_osc)
{
    free(p_osc->oscillator_state);
    free(p_osc);

    return MULTIFX_DEFAULT_RET;
}
