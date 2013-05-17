#include <MULTIFX_FX_class.h>
#include <stdlib.h>
#include <stdio.h>
#include <MULTIFX_defines.h>
#include <string.h>
#include <MULTIFX_errors.h>
struct OSCILLATOR_S {
MULTIFX_UINT16_T ena;
MULTIFX_FLOATING_T phase_offset;
MULTIFX_FLOATING_T bias;
MULTIFX_FLOATING_T amp;
MULTIFX_FLOATING_T sample_rate;
MULTIFX_FLOATING_T freq;
MULTIFX_FLOATING_T *tv_parameter;
MULTIFX_FLOATING_T *oscillator_state;
//MULTIFX_API_RET (*var_param_func)(MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_UINT32_T,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T*);//(MULTIFX_FLOATING_T f_samp, MULTIFX_FLOATING_T f_synth, MULTIFX_UINT32_T len_frame, MULTIFX_FLOATING_T *out_frame, MULTIFX_FLOATING_T phase_offset, MULTIFX_FLOATING_T amp,MULTIFX_FLOATING_T* old_phase)
MULTIFX_P_OSC_FUNC_T  var_param_func;
};

struct FX_S {
MULTIFX_CHAR_T  *fx_id;
MULTIFX_UINT32_T id_len;
MULTIFX_FLOATING_T *static_params;
MULTIFX_FLOATING_T *new_static_params;
MULTIFX_UINT32_T n_static_params;
MULTIFX_FLOATING_T *time_varying_params;
MULTIFX_UINT32_T n_time_varying_params;
MULTIFX_FLOATING_T *fx_in_buf;
MULTIFX_FLOATING_T *fx_out_buf;
MULTIFX_UINT32_T len_float_buff;
MULTIFX_FLOATING_T *fx_state;
MULTIFX_UINT32_T    state_len;
MULTIFX_P_PROC_FUNC_T processing_func;//
//MULTIFX_API_RET (*processing_func)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_UINT32_T);//param,timevarparam,inframe,outframe,state,len_frame
OSCILLATOR_T **array_osc;
};
//pfroc *static_params,*buffin,*buffout,bufflen
FX_T* FX_init( MULTIFX_UINT16_T n_bit,MULTIFX_UINT16_T stereo_mode, MULTIFX_UINT32_T fragment_size, MULTIFX_UINT32_T n_static_params,
              MULTIFX_UINT32_T  n_time_varying_params,MULTIFX_UINT32_T state_length,MULTIFX_FLOATING_T* in_buff, MULTIFX_CHAR_T* id)
{
    FX_T* pthis = calloc(1,sizeof(FX_T));
    MULTIFX_UINT32_T  len_buff_nbit=0,len_buff=0,i=0;


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
        pthis ->id_len = strlen((char *)id);//id_len;
        pthis -> fx_id = calloc(pthis ->id_len+1,sizeof(MULTIFX_CHAR_T));
        strncpy((char*)pthis -> fx_id,(char*)id,pthis ->id_len*sizeof(MULTIFX_CHAR_T));


        pthis -> n_static_params      = n_static_params;
        pthis -> static_params        = calloc(n_static_params*DOUBLE_BUFF,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> static_params == NULL)
        {
            fprintf(stderr,"\nstatic_params allocation failure\n");
            return NULL;
        }

        pthis -> new_static_params  = &(pthis -> static_params[n_static_params]);

        pthis -> n_time_varying_params      = n_time_varying_params;
        pthis -> time_varying_params  = calloc(n_time_varying_params*len_buff,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> time_varying_params == NULL)
        {
            fprintf(stderr,"\ntime_varying_params allocation failure\n");
            return NULL;
        }

        pthis -> fx_in_buf            = in_buff;

        pthis -> len_float_buff = len_buff;
        pthis -> fx_out_buf           = calloc(len_buff,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> fx_out_buf == NULL)
        {
            fprintf(stderr,"\nfx_out_buf allocation failure\n");
            return NULL;
        }

        pthis -> state_len  = state_length;
        pthis -> fx_state             = calloc(state_length,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> fx_state == NULL)
        {
            fprintf(stderr,"\nfx_state allocation failure\n");
            return NULL;
        }


        pthis -> array_osc = calloc(n_time_varying_params,sizeof(OSCILLATOR_T*));
        if (pthis -> array_osc == NULL)
        {
            fprintf(stderr,"\noscillator pointer array allocation failure\n");
            return NULL;
        }



        for(i=0;i<n_time_varying_params;i++)
        {
            pthis -> array_osc[i] = calloc(1,sizeof(OSCILLATOR_T));

            if (pthis -> array_osc[i] == NULL)
            {
                fprintf(stderr,"\noscillator pointer array allocation failure\n");
                return NULL;
            }

             (pthis -> array_osc[i])->oscillator_state = calloc(1,sizeof(MULTIFX_FLOATING_T));

             if ( (pthis -> array_osc[i])->oscillator_state == NULL)
            {
                fprintf(stderr,"\noscillator state allocation failure\n");
                return NULL;
            }
        }

        /********** TO DO Inizializzare funzioni FeedThrough *********************/


    }

    return pthis;

}



FX_T* FX_param_exchange_init (MULTIFX_UINT16_T n_bit,MULTIFX_UINT16_T stereo_mode)
{
    FX_T *pthis = NULL;
    MULTIFX_UINT32_T fragsize=0,n_time_varying_params=0,state_length=0;
    MULTIFX_FLOATING_T* in_buff=NULL;
    MULTIFX_CHAR_T* id="Parameter exchange structure";
    /** static params double buffered****/
    pthis= FX_init( n_bit,stereo_mode, fragsize, 2*MAX_STATIC_PARAMS,n_time_varying_params,state_length,in_buff, id);

    return pthis;
}

FX_T* FX_clone(FX_T* p_FX,MULTIFX_FLOATING_T* in_buff)
{

    FX_T* pthis = calloc(1,sizeof(FX_T));

    if (pthis!=NULL)
    {
            /*****TO DO Inserire sistema rapporti parentela cloni******************/
        pthis -> id_len = p_FX -> id_len ;
        pthis -> fx_id  = calloc(pthis -> id_len,sizeof(MULTIFX_CHAR_T));
        strcpy( (char*) pthis -> fx_id,(char*)p_FX -> fx_id);



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

    MULTIFX_UINT32_T i= 0;

    CHECKNFREE(p_FX -> fx_id);
    CHECKNFREE(p_FX -> static_params);
    CHECKNFREE(p_FX -> time_varying_params);
    CHECKNFREE(p_FX -> fx_out_buf);
    CHECKNFREE(p_FX -> fx_state);
    for (i=0;i<p_FX->n_time_varying_params;i++)
    {
        CHECKNFREE(p_FX -> array_osc[i]->oscillator_state);
        CHECKNFREE(p_FX -> array_osc[i]);
    }
    CHECKNFREE(p_FX -> array_osc);
    CHECKNFREE(p_FX);

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_osc_config(FX_T* p_FX,MULTIFX_UINT16_T *enabler,MULTIFX_FLOATING_T *low_limit, MULTIFX_FLOATING_T *high_limit,MULTIFX_FLOATING_T sample_rate,MULTIFX_FLOATING_T *freq ,MULTIFX_FLOATING_T *phase_off )
{
     MULTIFX_UINT32_T i= 0;


     for (i=0;i<p_FX->n_time_varying_params;i++)
     {
          (p_FX->array_osc[i])->ena  =enabler[i];
         (p_FX->array_osc[i])->sample_rate  =sample_rate;
        (p_FX->array_osc[i])->freq         = freq[i];
        (p_FX->array_osc[i])->phase_offset =phase_off[i];
        if (low_limit[i]>high_limit[i])
        {
            return MULTIFX_INPUT_ARGS_ERROR;
        }
        (p_FX->array_osc[i])->bias         = (low_limit[i]+high_limit[i])/2;
        (p_FX->array_osc[i])->amp          =  (high_limit[i]-low_limit[i])/2;

     }


    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_osc_implementation (FX_T* p_FX, MULTIFX_P_OSC_FUNC_T *pfunc)
{

    MULTIFX_UINT32_T i = 0;

    for (i=0;i<p_FX->n_time_varying_params;i++)
    {
        (p_FX->array_osc[i])->var_param_func = pfunc[i];
    }

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
        memcpy(p_FX->new_static_params,p_static_params,p_FX->n_static_params*sizeof(MULTIFX_FLOATING_T));
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

MULTIFX_API_RET FX_set_implementation (FX_T* p_FX, MULTIFX_P_PROC_FUNC_T pfunc)
{

    p_FX -> processing_func=pfunc;

    return MULTIFX_DEFAULT_RET;
}


MULTIFX_API_RET FX_process(FX_T* p_FX)
{
    MULTIFX_API_RET ret = 0,i=0;
    OSCILLATOR_T * p_tmp=NULL;
    /////

    for (i=0;i<p_FX->n_time_varying_params;i++)
    {
        p_tmp=(p_FX->array_osc[i]);
        if (p_tmp->ena == ENABLE)
        {
             p_tmp->tv_parameter = &(p_FX->time_varying_params[i*p_FX->len_float_buff]);//indirizza il parametro all'interno buffer parametri tempo varianti

             ret = (p_tmp->var_param_func)(p_tmp->sample_rate,p_tmp->freq,p_FX->len_float_buff,p_tmp->tv_parameter,p_tmp->phase_offset, p_tmp->amp,p_tmp->bias,p_tmp->oscillator_state);
        }

    }



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

MULTIFX_API_RET FX_static_param_update(FX_T* p_FX)
{
    MULTIFX_FLOATING_T *p_tmp;

    p_tmp = p_FX -> new_static_params;

     p_FX -> new_static_params =  p_FX -> static_params;

      p_FX -> static_params = p_tmp;

     return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_set_new_param(FX_T* p_FX,MULTIFX_FLOATING_T new_prm,MULTIFX_UINT32_T fx_idx)
{
    p_FX -> new_static_params[fx_idx] = new_prm;

     return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_printf(FX_T* p_FX, FILE** pid)
{
	fprintf(*pid,"%x    %f\n",p_FX->static_params,p_FX->static_params[3]);

	return MULTIFX_DEFAULT_RET;
}


