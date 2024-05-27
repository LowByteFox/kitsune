#ifndef _LIST_H_
#define _LIST_H_

struct kitsune_list_node {
        struct kitsune_list_node *next;
        struct kitsune_list_node *previous;
        void *data;
};

struct kitsune_list {
};

#endif
