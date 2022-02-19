#include "li/util/list.h"

#include "li/memory.h"
#include <stdlib.h>

li_list_t *li_lstnew(void *data) {
	li_list_t *list;

	list = li_safe_malloc(sizeof(*list));
	list->next = NULL;
	list->data = data;
	return (list);
}

void li_lstdel(li_list_t *lst, void (*del)(void*)) {
	if (del)
		del(lst->data);
	free(lst);
}

void li_lstclr(li_list_t *lst, void (*del)(void*)) {
	li_list_t *tmp;

	tmp = lst;
	while (lst) {
		tmp = lst->next;
		li_lstdel(lst, del);
		lst = tmp;
	}
}

void li_lstadd_back(li_list_t **lst, li_list_t *ele) {
	if (!*lst)
		*lst = ele;
	else {
		while (*lst) {
			if (!(*lst)->next)
				break;
			*lst = (*lst)->next;
		}
		(*lst)->next = ele;
	}
}

void li_lstadd_frnt(li_list_t **lst, li_list_t *ele) {
	ele->next = *lst;
	*lst = ele;
}

void li_lstrem(li_list_t **lst, int (*check)(void*,void*), void (*del)(void*), void *param) {
	li_list_t *tmp;

	tmp = *lst;
	while (*lst && check((*lst)->data, param)) {
		tmp = (*lst)->next;
		li_lstdel(*lst, del);
		*lst = tmp;
	}
	while (*lst) {
		if (check((*lst)->data, param)) {
			tmp->next = (*lst)->next;
			li_lstdel(*lst, del);
			*lst = tmp->next;
		} else {
			tmp = *lst;
			*lst = (*lst)->next;
		}
	}
}

void li_lstfor(const li_list_t *lst, void (*proc)(void*,void*), void *param) {
	while (lst) {
		proc(lst->data, param);
		lst = lst->next;
	}
}

int li_ptreq(void *a, void *b) {
	return a == b;
}
