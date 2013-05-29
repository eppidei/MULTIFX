#ifndef _MULTIFX_TYPEDEFS_H
#define _MULTIFX_TYPEDEFS_H
//#include <MULTIFX_FX_class.h>
#include <stdint.h>
#include <MULTIFX_defines.h>
#include <stdio.h>



typedef int64_t MULTIFX_INT64_T ;
typedef int32_t MULTIFX_INT32_T ;
typedef int16_t MULTIFX_INT16_T ;
typedef int8_t  MULTIFX_CHAR_T;
typedef uint64_t MULTIFX_UINT64_T ;
typedef uint32_t MULTIFX_UINT32_T ;
typedef uint16_t MULTIFX_UINT16_T ;
typedef uint8_t  MULTIFX_UCHAR_T ;
#ifdef USE_DOUBLE
typedef double MULTIFX_FLOATING_T;
#else
#ifdef USE_SINGLE
typedef float MULTIFX_FLOATING_T;
#endif
#endif
typedef MULTIFX_INT32_T MULTIFX_API_RET ;




typedef  struct MULTIFX_MENU_NODE_S MULTIFX_MENU_NODE_T;
typedef  struct MULTIFX_MENU_HEAD_S MULTIFX_MENU_HEAD_T ;
enum menu_item_t {root,sub_menu,static_parameter,oscillator_parameter};

/**************FX CLASS*******************/
typedef struct FX_S FX_T;



typedef MULTIFX_API_RET (* MULTIFX_P_OSC_FUNC_T)(MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_UINT32_T,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T*);

typedef MULTIFX_API_RET (*MULTIFX_P_PROC_FUNC_T)(MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_UINT32_T);

typedef struct OSCILLATOR_S OSCILLATOR_T ;

/*****OSCILLATORS******/

enum osc_param_order_t {osc_sample_rate_idx = 0, osc_freq_idx = 1,osc_offset_idx=2,osc_bias_idx=3,osc_amp_idx=4};

/**************UI CLASS*******************/
enum SM_UI_states { INIT, ATTACH_LEFT, ATTACH_RIGHT,INIZ_SEQUENCE,PROCESS,CHOOSE_CHANNEL,BROWSE_PARAMS,MODIFY_PARAM};

typedef  struct ncurses_UI_S ncurses_UI_T;

/****** TEST TONE*********/

enum test_tone_static_order_t {tt_freq_idx = 0, tt_offset_idx = 1, tt_amp_idx = 2};

/******* MOOG ***********/

enum moog_tv_order_t {moog_freq_tag_idx = 0, moog_k_idx = 1};

typedef struct MULTIFX_targs_S MULTIFX_targs_T;


struct MULTIFX_targs_S
{
    MULTIFX_INT32_T device_fd;
//    MULTIFX_INT32_T sample_rate;
//    MULTIFX_UINT16_T n_bit  ;
//     MULTIFX_UINT16_T stereo_mode  ;
    MULTIFX_UINT32_T buff_char_len;
    MULTIFX_CHAR_T *read_buffer;
    MULTIFX_CHAR_T *write_buffer;
    MULTIFX_FLOATING_T *read_buff_L;
    MULTIFX_FLOATING_T *read_buff_R;
    MULTIFX_FLOATING_T *write_buff_L;
    MULTIFX_FLOATING_T *write_buff_R;
   // FX_T              *params_exchange;
    MULTIFX_UINT16_T enable_mainloop;
    FX_T *p_left;
    MULTIFX_UINT16_T *update_flag_L;
    FX_T *p_right;
    MULTIFX_UINT16_T *update_flag_R;
    FILE **p_debug;
//    OSCILLATOR_T *p_oscL;
//    MULTIFX_FLOATING_T* p_param2vary_L;
//    MULTIFX_UINT32_T param2vary_idxL;
//    OSCILLATOR_T *p_oscR;
//    MULTIFX_FLOATING_T* p_param2vary_R;
//    MULTIFX_UINT32_T param2vary_idxR;
   // MULTIFX_UINT32_T l_frame;
};

#endif
