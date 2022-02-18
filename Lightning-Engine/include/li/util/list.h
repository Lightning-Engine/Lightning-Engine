#ifndef LI_LIST_H
#define LI_LIST_H

typedef struct li_list {
    struct li_list *next;
    void *data;
} li_list_t;

li_list_t *li_lstnew(void *data);
/*Deletes only one, does not clear the rest of the list*/
void li_lstdel(li_list_t *lst);
/*Calls li_lstdel on this node and all the following nodes*/
void li_lstclr(li_list_t *lst);
void li_lstadd_back(li_list_t **lst, li_list_t *ele);
void li_lstadd_frnt(li_list_t **lst, li_list_t *ele);
void li_lstrem(li_list_t **lst, int (*prc)(void*,void*), void *param);
int li_ptreq(void *a, void *b);
void li_lstfor(const li_list_t *lst, void (*proc)(void*,void*), void *param);

#endif
