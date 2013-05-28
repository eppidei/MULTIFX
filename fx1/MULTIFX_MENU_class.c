#include <MULTIFX_MENU_class.h>
#include <MULTIFX_defines.h>
#include <MULTIFX_errors.h>
#include <MULTIFX_typedef.h>
#include <string.h>
#include <stdlib.h>

struct MULTIFX_MENU_HEAD_S
{
    MULTIFX_UINT32_T 	id_assignment_counter;
    MULTIFX_UINT32_T 	node_counter;
    MULTIFX_MENU_NODE_T *p_root;
};

struct MULTIFX_MENU_NODE_S
{
	MULTIFX_UINT32_T 	node_id;
	MULTIFX_CHAR_T  	*node_name;
	enum menu_item_t 		node_type;
	MULTIFX_UINT32_T 	node_depth;
	MULTIFX_MENU_NODE_T	* p_parent;
    MULTIFX_UINT32_T 	n_children;
    MULTIFX_UINT32_T 	max_children;
    MULTIFX_MENU_NODE_T	** p_p_children;

};

static MULTIFX_API_RET MULTIFX_MENU_node_init(MULTIFX_MENU_NODE_T **node,MULTIFX_UINT32_T max_children,MULTIFX_CHAR_T  *node_name,enum menu_item_t node_typ)
{

	*node=calloc(1,sizeof(MULTIFX_MENU_NODE_T));
	 if (*node!=NULL)
	 {
			 (*node)->node_id=0;
			(*node)->node_depth=1;
			(*node)->p_parent=NULL;
			(*node)->n_children=0;
			(*node)->max_children=max_children;
			(*node)->node_name = calloc(MAX_CHAR_LEN,sizeof(MULTIFX_CHAR_T));
			if ((*node)->node_name!=NULL)
			{
				strncpy((char*)((*node)->node_name),node_name,MAX_CHAR_LEN*sizeof(MULTIFX_CHAR_T));

				(*node)->node_type = node_typ;


				(*node)->p_p_children=calloc(max_children,sizeof(MULTIFX_MENU_NODE_T*));
				if ( (*node)->p_p_children!=NULL)
				{
					(*node)->p_p_children=NULL;
				}
				else
				{
					fprintf(stderr,"\n pp_childern allocation error\n");
					return MULTIFX_MENU_PPCHILD_ALLOC_ERROR;
				}
			}
			else
			{
				fprintf (stderr,"\n node name allocation error\n");
				return MULTIFX_MENU_NODENAME_ERROR;
			}
	 }
	 else
	{
		fprintf(stderr,"\n node allocation error\n");
		return MULTIFX_MENU_ROOT_ALLOC_ERROR;
	}

	return MULTIFX_DEFAULT_RET;

}

 MULTIFX_API_RET MULTIFX_MENU_init(MULTIFX_MENU_NODE_T **root,MULTIFX_MENU_HEAD_T **head,MULTIFX_UINT32_T max_children,MULTIFX_CHAR_T  *root_name,enum menu_item_t node_typ)
{
    *head=calloc(1,sizeof(MULTIFX_MENU_HEAD_T));
    //MULTIFX_UINT32_T tmp_len;
    MULTIFX_API_RET ret = 0;

    if(*head!=NULL)
    {
        ret = MULTIFX_MENU_node_init(root,max_children,root_name,node_typ);
        if (ret<0)
        {
        	fprintf(stderr,"\n node init error\n");
        	return MULTIFX_MENU_NODE_INIT_ERROR;
        }


        (*head)->id_assignment_counter=0;
	   (*head)->node_counter=1;
	   (*head)->p_root=*root;

    }
    else
    {
        fprintf(stderr,"\n head allocation error\n");
        return MULTIFX_MENU_HEAD_ALLOC_ERROR;
    }



    return MULTIFX_DEFAULT_RET;
}

 MULTIFX_API_RET MULTIFX_MENU_del_all_children_recursive(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_HEAD_T *head)
{
	MULTIFX_UINT32_T i = 0;
	MULTIFX_UINT32_T n_children_start = node->max_children;

    if (node->p_p_children!=NULL)
    {
        for (i=0;i<n_children_start;i++)
        {
            if (node->p_p_children[i]!=NULL)
            {
            	MULTIFX_MENU_del_all_children_recursive(node->p_p_children[i],head);
            	CHECKNFREE((node->p_p_children[i])->node_name);
            	CHECKNFREE(node->p_p_children[i]);
                 head->node_counter=head->node_counter-1;
                 node->n_children--;

            }

        }

    }

   node->p_p_children=NULL;


    return MULTIFX_DEFAULT_RET;
}

 MULTIFX_API_RET MULTIFX_MENU_Release(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_HEAD_T *head)
 {

	 MULTIFX_API_RET ret = 0;
	 if(node!=NULL)
	 {
		 ret =  MULTIFX_MENU_delete_branch(node,head);
	 }

	 CHECKNFREE(head);
	 return ret;
 }

 MULTIFX_API_RET MULTIFX_MENU_delete_branch(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_HEAD_T *head)
{
    MULTIFX_UINT32_T i = 0;
    MULTIFX_MENU_NODE_T *par = (node->p_parent);

    MULTIFX_MENU_del_all_children_recursive(node,head);
    CHECKNFREE(node->p_p_children);

    if (par!=NULL)
    {

        for (i=0;i<par->max_children;i++)
        {
            if (par->p_p_children[i]==node)
            {
                par->p_p_children[i]=NULL;
                par->n_children = par->n_children - 1;
                break;
            }
            if (i==par->max_children-1)
            {
                fprintf(stderr,"error child unknown\n");
                return MULTIFX_MENU_CHILD_UNKNOWN;
            }
        }
    }

    if (node->node_id!=0) //se non è root che viene allocato nel main
    {
    	CHECKNFREE(node->node_name);
    	CHECKNFREE(node);
    }

    head->node_counter--;
    return MULTIFX_DEFAULT_RET;
}



 MULTIFX_API_RET MULTIFX_MENU_get_n_children(MULTIFX_MENU_NODE_T *parent,MULTIFX_UINT32_T *n_children)
 {
	 *n_children=parent->n_children;

	 return MULTIFX_DEFAULT_RET;
 }

 MULTIFX_API_RET MULTIFX_MENU_get_max_children(MULTIFX_MENU_NODE_T *parent,MULTIFX_UINT32_T *max_children)
  {
 	 *max_children=parent->max_children;

 	 return MULTIFX_DEFAULT_RET;
  }

 MULTIFX_API_RET MULTIFX_MENU_get_children_by_name(MULTIFX_MENU_NODE_T *parent,MULTIFX_MENU_NODE_T **child,MULTIFX_CHAR_T *name)
   {
  	 MULTIFX_UINT32_T i = 0;

  	 for (i=0;i<parent->max_children;i++)
  	 {
  		 if (parent->p_p_children[i]!=NULL)
  		 {
  			 if (strcmp ( (parent->p_p_children[i])->node_name,name) == 0)
			{
				*child = parent->p_p_children[i];
			}
  		 }
  	 }

  	 return MULTIFX_DEFAULT_RET;
   }

 MULTIFX_API_RET MULTIFX_MENU_get_root(MULTIFX_MENU_NODE_T *child,MULTIFX_MENU_NODE_T** root)
 {
	 MULTIFX_MENU_NODE_T *tmp_node = child->p_parent;

	 while (tmp_node->p_parent!=NULL)
	 {
		 tmp_node=tmp_node->p_parent;
	 }

	 *root = tmp_node;

	 return MULTIFX_DEFAULT_RET;
 }

 MULTIFX_API_RET MULTIFX_MENU_get_child_name(MULTIFX_MENU_NODE_T *parent,MULTIFX_UINT32_T idx,MULTIFX_CHAR_T *name)
 {
	 if (parent->p_p_children!=NULL)
	 {

		 if (parent->p_p_children[idx]!=NULL)
		 {
			 strncpy(name,(parent->p_p_children[idx])->node_name,MAX_CHAR_LEN*sizeof(MULTIFX_CHAR_T));
		 }
		 else
		 {
			 return MULTIFX_MENU_CHILD_UNKNOWN;
		 }
	 }
	 else
	 {
		 //fprintf(stderr,"\n p_p_children not allocated\n");
		 return MULTIFX_MENU_PPCHILD_ALLOC_ERROR;
	 }

	 return MULTIFX_DEFAULT_RET;
 }
 MULTIFX_API_RET MULTIFX_MENU_get_child_type(MULTIFX_MENU_NODE_T *parent,MULTIFX_UINT32_T idx,enum menu_item_t *type)
  {
 	 if (parent->p_p_children!=NULL)
 	 {

 		 if (parent->p_p_children[idx]!=NULL)
 		 {
 			 *type=parent->p_p_children[idx]->node_type;
 		 }
 		 else
 		 {
 			 return MULTIFX_MENU_CHILD_UNKNOWN;
 		 }
 	 }
 	 else
 	 {
 		 fprintf(stderr,"\n p_p_children not allocated\n");
 		 return MULTIFX_MENU_PPCHILD_ALLOC_ERROR;
 	 }

 	 return MULTIFX_DEFAULT_RET;
  }

 MULTIFX_API_RET MULTIFX_MENU_create_child_idx (MULTIFX_MENU_NODE_T **child, MULTIFX_MENU_NODE_T *parent, MULTIFX_MENU_HEAD_T *head,
		 MULTIFX_UINT32_T idx,MULTIFX_CHAR_T  *child_name,enum menu_item_t node_typ)
{
	MULTIFX_UINT32_T i=0;

    if ((parent)->p_p_children==NULL && parent->n_children==0)
    {
        (parent)->p_p_children=calloc(parent->max_children,sizeof(MULTIFX_MENU_NODE_T*));

        for (i=0;i<parent->max_children;i++)
        {
             (parent)->p_p_children[i]=NULL;

        }

    }

    i=idx;
    if ((parent)->p_p_children[i]==NULL && i<parent->max_children)
    {
        *child = calloc(1,sizeof(MULTIFX_MENU_NODE_T));

        if (*child!=NULL)
        {

         (parent)->p_p_children[i] = *child;
         parent->n_children = parent->n_children +1;
         (*child)->node_id = head->id_assignment_counter+1;
         (*child)->node_depth = parent->node_depth +1;
         (*child)->p_parent = parent;
         (*child)->max_children = parent->max_children;
         (*child)->node_type = node_typ;
         (*child)->node_name = calloc(MAX_CHAR_LEN,sizeof(MULTIFX_CHAR_T));
			 if((*child)->node_name!=NULL)
			 {
				 strncpy((char*)((*child)->node_name),child_name,MAX_CHAR_LEN*sizeof(MULTIFX_CHAR_T));

				head->id_assignment_counter++;
				head->node_counter++;
			 }
			 else
			 {
						fprintf (stderr,"\n node name allocation error\n");
						return MULTIFX_MENU_NODENAME_ERROR;
			}
        }
        else
        {
        	fprintf(stderr,"\n pp_childern allocation error\n");
        	return MULTIFX_MENU_PPCHILD_ALLOC_ERROR;
        }
    }
    else if (i>=parent->max_children)
    {
        fprintf(stderr,"\n max children exceeded\n");
        return MULTIFX_MENU_MAX_CHILD_EXCEEDED;;
    }
    else if ((parent)->p_p_children[i]!=NULL)
    {
        fprintf(stderr,"\n child already born or\n");
        return MULTIFX_MENU_MAX_CHILD_ALREADY_BORN;
    }



    return MULTIFX_DEFAULT_RET;
}

 MULTIFX_API_RET MULTIFX_MENU_plug_root_into_branch(MULTIFX_MENU_NODE_T *old_root,MULTIFX_MENU_HEAD_T *old_head, MULTIFX_MENU_NODE_T *branch, MULTIFX_MENU_HEAD_T* head)
 {
     MULTIFX_UINT32_T node_idx = 0;
     MULTIFX_INT32_T ret = 0;
     MULTIFX_MENU_NODE_T *parent = NULL;
     MULTIFX_UINT32_T  branch_depth = branch->node_depth;

     if (old_head==NULL)
     {
         fprintf(stderr,"\n head non existent\n");
         return MULTIFX_MENU_HEAD_NON_EXISTENT_ERROR;
     }
     else
     {
          ret=MULTIFX_MENU_get_node_idx (branch,&node_idx);
          STRAIGHT_RETURN(ret);

         if (node_idx<0)
         {
             fprintf(stderr,"\n child idx not found\n");
             return MULTIFX_MENU_CHILD_IDX_NOT_FOUND_ERROR;
         }
         parent = branch ->p_parent;
         ret = MULTIFX_MENU_delete_branch(branch,head);

         if (ret<0)
         {
             fprintf(stderr,"\n error in delete branch\n");
             return MULTIFX_MENU_DELETE_BRANCH_ERROR;
         }
         parent->p_p_children[node_idx]=old_root;
         (parent->p_p_children[node_idx])->p_parent=parent;
         parent->n_children += 1;
         head->node_counter += old_head->node_counter;
         if (parent->n_children>parent->max_children)
         {
             fprintf(stderr,"\n parent max children exceeded\n");
             return MULTIFX_MENU_PAR_MAX_CHILDREN_ERROR;
         }
         free(old_head);

         ret= MULTIFX_MENU_scan_all_children_recursive_set_newid(parent->p_p_children[node_idx], head);
         (parent->p_p_children[node_idx])->node_id = head->id_assignment_counter+1;
         head->id_assignment_counter++;

         (parent->p_p_children[node_idx])->node_depth +=branch_depth-1;

          ret= MULTIFX_MENU_scan_all_children_recursive_add_depth(parent->p_p_children[node_idx],branch_depth-1);
          if (ret<0)
         {
             fprintf(stderr,"\n scan_all_children_recursive_add_depth\n");
             return MULTIFX_MENU_ADD_DEPTH_ERROR;
         }
     }

     return MULTIFX_DEFAULT_RET;


 }

 MULTIFX_API_RET MULTIFX_MENU_unplug_branch_into_root(MULTIFX_MENU_NODE_T* branch, MULTIFX_MENU_HEAD_T* head,MULTIFX_MENU_HEAD_T *new_head)
 {
     MULTIFX_UINT32_T idx = 0,n_children = 0;
     MULTIFX_UINT32_T tmp_value = 0;
     MULTIFX_MENU_NODE_T* p_parent_node = branch -> p_parent;
     MULTIFX_INT32_T ret = 0;

     ret=MULTIFX_MENU_get_node_idx(branch,&idx);
     STRAIGHT_RETURN(ret);
     if (idx<0)
     {
         fprintf(stderr,"\n child idx not found\n");
         return MULTIFX_MENU_CHILD_IDX_NOT_FOUND_ERROR;
     }
     if (p_parent_node->p_p_children[idx]==NULL)
     {
         fprintf(stderr,"\n child not registered\n");
         return MULTIFX_MENU_CHILD_NOT_REGISTERED_ERROR;
     }
     MULTIFX_MENU_scan_all_children_recursive_getnumber(branch,&n_children);
     tmp_value = head->node_counter;
     head->node_counter-=n_children;
     if (tmp_value < head->node_counter)
     {
         fprintf(stderr,"\n node_counter cannot be negative (unsigned)\n");
         return MULTIFX_MENU_NEGATIVE_NODE_COUNT_ERROR;
     }
     p_parent_node->p_p_children[idx]=NULL;
     p_parent_node->n_children--;

     new_head->id_assignment_counter=head->id_assignment_counter;
     new_head->node_counter = n_children;
     new_head->p_root=branch;
     branch->p_parent = NULL;
     tmp_value = branch->node_depth;
     branch->node_depth=1;

     MULTIFX_MENU_scan_all_children_recursive_add_depth(branch, (-tmp_value+1));

     return MULTIFX_DEFAULT_RET;


 }

 MULTIFX_API_RET MULTIFX_MENU_scan_all_children_recursive_getnumber(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T *n_children)
 {
	 MULTIFX_UINT32_T i= 0;

     if (node->p_p_children!=NULL)
     {

         for (i=0;i<node->max_children;i++)
         {
              if (node->p_p_children[i]!=NULL)
              {
            	  MULTIFX_MENU_scan_all_children_recursive_getnumber(node->p_p_children[i],n_children);
                  //fprintf(stdout,"node id %u abs node_depth %u n_children %u parent_node id %u child index %u\n",(node->p_p_children[i])->node_id,(node->p_p_children[i])->node_depth,(node->p_p_children[i])->n_children,node->node_id,i);
                  (*n_children)++;
              }
         }
     }
     return 0;
 }

 MULTIFX_API_RET MULTIFX_MENU_scan_all_children_recursive_set_newid(MULTIFX_MENU_NODE_T *node, MULTIFX_MENU_HEAD_T *head)
 {
	 MULTIFX_UINT32_T i= 0;


     if (node->p_p_children!=NULL)
     {

         for (i=0;i<node->max_children;i++)
         {
              if (node->p_p_children[i]!=NULL)
              {
            	  MULTIFX_MENU_scan_all_children_recursive_set_newid(node->p_p_children[i],head);
                  (node->p_p_children[i])->node_id = head->id_assignment_counter + 1;
                  head->id_assignment_counter++;
                  //fprintf(stdout,"node id %u abs node_depth %u n_children %u parent_node id %u child index %u\n",(node->p_p_children[i])->node_id,(node->p_p_children[i])->node_depth,(node->p_p_children[i])->n_children,node->node_id,i);
              }
         }
     }
     return MULTIFX_DEFAULT_RET;
 }

 MULTIFX_API_RET MULTIFX_MENU_scan_all_children_recursive_add_depth(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T depth)
 {
	 MULTIFX_UINT32_T i= 0;


     if (node->p_p_children!=NULL)
     {

         for (i=0;i<node->max_children;i++)
         {
              if (node->p_p_children[i]!=NULL)
              {
            	  MULTIFX_MENU_scan_all_children_recursive_add_depth(node->p_p_children[i],depth);
                  (node->p_p_children[i])->node_depth += depth;
                  //fprintf(stdout,"node id %u abs node_depth %u n_children %u parent_node id %u child index %u\n",(node->p_p_children[i])->node_id,(node->p_p_children[i])->node_depth,(node->p_p_children[i])->n_children,node->node_id,i);
              }
         }
     }
     return MULTIFX_DEFAULT_RET;
 }

 MULTIFX_API_RET MULTIFX_MENU_scan_all_children_recursive(MULTIFX_MENU_NODE_T *node)
 {
     MULTIFX_UINT32_T i= 0;

     if (node->p_p_children!=NULL)
     {

         for (i=0;i<node->max_children;i++)
         {
              if (node->p_p_children[i]!=NULL)
              {
            	  MULTIFX_MENU_scan_all_children_recursive(node->p_p_children[i]);
                 // fprintf(stdout,"node id %u abs node_depth %u n_children %u parent_node id %u child index %u\n",(node->p_p_children[i])->node_id,(node->p_p_children[i])->node_depth,(node->p_p_children[i])->n_children,node->node_id,i);
              }
         }
     }
     return MULTIFX_DEFAULT_RET;
 }

 MULTIFX_API_RET MULTIFX_MENU_push_into_child(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_NODE_T **child,MULTIFX_UINT32_T child_idx)
 {
	 if (node->p_p_children!=NULL)
	 {
		 if (node->p_p_children[child_idx]!=NULL)
		 {
			 *child=node->p_p_children[child_idx];
			 return MULTIFX_DEFAULT_RET;
		 }
		 else
		 {
			 fprintf(stderr,"\n child not avalilable\n");
			 return MULTIFX_MENU_CHILD_IDX_NOT_FOUND_ERROR;
		 }


	 }
	 return MULTIFX_DEFAULT_RET;
 }
 MULTIFX_API_RET MULTIFX_MENU_pop_to_parent(MULTIFX_MENU_NODE_T *node,MULTIFX_MENU_NODE_T **parent)
  {
 	 if (node->p_parent!=NULL)
 	 {
 		 *parent=node->p_parent;
 		 return MULTIFX_DEFAULT_RET;
 	 }
 	 else
 	 {
 		 fprintf(stderr,"\n parent unknown maybe root\n");
 		 return MULTIFX_MENU_PARENT_UNKNOWN_ERROR;
 	 }
  }

 MULTIFX_API_RET MULTIFX_MENU_get_node_type(MULTIFX_MENU_NODE_T *node,enum menu_item_t *type)
 {
	 if (node!=NULL)
	 {
		 *type = node->node_type;
		 return MULTIFX_DEFAULT_RET;
	 }
	 else
	 {
		 fprintf(stderr,"\n node not allocated error\n");
		 return MULTIFX_MENU_NODE_NOT_ALLOCATED_ERROR;
	 }
 }

 MULTIFX_API_RET MULTIFX_MENU_get_node_name(MULTIFX_MENU_NODE_T *node,MULTIFX_CHAR_T * name)
  {

	 if (node!=NULL)
	 {
		 if (node->node_name!=NULL)
		 {
			 strncpy(name,node->node_name,MAX_CHAR_LEN*sizeof(MULTIFX_CHAR_T));
			 return MULTIFX_DEFAULT_RET;
		 }
		 else
		 {
			 fprintf(stderr,"\n MULTIFX_MENU_NODENAME_ERROR \n");
			 return MULTIFX_MENU_NODENAME_ERROR;
		 }
	 }
	 else
	 {
		 fprintf(stderr,"\n MULTIFX_MENU_NODE_INIT_ERROR \n");
		 return MULTIFX_MENU_NODE_INIT_ERROR;
	 }


  }

 MULTIFX_API_RET MULTIFX_MENU_get_node_idx(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T *child_idx)
 {
	 MULTIFX_UINT32_T i = 0;
	 MULTIFX_MENU_NODE_T *parent = node->p_parent;

	 for (i=0;i<parent->max_children;i++)
	 {
		 if ( node==parent->p_p_children[i] )
		 {
			 *child_idx=i;
			 break;
		 }
		 if (i==(parent->max_children)-1)
		 {
			 fprintf(stderr,"\n child not found\n");
			 return  MULTIFX_MENU_CHILD_UNKNOWN;
		 }
	 }

	 return MULTIFX_DEFAULT_RET;


 }

 MULTIFX_API_RET MULTIFX_MENU_get_parent_idx(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T *child_idx)
 {
	 MULTIFX_MENU_NODE_T *parent = NULL;
	 //MULTIFX_INT32_T ret = 0;

	 if (node!=NULL)
	 {
		 if (node->p_parent!=NULL)
		 {
			 parent = node->p_parent;

			 MULTIFX_MENU_get_node_idx(parent,child_idx);


		 }
	 }

	 return MULTIFX_DEFAULT_RET;
 }


 MULTIFX_API_RET MULTIFX_MENU_get_node_depth(MULTIFX_MENU_NODE_T *node,MULTIFX_UINT32_T * depth)
   {

 	 if (node!=NULL)
 	 {
 		 *depth = node->node_depth;
 	 }
 	 else
 	 {
 		 fprintf(stderr,"\n MULTIFX_MENU_NODE_INIT_ERROR \n");
 		 return MULTIFX_MENU_NODE_INIT_ERROR;
 	 }

 	return MULTIFX_DEFAULT_RET;

   }
