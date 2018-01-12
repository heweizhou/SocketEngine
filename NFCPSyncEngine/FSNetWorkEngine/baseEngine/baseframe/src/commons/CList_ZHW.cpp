//
//  CList_ZHW.cpp
//  Panorama
//
//  Created by zhw on 14-10-24.
//  Copyright (c) 2014年 zhouhewei. All rights reserved.
//

#include "CList_ZHW.h"

ListInfo* initList()
{
    return new ListInfo();
}

void destroyList( ListInfo* info )
{
    clearList( info );
    delete info;
}

void addList( ListInfo* info, ListNode* node )
{
    if ( info->head == NULL )
    {
        info->current = info->head = info->tail = node;
    }
    else
    {
        node->next = info->head;
        info->head->prev = node;
        info->head = node;
    }
    info->count++;
}

void appendList( ListInfo* info, ListNode* node )
{
    if ( info->head == NULL )
    {
        info->current = info->head = info->tail = node;
    }
    else
    {
        node->prev          = info->tail;
        info->tail->next    = node;
        info->tail          = node;
    }
    info->count++;
}

void insertBefor( ListInfo* info, ListNode* item, ListNode* newNode )
{
    newNode->prev = item->prev;
    newNode->next = item;
    
    item->prev->next = newNode;
    item->prev       = newNode;
    
    info->count++;
}

void insertAfter( ListInfo* info, ListNode* item, ListNode* newNode )
{
    newNode->next = item->next;
    newNode->prev = item;
    
    item->next->prev = newNode;
    item->next       = newNode;
    
    info->count++;
}

void clearList_refrence_only( ListInfo* info )
{
    info->head = info->tail = info->current = NULL;
    info->count = 0;
}

ListNode* removeFromList( ListInfo* info, ListNode* node )
{
    ListNode* ret = node;
    
    if ( node == info->head && node == info->tail )
    {
        info->head = info->tail = info->current = NULL;
        info->count = 0;
    }
    if( node == info->head )
    {
        info->head = node->next;
        node->next->prev = NULL;
    }
    else if( node == info->tail )
    {
        info->tail = node;
        node->prev->next = node->next;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    info->count--;
    return ret;
}

void clearList( ListInfo* info )
{
    
    ListNode* ptr = info->head;
    while ( ptr != NULL )
    {
        ListNode* t = ptr;
        ptr = ptr->next;
        removeFromList( info, t );
    }
    
    info->count = 0;
}
