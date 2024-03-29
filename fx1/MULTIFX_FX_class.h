#ifndef _FX_CLASS_H
#define _FX_CLASS_H

#include <MULTIFX_typedef.h>
#include <stdio.h>

FX_T* FX_init( MULTIFX_UINT16_T n_bit,MULTIFX_UINT16_T stereo_mode, MULTIFX_UINT32_T fragment_size, MULTIFX_UINT32_T ch_id,
			MULTIFX_UINT32_T ist_num,MULTIFX_UINT32_T n_fixed_parameters,MULTIFX_UINT32_T n_static_params,
              MULTIFX_UINT32_T  n_time_varying_params,MULTIFX_UINT32_T state_length,MULTIFX_FLOATING_T* in_buff, MULTIFX_CHAR_T* id,
              MULTIFX_UINT32_T num_osc_prms);

MULTIFX_API_RET FX_release (FX_T* p_FX);


MULTIFX_API_RET FX_osc_config(FX_T* p_FX,MULTIFX_UINT16_T *enabler,MULTIFX_FLOATING_T *low_limit, MULTIFX_FLOATING_T *high_limit,MULTIFX_FLOATING_T sample_rate,MULTIFX_FLOATING_T *freq ,MULTIFX_FLOATING_T *phase_off );

MULTIFX_API_RET FX_osc_implementation (FX_T* p_FX, MULTIFX_P_OSC_FUNC_T *pfunc);

MULTIFX_API_RET FX_set_fixed_params (FX_T* p_FX, MULTIFX_FLOATING_T* p_fixed_params,MULTIFX_UINT32_T len_fixed_param);

MULTIFX_API_RET FX_get_fixed_params(FX_T* p_FX, MULTIFX_FLOATING_T** p_fixed_params,MULTIFX_UINT32_T* len_fixed_param);

MULTIFX_API_RET FX_set_static_params (FX_T* p_FX, MULTIFX_FLOATING_T* p_static_params,MULTIFX_UINT32_T len_static_param);

MULTIFX_API_RET FX_get_static_params(FX_T* p_FX, MULTIFX_FLOATING_T** p_static_params,MULTIFX_UINT32_T* len_static_param);

MULTIFX_API_RET FX_set_state (FX_T* p_FX, MULTIFX_FLOATING_T* p_state,MULTIFX_UINT32_T len_state);

MULTIFX_API_RET FX_get_state (FX_T* p_FX, MULTIFX_FLOATING_T** p_state,MULTIFX_UINT32_T *len_state);

MULTIFX_API_RET FX_init_timevarying_params_buffer (FX_T* p_FX, MULTIFX_FLOATING_T* p_tv_params,MULTIFX_UINT32_T n_tv_param);

MULTIFX_API_RET FX_get_timevarying_params (FX_T* p_FX, MULTIFX_FLOATING_T** p_tv_params,MULTIFX_UINT32_T idx);

MULTIFX_API_RET FX_set_implementation (FX_T* p_FX, MULTIFX_P_PROC_FUNC_T pfunc);

MULTIFX_API_RET FX_process(FX_T* p_FX);

MULTIFX_API_RET FX_bufcpy(FX_T* p_FX,MULTIFX_FLOATING_T* buff);

MULTIFX_API_RET FX_get_out_buff(FX_T* p_FX,MULTIFX_FLOATING_T** buff);

MULTIFX_API_RET FX_static_param_update(FX_T* p_FX);

MULTIFX_API_RET FX_set_new_param(FX_T* p_FX,MULTIFX_FLOATING_T new_prm,MULTIFX_UINT32_T fx_idx);

MULTIFX_API_RET FX_OSC_set_new_param(FX_T* p_FX,MULTIFX_FLOATING_T new_prm,MULTIFX_UINT32_T osc_idx,MULTIFX_UINT32_T fx_idx);

MULTIFX_API_RET FX_printf(FX_T* p_FX, FILE** pid);

MULTIFX_API_RET FX_attach_menu(FX_T* p_FX,MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_HEAD_T *head);

MULTIFX_API_RET FX_get_menu(FX_T* p_FX,MULTIFX_MENU_NODE_T **node);

#endif
