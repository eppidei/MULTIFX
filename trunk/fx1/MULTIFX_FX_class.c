#include <MULTIFX_FX_class.h>
#include <stdlib.h>
#include <stdio.h>
#include <MULTIFX_defines.h>
#include <string.h>
#include <MULTIFX_errors.h>

/**TODO integruty checks, naming standard, indexes***/

struct OSCILLATOR_S
{
	MULTIFX_UINT16_T ena;
	MULTIFX_FLOATING_T* osc_params;
	MULTIFX_FLOATING_T* new_osc_params;
	MULTIFX_UINT32_T n_osc_params;
	MULTIFX_FLOATING_T *tv_parameter;//puntatore al buffer di fx dove scriverà i valori oscillanti
	MULTIFX_FLOATING_T *oscillator_state;
	MULTIFX_P_OSC_FUNC_T  var_param_func;
};

struct FX_S
{
	MULTIFX_CHAR_T  	*fx_id;
	MULTIFX_UINT32_T 	id_len;
	MULTIFX_UINT32_T 	instance_numb;
	MULTIFX_UINT32_T 	ch_id;
	MULTIFX_FLOATING_T 	*fixed_parameters;
	MULTIFX_UINT32_T 	n_fixed_parameters;
	MULTIFX_FLOATING_T 	*static_params;
	MULTIFX_FLOATING_T 	*new_static_params;
	MULTIFX_UINT32_T 	n_static_params;
	MULTIFX_FLOATING_T 	*time_varied_params_buff;
	MULTIFX_UINT32_T 	n_time_varying_params;
	MULTIFX_FLOATING_T 	*fx_in_buf;
	MULTIFX_FLOATING_T 	*fx_out_buf;
	MULTIFX_UINT32_T 	len_float_buff;
	MULTIFX_FLOATING_T 	*fx_state;
	MULTIFX_UINT32_T    state_len;
	MULTIFX_P_PROC_FUNC_T processing_func;//
	OSCILLATOR_T 		**array_osc;
	MULTIFX_MENU_HEAD_T *menu_head;
	MULTIFX_MENU_NODE_T	*menu_root;
};

FX_T* FX_init( MULTIFX_UINT16_T n_bit,MULTIFX_UINT16_T stereo_mode, MULTIFX_UINT32_T fragment_size, MULTIFX_UINT32_T ch_id,
			MULTIFX_UINT32_T ist_num,MULTIFX_UINT32_T n_fixed_parameters,MULTIFX_UINT32_T n_static_params,
              MULTIFX_UINT32_T  n_time_varying_params,MULTIFX_UINT32_T state_length,MULTIFX_FLOATING_T* in_buff, MULTIFX_CHAR_T* id,
              MULTIFX_UINT32_T num_osc_prms)
{
    FX_T* pthis = calloc(1,sizeof(FX_T));
    MULTIFX_UINT32_T  len_buff_nbit=0,len_buff=0,i=0;
    MULTIFX_API_RET ret = 0;


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

        pthis -> ch_id=ch_id;
        pthis -> instance_numb = ist_num;

        /*********************Memory allocation and initialization ****************************/
        pthis ->id_len = strlen((char *)id);//id_len;
        pthis -> fx_id = calloc(pthis ->id_len+1,sizeof(MULTIFX_CHAR_T));
        strncpy((char*)pthis -> fx_id,(char*)id,pthis ->id_len*sizeof(MULTIFX_CHAR_T));

        pthis -> n_fixed_parameters      = n_fixed_parameters;
        pthis -> fixed_parameters     =  calloc(n_fixed_parameters,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> fixed_parameters == NULL)
                {
                    fprintf(stderr,"\nfixed_params allocation failure\n");
                    return NULL;
                }

        pthis -> n_static_params      = n_static_params;
        pthis -> static_params        = calloc(n_static_params*DOUBLE_BUFF,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> static_params == NULL)
        {
            fprintf(stderr,"\nstatic_params allocation failure\n");
            return NULL;
        }

        pthis -> new_static_params  = &(pthis -> static_params[n_static_params]);

        pthis -> n_time_varying_params      = n_time_varying_params;
        pthis -> time_varied_params_buff  = calloc(n_time_varying_params*len_buff,sizeof(MULTIFX_FLOATING_T));
        if (pthis -> time_varied_params_buff == NULL)
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

            (pthis -> array_osc[i])->n_osc_params = num_osc_prms;

            (pthis -> array_osc[i])->osc_params = calloc(num_osc_prms*DOUBLE_BUFF,sizeof(MULTIFX_FLOATING_T));

            if ( (pthis -> array_osc[i])->osc_params == NULL)
			{
				fprintf(stderr,"\n oscillator params allocation failure\n");
				return NULL;
			}

            (pthis -> array_osc[i])->new_osc_params = &((pthis -> array_osc[i])->osc_params[num_osc_prms]);

             (pthis -> array_osc[i])->oscillator_state = calloc(1,sizeof(MULTIFX_FLOATING_T));

             if ( (pthis -> array_osc[i])->oscillator_state == NULL)
            {
                fprintf(stderr,"\n oscillator state allocation failure\n");
                return NULL;
            }
        }

//        ret = MENU_init(&(pthis->menu_root),&(pthis->menu_head),MENU_MAX_CHILDREN,pthis->fx_id);
//
//        if (ret<0)
//        {
//        	fprintf(stderr,"\nFX MENU INIT ERROR\n");
//        	return NULL;
//        }

        /********** TO DO Inizializzare funzioni FeedThrough *********************/


    }

    return pthis;

}


MULTIFX_API_RET FX_release (FX_T* p_FX)
{

    MULTIFX_UINT32_T i= 0;

    CHECKNFREE(p_FX -> fx_id);
    CHECKNFREE(p_FX -> fixed_parameters);
    CHECKNFREE(p_FX -> static_params);
    CHECKNFREE(p_FX -> time_varied_params_buff);
    CHECKNFREE(p_FX -> fx_out_buf);
    CHECKNFREE(p_FX -> fx_state);
    for (i=0;i<p_FX->n_time_varying_params;i++)
    {
        CHECKNFREE(p_FX -> array_osc[i]->osc_params);
        CHECKNFREE(p_FX -> array_osc[i]->oscillator_state);
        CHECKNFREE(p_FX -> array_osc[i]);
    }
    CHECKNFREE(p_FX -> array_osc);
    //MENU_delete_branch(p_FX->menu_root,p_FX->menu_head);
    CHECKNFREE(p_FX);

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_osc_config(FX_T* p_FX,MULTIFX_UINT16_T *enabler,MULTIFX_FLOATING_T *low_limit, MULTIFX_FLOATING_T *high_limit,MULTIFX_FLOATING_T sample_rate,MULTIFX_FLOATING_T *freq ,MULTIFX_FLOATING_T *phase_off )
{
     MULTIFX_UINT32_T i= 0;


     for (i=0;i<p_FX->n_time_varying_params;i++)
     {
		(p_FX->array_osc[i])->ena  			  = enabler[i];
		(p_FX->array_osc[i])->osc_params[osc_sample_rate_idx]   =sample_rate;
		(p_FX->array_osc[i])->osc_params[osc_freq_idx]   = freq[i];
		(p_FX->array_osc[i])->osc_params[osc_offset_idx]   =phase_off[i];
		(p_FX->array_osc[i])->new_osc_params[osc_sample_rate_idx]   =sample_rate;
		(p_FX->array_osc[i])->new_osc_params[osc_freq_idx]   = freq[i];
		(p_FX->array_osc[i])->new_osc_params[osc_offset_idx]   =phase_off[i];
        if (low_limit[i]>high_limit[i])
        {
            return MULTIFX_INPUT_ARGS_ERROR;
        }
        (p_FX->array_osc[i])->osc_params[osc_bias_idx]         = (low_limit[i]+high_limit[i])/2;//bias
        (p_FX->array_osc[i])->osc_params[osc_amp_idx]          =  (high_limit[i]-low_limit[i])/2;//amp
        (p_FX->array_osc[i])->new_osc_params[osc_bias_idx]         = (low_limit[i]+high_limit[i])/2;//bias
		(p_FX->array_osc[i])->new_osc_params[osc_amp_idx]          =  (high_limit[i]-low_limit[i])/2;//amp

     }


    return MULTIFX_DEFAULT_RET;
}

//MULTIFX_API_RET FX_get_osc_params(FX_T* p_FX, MULTIFX_FLOATING_T** p_osc_params,MULTIFX_UINT32_T* len_osc_param)
//{
//
//    *len_osc_param = p_FX->n_fixed_parameters;
//    *p_fixed_params = p_FX->fixed_parameters;
//
//    return MULTIFX_DEFAULT_RET;
//
//}

MULTIFX_API_RET FX_osc_implementation (FX_T* p_FX, MULTIFX_P_OSC_FUNC_T *pfunc)
{

    MULTIFX_UINT32_T i = 0;

    for (i=0;i<p_FX->n_time_varying_params;i++)
    {
        (p_FX->array_osc[i])->var_param_func = pfunc[i];
    }

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_set_fixed_params (FX_T* p_FX, MULTIFX_FLOATING_T* p_fixed_params,MULTIFX_UINT32_T len_fixed_param)
{
	if (len_fixed_param!=p_FX->n_fixed_parameters)
	    {
	        return MULTIFX_PARAMSIZE_ERROR;
	    }
	    else
	    {
	        memcpy(p_FX->fixed_parameters,p_fixed_params,p_FX->n_fixed_parameters*sizeof(MULTIFX_FLOATING_T));
	    }

	    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_get_fixed_params(FX_T* p_FX, MULTIFX_FLOATING_T** p_fixed_params,MULTIFX_UINT32_T* len_fixed_param)
{

    *len_fixed_param = p_FX->n_fixed_parameters;
    *p_fixed_params = p_FX->fixed_parameters;

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
        //memcpy(p_FX->new_static_params,p_static_params,p_FX->n_static_params*sizeof(MULTIFX_FLOATING_T)); inserito più correttamente nel update
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

MULTIFX_API_RET FX_init_timevarying_params_buffer (FX_T* p_FX, MULTIFX_FLOATING_T* p_tv_params,MULTIFX_UINT32_T n_tv_param)
{
    MULTIFX_UINT32_T i = 0,j=0;
    MULTIFX_FLOATING_T* p_fx_tv = p_FX->time_varied_params_buff;
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

MULTIFX_API_RET FX_get_timevarying_params (FX_T* p_FX, MULTIFX_FLOATING_T** p_tv_params,MULTIFX_UINT32_T idx)
{
	if (p_FX!=NULL)
	{
    //*n_tv_param = p_FX->n_time_varying_params;
    //*len_frame =  p_FX->len_float_buff;
		*p_tv_params = (p_FX->array_osc[idx])->osc_params;
	}
	else
	{
		return MULTIFX_FX_NOT_ALLOC_POINTER;
	}

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
             p_tmp->tv_parameter = &(p_FX->time_varied_params_buff[i*p_FX->len_float_buff]);//indirizza il parametro all'interno buffer parametri tempo varianti

             ret = (p_tmp->var_param_func)(p_tmp->osc_params[0],p_tmp->osc_params[1],p_FX->len_float_buff,p_tmp->tv_parameter,p_tmp->osc_params[2], p_tmp->osc_params[4],p_tmp->osc_params[3],p_tmp->oscillator_state);
        }

    }



    ret = (p_FX->processing_func)(p_FX->fixed_parameters,p_FX->static_params,p_FX->time_varied_params_buff,p_FX->fx_in_buf,p_FX->fx_out_buf,p_FX->fx_state,(p_FX->len_float_buff));

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
    MULTIFX_FLOATING_T *p_tmp, *p_tmp2;
    MULTIFX_UINT32_T i = 0;

    p_tmp = p_FX -> new_static_params;

     p_FX -> new_static_params =  p_FX -> static_params;

      p_FX -> static_params = p_tmp;

      /*******OSCILLATOR***********/

      for (i=0;i<p_FX -> n_time_varying_params;i++)
      {
    	  p_tmp2 = (p_FX->array_osc[i])-> new_osc_params;
    	  (p_FX->array_osc[i])-> new_osc_params = (p_FX->array_osc[i])-> osc_params;
    	  (p_FX->array_osc[i])-> osc_params = p_tmp2;
      }


     return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_set_new_param(FX_T* p_FX,MULTIFX_FLOATING_T new_prm,MULTIFX_UINT32_T fx_idx)
{

	memcpy(p_FX -> new_static_params,p_FX -> static_params,p_FX ->n_static_params*sizeof(MULTIFX_FLOATING_T));

    p_FX -> new_static_params[fx_idx] = new_prm;

     return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_printf(FX_T* p_FX, FILE** pid)
{
	MULTIFX_UINT32_T i = 0;

	fprintf(*pid,"stat param address %x\n",p_FX->static_params);


	for (i=0;i<p_FX->n_static_params;i++)
	{
		fprintf(*pid,"param %d %f\n",i,p_FX->static_params[i]);
	}

	return MULTIFX_DEFAULT_RET;
}
MULTIFX_API_RET FX_attach_menu(FX_T* p_FX,MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_HEAD_T *head)
{
	p_FX->menu_head=head;
	p_FX->menu_root=node;

	return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET FX_get_menu(FX_T* p_FX,MULTIFX_MENU_NODE_T **node)
{
	if (p_FX->menu_root!=NULL)
	{

	*node =p_FX->menu_root;
	return MULTIFX_DEFAULT_RET;
	}
	else
	{
		fprintf(stderr,"\menu not attached to fx\n");
		return MULTIFX_FX_MENU_NOT_ATTACHED_ERROR;
	}


}
