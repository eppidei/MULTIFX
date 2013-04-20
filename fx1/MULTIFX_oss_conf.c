#include <MULTIFX_oss_conf.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <MULTIFX_errors.h>
#include <sys/ioctl.h>
#include <MULTIFX_defines.h>


MULTIFX_API_RET open_device_RW (MULTIFX_CHAR_T *devname)
{

    /*****Open device for reading********/

    MULTIFX_INT32_T fd = 0;

    fd = open ((char*)devname, O_RDWR, 0);

    if (fd==-1)
    {
        #if VERBOSE > 0
         perror ((char*)devname);
         #endif
         return MULTIFX_OPEN_DEV_FAILURE;
    }
    else
    {
        return fd;
    }
}

MULTIFX_API_RET get_dev_infos (oss_audioinfo *infos, MULTIFX_INT32_T *fd)
{
    /****Getting device infos*********/

    MULTIFX_INT32_T ret = 0;
    oss_audioinfo test;

    ret = ioctl (*fd, SNDCTL_ENGINEINFO,&test);

    if ( ret == -1)
    {
        #if VERBOSE > 0
      perror("SNDCTL_ENGINEINFO");
      #endif
      return MULTIFX_GET_INFO_FAILURE;
    }
    else
    {
        *infos = test;
        printf ("\nUsing audio engine %d=%s \n", infos->dev, infos->name);
        return MULTIFX_DEFAULT_RET;
    }
}

MULTIFX_API_RET get_raw_data (MULTIFX_INT32_T *fd)
{
    /*****Tell device to keep raw data(no data conversions)*****/

    MULTIFX_INT32_T tmp = 0, ret = 0;

    ret = ioctl (*fd, SNDCTL_DSP_COOKEDMODE, &tmp);

    if ( ret == -1)
    {
        #if VERBOSE > 0
        perror("SNDCTL_DSP_COOKEDMODE");
        #endif
        return MULTIFX_GET_RAWDATA_FAILURE;
    }
    else
    {
        return MULTIFX_DEFAULT_RET;
    }
}

MULTIFX_API_RET check_full_duplex(MULTIFX_CHAR_T * devname,MULTIFX_INT32_T *fd)
{
    MULTIFX_INT32_T devcaps = 0, ret = 0;

    ret = ioctl (*fd, SNDCTL_DSP_GETCAPS, &devcaps);

    /******Check full duplex capabilities*******/
    if ( ret == -1)
    {
        #if VERBOSE > 0
        perror ("SNDCTL_DSP_GETCAPS");
        #endif
      return MULTIFX_SNDCTL_DSP_GETCAPS_FAILURE;
    }

  if (!(devcaps & PCM_CAP_DUPLEX))
    {
    #if VERBOSE > 0
      fprintf (stderr,
	       "%s doesn't support one device based full duplex scheme\n",
	       devname);
      fprintf (stderr, "Try use using two device scheme.\n");
      #endif
      return MULTIFX_FULLDUPLEX_NOTSUPPORTED;
    }
    else
    {
        return MULTIFX_DEFAULT_RET;
    }
}
MULTIFX_API_RET suggest_buff_dim(MULTIFX_INT32_T *fd, MULTIFX_INT32_T *frag_dim)
{

    MULTIFX_INT32_T ret = 0;

    ret = ioctl (*fd, SNDCTL_DSP_SETFRAGMENT, frag_dim) ;

    /*********Suggest a buffer dimension *************/
    if (ret == -1)
    {
    #if VERBOSE > 0
      perror ("SNDCTL_DSP_SETFRAGMENT");
      #endif
       return MULTIFX_SUGG_BUFF_FAILURE;
    }
    else
    {
        return MULTIFX_DEFAULT_RET;
    }
}

MULTIFX_API_RET set_stere_mode (MULTIFX_INT32_T *fd, MULTIFX_CHAR_T *devname)

{
    MULTIFX_INT32_T tmp=2, tmp_chk=2,ret =0;
    /*********** Setting stereo mode ***********************/

    ret = ioctl (*fd, SNDCTL_DSP_CHANNELS, &tmp);

  if ( ret == -1)
    {
        #if VERBOSE > 0
      perror ("SNDCTL_DSP_CHANNELS");
      #endif
      return MULTIFX_SUGG_SNDCTL_DSP_CHANNELS_FAILURE;
    }

  if (tmp != tmp_chk)
    {
        #if VERBOSE > 0
      fprintf (stderr, "%s doesn't support n_channel requested (%d) actual channels\n", devname, tmp);
      #endif
      return MULTIFX_SET_STEREO_FAILURE;
    }
    else
    {
        return MULTIFX_DEFAULT_RET;
    }
}

MULTIFX_API_RET set_mono_mode (MULTIFX_INT32_T *fd, MULTIFX_CHAR_T *devname)
{
    MULTIFX_INT32_T tmp=1, tmp_chk=1,ret =0;
    /*********** Setting stereo mode ***********************/

    ret = ioctl (*fd, SNDCTL_DSP_CHANNELS, &tmp);

  if ( ret == -1)
    {
      #if VERBOSE > 0
      perror ("SNDCTL_DSP_CHANNELS");
      #endif
      return MULTIFX_SUGG_SNDCTL_DSP_CHANNELS_FAILURE;
    }

  if (tmp != tmp_chk)
    {
      #if VERBOSE > 0
      fprintf (stderr, "%s doesn't support n_channel requested (%d) actual channels\n", devname, tmp);
      #endif
      return MULTIFX_SET_MONO_FAILURE;
    }
    else
    {
        return MULTIFX_DEFAULT_RET;
    }
}


MULTIFX_API_RET set_16bit_native_endian(MULTIFX_INT32_T *fd)
{
    MULTIFX_INT32_T ret = 0;
    MULTIFX_INT32_T tmp = AFMT_S16_NE;
    MULTIFX_INT32_T format = 0;

    ret = ioctl (*fd, SNDCTL_DSP_SETFMT, &tmp);
     /****** Request 16 bit native ENDIAN **************/

  if (ret == -1)
    {
        #if VERBOSE > 0
      perror ("SNDCTL_DSP_SETFMT");
      #endif
      return MULTIFX_SET_BIT_FAILURE;
    }

    if (tmp != AFMT_S16_NE && tmp != AFMT_S16_OE)
    {
        #if VERBOSE > 0
      fprintf (stderr, "16 bit sample format not supported (%x)\n",tmp);
      #endif
      return MULTIFX_SET_16BIT_NOTSUPPORTED;
    }

  format = tmp;

  if (format == AFMT_S16_OE)
    {
        #if VERBOSE > 0
      fprintf (stderr,
	       "Warning: Using 16 bit sample format with wrong endianess.\n");
	       #endif
	       return MULTIFX_OPPOSITE_ENDIANESS;
    }

    return MULTIFX_DEFAULT_RET;

}

MULTIFX_API_RET set_sample_rate(MULTIFX_INT32_T *fd,MULTIFX_INT32_T rate)
{

    MULTIFX_INT32_T ret = 0, tmp=0;
    /********************Settign sample rate ******************/

    tmp = rate;

    ret = ioctl (*fd, SNDCTL_DSP_SPEED, &tmp);

  if ( ret == -1)
    {
        #if VERBOSE > 0
      perror ("SNDCTL_DSP_SPEED");
      #endif
      return MULTIFX_SNDCTL_DSP_SPEED_FAILURE;
    }

  if (tmp != rate)
    {
        #if VERBOSE > 0
      fprintf (stderr, "Requested rate requested %d actual (%d)\n",rate, tmp);
      #endif
      return MULTIFX_SET_SAMPLE_RATE_FAILURE;
    }

    return MULTIFX_DEFAULT_RET;
}
MULTIFX_API_RET get_actual_frag_size(MULTIFX_INT32_T *fd, MULTIFX_INT32_T *actual_frag_size)
{
     MULTIFX_INT32_T ret = 0;

     ret = ioctl (*fd, SNDCTL_DSP_GETBLKSIZE, actual_frag_size);

    /*************Checking the actual frag size adopted *************/

    if ( ret == -1)
    {
        #if VERBOSE > 0
      perror ("SNDCTL_DSP_GETBLKSIZE");
      #endif
      return MULTIFX_SNDCTL_DSP_GETBLKSIZE_FAILURE;
    }

    return MULTIFX_DEFAULT_RET;

}

MULTIFX_API_RET check_buff_size(MULTIFX_INT32_T actual_frag_size, MULTIFX_INT32_T allocated_buffer_size)
{
    if (actual_frag_size > allocated_buffer_size)
    {
        #if VERBOSE > 0
      fprintf (stderr, "Too large fragment size %d\n", actual_frag_size);
      #endif
      return MULTIFX_BUFF_DIM_ERROR;
    }

    return MULTIFX_DEFAULT_RET;

}
MULTIFX_API_RET set_latency_effort(MULTIFX_INT32_T *fd, MULTIFX_INT32_T *policy_val)
{
    MULTIFX_INT32_T ret = 0;

    ret = ioctl(*fd, SNDCTL_DSP_POLICY, policy_val);

  if (ret==EIO)
  {
      #if VERBOSE > 0
      perror ("SNDCTL_DSP_POLICY");
      #endif
      return MULTIFX_SETPOLICY_FAILURE;
  }
  else if (ret==EINVAL)
  {
       return MULTIFX_CALL_NOTSUPPORTED;
  }
  else
  {
       return MULTIFX_DEFAULT_RET;
  }
}









