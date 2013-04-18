#include <MULTIFX_oss_utils.h>


MULTIFX_API_RET char2short_mono (MULTIFX_CHAR_T *in_buf, MULTIFX_INT16_T *w_buf,MULTIFX_UINT32_T frag_size)
{
    MULTIFX_UINT32_T i=0;
    MULTIFX_INT16_T tmp_data_LSBYTE=0,tmp_data_MSBYTE=0;

    for (i=0;i<frag_size;i+=2)
    {
        tmp_data_LSBYTE = 0;
        memcpy(&tmp_data_LSBYTE,&in_buf[i],1);
        tmp_data_MSBYTE=0;
        memcpy(&tmp_data_MSBYTE,&in_buf[i+1],1);
        tmp_data_MSBYTE = tmp_data_MSBYTE << 8;
        w_buf[i/2]      =  tmp_data_MSBYTE | tmp_data_LSBYTE;
    }

    return MULTIFX_DEFAULT_RET;

}

MULTIFX_API_RET concat_leo_stereo (MULTIFX_CHAR_T *in_buf, MULTIFX_INT16_T *w_bufL, MULTIFX_INT16_T *w_bufR,MULTIFX_UINT32_T frag_size)
{
    MULTIFX_UINT32_T i=0;
    MULTIFX_INT16_T tmp_data_LSBYTE_L=0,tmp_data_MSBYTE_L=0;
    MULTIFX_INT16_T tmp_data_LSBYTE_R=0,tmp_data_MSBYTE_R=0;

    for (i=0;i<frag_size;i+=4)
    {
        tmp_data_LSBYTE_L = 0;
        memcpy(&tmp_data_LSBYTE_L,&in_buf[i],1);
        tmp_data_LSBYTE_R = 0;
        memcpy(&tmp_data_LSBYTE_R,&in_buf[i+2],1);
        tmp_data_MSBYTE_L = 0;
        memcpy(&tmp_data_MSBYTE_L,&in_buf[i+1],1);
        tmp_data_MSBYTE_L = tmp_data_MSBYTE_L << 8;
        tmp_data_MSBYTE_R = 0;
        memcpy(&tmp_data_MSBYTE_R,&in_buf[i+3],1);
        tmp_data_MSBYTE_R = tmp_data_MSBYTE_R << 8;
        w_bufL[i/4]      =  tmp_data_MSBYTE_L | tmp_data_LSBYTE_L;
        w_bufR[i/4]      =  tmp_data_MSBYTE_R | tmp_data_LSBYTE_R;
    }

    return MULTIFX_DEFAULT_RET;

}

MULTIFX_API_RET short2char_mono (MULTIFX_CHAR_T *out_buf, MULTIFX_INT16_T *w_buf,MULTIFX_UINT32_T frag_size)
{
    MULTIFX_UINT32_T i = 0;
    MULTIFX_CHAR_T *p_temp;

    for (i=0;i<(frag_size);i+=2)
    {

        p_temp = (MULTIFX_CHAR_T*) &(w_buf[i/2]);
        out_buf[i] =  (p_temp[0]);
        out_buf[i+1] = (p_temp[1]);

    }

    return MULTIFX_DEFAULT_RET;
}

MULTIFX_API_RET short2char_stereo (MULTIFX_CHAR_T *out_buf, MULTIFX_INT16_T *w_bufL, MULTIFX_INT16_T *w_bufR,MULTIFX_UINT32_T frag_size)
{
    MULTIFX_UINT32_T i = 0;
    MULTIFX_CHAR_T *p_tempL,*p_tempR;

    for (i=0;i<(frag_size);i+=4)
    {
        p_tempL = (MULTIFX_CHAR_T*) &(w_bufL[i/4]);
        p_tempR = (MULTIFX_CHAR_T*) &(w_bufR[i/4]);
        out_buf[i] = (MULTIFX_CHAR_T)(p_tempL[0]);
         out_buf[i+1] = (MULTIFX_CHAR_T)(p_tempL[1]);
        out_buf[i+2] = (MULTIFX_CHAR_T)(p_tempR[0]);
        out_buf[i+3] = (MULTIFX_CHAR_T)(p_tempR[1]);
    }

    return MULTIFX_DEFAULT_RET;
}


MULTIFX_API_RET read_device_data (MULTIFX_INT32_T fd,MULTIFX_CHAR_T *rbuffer,MULTIFX_INT32_T fragsize)
{
    MULTIFX_INT32_T ret = 0;

     ret = read (fd, rbuffer, fragsize);

     if (ret==-1)
     {
         perror("read");
         return MULTIFX_RD_FAILURE;
     }
     else
     {
         return ret;
     }
}

MULTIFX_API_RET write_device_data (MULTIFX_INT32_T fd,MULTIFX_CHAR_T *wbuffer,MULTIFX_INT32_T fragsize)
{
    MULTIFX_INT32_T ret = 0;

     ret = write (fd, wbuffer, fragsize);

     if (ret==-1)
     {
         perror("read");
         return MULTIFX_WR_FAILURE;
     }
     else
     {
         return ret;
     }
}
