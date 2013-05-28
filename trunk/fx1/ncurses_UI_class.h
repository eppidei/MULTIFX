#ifndef _NCURSES_UI_CLASS_H
#define _NCURSES_UI_CLASS_H

#include <MULTIFX_typedef.h>

int UI_ncurses_on();
int UI_ncurses_off();

ncurses_UI_T* UI_init (MULTIFX_MENU_NODE_T * items_menu,  int w_h, int w_w, int w_x, int w_y,int m_x, int m_y, int p_x, int p_y, char * title, int t_x, int t_y);
int  UI_print_menu(ncurses_UI_T* p_ui, int highlight_idx);
int UI_release(ncurses_UI_T* p_ui);
int  UI_menu_selection(ncurses_UI_T* p_ui, int *chosen_idx);
int UI_param_change(ncurses_UI_T* p_ui, float *param, float delta, int *up_level);

#endif
