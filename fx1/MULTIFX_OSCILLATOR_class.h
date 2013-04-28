#ifndef _MULTIFX_OSCILLATOR_CLASS_H
#define _MULTIFX_OSCILLATOR_CLASS_H

#include <MULTIFX_typedef.h>




OSCILLATOR_T* OSC_init();
MULTIFX_API_RET OSC_configure (OSCILLATOR_T *p_osc,MULTIFX_FLOATING_T low_limit, MULTIFX_FLOATING_T high_limit,MULTIFX_FLOATING_T sample_rate,MULTIFX_FLOATING_T freq ,MULTIFX_FLOATING_T phase_off);
MULTIFX_API_RET OSC_set_implementation (OSCILLATOR_T *p_osc, MULTIFX_INT32_T (*pfunc)(MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_UINT32_T,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T*));
MULTIFX_API_RET OSC_trigger(OSCILLATOR_T *p_osc,MULTIFX_FLOATING_T* tv_parameters,MULTIFX_UINT32_T param_idx,MULTIFX_UINT32_T len_frame);
MULTIFX_API_RET OSC_release (OSCILLATOR_T* p_osc);

#endif
