#include <MULTIFX_threads.h>
#include <MULTIFX_typedef.h>
#include <stdlib.h>
#include <MULTIFX_errors.h>
#include <MULTIFX_FX_library.h>
#include <MULTIFX_FX_library_utils.h>
#include <MULTIFX_FX_class.h>
#include <pthread.h>
#include <MULTIFX_OSCILLATOR_class.h>
#include <MULTIFX_oss_utils.h>
#include <semaphore.h>
#include <stdio.h>

extern sem_t sem;

void *ProcMainLoop(void *threadarg)
{
    /****Thread arguments********/

    MULTIFX_targs_T* targuments     = threadarg;
    MULTIFX_INT32_T fd_dev          = targuments->device_fd;
    MULTIFX_INT32_T frag_size       = targuments->buff_char_len;
    MULTIFX_API_RET ret             = 0;
    MULTIFX_CHAR_T *rbuffer         = targuments->read_buffer;
    MULTIFX_CHAR_T *wbuffer         = targuments->write_buffer;
    MULTIFX_FLOATING_T *rbuffer_FLL = targuments->read_buff_L;
    MULTIFX_FLOATING_T *rbuffer_FLR = targuments->read_buff_R;
    MULTIFX_FLOATING_T *wbuffer_FLL = targuments->write_buff_L;
    MULTIFX_FLOATING_T *wbuffer_FLR = targuments->write_buff_R;
    MULTIFX_UINT16_T flag           = targuments->enable_mainloop;
    FX_T*             left_chain    = targuments->p_left;
    FX_T*             right_chain    = targuments->p_right;
    FILE**            debu_file       =targuments->p_debug;
    MULTIFX_UINT16_T*    up_flag_L    =targuments->update_flag_L;
    MULTIFX_UINT16_T*    up_flag_R    =targuments->update_flag_R;

  /************************************/

    MULTIFX_INT32_T rd_len = 0,wr_len=0,loopcount = 0;


    rd_len = frag_size;

    while ((rd_len==frag_size) && flag==ENABLE)
    {
        flag           = targuments->enable_mainloop;

        rd_len=read_device_data (fd_dev,rbuffer,frag_size);
        STRAIGHT_tRETURN(rd_len);

        ret = char2float_stereo (rbuffer, rbuffer_FLL, rbuffer_FLR, frag_size);
        STRAIGHT_tRETURN(ret);




sem_wait(&sem);

if(*up_flag_L==1)
{
	ret=FX_static_param_update(left_chain);
	STRAIGHT_tRETURN(ret);
	*up_flag_L=0;
}
//#ifdef DEBUG
 //FX_printf(left_chain,debu_file);
//#endif

 if(*up_flag_R==1)
 {
 ret=FX_static_param_update(right_chain);
 STRAIGHT_tRETURN(ret);
 *up_flag_R=0;
 }

#ifdef DEBUG
 FX_printf(right_chain,debu_file);
#endif



 sem_post(&sem);

        /******** Processing ***************/



       /******* LEFT **************/

       ret=FX_process(left_chain);
       STRAIGHT_tRETURN(ret);
       ret=FX_bufcpy(left_chain,wbuffer_FLL);
       STRAIGHT_tRETURN(ret);

        /************ RIGHT ****************/

       ret=FX_process(right_chain);
       STRAIGHT_tRETURN(ret);
        ret=FX_bufcpy(right_chain,wbuffer_FLR);
        STRAIGHT_tRETURN(ret);
        /***************************************/

        ret = float2char_stereo (wbuffer, wbuffer_FLL, wbuffer_FLR, frag_size);
        STRAIGHT_tRETURN(ret);

        wr_len=write_device_data (fd_dev,wbuffer,frag_size);
        STRAIGHT_tRETURN(wr_len);

      /*  #ifdef DEBUG

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

        #endif*/

        loopcount++;
    }




    pthread_exit(NULL);

}
