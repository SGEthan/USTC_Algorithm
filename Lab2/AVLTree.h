#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<math.h>

typedef struct AVLNode 
{
    int                 data, height;
    struct AVLNode      *lchild, *rchild, *parent;
}AVLNode, *AVLTree;

AVLTree AVLTree_gen(int *array, int n, AVLTree Tree);
int treeHeight(AVLTree root);
int AVLTreeGetBalanceFactor(AVLTree root);
AVLTree AVLTreeRotateRight(AVLTree root);
AVLTree AVLTreeRotateLeft(AVLTree root);
AVLTree AVLTreeBalance(AVLTree root);
AVLTree AVLTreeInsert(int data, AVLTree Tree);
AVLTree AVL_Recursive_Insert(AVLNode * p, AVLTree Tree);
AVLTree AVL_search(int data, AVLTree Tree);
AVLTree AVL_Delete(int data, AVLTree Tree);
int AVL_prior_rm(AVLNode *p, AVLTree Tree);
int AVL_recursive_prior(AVLNode *p);
void AVL_print(AVLTree Tree);

AVLTree AVLTree_gen(int *array, int n, AVLTree Tree)
{
    Tree->data = array[0];
    Tree->height = 0;
    Tree->lchild = NULL;
    Tree->rchild = NULL;
    Tree->parent = NULL;
    int i = 1;
    while (i < n)
    {   
        if (!AVL_search(array[i], Tree))
            Tree = AVLTreeInsert(array[i], Tree);
        i += 1;
    }
    return Tree;
}

int treeHeight(AVLTree root)
{
    if(root == NULL)
        return 0;
    else 
    {
        root->height = max(treeHeight(root->lchild), treeHeight(root->rchild)) + 1;
        return root->height;
    }
}

int AVLTreeGetBalanceFactor(AVLTree root)
{
    if(root == NULL)
        return 0;
    else
    {
        int l_height = root->lchild ? treeHeight(root->lchild) : 0;
        int r_height = root->rchild ? treeHeight(root->rchild) : 0;
        return l_height - r_height;
    }
}

AVLTree AVLTreeRotateRight(AVLTree root)
{
    AVLNode* left = root->lchild;
    int flag;

    root->lchild = left->rchild; // 将将要被抛弃的节点连接为旋转后的 root 的左孩子
    if(root->lchild)
        root->lchild->parent = root;

    if(root->parent)
    {
        if(root->parent->lchild == root)
            flag = 1;
        else
            flag = 0;
    }

    left->rchild = root; // 调换父子关系
    left->parent = root->parent;
    root->parent = left;

    if(left->parent)
    {
        flag ? (left->parent->lchild = left) : (left->parent->rchild = left);
        treeHeight(root->parent);
        return left;
    }

    treeHeight(left);
    return left;
}

AVLTree AVLTreeRotateLeft(AVLTree root)
{
    AVLNode* right = root->rchild;
    int flag;
    
    root->rchild = right->lchild; // 将将要被抛弃的节点连接为旋转后的 root 的right
    if(root->rchild)
        root->rchild->parent = root;

    if(root->parent)
    {
        if(root->parent->lchild == root)
            flag = 1;
        else
            flag = 0;
    }

    right->lchild = root; // 调换父子关系
    right->parent = root->parent;
    root->parent = right;

    if(right->parent)
    {
        flag ? (right->parent->lchild = right) : (right->parent->rchild = right);
        treeHeight(root->parent);
        return right;
    }

    treeHeight(right);
    return right;
}

AVLTree AVLTreeBalance(AVLTree root)
{
    int factor = AVLTreeGetBalanceFactor(root);

    if(factor > 1 && AVLTreeGetBalanceFactor(root->lchild) > 0)
    {//LL
        root = AVLTreeRotateRight(root);
    }
    else if(factor > 1 && AVLTreeGetBalanceFactor(root->lchild) <= 0)
    {//LR
        root->lchild = AVLTreeRotateLeft(root->lchild);
        root = AVLTreeRotateRight(root);
    }
    else if (factor < -1 && AVLTreeGetBalanceFactor(root->rchild) < 0)
    {//RR
        root = AVLTreeRotateLeft(root);
    }
    else if (factor < -1 && AVLTreeGetBalanceFactor(root->rchild) >= 0)
    {//RL
        root->rchild = AVLTreeRotateRight(root->rchild);
        root = AVLTreeRotateLeft(root);
    }
    return root;
}

AVLTree AVLTreeInsert(int data, AVLTree Tree)
{
    AVLNode *p = (AVLNode *)malloc(sizeof(AVLNode));
    p->data = data;
    p->height = 0;
    p->lchild = NULL;
    p->rchild = NULL;
    p->parent = NULL;

    return AVL_Recursive_Insert(p, Tree);
}

AVLTree AVL_Recursive_Insert(AVLNode * p, AVLTree Tree)
{
    if (p->data <= Tree->data && Tree->lchild == NULL)
    {
        Tree->lchild = p;
        p->parent = Tree;
    }
    else if (p->data <= Tree->data)
        AVL_Recursive_Insert(p, Tree->lchild);
    else if (p->data >= Tree->data && Tree->rchild == NULL)
    {
        Tree->rchild = p;
        p->parent = Tree;
    }
    else
        AVL_Recursive_Insert(p, Tree->rchild);

    return AVLTreeBalance(Tree);
}

AVLTree AVL_search(int data, AVLTree Tree)
{
    AVLNode *p = Tree;
    int deep = 0;
    while(1)
    {
        if (data == p->data) // if hit
        {
            // printf("%d ", deep);
            return p;
        }

        if (p->lchild == NULL && p->rchild == NULL) // reach the bottom
        {
            // printf("%d ", deep);
            return NULL;
        }

        if (data < p->data) // go to left
        {
            if (p->lchild == NULL)
                return NULL;
            else
            {
                deep++;
                p = p->lchild;
            }
        }
        else if (data > p->data)
        {
            if (p->rchild == NULL)
                return NULL;
            else
            {
                deep++;
                p = p->rchild;
            }
        }
    }
}

AVLTree AVL_Delete(int data, AVLTree Tree)
{
    AVLNode *p = Tree;
    if (data == p->data) // if hit
    {
        if (p->lchild == NULL && p->rchild == NULL)
        {// leaf node
            AVLNode *parent = p->parent;
            int flag = (parent->lchild == p);
            free(p);
            flag ? (parent->lchild = NULL) : (parent->rchild = NULL);
            return NULL;
        }
        else if (p->lchild && p->rchild == NULL)
        {// left not NULL
            p->data = p->lchild->data;
            free(p->lchild);
            p->lchild = NULL;
            return NULL;
        }
        else if (p->rchild && p->lchild == NULL)
        {// right not NULL
            p->data = p->rchild->data;
            free(p->rchild);
            p->rchild = NULL;
            return NULL;
        }
        else
        {// substitute with its prior
            p->data = AVL_prior_rm(p, Tree);
        }
    }
    else if (data < p->data) // go to left
    {
        if (p->lchild)
            return NULL;
        AVL_Delete(data, p->lchild);
        AVLTreeBalance(p);
    }
    else // go to right
    {
        if (p->rchild)
            return NULL;
        AVL_Delete(data, p->rchild);
        AVLTreeBalance(p);
    }
}

int AVL_prior_rm(AVLNode *p, AVLTree Tree)
{
    AVLNode *q = p->lchild;
    if (q->rchild == NULL)
    {
        int key = q->data;
        if (q->lchild)
        {
            q->data = q->lchild->data;
            free(q->lchild);
            q->lchild = NULL;
        }
        else
        {
            AVLNode *parent = q->parent;
            int flag = (parent->lchild == q);
            free(q);
            flag ? (parent->lchild = NULL) : (parent->rchild = NULL);
        }
        return key;
    }
    int key = AVL_recursive_prior(q);
    AVLTreeBalance(q);
    return key;
}

int AVL_recursive_prior(AVLNode *p)
{
    if (p->rchild == NULL)
    {
        int key = p->data;
        if (p->lchild)
        {
            p->data = p->lchild->data;
            free(p->lchild);
            p->lchild = NULL;
        }
        else
        {
            AVLNode *parent = p->parent;
            int flag = (parent->lchild == p);
            free(p);
            flag ? (parent->lchild = NULL) : (parent->rchild = NULL);
        }
        return key;
    }
    else
    {
        int key = AVL_recursive_prior(p->rchild);
        AVLTreeBalance(p);
        return key;
    }
}

void AVL_print(AVLTree Tree)
{
    if (Tree->lchild)
        AVL_print(Tree->lchild);

    printf("%d\n", Tree->data);
    
    if(Tree->rchild)
        AVL_print(Tree->rchild);
}
