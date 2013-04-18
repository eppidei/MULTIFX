#ifndef _MULTIFX_TYPEDEFS_H
#define _MULTIFX_TYPEDEFS_H

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

#endif
