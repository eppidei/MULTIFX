#ifndef _MULTIFX_OSS_STD_CFG_H
#define _MULTIFX_OSS_STD_CFG_H

#include <MULTIFX_typedef.h>

#include <soundcard.h>


MULTIFX_API_RET open_stereo_full_duplex_device (MULTIFX_CHAR_T *devname , MULTIFX_INT32_T *fd,MULTIFX_INT32_T buff_size,
                                         MULTIFX_INT32_T *frag_dim_request, MULTIFX_INT32_T *real_frag_size,
                                         MULTIFX_INT32_T rate,oss_audioinfo *infos, MULTIFX_INT32_T *latency_effort);


MULTIFX_API_RET open_mono_full_duplex_device (MULTIFX_CHAR_T *devname , MULTIFX_INT32_T *fd,MULTIFX_INT32_T buff_size,
                                         MULTIFX_INT32_T *frag_dim_request, MULTIFX_INT32_T *real_frag_size,
                                         MULTIFX_INT32_T rate,oss_audioinfo *infos, MULTIFX_INT32_T *latency_effort);



#endif
