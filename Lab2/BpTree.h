#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<math.h>

typedef struct BpNode
{
    int KeyCount;
    int KidCount;
    int data[6];
    struct BpNode *parent, *next;
    struct BpNode *kid[6];
} BpNode, *BpTree;

typedef struct BpTreeAns
{
    BpNode* node;
    int index;
    int found;
} BpTreeAns;

BpTree BpTree_gen(int *array, int n, BpTree Tree);
BpTreeAns BpTreeQuery(int data, BpTree Tree);
BpTree BpTreeInsert(int data, BpTree Tree);
void BpTreeListInsert(BpNode *p, int data);
int BpTreeListIndex(int *list, int data);
int BpTreeKidIndex(BpNode **list, BpNode *data);
BpTree BpTreeNodeInsert(int data, BpNode *p);
BpTree BpTreeModify(BpNode * p);
BpTree BpTreeDelete(int data, BpTree Tree);
BpTreeAns BpTreePrior(int data, BpNode *node);
BpTree BpTreeDataRemove(BpNode * node, int index, BpTree Tree);
void BpTreeListRemove(BpNode *node, int index);
BpTree BpTreeNodeAdjust(BpNode *node, BpTree Tree);
BpNode *BpTreeLbrother(BpNode *node);
BpNode *BpTreeRbrother(BpNode *node);

BpTree BpTree_gen(int *array, int n, BpTree Tree)
{
    Tree->data[0] = array[0];
    Tree->KeyCount = 1;
    Tree->KidCount = 0;
    Tree->parent = NULL;
    Tree->next = NULL;

    int i = 1;
    while (i < n)
    {
        Tree = BpTreeInsert(array[i], Tree);
        i += 1;
    }
    return Tree;
}

BpTreeAns BpTreeQuery(int data, BpTree Tree)
{
    BpNode *p = Tree;
    BpTreeAns ans;
    ans.found = 0;
    ans.index = 0;
    ans.node = p;

    if (p->KidCount == 0)
    {
        for (int i = 0; i < p->KeyCount; i++)
        {
            if (p->data[i] == data)
            { // if found
                ans.index = i;
                ans.found = 1;
                return ans;
            }
        }
        return ans;
    }


    if (data <= p->data[0])
        return BpTreeQuery(data, p->kid[0]);

    for (int i = 0; i < p->KeyCount - 1; i++)
        if (p->data[i] < data && p->data[i + 1] >= data)
            return BpTreeQuery(data, p->kid[i + 1]);
    
    return BpTreeQuery(data, p->kid[p->KeyCount - 1]);
}

BpTree BpTreeInsert(int data, BpTree Tree)
{
    BpTreeAns ans = BpTreeQuery(data, Tree);
    if (ans.found == 1) // if found
        return Tree;

    BpNode *tmp = BpTreeNodeInsert(data, ans.node);
    if (tmp)
        Tree = tmp;
    return Tree;
}

BpTree BpTreeNodeInsert(int data, BpNode *p)
{
    BpTreeListInsert(p, data);
    p->KeyCount++;
    return BpTreeModify(p);
}

BpTree BpTreeModify(BpNode * p)
{
    if (p->KeyCount < 6)
        return NULL;

    if(p->parent == NULL)
    {
        // root
        BpNode *head = (BpNode *)malloc(sizeof(BpNode));
        BpNode *q = (BpNode *)malloc(sizeof(BpNode));

        // new root
        head->data[0] = p->data[2];
        head->data[1] = p->data[5];
        head->KeyCount = 2;
        head->kid[0] = p;
        head->kid[1] = q;
        head->KidCount = 2;
        head->parent = NULL;

        // new brother
        q->data[0] = p->data[3];
        q->data[1] = p->data[4];
        q->data[2] = p->data[5];
        q->KeyCount = 3;

        if (p->KidCount != 0)
        { // if not leaf
            q->kid[0] = p->kid[3];
            q->kid[1] = p->kid[4];
            q->kid[2] = p->kid[5];
            q->KidCount = 3;
            q->kid[0]->parent = q;
            q->kid[1]->parent = q;
            q->kid[2]->parent = q;
        }
        else
            q->KidCount = 0;
        q->parent = head;

        // modify the original
        p->KeyCount = 3;
        if (p->KidCount != 0) // if leaf
            p->KidCount = 3;
        p->parent = head;
        p->next = q;
        return head;
    }

    if (p->KidCount == 0)
    {   
        // for leaf node
        BpNode *parent = p->parent;
        BpTreeListInsert(parent, p->data[2]);
        int index = BpTreeListIndex(parent->data, p->data[2]);

        BpNode *q = (BpNode *)malloc(sizeof(BpNode));

        // new brother
        q->data[0] = p->data[3];
        q->data[1] = p->data[4];
        q->data[2] = p->data[5];
        q->KeyCount = 3;
        q->KidCount = 0;
        q->parent = parent;
        q->next = NULL;

        // modify parent
        parent->kid[index] = p;
        for (int j = 5; j > index + 1; j--)
            parent->kid[j] = parent->kid[j - 1];
        parent->kid[index + 1] = q;
        parent->KeyCount++;
        parent->KidCount++;

        // modify the original node
        p->KeyCount = 3;
        p->next = q;
        return BpTreeModify(parent);
    }

    // for other nodes
    BpNode *parent = p->parent;
    BpNode *q = (BpNode *)malloc(sizeof(BpNode));

    BpTreeListInsert(parent, p->data[2]);
    int index = BpTreeListIndex(parent->data, p->data[2]);

    // new brother
    q->data[0] = p->data[3];
    q->data[1] = p->data[4];
    q->data[1] = p->data[5];
    q->KeyCount = 3;
    q->kid[0] = p->kid[3];
    q->kid[1] = p->kid[4];
    q->kid[2] = p->kid[5];
    q->KidCount = 3;
    q->kid[0]->parent = q;
    q->kid[1]->parent = q;
    q->kid[2]->parent = q;
    q->KidCount = 3;
    q->parent = parent;
    q->next = NULL;

    // modify parent
    parent->kid[index] = p;
    for (int j = 5; j > index + 1; j--)
        parent->kid[j] = parent->kid[j - 1];
    parent->kid[index + 1] = q;
    parent->KeyCount++;
    parent->KidCount++;

    p->KeyCount = 2;
    p->KidCount = 3;
    p->next = q;
    return BpTreeModify(parent);
}

void BpTreeListInsert(BpNode *p, int data)
{
    // insert into the data list
    int i = 0;
    while (p->data[i] < data && i < p->KeyCount)
        i++;

    for (int j = 5; j > i; j--)
        p->data[j] = p->data[j - 1];

    p->data[i] = data;
}

int BpTreeListIndex(int *list, int data)
{
    for (int i = 0; i < 6; i++)
    {
        if(list[i]==data)
            return i;
    }
}

BpTree BpTreeDelete(int data, BpTree Tree)
{
    BpTreeAns ans = BpTreeQuery(data, Tree);
    if (!ans.found)
        return NULL;

    return BpTreeDataRemove(ans.node, ans.index, Tree);
}

BpTree BpTreeDataRemove(BpNode * node, int index, BpTree Tree)
{
    BpTreeListRemove(node, index);
    return BpTreeNodeAdjust(node, Tree);
}

BpTree BpTreeNodeAdjust(BpNode *node, BpTree Tree)
{
    if (!node->parent) // if root
        return Tree;

    if (node->KeyCount > 2) // if sufficient
        return Tree;

    BpNode *parent = node->parent;
    BpNode *Lbrother = BpTreeLbrother(node);
    BpNode *Rbrother = BpTreeRbrother(node);
    if (Lbrother && Lbrother->KeyCount > 3)
    {
        int index = BpTreeKidIndex(parent->kid, node);
        BpTreeListInsert(node, parent->data[index - 1]); // insert data from parent
        node->KeyCount++;
        parent->data[index - 1] = Lbrother->data[--Lbrother->KeyCount - 1]; // move brother data to parent
        return Tree;
    }
    else if (Rbrother && Rbrother->KeyCount > 3)
    {
        int index = BpTreeKidIndex(parent->kid, node);
        BpTreeListInsert(node, Rbrother->data[0]); // insert data from parent
        node->KeyCount++;
        parent->data[index] = Rbrother->data[0]; // move brother data to parent
        BpTreeListRemove(Rbrother, 0);
        return Tree;
    }
    else
    {
        if (Lbrother)
        {
            // merge with Lbrother
            int index = BpTreeKidIndex(parent->kid, node);

            for (int j = 0; j < node->KeyCount; j++) // merge keys
            {
                BpTreeListInsert(Lbrother, node->data[j]);
                Lbrother->KeyCount++;
            }
            for (int j = 0; j < node->KidCount; j++) // merge kids
            {
                Lbrother->kid[Lbrother->KidCount + j] = node->kid[j];
                node->kid[j]->parent = Lbrother;
            }
            Lbrother->KidCount += node->KidCount;
            Lbrother->next = node->next;

            BpTreeListRemove(parent, index - 1);  // remove key in parent
            for (int j = index; j < parent->KidCount - 1; j++)
                parent->data[j] = parent->data[j + 1];  //remove node in parent

            parent->KidCount--;
            if (parent->KidCount == 1)
                return node;

            return BpTreeNodeAdjust(parent, Tree); // recursively call
        }
        else
        {
            // merge with Rbrother
            int index = BpTreeKidIndex(parent->kid, Rbrother);

            for (int j = 0; j < Rbrother->KeyCount; j++) // merge keys
            {
                BpTreeListInsert(node, Rbrother->data[j]);
                node->KeyCount++;
            }
            for (int j = 0; j < Rbrother->KidCount; j++) // merge kids
            {
                node->kid[node->KidCount + j] = Rbrother->kid[j];
                Rbrother->kid[j]->parent = node;
            }
            node->KidCount += Rbrother->KidCount;
            node->next = Rbrother;

            BpTreeListRemove(parent, index - 1);  // remove key in parent
            for (int j = index; j < parent->KidCount - 1; j++)
                parent->data[j] = parent->data[j + 1];  //remove node in parent

            parent->KidCount--;
            if (parent->KidCount == 1)
                return node;
            
            return BpTreeNodeAdjust(parent, Tree); // recursively call
        }
    }
}

BpNode *BpTreeLbrother(BpNode * node)
{
    // return its left brother
    BpNode *parent = node->parent;
    int index = BpTreeKidIndex(parent->kid, node);
    return index ? parent->kid[index - 1] : NULL;
}

BpNode *BpTreeRbrother(BpNode * node)
{
    // return its left brother
    BpNode *parent = node->parent;
    int index = BpTreeKidIndex(parent->kid, node);
    return (index != parent->KidCount - 1) ? parent->kid[index + 1] : NULL;
}

int BpTreeKidIndex(BpNode **list, BpNode * data)
{
    for (int i = 0; i < 6; i++)
    {
        if(list[i]==data)
            return i;
    }
}

void BpTreeListRemove(BpNode *node, int index)
{
    for (int i = index; i < node->KeyCount - 1; i++)
        node->data[i] = node->data[i + 1];

    node->KeyCount--;
}
