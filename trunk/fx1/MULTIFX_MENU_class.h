#ifndef _MULTIFX_MENU_CLASS_H
#define _MULTIFX_MENU_CLASS_H

#include <MULTIFX_typedef.h>

//static MULTIFX_API_RET MULTIFX_MENU_node_init(MULTIFX_MENU_NODE_T **node,MULTIFX_UINT32_T max_children,MULTIFX_CHAR_T  *node_name,enum menu_item_t node_typ);

 MULTIFX_API_RET MULTIFX_MENU_init(MULTIFX_MENU_NODE_T **root,MULTIFX_MENU_HEAD_T **head,MULTIFX_UINT32_T max_children,MULTIFX_CHAR_T  *root_name,enum menu_item_t node_typ);

 MULTIFX_API_RET MULTIFX_MENU_del_all_children_recursive(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_HEAD_T *head);

 MULTIFX_API_RET MULTIFX_MENU_Release(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_HEAD_T *head);

 MULTIFX_API_RET MULTIFX_MENU_delete_branch(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_HEAD_T *head);

 MULTIFX_API_RET MULTIFX_MENU_get_n_children(MULTIFX_MENU_NODE_T *parent,MULTIFX_UINT32_T *n_children);

 MULTIFX_API_RET MULTIFX_MENU_get_max_children(MULTIFX_MENU_NODE_T *parent,MULTIFX_UINT32_T *max_children);

 MULTIFX_API_RET MULTIFX_MENU_get_children_by_name(MULTIFX_MENU_NODE_T *parent,MULTIFX_MENU_NODE_T **child,MULTIFX_CHAR_T *name);

 MULTIFX_API_RET MULTIFX_MENU_get_root(MULTIFX_MENU_NODE_T *child,MULTIFX_MENU_NODE_T** root);

 MULTIFX_API_RET MULTIFX_MENU_get_child_name(MULTIFX_MENU_NODE_T *parent,MULTIFX_UINT32_T idx,MULTIFX_CHAR_T *name);

 MULTIFX_API_RET MULTIFX_MENU_get_child_type(MULTIFX_MENU_NODE_T *parent,MULTIFX_UINT32_T idx,enum menu_item_t *type);

 MULTIFX_API_RET MULTIFX_MENU_create_child_idx (MULTIFX_MENU_NODE_T **child, MULTIFX_MENU_NODE_T *parent, MULTIFX_MENU_HEAD_T *head,
		 MULTIFX_UINT32_T idx,MULTIFX_CHAR_T  *child_name,enum menu_item_t node_typ);

 MULTIFX_API_RET MULTIFX_MENU_plug_root_into_branch(MULTIFX_MENU_NODE_T *old_root,MULTIFX_MENU_HEAD_T *old_head, MULTIFX_MENU_NODE_T *branch, MULTIFX_MENU_HEAD_T* head);

 MULTIFX_API_RET MULTIFX_MENU_unplug_branch_into_root(MULTIFX_MENU_NODE_T* branch, MULTIFX_MENU_HEAD_T* head,MULTIFX_MENU_HEAD_T *new_head);

 MULTIFX_API_RET MULTIFX_MENU_scan_all_children_recursive_getnumber(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T *n_children);

 MULTIFX_API_RET MULTIFX_MENU_scan_all_children_recursive_set_newid(MULTIFX_MENU_NODE_T *node, MULTIFX_MENU_HEAD_T *head);

 MULTIFX_API_RET MULTIFX_MENU_scan_all_children_recursive_add_depth(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T depth);

 MULTIFX_API_RET MULTIFX_MENU_scan_all_children_recursive(MULTIFX_MENU_NODE_T *node);

 MULTIFX_API_RET MULTIFX_MENU_push_into_child(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_NODE_T **child,MULTIFX_UINT32_T child_idx);

 MULTIFX_API_RET MULTIFX_MENU_pop_to_parent(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_NODE_T **parent);

 MULTIFX_API_RET MULTIFX_MENU_get_node_type(MULTIFX_MENU_NODE_T *node,enum menu_item_t *type);

 MULTIFX_API_RET MULTIFX_MENU_get_node_name(MULTIFX_MENU_NODE_T *node,MULTIFX_CHAR_T * name);

 MULTIFX_API_RET MULTIFX_MENU_get_node_idx(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T *child_idx);

 MULTIFX_API_RET MULTIFX_MENU_get_parent_idx(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T *child_idx);

 MULTIFX_API_RET MULTIFX_MENU_get_node_depth(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T * depth);

#endif
