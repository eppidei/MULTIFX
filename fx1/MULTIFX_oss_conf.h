#ifndef _MULTIFX_OSS_CONF_H
#define _MULTIFX_OSS_CONF_H


#include <MULTIFX_typedef.h>


#include <soundcard.h>



MULTIFX_API_RET open_device_RW (MULTIFX_CHAR_T * devname);
MULTIFX_API_RET get_dev_infos (oss_audioinfo *infos, MULTIFX_INT32_T *fd);
MULTIFX_API_RET get_raw_data (MULTIFX_INT32_T *fd);
MULTIFX_API_RET check_full_duplex(MULTIFX_CHAR_T * devname,MULTIFX_INT32_T *fd);
MULTIFX_API_RET suggest_buff_dim(MULTIFX_INT32_T *fd, MULTIFX_INT32_T *frag_dim);
MULTIFX_API_RET set_stere_mode (MULTIFX_INT32_T *fd, MULTIFX_CHAR_T *devname);
MULTIFX_API_RET set_mono_mode (MULTIFX_INT32_T *fd, MULTIFX_CHAR_T *devname);
MULTIFX_API_RET set_16bit_native_endian(MULTIFX_INT32_T *fd);
MULTIFX_API_RET set_sample_rate(MULTIFX_INT32_T *fd,MULTIFX_INT32_T rate);
MULTIFX_API_RET get_actual_frag_size(MULTIFX_INT32_T *fd, MULTIFX_INT32_T *actual_frag_size);
MULTIFX_API_RET check_buff_size(MULTIFX_INT32_T actual_frag_size, MULTIFX_INT32_T allocated_buffer_size);
MULTIFX_API_RET set_latency_effort(MULTIFX_INT32_T *fd, MULTIFX_INT32_T *policy_val);


#endif
