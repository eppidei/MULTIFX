#ifndef _FX_CLASS_H
#define _FX_CLASS_H

#include <MULTIFX_typedef.h>
typedef struct FX_S FX_T;

FX_T* FX_init(MULTIFX_FLOATING_T* p_params,  int out_buf_size,MULTIFX_INT32_T (*pfproc) (MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*));


#endif
