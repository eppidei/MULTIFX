#include <MULTIFX_oss_std_cfg.h>

MULTIFX_API_RET open_stereo_full_duplex_device (MULTIFX_CHAR_T *devname , MULTIFX_INT32_T *fd,MULTIFX_INT32_T buff_size,
                                         MULTIFX_INT32_T *frag_dim_request, MULTIFX_INT32_T *real_frag_size,
                                         MULTIFX_INT32_T rate,oss_audioinfo *infos, MULTIFX_INT32_T *latency_effort)
{
    MULTIFX_API_RET ret = 0;

ret = open_device_RW (devname);
*fd = ret;
STRAIGHT_RETURN(ret);

ret =  get_dev_infos (infos, fd);
STRAIGHT_RETURN(ret);

ret =  get_raw_data (fd);
STRAIGHT_RETURN(ret);

ret =  check_full_duplex(devname,fd);
STRAIGHT_RETURN(ret);

ret =  suggest_buff_dim(fd, frag_dim_request);
STRAIGHT_RETURN(ret);

ret =  set_stere_mode (fd, devname);
STRAIGHT_RETURN(ret);

ret =  set_16bit_native_endian(fd);
STRAIGHT_RETURN(ret);

ret =  set_sample_rate(fd,rate);
STRAIGHT_RETURN(ret);

ret =  get_actual_frag_size(fd, real_frag_size);
STRAIGHT_RETURN(ret);

ret =  check_buff_size(*real_frag_size, buff_size);
STRAIGHT_RETURN(ret);

ret =  set_latency_effort(fd, latency_effort);
STRAIGHT_RETURN(ret);

return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET open_mono_full_duplex_device (MULTIFX_CHAR_T *devname , MULTIFX_INT32_T *fd,MULTIFX_INT32_T buff_size,
                                         MULTIFX_INT32_T *frag_dim_request, MULTIFX_INT32_T *real_frag_size,
                                         MULTIFX_INT32_T rate,oss_audioinfo *infos, MULTIFX_INT32_T *latency_effort)
{
    MULTIFX_API_RET ret = 0;

ret = open_device_RW (devname);
*fd = ret;
STRAIGHT_RETURN(ret);

ret =  get_dev_infos (infos, fd);
STRAIGHT_RETURN(ret);

ret =  get_raw_data (fd);
STRAIGHT_RETURN(ret);

ret =  check_full_duplex(devname,fd);
STRAIGHT_RETURN(ret);

ret =  suggest_buff_dim(fd, frag_dim_request);
STRAIGHT_RETURN(ret);

ret =  set_mono_mode (fd, devname);
STRAIGHT_RETURN(ret);

ret =  set_16bit_native_endian(fd);
STRAIGHT_RETURN(ret);

ret =  set_sample_rate(fd,rate);
STRAIGHT_RETURN(ret);

ret =  get_actual_frag_size(fd, real_frag_size);
STRAIGHT_RETURN(ret);

ret =  check_buff_size(*real_frag_size, buff_size);
STRAIGHT_RETURN(ret);

ret =  set_latency_effort(fd, latency_effort);
STRAIGHT_RETURN(ret);

return MULTIFX_DEFAULT_RET;
}
