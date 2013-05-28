/*******************************************PARAMETER UPDATE POLICY e some ideas**************************************************/
//1) static fx_parameters and oscillator freq don't change within a frame
//2) Fx parameters are passed by reference and not locally copied so that there is only one repository for parameters values
//3) static fx_paramereters are so within a frame
//4) use default types for std functions

/**********************************************************************************************************************/

#include <MULTIFX_defines.h>
#include <MULTIFX_errors.h>
#include <MULTIFX_oss_std_cfg.h>
#include <MULTIFX_FX_class.h>
#include <MULTIFX_MENU_class.h>
#include <MULTIFX_OSCILLATOR_class.h>
#include <MULTIFX_FX_library.h>
#include <MULTIFX_FX_library_utils.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <MULTIFX_threads.h>
#include <ncurses_UI_class.h>
#include <ncurses.h>
#include <semaphore.h>
#include <string.h>

sem_t sem;

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
     //FX_T* param_ex=NULL;
     FILE *dbg_file=NULL;
     MULTIFX_UINT16_T flag_update_left=0,flag_update_right=0;

     /*************MOOG PARAMETERS*/
     MULTIFX_UINT32_T moog_instances = 0;
    MULTIFX_UINT32_T num_osc_prms = 5;
    MULTIFX_FLOATING_T moog_fix_prm = rate;
    MULTIFX_UINT32_T moog_n_fix_prm = 1;

    //MULTIFX_FLOATING_T moog_st_prm = rate;
    MULTIFX_UINT32_T moog_n_st_prm = 0;
    MULTIFX_FLOATING_T Fc = 5000;
    MULTIFX_FLOATING_T kk = 3.5;
    static MULTIFX_FLOATING_T moog_tv_prm[MOOG_N_TV_PARAMS] ;
   moog_tv_prm[0] = Fc;
   moog_tv_prm[1] = kk;
    MULTIFX_UINT32_T moog_n_tv_prm = MOOG_N_TV_PARAMS;
    MULTIFX_UINT32_T moog_state_length = MOOG_FILTER_ORDER;
    static MULTIFX_FLOATING_T moog_init_state[MOOG_FILTER_ORDER];//={0,0,0,0,0};
    MULTIFX_P_PROC_FUNC_T p_moog_fx_func =moog_filter;
       /***********MOOG OSC PARAMETERS****************/

    MULTIFX_UINT16_T osc_enabled[MOOG_N_TV_PARAMS]={ENABLE,DISABLE};
    MULTIFX_FLOATING_T l_lim[MOOG_N_TV_PARAMS] = {700,0.2};
   MULTIFX_FLOATING_T h_lim[MOOG_N_TV_PARAMS] = {3000,3.9};
    MULTIFX_FLOATING_T osc_f[MOOG_N_TV_PARAMS] = {1,1};
    MULTIFX_FLOATING_T ph_off[MOOG_N_TV_PARAMS] = {0,0};
 MULTIFX_P_OSC_FUNC_T osc_imp[MOOG_N_TV_PARAMS]= {oscillator,oscillator};

      /*********SIN SRC PARAMETERS******/
 MULTIFX_UINT32_T sin_src_instances = 0;
    MULTIFX_FLOATING_T f_sy = 1000;
    MULTIFX_FLOATING_T f_sa = rate;
    MULTIFX_FLOATING_T ph_of = 0;
    MULTIFX_FLOATING_T amp = 0.5;
    //MULTIFX_UINT32_T n_st_prm = 4;
    MULTIFX_UINT32_T n_vr_prm = 0;
    MULTIFX_UINT32_T sinsrc_state_leng = SINSRC_STATE_LEN;
    MULTIFX_UINT32_T n_sinsrc_fixed_prm = 1;
    MULTIFX_FLOATING_T sin_fixed_param = f_sa;
    static MULTIFX_FLOATING_T sin_static_param[SINSRC_N_STATIC_PARAMS];
    MULTIFX_UINT32_T n_sinsrc_statprm = SINSRC_N_STATIC_PARAMS;
    sin_static_param[0]=f_sy;
     //sin_static_param[1]=f_sa;
      sin_static_param[1]=ph_of;
       sin_static_param[2]=amp;
    MULTIFX_FLOATING_T sin_init_state=0;
   // MULTIFX_FLOATING_T *p_out_buff;
MULTIFX_P_PROC_FUNC_T p_sinsrc_fx_func=test_tone;

/******** CLI **************/
  ncurses_UI_T* uiL=NULL, *uiR=NULL, *uiL2=NULL,*uiL3=NULL,*ui=NULL;
  /*** Menu channel*******/
  MULTIFX_MENU_NODE_T* choose_channel_root=NULL,* ch_l=NULL,*ch_r=NULL,*stop=NULL ;
  MULTIFX_CHAR_T     *choose_channel_name = "channel selection";
  MULTIFX_MENU_HEAD_T* choose_channel_head=NULL;
  enum menu_item_t menu_type;

  /*********Menu effects*********/
  MULTIFX_MENU_NODE_T* set_effect_root=NULL,*moog=NULL,*sin_test=NULL;
  static MULTIFX_CHAR_T     set_effect_name[MAX_CHAR_LEN] = "effect selection",moog_name[MAX_CHAR_LEN]="moog",sin_src_name[MAX_CHAR_LEN]="sin_test";
  MULTIFX_MENU_HEAD_T* set_effect_head=NULL;
  MULTIFX_UINT32_T max_n_effects = 10;


/**********Sin test option menu*********/
  MULTIFX_MENU_NODE_T *freq=NULL,*offset=NULL,*ampl=NULL;
  MULTIFX_CHAR_T *freq_name="frequenza",*offset_name="offset",*amp_name="ampiezza";

/************Moog menu***************/
  MULTIFX_MENU_NODE_T *tv_params=NULL,*static_params=NULL;
  MULTIFX_CHAR_T *tv_params_name="time varying",*static_params_name="static_parameters";
  MULTIFX_MENU_NODE_T *freq_taglio=NULL,*kappa=NULL;
  MULTIFX_CHAR_T *freq_taglio_name="freq_taglio",*kappa_name="K";
/**********Oscillator Menu**********/
  //MULTIFX_MENU_NODE_T* oscillator_options=NULL;
 // MULTIFX_CHAR_T * oscillator_options_name="oscillator";
  MULTIFX_MENU_NODE_T *osc_freq=NULL,*osc_amp=NULL;
  MULTIFX_CHAR_T *osc_freq_name="frequenza",*osc_amp_name="ampiezza";
  /************tmp values *****************/
  MULTIFX_MENU_NODE_T* tmp_node1=NULL, *tmp_node2=NULL;
 static char param_name[MAX_CHAR_LEN];
 MULTIFX_FLOATING_T *p_stat_prm=NULL,*p_tv_params=NULL;
 MULTIFX_UINT32_T tmp_idx=0, tmp_depth=0, tmp_depth2=0;

/*****************************************/

  int width = 80, height = 50;
  int chos_fx_per_ch[N_CHANNELS]={0,0};
  //int chos_param[N_CHANNELS]={0,0};
  int selection=0,ch_idx=0;
  FX_T* p_FX[N_EFFECTS_X_CH*N_CHANNELS] ={NULL,NULL};
  //char c;
  enum SM_UI_states states=INIT;
  MULTIFX_FLOATING_T prm = 0,prm_tmp = 0;
  MULTIFX_UINT32_T len_prm=0;
  int upl = 0;



    ret = open_stereo_full_duplex_device (devname , &fd_dev,MAX_BUFF_DIM,
                                         &frag_dim_req, &frag_size,
                                         rate,&audio_info, &latency_effort);
    STRAIGHT_RETURN(ret);

   //param_ex = FX_param_exchange_init ( n_bit, stereo_mode);

    dbg_file=fopen("/home/leonardo/gres_test.txt","w");
    ALLOCATION_CHECK(dbg_file);

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
   // thread_arg.params_exchange = param_ex;
    thread_arg.enable_mainloop = ENABLE;
    thread_arg.p_debug=&dbg_file;//
    thread_arg.update_flag_L= &flag_update_left;
    thread_arg.update_flag_R= &flag_update_right;








    while (1)
    {
        switch(states)
        {
            case INIT :         UI_ncurses_on();

								ret = MULTIFX_MENU_init(&set_effect_root,&set_effect_head,max_n_effects,set_effect_name,root);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&moog,set_effect_root, set_effect_head,0,moog_name,sub_menu);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&sin_test,set_effect_root, set_effect_head,1,sin_src_name,sub_menu);
								STRAIGHT_RETURN(ret);
								/****************sin_test*********************/
								ret = MULTIFX_MENU_create_child_idx (&freq,sin_test, set_effect_head,0,freq_name,static_parameter);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&ampl,sin_test, set_effect_head,1,amp_name,static_parameter);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&offset,sin_test, set_effect_head,2,offset_name,static_parameter);
								STRAIGHT_RETURN(ret);
								/*******************moog***********************/
								ret = MULTIFX_MENU_create_child_idx (&static_params,moog, set_effect_head,0,static_params_name,sub_menu);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&tv_params,moog, set_effect_head,1,tv_params_name,sub_menu);
								STRAIGHT_RETURN(ret);

								ret = MULTIFX_MENU_create_child_idx (&freq_taglio,tv_params, set_effect_head,0,freq_taglio_name,sub_menu);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&kappa,tv_params, set_effect_head,1,kappa_name,sub_menu);
								STRAIGHT_RETURN(ret);

								ret = MULTIFX_MENU_create_child_idx (&osc_freq,freq_taglio, set_effect_head,0,osc_freq_name,oscillator_parameter);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&osc_freq,kappa, set_effect_head,0,osc_freq_name,oscillator_parameter);
								STRAIGHT_RETURN(ret);
//								ret = MULTIFX_MENU_create_child_idx (&osc_amp,tv_params, set_effect_head,1,osc_amp_name,oscillator_parameter);
//								STRAIGHT_RETURN(ret);

								/**************************L-R Channels***********************************/
								 ret = MULTIFX_MENU_init(&choose_channel_root,&choose_channel_head,max_n_effects,choose_channel_name,root);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&ch_l,choose_channel_root, choose_channel_head,0,"LEFT",sub_menu);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&ch_r,choose_channel_root, choose_channel_head,1,"RIGHT",sub_menu);
								STRAIGHT_RETURN(ret);
								ret = MULTIFX_MENU_create_child_idx (&stop,choose_channel_root, choose_channel_head,2,"STOP",sub_menu);
								STRAIGHT_RETURN(ret);
								/***********************************************************************************/
                                uiL = UI_init (set_effect_root,height,width,  0, 0,4,5,0,0,"ATTACH FX LEFT CHANNEL",8,2);
                                uiR = UI_init (set_effect_root,height,width,  0, 0,4,5,0,0,"ATTACH FX RIGHT CHANNEL",8,2);

                                states = ATTACH_LEFT;
                                break;
            case ATTACH_LEFT :
                                UI_print_menu(uiL, 1);
                                UI_menu_selection(uiL, &chos_fx_per_ch[0]);
                                if (chos_fx_per_ch[0]>=0)
                                {
                                    states = ATTACH_RIGHT;
                                }

                                break;
            case ATTACH_RIGHT :
                                UI_print_menu(uiR, 1);
                                UI_menu_selection(uiR, &chos_fx_per_ch[1]);
                              if ( (chos_fx_per_ch[1])>=0)
                                {
                                  states = INIZ_SEQUENCE;
                                }

                                UI_release(uiL);
                                UI_release(uiR);

                                break;
            case INIZ_SEQUENCE : //chos_fx_per_ch[0]=1;
                                //chos_fx_per_ch[1]=0;
                                for (i=0;i<N_CHANNELS;i++)
                                {
                                    if (chos_fx_per_ch[i]==0) //moog
                                    {
                                    	moog_instances++;
                                          /***** MOOG INITIALIZATION ************/
                                         p_FX[i]=FX_init(n_bit,stereo_mode,frag_size,i,moog_instances,moog_n_fix_prm,moog_n_st_prm,moog_n_tv_prm,moog_state_length,rbuffer_FLL,moog_name,num_osc_prms);
                                        ALLOCATION_CHECK(p_FX[i]);
                                        ret=FX_set_fixed_params (p_FX[i], &moog_fix_prm,moog_n_fix_prm);
                                        STRAIGHT_RETURN(ret);
                                        //ret=FX_set_static_params (p_FX[i], &moog_st_prm,moog_n_st_prm);
                                        //STRAIGHT_RETURN(ret);
                                        ret=FX_set_state (p_FX[i], moog_init_state,moog_state_length);
                                        STRAIGHT_RETURN(ret);
                                        ret=FX_init_timevarying_params_buffer (p_FX[i], moog_tv_prm,moog_n_tv_prm);
                                        STRAIGHT_RETURN(ret);
                                        ret=FX_set_implementation (p_FX[i], p_moog_fx_func);
                                        STRAIGHT_RETURN(ret);
                                        /*********OSCILLATOR CONFIGURATION****************/
                                        ret = FX_osc_config(p_FX[i],osc_enabled,l_lim, h_lim,rate,osc_f,ph_off);
                                        ret = FX_osc_implementation (p_FX[i], osc_imp);
                                         STRAIGHT_RETURN(ret);
                                         /***************MENU ATTACH*********************/
                                         ret= FX_attach_menu(p_FX[i],moog,set_effect_head);
                                          STRAIGHT_RETURN(ret);

                                    }
                                    else if (chos_fx_per_ch[i]==1) //sin_test
                                    {
                                          /***** TEST TONE INITIALIZATION ************/
											sin_src_instances++;
                                           p_FX[i]=FX_init(n_bit,stereo_mode,frag_size,i,sin_src_instances,n_sinsrc_fixed_prm,n_sinsrc_statprm,n_vr_prm,sinsrc_state_leng,NULL,sin_src_name,0);
                                           ALLOCATION_CHECK(p_FX[i]);
                                           ret=FX_set_fixed_params (p_FX[i], &sin_fixed_param,n_sinsrc_fixed_prm);
                                           STRAIGHT_RETURN(ret);
                                           ret= FX_set_static_params (p_FX[i], sin_static_param,n_sinsrc_statprm);
                                           STRAIGHT_RETURN(ret);
                                           ret=FX_set_state (p_FX[i], &sin_init_state,1);
                                           STRAIGHT_RETURN(ret);
                                          ret=FX_set_implementation (p_FX[i], p_sinsrc_fx_func);
                                          STRAIGHT_RETURN(ret);
                                          /************MENU ATTACH**************/
                                          ret =  FX_attach_menu(p_FX[i],sin_test,set_effect_head);
                                         STRAIGHT_RETURN(ret);
                                    }
                                }

                                thread_arg.p_left=p_FX[0];
                                thread_arg.p_right=p_FX[1];

                                states = PROCESS;
                                break;

            case PROCESS   : sem_init(&sem,0,1);
                                tret = pthread_create(&threads, NULL, ProcMainLoop, (void *)&thread_arg);


                                 ui = UI_init (choose_channel_root,height,width,  0, 0,4,5,0,0,"CHOOSE CHANNEL FOR PARAMETER CHANGING",8,2);
                                  states = CHOOSE_CHANNEL;
                                break;
            case CHOOSE_CHANNEL :
                                     UI_print_menu(ui, 1);
                                    UI_menu_selection(ui, &ch_idx);

                                    if (ch_idx<0)
                                    {
                                        states= CHOOSE_CHANNEL;
                                    }
                                    else if (ch_idx==0 || ch_idx==1 )
                                    {
                                    	ret =  FX_get_menu(p_FX[ch_idx],&tmp_node1);
                                    	STRAIGHT_RETURN(ret);
                                    	ret = MULTIFX_MENU_get_node_depth(tmp_node1,&tmp_depth);
                                        states= BROWSE_PARAMS;
                                    }
                                     else if (ch_idx==2)
                                     {
                                         thread_arg.enable_mainloop = DISABLE;

                                         goto end;
                                     }

                                break;

            case BROWSE_PARAMS :
								uiL = UI_init (tmp_node1,height,width,  0, 0,4,5,0,0,"CHANGE PARAMS CHANNEL",8,2);
                                UI_print_menu(uiL, 1);
                                UI_menu_selection(uiL, &selection);
                                if (selection<0)
                                {
                                	ret = MULTIFX_MENU_get_node_depth(tmp_node1,&tmp_depth2);

                                	if ( tmp_depth2== (tmp_depth) )
                                	{
										states= CHOOSE_CHANNEL;

                                	}
                                	else if ((tmp_depth2>(tmp_depth)))
                                	{
                                		tmp_node2=tmp_node1;
                                		ret =  MULTIFX_MENU_pop_to_parent(tmp_node2,&tmp_node1);
                                		STRAIGHT_RETURN(ret);
                                		states= BROWSE_PARAMS;
                                	}
                                	//UI_release(uiL);
                                }
                                else
                                {
                                	tmp_node2=tmp_node1;
									ret = MULTIFX_MENU_push_into_child(tmp_node2,&tmp_node1,selection);
									STRAIGHT_RETURN(ret);

                                	ret = MULTIFX_MENU_get_node_type(tmp_node1,&menu_type);
                                	STRAIGHT_RETURN(ret);

                                	//MULTIFX_MENU_get_type(tmp_node1,&menu_type);

                                	if (menu_type==sub_menu)
                                	{
                                	//	UI_release(uiL);
                                		states= BROWSE_PARAMS;
                                	}
                                	else if (menu_type==static_parameter)
                                	{
                                		FX_get_static_params(p_FX[ch_idx], &p_stat_prm,&len_prm);
                                		ret = MULTIFX_MENU_get_node_idx(tmp_node1,&tmp_idx);
                                		STRAIGHT_RETURN(ret);
                                		prm = p_stat_prm[tmp_idx];
                                		MULTIFX_MENU_get_node_name(tmp_node1,param_name);
                                		uiL3 = UI_init (tmp_node1,height,width,  0, 0,4,5,0,0,param_name,8,2);
                                		states= MODIFY_PARAM;
                                	}
                                	else if (menu_type==oscillator_parameter)
                                	{

                                		ret = MULTIFX_MENU_get_parent_idx(tmp_node1,&tmp_idx);
                                		STRAIGHT_RETURN(ret);
                                		ret= FX_get_timevarying_params (p_FX[ch_idx], &p_tv_params,tmp_idx);

                                		MULTIFX_MENU_get_node_name(tmp_node1,param_name);
                                		if (strcmp(osc_freq_name,param_name)==0)
                                		{
                                			prm = p_tv_params[osc_freq_idx];
                                			uiL3 = UI_init (tmp_node1,height,width,  0, 0,4,5,0,0,param_name,8,2);
                                		}
                                		else
                                		{
                                			uiL3 = UI_init (tmp_node1,height,width,  0, 0,4,5,0,0,"Unknown parameter",8,2);
                                		}

                                		states= MODIFY_PARAM;
                                	}
                                	else
                                	{

                                		fprintf(stderr,"\n menu case not encountered\n");
                                		return -1;
                                	}

                                }
                                UI_release(uiL);
                                break;

            case MODIFY_PARAM   :
								  UI_param_change(uiL3, &prm,.1, &upl);//esce solo in caso di p o m

                            if (upl==1)
                            {
                                UI_release(uiL3);
                                tmp_node2=tmp_node1;
                                ret =  MULTIFX_MENU_pop_to_parent(tmp_node2,&tmp_node1);
                                STRAIGHT_RETURN(ret);
                                states=BROWSE_PARAMS;
                            }
//                            else if (upl==2)
//                            {
//                            	UI_release(uiL3);
//                            	uiL4 = UI_init (0 ,"",height,width,  0, 0,4,5,0,0,oscillator_menu,8,2);
//                            	states=LEV3;
//                            }
                            else
                            {

									if (ch_idx==0)
									{
//#ifdef DEBUG
//										fprintf(dbg_file,"%f\n",prm);
//#endif
									   FX_set_new_param(thread_arg.p_left,prm,tmp_idx);
									   flag_update_left=1;
//#ifdef DEBUG
//										fprintf(dbg_file,"%u\n",flag_update_left);
//#endif

									}
									else if (ch_idx==1)
									{
#ifdef DEBUG
										fprintf(dbg_file,"%f\n",prm);
#endif
									   FX_set_new_param(thread_arg.p_right,prm,tmp_idx);
									   flag_update_right=1;
#ifdef DEBUG
										fprintf(dbg_file,"%u\n",flag_update_right);
#endif
									}


                            }
                            break;
            default     : break;


        }
    }




end :
UI_release(ui);
//UI_release(uiL);
//UI_release(uiR);
//UI_release(uiL3);
UI_ncurses_off();
fclose(dbg_file);
tret =pthread_join(threads, (void **)&texit);
if (tret==0)
{
    printf("pthread joined successuflly with exit value?????????? %d\n",texit);
}

   // FX_release(param_ex);

    if(p_FX[0]!=NULL)
    {
         FX_release(p_FX[0]);
    }

    if(p_FX[1]!=NULL)
    {
         FX_release(p_FX[1]);
    }
    close(fd_dev);

sem_destroy(&sem);

/***************MENU releases*********************/
MULTIFX_MENU_Release(choose_channel_root,choose_channel_head);
MULTIFX_MENU_Release(set_effect_root,set_effect_head);
//MENU_Release(sin_test_option);
//MENU_Release(oscillator_options);
printf("closing\n");
    return 0;
}
