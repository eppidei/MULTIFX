#ifndef _MULTIFX_DEFINES_H
#define _MULTIFX_DEFINES_H


#define USE_SINGLE

#define MULTIFX_DEFAULT_RET (0)
#define STRAIGHT_RETURN(x) if (x < 0) {return x;}
#define STRAIGHT_tRETURN(x) if (x < 0) {pthread_exit((void*)&x);}
#define ALLOCATION_CHECK(x) if (x==NULL) {return MULTIFX_ALLOCATION_ERROR;}
#define ALLOCATION_tCHECK(x) if (x==NULL) {MULTIFX_INT32_T my_ret=MULTIFX_ALLOCATION_ERROR; pthread_exit((void*)&my_ret);}
#define PI_G (3.141592653589793115997963468544185161590576171875L)
#define CHECKNFREE(x)  if(x!=NULL) { free(x); }


#define VERBOSE (1)
#define MAX_N_FRAG (1024)
#define MAX_N_FRAG_MSB (MAX_N_FRAG << 16)
#define MAX_FRAG_SIZE (2048)
#define MAX_BUFF_DIM (8*1024)
#define SAMPLING_RATE (48000)

#undef DEBUG

#define MAX_STATIC_PARAMS (10)
#define MAX_STATE_LEN (20)
#define MOOG_ORDER (5)
#define MAX_FX_IDLEN (10+1)
#define ENABLE (1)
#define DISABLE (0)


#define STD_FX_LIB_CALL MULTIFX_FLOATING_T *params,MULTIFX_FLOATING_T *time_var_params,MULTIFX_FLOATING_T *in_frame, MULTIFX_FLOATING_T *out_frame,MULTIFX_FLOATING_T *state, MULTIFX_UINT32_T frame_len


#define MAX_CHAR_LEN (20+1)
#define MAX_FX_OPTIONS   (3)
#define MAX_FX_MENU   (4)
#define N_EFFECTS  (2)


/*******************MOOG*****************************/
#define MOOG_N_TV_PARAMS (2)
#define MOOG_FILTER_ORDER (5)

#endif


