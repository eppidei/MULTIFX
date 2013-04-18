#include <MULTIFX_defines.h>
#include <MULTIFX_typedef.h>
#include <soundcard.h>
#include <MULTIFX_oss_std_cfg.h>
#include <MULTIFX_oss_utils.h>
#include <unistd.h>


int main ()
{

    static MULTIFX_CHAR_T rbuffer[MAX_BUFF_DIM];
    static MULTIFX_CHAR_T wbuffer[MAX_BUFF_DIM];
    MULTIFX_CHAR_T *devname = "/dev/dsp";
    MULTIFX_API_RET ret = 0;
    MULTIFX_INT32_T fd_dev = 0;
    MULTIFX_INT32_T frag_dim_req = MAX_BUFF_DIM;
    MULTIFX_INT32_T frag_size = 0;
    MULTIFX_INT32_T rate = SAMPLING_RATE;
    oss_audioinfo audio_info ;
    MULTIFX_INT32_T latency_effort = 5;
    MULTIFX_INT32_T rd_len = 0, wr_len = 0;



    ret = open_stereo_full_duplex_device (devname , &fd_dev,MAX_BUFF_DIM,
                                         &frag_dim_req, &frag_size,
                                         rate,&audio_info, &latency_effort);
    STRAIGHT_RETURN(ret);
    rd_len = frag_size;

    while (rd_len==frag_size)
    {
        rd_len=read_device_data (fd_dev,rbuffer,frag_size);

        STRAIGHT_RETURN(rd_len);

        wr_len=write_device_data (fd_dev,wbuffer,frag_size);

        STRAIGHT_RETURN(wr_len);

    }



    close(fd_dev);

    return 0;
}
