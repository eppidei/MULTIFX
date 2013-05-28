#include <ncurses_UI_class.h>
#include <MULTIFX_MENU_class.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <MULTIFX_errors.h>




struct ncurses_UI_S {
    WINDOW *menu_win;
    char   *menu_title;
    int    title_x;
    int    title_y;
    int     n_items ;
    char   *menu_items;
    int     w_height;
    int     w_width  ;
    int     w_x  ;
    int     w_y;
    int     menu_x;
    int     menu_y;
    int     print_state_x;
    int     print_state_y;
};

int UI_ncurses_on()
{
     initscr();
    clear();
    noecho();
    cbreak();

    return 0;
}
int UI_ncurses_off()
{
     endwin();

    return 0;
}

ncurses_UI_T* UI_init (MULTIFX_MENU_NODE_T * menu_parent, int w_h, int w_w, int w_x, int w_y,int m_x, int m_y, int p_x, int p_y, char * title, int t_x, int t_y)
{

    MULTIFX_API_RET ret = 0;
    MULTIFX_UINT32_T n_items,max_items;
    static MULTIFX_CHAR_T name[MAX_CHAR_LEN];
    MULTIFX_UINT16_T name_id = 0, prm_type = 0;
  //  char *tmp=NULL;
    unsigned int i  = 0;
    int curses_ret = 0;

    ncurses_UI_T* pthis = calloc(1,sizeof(ncurses_UI_T));

    if (pthis!=NULL)
    {


         pthis->title_x    = t_x;
         pthis->title_y    = t_y;
         pthis->w_height    = w_h;
         pthis->w_width     = w_w;
         pthis->w_x         = w_x;
         pthis->w_y         = w_y;
         pthis->menu_x      = m_x;
         pthis->menu_y      = m_y;
         pthis->print_state_x = p_x;
         pthis->print_state_y = p_y;

        pthis->menu_win = newwin( pthis->w_height, pthis->w_width, pthis->w_x, pthis->w_y);

       if (pthis->menu_win!=NULL)
       {

			MULTIFX_MENU_get_max_children(menu_parent,&max_items);

			 ret =MULTIFX_MENU_get_n_children(menu_parent,&n_items);

			  pthis->n_items = n_items;


			pthis->menu_items = calloc(n_items*MAX_CHAR_LEN,sizeof(char));

			if (pthis->menu_items!=NULL)
			{




				for (i=0;i<max_items;i++)
				{
					 ret = MULTIFX_MENU_get_child_name(menu_parent,i,name);
					if (ret==0)
					{
						strncpy(&(pthis->menu_items[i*MAX_CHAR_LEN]),name,MAX_CHAR_LEN);
					}
				}

				curses_ret=keypad(pthis->menu_win, TRUE);

				if (curses_ret!=0)
				{
					fprintf(stderr,"\n keypad error %d\n",curses_ret);
					return NULL;
				}

				pthis->menu_title= calloc(MAX_CHAR_LEN,sizeof(char));

					if (pthis->menu_title!= NULL)
					{
						strncpy(pthis->menu_title,title,MAX_CHAR_LEN);
					}
					else
					{
						fprintf(stderr,"\n menu title allocation error\n");
						return NULL;
					}
		   }

			else
		   {
			fprintf(stderr,"\n menu items allocation error\n");
			return NULL;
		   }
       }
       else
       {
    	   fprintf(stderr,"\n new win allocation error\n");
    	   return NULL;
       }
    }

    return pthis;
}

int UI_release(ncurses_UI_T* p_ui)
{
    CHECKNFREE(p_ui->menu_items);
    CHECKNFREE(p_ui->menu_title);
    if (p_ui->menu_win!=NULL)
    {
    	delwin(p_ui->menu_win);
    }
    CHECKNFREE(p_ui);

    return 0;
}


int  UI_print_menu(ncurses_UI_T* p_ui, int highlight_idx)
{
	int i;
	int x,y;

	box(p_ui->menu_win, 0, 0);
	x = p_ui->title_x;
	y = p_ui->title_y;
	mvwprintw(p_ui->menu_win,y, x, "%s", p_ui->menu_title);
	if (p_ui->n_items >0)
	{
	x = p_ui->menu_x;
	y = p_ui->menu_y;
	for(i = 0; i < p_ui->n_items; ++i)
	{
	    if(highlight_idx == i + 1) /* High light the present choice */
		{	wattron(p_ui->menu_win, A_REVERSE);
			mvwprintw(p_ui->menu_win, y, x, "%s", &(p_ui->menu_items[i*MAX_CHAR_LEN]));
			wattroff(p_ui->menu_win, A_REVERSE);
		}
		else
		{
		    mvwprintw(p_ui->menu_win, y, x, "%s",&(p_ui->menu_items[i*MAX_CHAR_LEN]));
		}
		++y;
	}
	}
	wrefresh(p_ui->menu_win);

	 p_ui->print_state_x = x;
	  p_ui->print_state_y = y;

	return 0;
}


int  UI_menu_selection(ncurses_UI_T* p_ui, int *chosen_idx)
{
    int highlight = 1;
    int new_printy = 0;
    int c;
    while(1)
	{
	    c = wgetch(p_ui->menu_win);
	    *chosen_idx = 0;
		switch(c)
		{
		    	case KEY_UP:
				if(highlight == 1)
					highlight = p_ui->n_items;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == p_ui->n_items)
					highlight = 1;
				else
					++highlight;
				break;
			case 10:
				*chosen_idx = highlight-1;
				new_printy = p_ui->print_state_y +2;
				//mvwprintw(p_ui->menu_win,new_printy,1,"Chosen index %d\n",*chosen_idx);
				p_ui->print_state_y = new_printy;
				wrefresh(p_ui->menu_win);
//				sleep(1);
				return 0;
            case (117):
                *chosen_idx = -1;
                wrefresh(p_ui->menu_win);
                return 0;
			default:
                new_printy = p_ui->print_state_y +2;
				mvwprintw(p_ui->menu_win,new_printy,1,"Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
				p_ui->print_state_y = new_printy;
				wrefresh(p_ui->menu_win);
				break;
		}
		UI_print_menu(p_ui, highlight);

	}


	return -1;
}

int UI_param_change(ncurses_UI_T* p_ui, float *param, float delta, int *up_level)
{
    int c;
    int new_printy = p_ui->print_state_y ;
    enum { plus = 112, minus = 109, out = 117};//, sub_menu=115};//p,m,u,s


    mvwprintw(p_ui->menu_win,new_printy,1,p_ui->menu_title,*param);
    wrefresh(p_ui->menu_win);
    p_ui->print_state_y = new_printy;
    while (1)
    {
        c = wgetch(p_ui->menu_win);
        *up_level = 0;
        switch(c)
        {
            case plus : *param += delta;
                        new_printy =  p_ui->print_state_y +3;
                        mvwprintw(p_ui->menu_win,new_printy,1,"%f",*param);
                        wrefresh(p_ui->menu_win);
                        return 0;

            case minus :  *param -= delta;
                        new_printy =  p_ui->print_state_y +3;
                        mvwprintw(p_ui->menu_win,new_printy,1,"%f",*param);
                        wrefresh(p_ui->menu_win);
                        return 0;
            case out :  *up_level = 1;
                        return 0;
//            case sub_menu : *up_level = 2;
//							return 0;
            default  : new_printy =  p_ui->print_state_y +1;
                        mvwprintw(p_ui->menu_win,new_printy,1,"PRESS %c or %c", plus, minus);
                        wrefresh(p_ui->menu_win);
                        break;

        }
    }
   // p_ui->print_state_y=new_printy;
    return 0;
}
