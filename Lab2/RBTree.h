#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<math.h>
#define RED 1
#define BLACK 0

typedef struct RBNode
{
    int data;
    int color;
    struct RBNode *lchild, *rchild, *parent;
} RBNode, *RBTree;

RBTree RBTree_gen(int *array, int n, RBTree Tree);
RBTree RBTreeRotateRight(RBTree root);
RBTree RBTreeRotateLeft(RBTree root);
RBTree RBTreeInsert(int data, RBTree Tree);
RBTree RB_Recursive_Insert(RBNode * p, RBTree Tree);
RBTree RBTreeBalance(RBTree p);
RBTree RBTreeRotateRight(RBTree root);
RBTree RBTreeRotateLeft(RBTree root);
RBTree RBTree_search(int data, RBTree Tree);
RBTree RBTree_Delete(int data, RBTree Tree);
RBTree RBTree_node_rm(RBNode *p, RBTree Tree);
RBNode *RBTree_prior(RBNode *p, RBTree Tree);
void RB_print(RBTree Tree);

static RBNode leaf_content = {-1, BLACK, NULL, NULL, NULL};
RBNode *leaf = &leaf_content;

RBTree RBTree_gen(int *array, int n, RBTree Tree)
{
    Tree->data = array[0];
    Tree->color = BLACK;
    Tree->lchild = leaf;
    Tree->rchild = leaf;
    Tree->parent = NULL;
    int i = 1;

    while (i < n)
    {
        if (!RBTree_search(array[i],Tree))
            Tree = RBTreeInsert(array[i], Tree);
        i += 1;
    }
    return Tree;
}

RBTree RBTreeInsert(int data, RBTree Tree)
{
    RBNode *p = (RBNode *)malloc(sizeof(RBNode));
    p->data = data;
    p->color = RED;
    p->lchild = leaf;
    p->rchild = leaf;
    p->parent = NULL;

    RBNode *tmp = RB_Recursive_Insert(p, Tree);
    if(tmp) // if root changed
        Tree = tmp;
    return Tree;
}

RBTree RB_Recursive_Insert(RBNode * p, RBTree Tree)
{
    if (p->data <= Tree->data && Tree->lchild->data == -1)
    {
        Tree->lchild = p;
        p->parent = Tree;
        return RBTreeBalance(p);
    }
    else if (p->data <= Tree->data)
        return RB_Recursive_Insert(p, Tree->lchild);
    else if (p->data >= Tree->data && Tree->rchild->data == -1)
    {
        Tree->rchild = p;
        p->parent = Tree;
        return RBTreeBalance(p);
    }
    else
        return RB_Recursive_Insert(p, Tree->rchild);

}

RBTree RBTreeRotateRight(RBTree root)
{
    RBNode* left = root->lchild;
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
        return left;
    }

    return left;
}

RBTree RBTreeRotateLeft(RBTree root)
{
    RBNode* right = root->rchild;
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
        return right;
    }

    return right;
}

RBTree RBTreeBalance(RBTree p)
{
    if (!(p->parent)) // if reach the root or parent is black
    {
        p->color = BLACK;
        return 0;
    }

    if (p->parent->color == BLACK) // if parent is black
        return 0;

    RBNode *parent = p->parent;
    RBNode *grandp = parent->parent;
    int flag = grandp->lchild == parent;
    RBNode *uncle = flag ? grandp->rchild : grandp->lchild;

    if (parent->color == uncle->color)
    {// case when parent and uncle are both red
        parent->color = BLACK;
        uncle->color = BLACK;

        grandp->color = RED;
        return RBTreeBalance(grandp); // Recursively modify the tree
    }
    else
    {// case when uncle is black
        if (flag ? (parent->rchild == p) : (parent->lchild == p))
        {// case when should rotate parent first
            flag ? RBTreeRotateLeft(parent) : RBTreeRotateRight(parent);
            RBNode *tmp = p;
            p = parent;
            parent = tmp;
        }
        parent->color = BLACK;
        grandp->color = RED;
        flag ? RBTreeRotateRight(grandp) : RBTreeRotateLeft(grandp);
        return parent->parent ? 0 : parent; // return parent if it's root
    }
    
}

RBTree RBTree_search(int data, RBTree Tree)
{
    RBNode *p = Tree;
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

RBTree RBTree_Delete(int data, RBTree Tree)
{
    RBNode *p = RBTree_search(data, Tree);
    if (p)
        return RBTree_node_rm(p, Tree);
    else
        return NULL;
}

RBTree RBTree_node_rm(RBNode* p, RBTree Tree)
{
    if (p->lchild == leaf && p->rchild == leaf)
    { // for leaf nodes
        RBNode *parent = p->parent;
        int flag = parent->lchild == p;

        if (p->color == RED)
        { // for red ones, free it directly
            flag ? (parent->lchild = leaf) : (parent->rchild = leaf);
            free(p);
            return Tree;
        }

        else
        { // for black ones, should discuss
            RBNode *brother = flag ? parent->rchild : parent->lchild;
            if (brother->color == BLACK && brother->lchild == leaf && brother->rchild == leaf)
            { // case 1
                flag ? (parent->lchild = leaf) : (parent->rchild = leaf);
                free(p);
                parent->color = BLACK;
                brother->color = RED;
                return Tree;
            }
            else if (brother->color == BLACK && (flag ? brother->lchild : brother->rchild)->color == RED && (flag ? brother->rchild : brother->lchild) == leaf)
            { // case 2
                flag ? (parent->lchild = leaf) : (parent->rchild = leaf);
                free(p);
                (flag ? brother->lchild : brother->rchild)->color = parent->color;
                parent->color = BLACK;
                flag ? RBTreeRotateRight(brother) : RBTreeRotateLeft(brother);
                flag ? RBTreeRotateLeft(parent) : RBTreeRotateRight(parent);
                return (parent->parent->parent) ? parent->parent : Tree; // return Tree if not root
            }
            else if (brother->color == BLACK && (flag ? brother->rchild : brother->lchild)->color == RED && (flag ? brother->lchild : brother->rchild) == leaf)
            { // case 3
                flag ? (parent->lchild = leaf) : (parent->rchild = leaf);
                free(p);
                brother->color = parent->color;
                parent->color = BLACK;
                (flag ? brother->lchild : brother->rchild)->color = BLACK;
                flag ? RBTreeRotateLeft(parent) : RBTreeRotateRight(parent);
                return (parent->parent->parent) ? Tree : parent->parent; // return Tree if not root
            }
            else if (brother->color == BLACK && (flag ? brother->rchild : brother->lchild)->color == RED && (flag ? brother->lchild : brother->rchild)->color == RED)
            { // case 4
                flag ? (parent->lchild = leaf) : (parent->rchild = leaf);
                free(p);
                flag ? RBTreeRotateLeft(parent) : RBTreeRotateRight(parent);
                brother->color = parent->color;
                brother->lchild->color = BLACK;
                brother->rchild->color = BLACK;
                return (parent->parent->parent) ? Tree : parent->parent; // return Tree if not root
            }
            else
            { // case 5
                flag ? (parent->lchild = leaf) : (parent->rchild = leaf);
                free(p);
                brother->color = BLACK;
                (flag ? brother->lchild : brother->rchild)->color = BLACK;
                flag ? RBTreeRotateLeft(parent) : RBTreeRotateRight(parent);
                return (parent->parent->parent) ? Tree : parent->parent; // return Tree if not root
            }
        }
    }
    else if (p->lchild != leaf && p->rchild == leaf)
    { // non-leaf node case 1
        p->data = p->lchild->data;
        free(p->lchild);
        p->lchild = leaf;
        return Tree;
    }
    else if (p->rchild != leaf && p->lchild == leaf)
    { // non-leaf node case 2
        p->data = p->rchild->data;
        free(p->rchild);
        p->rchild = leaf;
        return Tree;
    }
    else
    { // non-leaf node case 3
        RBNode *q = RBTree_prior(p, Tree);
        p->data = q->data;
        return RBTree_node_rm(q, Tree); // recursively call
    }
}

RBNode *RBTree_prior(RBNode *p, RBTree Tree)
{
    RBNode *q = p->lchild;
    while (q->rchild != leaf)
        q = q->rchild;

    return q;
}

void RB_print(RBTree Tree)
{
    if (Tree->lchild)
        RB_print(Tree->lchild);

    if(Tree->data != -1)
        printf("%d\n", Tree->data);
    
    if(Tree->rchild)
        RB_print(Tree->rchild);
}

