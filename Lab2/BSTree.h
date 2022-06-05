#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<math.h>
typedef struct TNode
{
    int             data;
    struct TNode    *lchild, *rchild, *parent;
}TNode, *BST;

void BST_gen(int *array, int n, BST Tree);
void BST_insert(int data, BST Tree);
BST BST_search(int data, BST Tree);
void BST_delete(int data, BST Tree);
void BST_recursive_rm(TNode* Node);

void BST_gen(int *array, int n, BST Tree)
{
    Tree->data = array[0];
    Tree->lchild = NULL;
    Tree->rchild = NULL;
    Tree->parent = NULL;
    int i = 1;
    while (i < n)
    {
        if(!BST_search(array[i],Tree))
            BST_insert(array[i], Tree);
        i += 1;
    }
}

void BST_insert(int data, BST Tree)
{
    TNode *q = (TNode *)malloc(sizeof(TNode));
    q->data = data;
    q->lchild = NULL;
    q->rchild = NULL;
    q->parent = NULL;
    TNode *p = Tree;

    while(1)
    {
        if (p->data > data)
        {
            if (p->lchild == NULL)
            {
                p->lchild = q;
                q->parent = p;
                break;
            }
            else
                p = p->lchild;
        }
        else
        {
            if(p->rchild == NULL)
            {
                p->rchild = q;
                q->parent = p;
                break;
            }
            else
                p = p->rchild;
        }

    }
}

BST BST_search(int data, BST Tree)
{
    TNode *p = Tree;
    while(1)
    {
        if (data == p->data) // if hit
            return p;

        if (p->lchild == NULL && p->rchild == NULL) // reach the bottom
            return NULL;

        if (data < p->data) // go to left
        {
            if (p->lchild == NULL)
                return NULL;
            else
                p = p->lchild;
        }
        else if (data > p->data)
        {
            if (p->rchild == NULL)
                return NULL;
            else
                p = p->rchild;
        }
    }
}

void BST_delete(int data, BST Tree)
{
    BST p = BST_search(data, Tree);
    if (p)
        BST_recursive_rm(p);
}

void BST_recursive_rm(TNode *Node)
{
    if (Node->lchild == NULL)
    {
        if(Node->rchild == NULL)
        { // if leaf
            TNode *q = Node->parent;

            if(q)
            {
                if(q->lchild == Node)
                    q->lchild = NULL;
                else
                    q->rchild = NULL;
            }
            free(Node);
        }
        else
        {
            Node->data = Node->rchild->data;
            BST_recursive_rm(Node->rchild);
        }
    }
    else
    {
        Node->data = Node->lchild->data;
        BST_recursive_rm(Node->lchild);
    }
}

