#ifndef _MULTIFX_OSS_UTILS_H
#define _MULTIFX_OSS_UTILS_H

#include <MULTIFX_defines.h>
#include <MULTIFX_typedef.h>
#include <string.h>
#include <MULTIFX_errors.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

MULTIFX_API_RET char2short_mono (MULTIFX_CHAR_T *in_buf, MULTIFX_INT16_T *w_buf,MULTIFX_UINT32_T frag_size);
MULTIFX_API_RET concat_leo_stereo (MULTIFX_CHAR_T *in_buf, MULTIFX_INT16_T *w_bufL, MULTIFX_INT16_T *w_bufR,MULTIFX_UINT32_T frag_size);
MULTIFX_API_RET short2char_mono (MULTIFX_CHAR_T *out_buf, MULTIFX_INT16_T *w_buf,MULTIFX_UINT32_T frag_size);
MULTIFX_API_RET short2char_stereo (MULTIFX_CHAR_T *out_buf, MULTIFX_INT16_T *w_bufL, MULTIFX_INT16_T *w_bufR,MULTIFX_UINT32_T frag_size);
MULTIFX_API_RET read_device_data (MULTIFX_INT32_T fd,MULTIFX_CHAR_T *rbuffer,MULTIFX_INT32_T fragsize);
MULTIFX_API_RET write_device_data (MULTIFX_INT32_T fd,MULTIFX_CHAR_T *wbuffer,MULTIFX_INT32_T fragsize);

#endif
