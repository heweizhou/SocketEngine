//
//  CList_ZHW.h
//  Panorama
//
//  Created by zhw on 14-10-24.
//  Copyright (c) 2014年 zhouhewei. All rights reserved.
//

#ifndef __Panorama__CList_ZHW__
#define __Panorama__CList_ZHW__

#include <stdio.h>
#include <stdlib.h>

#define _list_entry_(ptr, type, member) ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define _list_for_each_(pos, head) \
for (pos = head; pos != NULL; \
pos = pos->next)

#define _list_for_each_reverse_(pos, tail) \
for (pos = tail; pos != NULL; \
pos = pos->prev)

typedef struct _TAG_LIST_NODE
{
    _TAG_LIST_NODE():prev(NULL),next(NULL){}
    struct _TAG_LIST_NODE* prev;
    struct _TAG_LIST_NODE* next;
}ListNode;

typedef struct _TAG_LIST_INFO_
{
    _TAG_LIST_INFO_():head(NULL),tail(NULL),count(0){}
    ListNode*   current;
    ListNode*   head;
    ListNode*   tail;
    unsigned int    count;
}ListInfo;


void        addList( ListInfo* info, ListNode* node );
void        appendList( ListInfo* info, ListNode* node );
void        insertBefor( ListInfo* info, ListNode* item, ListNode* newNode );
void        insertAfter( ListInfo* info, ListNode* item, ListNode* newNode );
ListNode*   removeFromList( ListInfo* info, ListNode* node );
void        clearList( ListInfo* info );
void        clearList_refrence_only( ListInfo* info );
void        destroyList( ListInfo* info );

ListInfo* initList();
ListNode* removeFromList( ListInfo info, ListNode* node );

#endif /* defined(__Panorama__CList_ZHW__) */
