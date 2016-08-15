/*************************************************************************
	> File Name: binary_tree.c
	> Author: 
	> Mail: 
	> Created Time: Fri 12 Aug 2016 19:07:33 GMT
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct TreeNode *Position;
typedef struct TreeNode *SearchTree;

SearchTree MakeEmpty (SearchTree T)
Position Find(ElementType X ,SearchTree T);
Position FindMin(SearchTree T);
Position FindMax(SearchTree T);
SearchTree Insert(ElementType X,SearchTree T);
SearchTree Delete(ElementType X,SearchTree T);
ElementType Retrieve
struct TreeNode
{
    char Element;
    Tree Left;
    Tree Right;
};
//清空一个二叉树
SearchTree MakeEmpty(SearchTree T)
{
    if(T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return(NULL);
}
//二叉樹查找
Position Find(ElementType X,SearchTree T)
{
    if(T == NULL)
    {
        return NULL;
    }
    else
    if(T->L)
}
