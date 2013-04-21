#ifndef _FX_CLASS_H
#define _FX_CLASS_H

#include <MULTIFX_typedef.h>
typedef struct FX_S FX_T;

FX_T* FX_init( MULTIFX_UINT32_T out_buf_char_size, MULTIFX_UINT32_T n_params, MULTIFX_UINT32_T state_length,MULTIFX_FLOATING_T* in_buff);
MULTIFX_API_RET FX_set_params (FX_T* p_FX, MULTIFX_FLOATING_T* p_params, MULTIFX_FLOATING_T*p_state);
MULTIFX_API_RET FX_set_implementation (FX_T* p_FX, MULTIFX_INT32_T (*pfunc)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_UINT32_T));
MULTIFX_API_RET FX_process(FX_T* p_FX);
MULTIFX_API_RET FX_bufcpy(FX_T* p_FX,MULTIFX_FLOATING_T* buff);
FX_T* FX_clone(FX_T* p_FX,MULTIFX_FLOATING_T* in_buff);


#endif
