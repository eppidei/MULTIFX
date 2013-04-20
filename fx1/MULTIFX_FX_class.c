#include <MULTIFX_FX_class.h>
#include <stdlib.h>


struct FX_S {
MULTIFX_FLOATING_T params[2];
MULTIFX_FLOATING_T *fx_out_buf;
MULTIFX_INT32_T (*processing_func)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*);
};

FX_T* FX_init(MULTIFX_FLOATING_T* p_params,  int out_buf_size,MULTIFX_INT32_T (*pfproc) (MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*))
{
    FX_T* pthis = calloc(1,sizeof(FX_T));
    if (pthis!=NULL)
    {
        pthis->params[0]=p_params[0];
        pthis->params[1]=p_params[1];
        pthis->fx_out_buf = calloc(out_buf_size,sizeof(MULTIFX_FLOATING_T));
        pthis->processing_func=pfproc;
    }

    return pthis;
}
