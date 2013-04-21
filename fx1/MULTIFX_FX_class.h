#ifndef _FX_CLASS_H
#define _FX_CLASS_H

#include <MULTIFX_typedef.h>
typedef struct FX_S FX_T;

FX_T* FX_init( MULTIFX_UINT16_T n_bit,MULTIFX_UINT16_T stereo_mode, MULTIFX_UINT32_T fragment_size, MULTIFX_UINT32_T n_static_params, MULTIFX_UINT32_T  n_time_varying_params,MULTIFX_UINT32_T state_length,MULTIFX_FLOATING_T* in_buff);
MULTIFX_API_RET FX_set_static_params (FX_T* p_FX, MULTIFX_FLOATING_T* p_static_params,MULTIFX_UINT32_T len_static_param);
MULTIFX_API_RET FX_set_state (FX_T* p_FX, MULTIFX_FLOATING_T* p_state,MULTIFX_UINT32_T len_state);
MULTIFX_API_RET FX_init_timevarying_params (FX_T* p_FX, MULTIFX_FLOATING_T* p_tv_params,MULTIFX_UINT32_T n_tv_param);
MULTIFX_API_RET FX_set_implementation (FX_T* p_FX, MULTIFX_INT32_T (*pfunc)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_UINT32_T));
MULTIFX_API_RET FX_process(FX_T* p_FX);
MULTIFX_API_RET FX_bufcpy(FX_T* p_FX,MULTIFX_FLOATING_T* buff);
MULTIFX_API_RET FX_release (FX_T* p_FX);
FX_T* FX_clone(FX_T* p_FX,MULTIFX_FLOATING_T* in_buff);


#endif
