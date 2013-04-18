#include <MULTIFX_defines.h>
#include <MULTIFX_typedef.h>
#include <soundcard.h>
#include <MULTIFX_oss_std_cfg.h>
#include <MULTIFX_oss_utils.h>
#include <unistd.h>

/**************************/

typedef struct FX_S FX_T;
//FX_T* FX_init(MULTIFX_FLOATING_T *p_params);



struct FX_S {
MULTIFX_FLOATING_T params[2];
MULTIFX_FLOATING_T *fx_out_buf;
MULTIFX_FLOATING_T (*processing_func)(MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*, MULTIFX_FLOATING_T*);
};

FX_T* FX_init(MULTIFX_FLOATING_T* p_params,  int out_buf_size,MULTIFX_FLOATING_T (*fpoint) (MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*,MULTIFX_FLOATING_T*))
{
    FX_T* pthis = calloc(1,sizeof(FX_T));
    if (pthis!=NULL)
    {
        pthis->params[0]=p_params[0];
        pthis->params[1]=p_params[1];
        pthis->fx_out_buf = calloc(out_buf_size,sizeof(MULTIFX_FLOATING_T));
        pthis->processing_func=fpoint;
    }

    return pthis;
}

int MULTIFX_sum(MULTIFX_FLOATING_T* a,MULTIFX_FLOATING_T* b,MULTIFX_FLOATING_T* buf_in,MULTIFX_FLOATING_T* buf_out)
{
    buf_out[0]=*a+*b+buf_in[0];

    return 0;
}

int MULTIFX_div(MULTIFX_FLOATING_T *a,MULTIFX_FLOATING_T *b,MULTIFX_FLOATING_T* buf_in,MULTIFX_FLOATING_T* buf_out)
{
    buf_out[0]=(*a)/(*b)+buf_in[0];

    return 0;
}


/****************************/

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

    /***************/
    MULTIFX_FLOATING_T adder_par[2] ={0,0};
    FX_T* adder = FX_init(0,0,&MULTIFX_sum);

    /**************/



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
