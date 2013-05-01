/*******************************************PARAMETER UPDATE POLICY e some ideas**************************************************/
//1) static fx_parameters and oscillator freq don't change within a frame
//2) Fx parameters are passed by reference and not locally copied so that there is only one repository for parameters values
//3) static fx_paramereters are so within a frame
//4) use default types for std functions

/**********************************************************************************************************************/

#include <MULTIFX_errors.h>
#include <MULTIFX_oss_std_cfg.h>
//#include <MULTIFX_oss_utils.h>
#include <MULTIFX_FX_class.h>
#include <MULTIFX_OSCILLATOR_class.h>
#include <MULTIFX_FX_library.h>
#include <MULTIFX_FX_library_utils.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <pthread.h>
#include <MULTIFX_threads.h>

#include <ncurses_UI_class.h>
#include <ncurses.h>



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
     MULTIFX_UINT32_T i =0;
     /********THREADS PARAMETERS**************/
     pthread_t threads;
     MULTIFX_INT32_T tret = 0,*texit;
     MULTIFX_targs_T thread_arg;
     FX_T* param_ex=NULL;
     /******** CLI **************/
    ncurses_UI_T* uiL=NULL, *uiR=NULL, *ui3=NULL;
    char items[MAX_FX_MENU][MAX_CHAR_LEN]={"moog","sin_test","empty"};
    char option_menu[MAX_FX_MENU*MAX_FX_OPTIONS][MAX_CHAR_LEN]={"freq","k","empty","freq","amp","offset"};
    int width = 80, height = 50;
    int chos_indx[N_EFFECTS]={0,0};
    FX_T* p_FX[N_EFFECTS] ={NULL,NULL};
    char c;
    enum SM_UI_states states=INIT;
    float prm = 0;
    int upl = 0;
     /*************MOOG PARAMETERS*/

    MULTIFX_FLOATING_T moog_st_prm = rate;
    MULTIFX_UINT32_T moog_n_st_prm = 1;
    MULTIFX_FLOATING_T Fc = 5000;
    MULTIFX_FLOATING_T kk = 3.5;
    static MULTIFX_FLOATING_T moog_tv_prm[MOOG_N_TV_PARAMS] ;
   moog_tv_prm[0] = Fc;
   moog_tv_prm[1] = kk;
    MULTIFX_UINT32_T moog_n_tv_prm = MOOG_N_TV_PARAMS;
    MULTIFX_UINT32_T moog_state_length = MOOG_FILTER_ORDER;
    static MULTIFX_FLOATING_T moog_init_state[MOOG_FILTER_ORDER];//={0,0,0,0,0};
       /***********MOOG OSC PARAMETERS****************/

    MULTIFX_UINT16_T osc_enabled[MOOG_N_TV_PARAMS]={ENABLE,DISABLE};
    MULTIFX_FLOATING_T l_lim[MOOG_N_TV_PARAMS] = {700,0.2};
   MULTIFX_FLOATING_T h_lim[MOOG_N_TV_PARAMS] = {3000,3.9};
    MULTIFX_FLOATING_T osc_f[MOOG_N_TV_PARAMS] = {1,1};
    MULTIFX_FLOATING_T ph_off[MOOG_N_TV_PARAMS] = {0,0};
 MULTIFX_P_OSC_FUNC_T osc_imp[MOOG_N_TV_PARAMS]= {oscillator,oscillator};

      /*********SIN SRC PARAMETERS******/

    MULTIFX_FLOATING_T f_sy = 1000;
    MULTIFX_FLOATING_T f_sa = rate;
    MULTIFX_FLOATING_T ph_of = 0;
    MULTIFX_FLOATING_T amp = 0.1;
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



    ret = open_stereo_full_duplex_device (devname , &fd_dev,MAX_BUFF_DIM,
                                         &frag_dim_req, &frag_size,
                                         rate,&audio_info, &latency_effort);
    STRAIGHT_tRETURN(ret);

   param_ex = FX_param_exchange_init ( n_bit, stereo_mode);

    thread_arg.device_fd    =fd_dev;
//    thread_arg.sample_rate  =rate;
  //  thread_arg.n_bit        =n_bit;
    thread_arg.buff_char_len=frag_size;
    //thread_arg.stereo_mode  =stereo_mode;
    thread_arg.read_buffer  =rbuffer;
    thread_arg.write_buffer =wbuffer;
    thread_arg.read_buff_L  =rbuffer_FLL;
    thread_arg.read_buff_R  =rbuffer_FLR;
    thread_arg.write_buff_L =wbuffer_FLL;
    thread_arg.write_buff_R =wbuffer_FLR;
    thread_arg.params_exchange = param_ex;
    thread_arg.enable_mainloop = ENABLE;




    while (1)
    {
        switch(states)
        {
//            case INIT :
//                                uiL = UI_init (MAX_FX_MENU, &items[0][0],height,width,  0, 0,4,5,0,0,"ATTACH FX LEFT CHANNEL",8,2);
//                                uiR = UI_init (MAX_FX_MENU, &items[0][0],height,width,  0, 0,4,5,0,0,"ATTACH FX RIGHT CHANNEL",8,2);
//                                states = ATTACH_LEFT;
//                                break;
//            case ATTACH_LEFT :
//                                UI_print_menu(uiL, 1);
//                                UI_menu_selection(uiL, &chos_indx[0]);
//                                if (chos_indx[0]>=0)
//                                {
//                                    states = ATTACH_RIGHT;
//                                }
//
//                                break;
//            case ATTACH_RIGHT :
//                                UI_print_menu(uiR, 1);
//                                UI_menu_selection(uiR, &chos_indx[1]);
//                              if ( (chos_indx[1])>=0)
//                                {
//                                  states = INIZ_SEQUENCE;
//                                }
//
//                                UI_release(uiL);
//                                UI_release(uiR);
//
//                                break;
            case INIT : chos_indx[0]=1;
                        chos_indx[1]=0;
                                for (i=0;i<N_EFFECTS;i++)
                                {
                                    if (chos_indx[i]==0) //moog
                                    {
                                          /***** MOOG INITIALIZATION ************/
                                         p_FX[i]=FX_init(n_bit,stereo_mode,frag_size,moog_n_st_prm,moog_n_tv_prm,moog_state_length,rbuffer_FLL,(MULTIFX_CHAR_T*)"moog",4);
                                        ALLOCATION_CHECK(p_FX[i]);
                                        ret=FX_set_static_params (p_FX[i], &moog_st_prm,1);
                                        STRAIGHT_RETURN(ret);
                                        ret=FX_set_state (p_FX[i], moog_init_state,5);
                                        STRAIGHT_RETURN(ret);
                                        ret=FX_init_timevarying_params (p_FX[i], moog_tv_prm,moog_n_tv_prm);
                                        STRAIGHT_RETURN(ret);
                                        ret=FX_set_implementation (p_FX[i], &moog_filter);
                                        STRAIGHT_RETURN(ret);
                                        /*********OSCILLATOR CONFIGURATION****************/
                                        ret = FX_osc_config(p_FX[i],osc_enabled,l_lim, h_lim,rate,osc_f,ph_off);
                                        ret = FX_osc_implementation (p_FX[i], osc_imp);
                                         STRAIGHT_RETURN(ret);

                                    }
                                    else if (chos_indx[i]==1) //sin_test
                                    {
                                          /***** TEST TONE INITIALIZATION ************/

                                           p_FX[i]=FX_init(n_bit,stereo_mode,frag_size,n_st_prm,n_vr_prm,state_length,NULL,(MULTIFX_CHAR_T*)"sin_src",6);
                                           ALLOCATION_CHECK(p_FX[i]);
                                           ret= FX_set_static_params (p_FX[i], sin_static_param,4);
                                           STRAIGHT_RETURN(ret);
                                           ret=FX_set_state (p_FX[i], &sin_init_state,1);
                                           STRAIGHT_RETURN(ret);
                                          ret=FX_set_implementation (p_FX[i], &test_tone);
                                          STRAIGHT_RETURN(ret);
                                    }
                                }

                                thread_arg.p_left=p_FX[0];
                                thread_arg.p_right=p_FX[1];

                                states = PROCESS;
                                break;

            case PROCESS   : tret = pthread_create(&threads, NULL, ProcMainLoop, (void *)&thread_arg);
                                  states = CHANGE_PARAMS;
                                break;
            case CHANGE_PARAMS : //sleep(5);
                                //thread_arg.enable_mainloop = DISABLE;
                                // goto end;
                                 break;
//            case CHANGE_PARAMS : ui1 = UI_init (1, &items[chos_indx[0]][0],height,width,  0, 0,4,5,0,0,"CHANGE PARAMS LEFT CHANNEL",8,2);
//                                ui2 = UI_init (1, &items[chos_indx[1]][0],height,width,  0, 0,4,5,0,0,"CHANGE PARAMS RIGHT CHANNEL",8,2);
//                                states= LEV1_L;
//                                break;
//
//            case LEV1_L  : ui2 = UI_init (MAX_FX_OPTIONS ,&option_menu[chos_indx[0]*MAX_FX_OPTIONS][0],height,width,  0, 0,4,5,0,0,&(items[chos_indx[0]][0]),8,2);
//                         UI_print_menu(ui2, 1);
//                         UI_menu_selection(ui2, &chos_indx);
//                         if (chos_indx<0)
//                         {
//                               UI_release(ui2);
//                               states = SHOW_MAIN;
//                         }
//                         else
//                         {
//                             states = LEV2;
//                         }
//                         break;
//            case LEV2_L   : ui3 = UI_init (0 ,"",height,width,  0, 0,4,5,0,0,"Parameter",8,2);
//                            UI_param_change(ui3, &prm,.5, &upl);
//                            if (upl==1)
//                            {
//                                UI_release(ui3);
//                                states=LEV1;
//                            }
//                            break;
            default     : break;

        }
    }




end :


tret =pthread_join(threads, (void **)&texit);
if (tret==0)
{
    printf("pthread joined successuflly with exit value?????????? %d\n",texit);
}

 // UI_release(ui);
    FX_release(param_ex);
//    if (p_osc[0]!=NULL)
//    {
//         OSC_release(p_osc[0]);
//    }
//
//    if (p_osc[1]!=NULL)
//    {
//         OSC_release(p_osc[1]);
//    }
    if(p_FX[0]!=NULL)
    {
         FX_release(p_FX[0]);
    }

    if(p_FX[1]!=NULL)
    {
         FX_release(p_FX[1]);
    }
    close(fd_dev);
printf("closing\n");

    return 0;
}
