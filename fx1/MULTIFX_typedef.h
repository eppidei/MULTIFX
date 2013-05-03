#ifndef _MULTIFX_TYPEDEFS_H
#define _MULTIFX_TYPEDEFS_H
//#include <MULTIFX_FX_class.h>
#include <stdint.h>
#include <MULTIFX_defines.h>



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


/**************FX CLASS*******************/
typedef struct FX_S FX_T;

typedef MULTIFX_API_RET (* MULTIFX_P_OSC_FUNC_T)(MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_UINT32_T,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T,MULTIFX_FLOATING_T*);

typedef MULTIFX_API_RET (*MULTIFX_P_PROC_FUNC_T)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_UINT32_T);

typedef struct OSCILLATOR_S OSCILLATOR_T ;

/**************UI CLASS*******************/
enum SM_UI_states { INIT, ATTACH_LEFT, ATTACH_RIGHT,INIZ_SEQUENCE,PROCESS,CHOOSE_CHANNEL,CHANGE_PARAMS,LEV1,LEV2};

typedef  struct ncurses_UI_S ncurses_UI_T;

/**************FX Manager ********************/
//typedef struct FX_Manager_S FX_Manager_T;
//enum commands {NOTHING2DO,FX_INIT,FX_RELEASE,FX_CONFIGURE};
/*****************************************/



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
    FX_T              *params_exchange;
    MULTIFX_UINT16_T enable_mainloop;
    FX_T *p_left;
    FX_T *p_right;
//    OSCILLATOR_T *p_oscL;
//    MULTIFX_FLOATING_T* p_param2vary_L;
//    MULTIFX_UINT32_T param2vary_idxL;
//    OSCILLATOR_T *p_oscR;
//    MULTIFX_FLOATING_T* p_param2vary_R;
//    MULTIFX_UINT32_T param2vary_idxR;
   // MULTIFX_UINT32_T l_frame;
};

#endif
