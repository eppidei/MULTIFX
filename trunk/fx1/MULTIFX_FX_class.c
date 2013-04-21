#include <MULTIFX_FX_class.h>
#include <stdlib.h>
#include <MULTIFX_defines.h>
#include <string.h>


struct FX_S {
MULTIFX_FLOATING_T params[MAX_PARAMS];
MULTIFX_UINT32_T n_params;
MULTIFX_FLOATING_T *fx_in_buf;
MULTIFX_FLOATING_T *fx_out_buf;
MULTIFX_UINT32_T len_buff_char;
MULTIFX_UINT32_T len_buff_16;
MULTIFX_UINT32_T len_buff_16_stereo;
MULTIFX_FLOATING_T fx_state[MAX_STATE_LEN];
MULTIFX_UINT32_T    state_len;
MULTIFX_API_RET (*processing_func)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_UINT32_T);
};
//pfroc *params,*buffin,*buffout,bufflen
FX_T* FX_init( MULTIFX_UINT32_T out_buf_char_size, MULTIFX_UINT32_T n_params, MULTIFX_UINT32_T state_length,MULTIFX_FLOATING_T* in_buff)
{
    FX_T* pthis = calloc(1,sizeof(FX_T));
    MULTIFX_UINT32_T i = 0;
    if (pthis!=NULL)
    {
        for (i=0;i<MAX_PARAMS;i++)
        {
             pthis->params[i]=0;
        }
        pthis->n_params = n_params;
        for (i=0;i<MAX_STATE_LEN;i++)
        {
             pthis->fx_state[i]=0;
        }
        pthis->state_len        = state_length;
       pthis->len_buff_char     =out_buf_char_size;
       pthis->len_buff_16       =out_buf_char_size/2;
       pthis->len_buff_16_stereo=out_buf_char_size/4;
       pthis->fx_out_buf        = calloc(pthis->len_buff_16_stereo,sizeof(MULTIFX_FLOATING_T));
       pthis->fx_in_buf         =in_buff;
    }

    return pthis;
}

MULTIFX_API_RET FX_set_params (FX_T* p_FX, MULTIFX_FLOATING_T* p_params, MULTIFX_FLOATING_T*p_state)
{
    MULTIFX_UINT32_T i = 0;

    for (i=0;i<p_FX->n_params;i++)
    {
        p_FX->params[i]=p_params[i];
    }

    for (i=0;i<p_FX->state_len;i++)
    {
        p_FX->fx_state[i]=p_state[i];
    }

    return MULTIFX_DEFAULT_RET;
}


MULTIFX_API_RET FX_set_implementation (FX_T* p_FX, MULTIFX_INT32_T (*pfunc)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_UINT32_T))
{

    p_FX -> processing_func=pfunc;

    return MULTIFX_DEFAULT_RET;
}


MULTIFX_API_RET FX_process(FX_T* p_FX)
{
    MULTIFX_API_RET ret = 0;

    ret = (p_FX->processing_func)(p_FX->params,p_FX->fx_in_buf,p_FX->fx_out_buf,p_FX->fx_state,(p_FX->len_buff_16_stereo));

    return ret;
}

MULTIFX_API_RET FX_bufcpy(FX_T* p_FX,MULTIFX_FLOATING_T* buff)
{
    memcpy(buff,p_FX->fx_out_buf,(p_FX->len_buff_16_stereo)*sizeof(MULTIFX_FLOATING_T));

    return MULTIFX_DEFAULT_RET;
}

FX_T* FX_clone(FX_T* p_FX,MULTIFX_FLOATING_T* in_buff)
{
    MULTIFX_UINT32_T i = 0;

    FX_T* pthis = calloc(1,sizeof(FX_T));

    if (pthis!=NULL)
    {
        for (i=0;i<MAX_PARAMS;i++)
        {
             pthis->params[i]=p_FX->params[i];
        }
        pthis->n_params             =p_FX->n_params;
        for (i=0;i<MAX_STATE_LEN;i++)
        {
             pthis->fx_state[i]=p_FX->fx_state[i];
        }
        pthis->state_len = p_FX->state_len;
       pthis->len_buff_char         =p_FX->len_buff_char;
       pthis->len_buff_16           =p_FX->len_buff_16;
       pthis->len_buff_16_stereo    =p_FX->len_buff_16_stereo;
       pthis->fx_out_buf            = calloc(pthis->len_buff_16_stereo,sizeof(MULTIFX_FLOATING_T));
       pthis->fx_in_buf             = in_buff;
        pthis->processing_func= p_FX -> processing_func;
    }



    return pthis;
}
