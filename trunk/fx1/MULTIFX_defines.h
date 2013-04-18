#ifndef _MULTIFX_DEFINES_H
#define _MULTIFX_DEFINES_H


#define USE_DOUBLE

#define MULTIFX_DEFAULT_RET (0)
#define STRAIGHT_RETURN(x) if (x < 0) return x;


#define VERBOSE (1)
#define MAX_N_FRAG (1024)
#define MAX_N_FRAG_MSB (MAX_N_FRAG << 16)
#define MAX_FRAG_SIZE (2048)
#define MAX_BUFF_DIM (32*1024)
#define SAMPLING_RATE (48000)

#endif

