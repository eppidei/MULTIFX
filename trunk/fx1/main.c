//#include <MULTIFX_defines.h>
//#include <MULTIFX_typedef.h>
//#include <soundcard.h>
#include <MULTIFX_oss_std_cfg.h>
#include <MULTIFX_oss_utils.h>
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
//    MULTIFX_FLOATING_T adder_par[2] ={1.1,2.2}, divider_par[2] ={4,2};
//   FX_T  *adder = NULL, *divider =NULL;

    /**************/



    ret = open_stereo_full_duplex_device (devname , &fd_dev,MAX_BUFF_DIM,
                                         &frag_dim_req, &frag_size,
                                         rate,&audio_info, &latency_effort);
    STRAIGHT_RETURN(ret);


//
//    adder=FX_init(adder_par,frag_size,&MULTIFX_sum);
//    divider=FX_init(divider_par,frag_size,&MULTIFX_div);



    rd_len = frag_size;

    while ((rd_len==frag_size) && loopcount<1)
    {
        rd_len=read_device_data (fd_dev,rbuffer,frag_size);
        STRAIGHT_RETURN(rd_len);

        ret = char2float_stereo (rbuffer, rbuffer_FLL, rbuffer_FLR, frag_size);
        STRAIGHT_RETURN(ret);

        /******** Processing ***************/

        memcpy(wbuffer_FLL,rbuffer_FLL,frag_size/2/2*sizeof(MULTIFX_FLOATING_T));
        memcpy(wbuffer_FLR,rbuffer_FLR,frag_size/2/2*sizeof(MULTIFX_FLOATING_T));

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
