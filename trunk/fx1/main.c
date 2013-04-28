/*******************************************PARAMETER UPDATE POLICY**************************************************/
//1) static fx_parameters and oscillator freq don't change within a frame
//2) Fx parameters are passed by reference and not locally copied so that there is only one repository for parameters values
//3) static fx_paramereters are so within a frame

/**********************************************************************************************************************/

//#include <MULTIFX_errors.h>
#include <MULTIFX_oss_std_cfg.h>
//#include <MULTIFX_oss_utils.h>
#include <MULTIFX_FX_class.h>
//#include <MULTIFX_OSCILLATOR_class.h>
//#include <MULTIFX_FX_library.h>
//#include <MULTIFX_FX_library_utils.h>
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
     /********THREADS PARAMETERS**************/
     pthread_t threads;
     MULTIFX_INT32_T tret = 0,*texit;
     MULTIFX_targs_T thread_arg;
     FX_T* param_ex=NULL;
     /******** CLI **************/
    ncurses_UI_T* ui=NULL, *ui2=NULL, *ui3=NULL;
    char items[MAX_FX_MENU][MAX_CHAR_LEN]={"moog","sin_test","empty"};
    char option_menu[MAX_FX_MENU*MAX_FX_OPTIONS][MAX_CHAR_LEN]={"freq","k","empty","freq","amp","offset"};
    int width = 80, height = 50;
    int chos_indx=0;
    char c;
    enum SM_UI_states states=INIT;
    float prm = 0;
    int upl = 0;


    ret = open_stereo_full_duplex_device (devname , &fd_dev,MAX_BUFF_DIM,
                                         &frag_dim_req, &frag_size,
                                         rate,&audio_info, &latency_effort);
    STRAIGHT_tRETURN(ret);

   param_ex = FX_param_exchange_init ( n_bit, stereo_mode);

    thread_arg.device_fd    =fd_dev;
    thread_arg.sample_rate  =rate;
    thread_arg.n_bit        =n_bit;
    thread_arg.buff_char_len=frag_size;
    thread_arg.stereo_mode  =stereo_mode;
    thread_arg.read_buffer  =rbuffer;
    thread_arg.write_buffer =wbuffer;
    thread_arg.read_buff_L  =rbuffer_FLL;
    thread_arg.read_buff_R  =rbuffer_FLR;
    thread_arg.write_buff_L =wbuffer_FLL;
    thread_arg.write_buff_R =wbuffer_FLR;
    thread_arg.params_exchange = param_ex;
    thread_arg.enable_mainloop = ENABLE;

tret = pthread_create(&threads, NULL, ProcMainLoop, (void *)&thread_arg);


    while (1)
    {
        switch(states)
        {
            case INIT :  ui = UI_init (MAX_FX_MENU, &items[0][0],height,width,  0, 0,4,5,0,0,"MAIN MENU'",8,2);
                              states = SHOW_MAIN;
                              break;
            case SHOW_MAIN :
                                UI_print_menu(ui, 1);
                                UI_menu_selection(ui, &chos_indx);
                                if (chos_indx>=0)
                                {
                                    states = LEV1;
                                }

                                break;
            case LEV1  : ui2 = UI_init (MAX_FX_OPTIONS ,&option_menu[chos_indx*MAX_FX_OPTIONS][0],height,width,  0, 0,4,5,0,0,&(items[chos_indx][0]),8,2);
                         UI_print_menu(ui2, 1);
                         UI_menu_selection(ui2, &chos_indx);
                         if (chos_indx<0)
                         {
                               UI_release(ui2);
                               states = SHOW_MAIN;
                         }
                         else
                         {
                             states = LEV2;
                         }
                         break;
            case LEV2   : ui3 = UI_init (0 ,"",height,width,  0, 0,4,5,0,0,"Parameter",8,2);
                            UI_param_change(ui3, &prm,.5, &upl);
                            if (upl==1)
                            {
                                UI_release(ui3);
                                states=LEV1;
                            }
                            break;
            default     : break;

        }
    }




end :


tret =pthread_join(threads, (void **)&texit);
if (tret==0)
{
    printf("pthread joined successuflly with exit value?????????? %d\n",texit);
}

  UI_release(ui);
    FX_release(param_ex);
    close(fd_dev);
printf("closing\n");

    return 0;
}
