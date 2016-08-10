/*************************************************************************
	> File Name: Stack.h
	> Author: K I D
	> Mail: kuailexy@126.com
	> Created Time: Tue 09 Aug 2016 10:12:47 GMT
 ************************************************************************/

#ifndef _STACK_H
#define _STACK_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<error.h>

struct  Node;
typedef struct Node *PtrToNode;
typedef PtrToNode Stack;
typedef  char    ElementType;

int IsEmpty(Stack S);
Stack CreatStack(void);
void MakeEmpty(Stack S);
void DisposeStack(Stack S);
void Push(ElementType X,Stack S);
ElementType Top (Stack S);
void Pop(Stack S);
void Show(Stack S);

#endif
struct Node
{
    ElementType Element;
    PtrToNode next;
};
