/*************************************************************************
	> File Name: ADT.c
	> Author: K I D
	> Mail: kuailexy@126.com
	> Created Time: Tue 09 Aug 2016 10:11:55 GMT
 ************************************************************************/

#include "Stack.h"

//typedef  int ElementType;

int main(void)
{
    Stack S = NULL;
    char choose=99;
    puts("栈测试程序【Program test about ADT 】");
    while(choose != 0)
    {
    puts("请选择想要测试的功能：");
    printf("1：创建一个空栈（creat）\n2：进栈操作（push）\n3：出栈操作（pop）\n4：显示当前栈中所有数据（show）\n0：结束测试（exit）");
    choose = getchar();
    fflush(stdin);
    switch(choose)
        {
            case '1':
            {
                if((S=CreatStack()) != NULL)
                {
                    puts("success!");
                }
                break;
            }
            case '2':
            {
                char ch;
                printf("请输入一个字符:");
                ch=getchar();
                fflush(stdin);
                Push(ch,S);
                break;
            }
            case '3':
            {
               Pop(S);
                break;
            }
            case '4':
            {
                Show(S);
                break;
            }
            default:
            break;
        }
        choose = getchar();
        fflush(stdin);
    }
    return 0;
}

int IsEmpty(Stack S)
{
    return (S->next ==NULL);
}

Stack CreatStack(void)
{
    Stack S;
    S = (PtrToNode) malloc(sizeof(struct Node));
    if(S == NULL);
    {
        perror("malloc");
        return NULL;
    }
    S->next == NULL;
    MakeEmpty(S);
    return S;
}

void MakeEmpty(Stack S)
{
    if(S == NULL)
    {
        perror("Must use CrateStack first");
    }
    else
    {
        while(! IsEmpty(S))
        Pop(S);
    }
}

void Push(ElementType X ,Stack S)
{
    PtrToNode TmpCell;
    TmpCell = (Stack) malloc(sizeof(struct Node));
    if (TmpCell == NULL)
    {
        perror("malloc");
    }
    else
    {
        TmpCell -> Element = X;
        TmpCell ->next =S ->next;
        S -> next =TmpCell;
    }
}

ElementType Top(Stack S)
{
    if(! IsEmpty(S))
    return S->next->Element;
    perror("Empty Stack");
    return 0;
}
void Pop(Stack S)
{
    PtrToNode FirstCell;
    if(IsEmpty(S))
    {
        perror("Empty Stack");
    }
    else
    {
        printf("%c has been poped\n",FirstCell->Element);
        FirstCell = S->next;
        S->next = S->next->next;
        free(FirstCell);
    }
}
void Show(Stack S)
{
    for(Stack Temp = S ; Temp != NULL ; Temp=Temp->next)
    {
        printf("%d\n",Temp->Element);
    }
}
