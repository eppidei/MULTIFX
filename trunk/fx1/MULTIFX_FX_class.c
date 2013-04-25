#include <MULTIFX_FX_class.h>
#include <stdlib.h>
#include <stdio.h>
#include <MULTIFX_defines.h>
#include <string.h>
#include <MULTIFX_errors.h>


struct FX_S {
MULTIFX_FLOATING_T *static_params;
MULTIFX_UINT32_T n_static_params;
MULTIFX_FLOATING_T *time_varying_params;
MULTIFX_UINT32_T n_time_varying_params;
MULTIFX_FLOATING_T *fx_in_buf;
MULTIFX_FLOATING_T *fx_out_buf;
MULTIFX_UINT32_T len_float_buff;
MULTIFX_FLOATING_T *fx_state;
MULTIFX_UINT32_T    state_len;
MULTIFX_API_RET (*processing_func)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_UINT32_T);//param,timevarparam,inframe,outframe,state,len_frame
//MULTIFX_API_RET (*var_param_func)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_UINT32_T);//in_frame,timevarparam,len_frame
};
//pfroc *static_params,*buffin,*buffout,bufflen
FX_T* FX_init( MULTIFX_UINT16_T n_bit,MULTIFX_UINT16_T stereo_mode, MULTIFX_UINT32_T fragment_size, MULTIFX_UINT32_T n_static_params,MULTIFX_UINT32_T  n_time_varying_params,MULTIFX_UINT32_T state_length,MULTIFX_FLOATING_T* in_buff)
{
    FX_T* pthis = calloc(1,sizeof(FX_T));
    MULTIFX_UINT32_T  len_buff_nbit=0,len_buff=0;


    if (pthis!=NULL)
    {
        /************ buf dimension calc ***********/
        if (n_bit==16)
        {
            len_buff_nbit = fragment_size/2;
        }
        else
        {
            fprintf(stderr,"\nNumber of bits not handled\n");
            return NULL;
        }
        if (stereo_mode==1)
        {
            len_buff= len_buff_nbit/2;
        }
        else if (stereo_mode==0)
        {
            len_buff = len_buff_nbit;
        }
        else
        {
            fprintf(stderr,"\nStereo mode not defined\n");
            return NULL;
        }

        /*********************Memory allocation and initialization ****************************/
        pthis -> n_static_params      = n_static_params;
        pthis -> static_params        = calloc(n_static_params,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> static_params == NULL)
        {
            fprintf(stderr,"\nstatic_params allocation failure\n");
            return NULL;
        }

        pthis -> n_time_varying_params      = n_time_varying_params;
        pthis -> time_varying_params  = calloc(n_time_varying_params*len_buff,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> time_varying_params == NULL)
        {
            fprintf(stderr,"\time_varying_params allocation failure\n");
            return NULL;
        }

        pthis -> fx_in_buf            = in_buff;

        pthis -> len_float_buff = len_buff;
        pthis -> fx_out_buf           = calloc(len_buff,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> fx_out_buf == NULL)
        {
            fprintf(stderr,"\fx_out_buf allocation failure\n");
            return NULL;
        }

        pthis -> state_len  = state_length;
        pthis -> fx_state             = calloc(state_length,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> fx_state == NULL)
        {
            fprintf(stderr,"\fx_state allocation failure\n");
            return NULL;
        }

        /********** TO DO Inizializzare funzioni FeedThrough *********************/


    }

    return pthis;

}

FX_T* FX_clone(FX_T* p_FX,MULTIFX_FLOATING_T* in_buff)
{

    FX_T* pthis = calloc(1,sizeof(FX_T));

    if (pthis!=NULL)
    {

         pthis -> len_float_buff        = p_FX -> len_float_buff;

         pthis -> n_static_params       = p_FX -> n_static_params;
         pthis -> static_params         = calloc( pthis -> n_static_params,sizeof(MULTIFX_FLOATING_T));
         if (pthis -> static_params == NULL)
        {
            fprintf(stderr,"\nstatic_params allocation failure\n");
            return NULL;
        }
         memcpy(pthis -> static_params,p_FX -> static_params,pthis -> n_static_params*sizeof(MULTIFX_FLOATING_T) );

         pthis -> n_time_varying_params = p_FX -> n_time_varying_params;
         pthis -> time_varying_params   = calloc(pthis -> n_time_varying_params* pthis -> len_float_buff ,sizeof(MULTIFX_FLOATING_T));
         if (pthis -> time_varying_params == NULL)
        {
            fprintf(stderr,"\time_varying_params allocation failure\n");
            return NULL;
        }
         memcpy(pthis -> time_varying_params,p_FX -> time_varying_params,pthis -> n_time_varying_params*pthis -> len_float_buff*sizeof(MULTIFX_FLOATING_T) );

         pthis -> fx_in_buf            = in_buff;

         pthis -> fx_out_buf           = calloc(pthis -> len_float_buff,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> fx_out_buf == NULL)
        {
            fprintf(stderr,"\fx_out_buf allocation failure\n");
            return NULL;
        }

        pthis -> state_len  = p_FX->state_len;
        pthis -> fx_state             = calloc(pthis -> state_len,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> fx_state == NULL)
        {
            fprintf(stderr,"\fx_state allocation failure\n");
            return NULL;
        }
        memcpy(pthis -> fx_state,p_FX->fx_state,pthis -> state_len*sizeof(MULTIFX_FLOATING_T));
    }


    return pthis;
}

MULTIFX_API_RET FX_release (FX_T* p_FX)
{

    free(p_FX -> static_params);
    free(p_FX -> time_varying_params);
    free(p_FX -> fx_out_buf);
    free(p_FX -> fx_state);
    free(p_FX);

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_set_static_params (FX_T* p_FX, MULTIFX_FLOATING_T* p_static_params,MULTIFX_UINT32_T len_static_param)
{


    if (len_static_param!=p_FX->n_static_params)
    {
        return MULTIFX_PARAMSIZE_ERROR;
    }
    else
    {
        memcpy(p_FX->static_params,p_static_params,p_FX->n_static_params*sizeof(MULTIFX_FLOATING_T));
    }

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_get_static_params(FX_T* p_FX, MULTIFX_FLOATING_T** p_static_params,MULTIFX_UINT32_T* len_static_param)
{

    *len_static_param = p_FX->n_static_params;
    *p_static_params = p_FX->static_params;

    return MULTIFX_DEFAULT_RET;

}

MULTIFX_API_RET FX_set_state (FX_T* p_FX, MULTIFX_FLOATING_T* p_state,MULTIFX_UINT32_T len_state)
{

     if (len_state!=p_FX->state_len)
    {
        return MULTIFX_STATESIZE_ERROR;
    }
    else
    {
        memcpy(p_FX->fx_state,p_state,p_FX->state_len*sizeof(MULTIFX_FLOATING_T));
    }


     return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_get_state (FX_T* p_FX, MULTIFX_FLOATING_T** p_state,MULTIFX_UINT32_T *len_state)
{
    *len_state=p_FX->state_len;
    *p_state = p_FX->fx_state;
    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_init_timevarying_params (FX_T* p_FX, MULTIFX_FLOATING_T* p_tv_params,MULTIFX_UINT32_T n_tv_param)
{
    MULTIFX_UINT32_T i = 0,j=0;
    MULTIFX_FLOATING_T* p_fx_tv = p_FX->time_varying_params;
    MULTIFX_UINT32_T   len_fx_buff = p_FX->len_float_buff;

    if (n_tv_param!=p_FX->n_time_varying_params)
    {
        return MULTIFX_TVPARAMSIZE_ERROR;
    }
    else
    {
        for (i=0;i<p_FX->n_time_varying_params;i++)
        {
            for (j=0;j<len_fx_buff;j++)
            /*setta un valore costante per tutto il frame*/
            {
                p_fx_tv[j+i*len_fx_buff]=p_tv_params[i];
            }

        }

    }

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_get_timevarying_params (FX_T* p_FX, MULTIFX_FLOATING_T** p_tv_params,MULTIFX_UINT32_T *n_tv_param, MULTIFX_UINT32_T* len_frame)
{
    *n_tv_param = p_FX->n_time_varying_params;
    *len_frame =  p_FX->len_float_buff;
    *p_tv_params = p_FX->time_varying_params;

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_set_implementation (FX_T* p_FX, MULTIFX_INT32_T (*pfunc)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_UINT32_T))
{

    p_FX -> processing_func=pfunc;

    return MULTIFX_DEFAULT_RET;
}


MULTIFX_API_RET FX_process(FX_T* p_FX)
{
    MULTIFX_API_RET ret = 0;

    ret = (p_FX->processing_func)(p_FX->static_params,p_FX->time_varying_params,p_FX->fx_in_buf,p_FX->fx_out_buf,p_FX->fx_state,(p_FX->len_float_buff));

    return ret;
}

MULTIFX_API_RET FX_bufcpy(FX_T* p_FX,MULTIFX_FLOATING_T* buff)
{
    memcpy(buff,p_FX->fx_out_buf,(p_FX->len_float_buff)*sizeof(MULTIFX_FLOATING_T));

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_get_out_buff(FX_T* p_FX,MULTIFX_FLOATING_T** buff)
{
    *buff = p_FX->fx_out_buf;

    return MULTIFX_DEFAULT_RET;

}

