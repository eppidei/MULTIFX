//#include <MULTIFX_defines.h>
//#include <MULTIFX_typedef.h>
//#include <soundcard.h>
#include <MULTIFX_oss_std_cfg.h>
#include <MULTIFX_oss_utils.h>
#include <MULTIFX_FX_class.h>
#include <MULTIFX_FX_library.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/****************************/

int main ()
{

    static MULTIFX_CHAR_T rbuffer[MAX_BUFF_DIM];
    static MULTIFX_CHAR_T wbuffer[MAX_BUFF_DIM];
//    MULTIFX_INT16_T *rbuffer_16L=calloc(MAX_BUFF_DIM/2/2,sizeof(MULTIFX_INT16_T));
//    MULTIFX_INT16_T *rbuffer_16R=calloc(MAX_BUFF_DIM/2/2,sizeof(MULTIFX_INT16_T));
    MULTIFX_FLOATING_T *rbuffer_FLL=calloc(MAX_BUFF_DIM/2/2,sizeof(MULTIFX_FLOATING_T));
    MULTIFX_FLOATING_T *rbuffer_FLR=calloc(MAX_BUFF_DIM/2/2,sizeof(MULTIFX_FLOATING_T));
//    MULTIFX_INT16_T *wbuffer_16L=calloc(MAX_BUFF_DIM/2/2,sizeof(MULTIFX_INT16_T));
//    MULTIFX_INT16_T *wbuffer_16R=calloc(MAX_BUFF_DIM/2/2,sizeof(MULTIFX_INT16_T));
    MULTIFX_FLOATING_T *wbuffer_FLL=calloc(MAX_BUFF_DIM/2/2,sizeof(MULTIFX_FLOATING_T));
    MULTIFX_FLOATING_T *wbuffer_FLR=calloc(MAX_BUFF_DIM/2/2,sizeof(MULTIFX_FLOATING_T));
    MULTIFX_CHAR_T *devname = "/dev/dsp";
    MULTIFX_API_RET ret = 0;
    MULTIFX_INT32_T fd_dev = 0;
    MULTIFX_INT32_T frag_dim_req = MAX_BUFF_DIM;
    MULTIFX_INT32_T frag_size = 0;
    MULTIFX_INT32_T rate = SAMPLING_RATE;
    oss_audioinfo audio_info ;
    MULTIFX_INT32_T latency_effort = 5;
    MULTIFX_INT32_T rd_len = 0, wr_len = 0;
    MULTIFX_UINT32_T loopcount = 0,i=0;
    FILE *fid_in,*fid_out;
    fid_in  = fopen("in_samples_stereo.txt","w");
    fid_out = fopen("out_samples_stereo.txt","w");

    /***************/
    FX_T *sin_src=NULL, *sin_src2;
    MULTIFX_FLOATING_T sin_src_param[MAX_PARAMS],sin_src2_param[MAX_PARAMS];
    MULTIFX_FLOATING_T sin_init_state[MAX_STATE_LEN];

    /**************/



    ret = open_stereo_full_duplex_device (devname , &fd_dev,MAX_BUFF_DIM,
                                         &frag_dim_req, &frag_size,
                                         rate,&audio_info, &latency_effort);
    STRAIGHT_RETURN(ret);



   sin_src=FX_init(frag_size,4,1,rbuffer_FLL);
   sin_src_param[0]= 1000;
   sin_src_param[1]= rate;
   sin_src_param[2]= 0;
   sin_src_param[3]= .9;
   sin_init_state[0]=0;
   ret = FX_set_params (sin_src, sin_src_param,sin_init_state);
   ret =  FX_set_implementation (sin_src,&test_tone);

   sin_src2 = FX_clone(sin_src,rbuffer_FLR);
    sin_src2_param[0]= 1200;
   sin_src2_param[1]= rate;
   sin_src2_param[2]= 0;
   sin_src2_param[3]= .8;

 ret = FX_set_params (sin_src2, sin_src2_param,sin_init_state);

    rd_len = frag_size;

    while ((rd_len==frag_size) )
    {
        rd_len=read_device_data (fd_dev,rbuffer,frag_size);
        STRAIGHT_RETURN(rd_len);

        ret = char2float_stereo (rbuffer, rbuffer_FLL, rbuffer_FLR, frag_size);
        STRAIGHT_RETURN(ret);

        /******** Processing ***************/

       // ret = *(moog->processing_func)(moog->params,rbuffer_FLL,moog->fx_out_buf,frag_size/2/2);

       /******* LEFT **************/
        FX_process(sin_src);
        FX_bufcpy(sin_src,wbuffer_FLL);
       // memcpy(wbuffer_FLL,rbuffer_FLL,frag_size/2/2*sizeof(MULTIFX_FLOATING_T));
        /************ RIGHT ****************/
       // memcpy(wbuffer_FLR,rbuffer_FLR,frag_size/2/2*sizeof(MULTIFX_FLOATING_T));
         FX_process(sin_src2);
        FX_bufcpy(sin_src2,wbuffer_FLR);
        /***************************************/

        ret = float2char_stereo (wbuffer, wbuffer_FLL, wbuffer_FLR, frag_size);
        STRAIGHT_RETURN(ret);

        wr_len=write_device_data (fd_dev,wbuffer,frag_size);
        STRAIGHT_RETURN(wr_len);

        #ifdef DEBUG

        ret = memcmp(rbuffer,wbuffer,sizeof(rbuffer));
        if(ret!=0)
        {
            printf("error:::::\n");
            for (i=0;i<sizeof(rbuffer);i++)
            {
                if (rbuffer[i]!=wbuffer[i])
                {
                    fprintf (fid_in,"%2.2hhX\n",rbuffer[i]);
                    fprintf (fid_out,"%2.2hhX\n",wbuffer[i]);
                }

            }
        }

        #endif

        loopcount++;
    }


end :

//    free(rbuffer_16L);
//    free(rbuffer_16R);
//    free(wbuffer_16L);
//    free(wbuffer_16R);
    free(rbuffer_FLL);
    free(rbuffer_FLR);
    free(wbuffer_FLL);
    free(wbuffer_FLR);
    close(fd_dev);
    fclose(fid_in);
    fclose(fid_out);

    return 0;
}
