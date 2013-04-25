//#include <MULTIFX_defines.h>
//#include <MULTIFX_typedef.h>
//#include <soundcard.h>
#include <MULTIFX_errors.h>
#include <MULTIFX_oss_std_cfg.h>
#include <MULTIFX_oss_utils.h>
#include <MULTIFX_FX_class.h>
#include <MULTIFX_OSCILLATOR_class.h>
#include <MULTIFX_FX_library.h>
#include <MULTIFX_FX_library_utils.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


void *ProcMainLoop(void *threadarg)
{
    /****Thread arguments********/

    MULTIFX_targs_T* targuments     = threadarg;
    MULTIFX_INT32_T fd_dev          = targuments->device_fd;
    MULTIFX_INT32_T frag_size       = targuments->buff_char_len;
    MULTIFX_INT32_T rate            = targuments->sample_rate;
    MULTIFX_UINT16_T n_bit          = targuments->n_bit;
    MULTIFX_UINT16_T stereo_mode    = targuments->stereo_mode;
    MULTIFX_API_RET ret             = 0;
    MULTIFX_CHAR_T *rbuffer         = targuments->read_buffer;
    MULTIFX_CHAR_T *wbuffer         = targuments->write_buffer;
    MULTIFX_FLOATING_T *rbuffer_FLL = targuments->read_buff_L;
    MULTIFX_FLOATING_T *rbuffer_FLR = targuments->read_buff_R;
    MULTIFX_FLOATING_T *wbuffer_FLL = targuments->write_buff_L;
    MULTIFX_FLOATING_T *wbuffer_FLR = targuments->write_buff_R;
  /************************************/
     #ifdef DEBUG
    FILE *fid_in,*fid_out;
    fid_in  = fopen("in_samples_stereo.txt","w");
    fid_out = fopen("out_samples_stereo.txt","w");
    #endif
    MULTIFX_INT32_T rd_len = 0,wr_len=0,loopcount = 0;
      /*********SIN SRC PARAMETERS******/
    FX_T *sin_src=NULL;
    MULTIFX_FLOATING_T f_sy = 1000;
    MULTIFX_FLOATING_T f_sa = rate;
    MULTIFX_FLOATING_T ph_of = 0;
    MULTIFX_FLOATING_T amp = 0.05;
    MULTIFX_UINT32_T n_st_prm = 4;
    MULTIFX_UINT32_T n_vr_prm = 0;
    MULTIFX_UINT32_T state_length = 1;
    static MULTIFX_FLOATING_T sin_static_param[4];
    sin_static_param[0]=f_sy;
     sin_static_param[1]=f_sa;
      sin_static_param[2]=ph_of;
       sin_static_param[3]=amp;
    MULTIFX_FLOATING_T sin_init_state=0;
    MULTIFX_FLOATING_T *p_out_buff;

    /*************MOOG PARAMETERS*/
    FX_T *moog=NULL;
    MULTIFX_FLOATING_T moog_st_prm = rate;
    MULTIFX_UINT32_T moog_n_st_prm = 1;
    MULTIFX_FLOATING_T Fc = 5000;
    MULTIFX_FLOATING_T kk = 3.5;
    static MULTIFX_FLOATING_T moog_tv_prm[2] ;
   moog_tv_prm[0] = Fc;
   moog_tv_prm[1] = kk;
    MULTIFX_UINT32_T moog_n_tv_prm = 2;
    MULTIFX_UINT32_T moog_state_length = 5;
    static MULTIFX_FLOATING_T moog_init_state[5];//={0,0,0,0,0};
    /***********OSC PARAMETERS****************/
    OSCILLATOR_T *sin_osc=NULL;
    MULTIFX_FLOATING_T l_lim = 700;
   MULTIFX_FLOATING_T h_lim = 3000;
    MULTIFX_FLOATING_T osc_f = 1;
    MULTIFX_FLOATING_T *prm_2_vary;
    MULTIFX_UINT32_T n_tv_pm = 0;
    MULTIFX_UINT32_T len_frame = 0;

    ///***** TEST TONE INITIALIZATION ************/

   sin_src=FX_init(n_bit,stereo_mode,frag_size,n_st_prm,n_vr_prm,state_length,NULL);
   ALLOCATION_tCHECK(sin_src);
   ret= FX_set_static_params (sin_src, sin_static_param,4);
   STRAIGHT_tRETURN(ret);
   ret=FX_set_state (sin_src, &sin_init_state,1);
   STRAIGHT_tRETURN(ret);
  ret=FX_set_implementation (sin_src, &test_tone);
  STRAIGHT_tRETURN(ret);

  /***** MOOG INITIALIZATION ************/
 // FX_get_out_buff(sin_src,&p_out_buff);
  moog=FX_init(n_bit,stereo_mode,frag_size,moog_n_st_prm,moog_n_tv_prm,moog_state_length,rbuffer_FLL);
  ALLOCATION_tCHECK(moog);
ret=FX_set_static_params (moog, &moog_st_prm,1);
STRAIGHT_tRETURN(ret);
ret=FX_set_state (moog, moog_init_state,5);
STRAIGHT_tRETURN(ret);
ret=FX_init_timevarying_params (moog, moog_tv_prm,moog_n_tv_prm);
STRAIGHT_tRETURN(ret);
ret=FX_set_implementation (moog, &moog_filter);
STRAIGHT_tRETURN(ret);
/*********OSCILLATOR CONFIGURATION****************/
ret=FX_get_timevarying_params (moog, &prm_2_vary,&n_tv_pm, &len_frame);
STRAIGHT_tRETURN(ret);
sin_osc=OSC_init();
ALLOCATION_tCHECK(sin_osc);
ret=OSC_configure (sin_osc,l_lim, h_lim,rate,osc_f ,0);
STRAIGHT_tRETURN(ret);
ret=OSC_set_implementation(sin_osc,&oscillator);
STRAIGHT_tRETURN(ret);



    rd_len = frag_size;

    while ((rd_len==frag_size))
    {
        rd_len=read_device_data (fd_dev,rbuffer,frag_size);
        STRAIGHT_tRETURN(rd_len);

        ret = char2float_stereo (rbuffer, rbuffer_FLL, rbuffer_FLR, frag_size);
        STRAIGHT_tRETURN(ret);

        /******** Processing ***************/

       // ret = *(moog->processing_func)(moog->params,rbuffer_FLL,moog->fx_out_buf,frag_size/2/2);

       /******* LEFT **************/
       ret=FX_process(sin_src);
       STRAIGHT_tRETURN(ret);
       ret=FX_bufcpy(sin_src,wbuffer_FLL);
       STRAIGHT_tRETURN(ret);
       // memcpy(wbuffer_FLL,rbuffer_FLL,frag_size/2/2*sizeof(MULTIFX_FLOATING_T));
        /************ RIGHT ****************/
       // memcpy(wbuffer_FLR,rbuffer_FLR,frag_size/2/2*sizeof(MULTIFX_FLOATING_T));
        // FX_process(sin_src);
         ret=OSC_trigger(sin_osc,prm_2_vary,0,len_frame);
         STRAIGHT_tRETURN(ret);
       ret=FX_process(moog);
       STRAIGHT_tRETURN(ret);
        ret=FX_bufcpy(moog,wbuffer_FLR);
        STRAIGHT_tRETURN(ret);
        /***************************************/

        ret = float2char_stereo (wbuffer, wbuffer_FLL, wbuffer_FLR, frag_size);
        STRAIGHT_tRETURN(ret);

        wr_len=write_device_data (fd_dev,wbuffer,frag_size);
        STRAIGHT_tRETURN(wr_len);

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

     #ifdef DEBUG
    fclose(fid_in);
    fclose(fid_out);
    #endif

    OSC_release(sin_osc);
    FX_release(sin_src);
    FX_release(moog);
    pthread_exit(NULL);

}

/****************************/

int main ()
{
    /********************GENERAL PARAMETERS********************************/
static MULTIFX_CHAR_T rbuffer[MAX_BUFF_DIM];
    static MULTIFX_CHAR_T wbuffer[MAX_BUFF_DIM];


 static MULTIFX_FLOATING_T rbuffer_FLL[MAX_BUFF_DIM/2/2];
 static MULTIFX_FLOATING_T rbuffer_FLR[MAX_BUFF_DIM/2/2];

static MULTIFX_FLOATING_T wbuffer_FLL[MAX_BUFF_DIM/2/2];
 static MULTIFX_FLOATING_T wbuffer_FLR[MAX_BUFF_DIM/2/2];
    MULTIFX_CHAR_T *devname = "/dev/dsp";
    MULTIFX_API_RET ret = 0;
    MULTIFX_INT32_T fd_dev = 0;
    MULTIFX_INT32_T frag_dim_req = MAX_BUFF_DIM;
    MULTIFX_INT32_T frag_size = 0;
    MULTIFX_INT32_T rate = SAMPLING_RATE;
    oss_audioinfo audio_info ;
    MULTIFX_INT32_T latency_effort = 5;
   // MULTIFX_UINT32_T i=0;

    MULTIFX_UINT16_T stereo_mode  = 1;
     MULTIFX_UINT16_T n_bit  = 16;
     /********THREADS PARAMETERS**************/
     pthread_t threads;
     MULTIFX_INT32_T tret = 0,*texit;
     MULTIFX_targs_T thread_arg;





    ret = open_stereo_full_duplex_device (devname , &fd_dev,MAX_BUFF_DIM,
                                         &frag_dim_req, &frag_size,
                                         rate,&audio_info, &latency_effort);
    STRAIGHT_tRETURN(ret);

    thread_arg.device_fd=fd_dev;
    thread_arg.sample_rate=rate;
    thread_arg.n_bit=n_bit;
    thread_arg.buff_char_len=frag_size;
    thread_arg.stereo_mode=stereo_mode;
    thread_arg.read_buffer=rbuffer;
    thread_arg.write_buffer=wbuffer;
    thread_arg.read_buff_L=rbuffer_FLL;
    thread_arg.read_buff_R=rbuffer_FLR;
    thread_arg.write_buff_L=wbuffer_FLL;
    thread_arg.write_buff_R=wbuffer_FLR;

tret = pthread_create(&threads, NULL, ProcMainLoop, (void *)&thread_arg);




end :

tret =pthread_join(threads, (void **)&texit);
    close(fd_dev);


    return 0;
}
