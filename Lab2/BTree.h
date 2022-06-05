#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<math.h>

typedef struct BNode
{
    int KeyCount;
    int KidCount;
    int data[5];
    struct BNode *parent;
    struct BNode *kid[6];
} BNode, *BTree;

typedef struct BTreeAns
{
    BNode* node;
    int index;
    int found;
} BTreeAns;

BTree BTree_gen(int *array, int n, BTree Tree);
BTreeAns BTreeQuery(int data, BTree Tree);
BTree BTreeInsert(int data, BTree Tree);
void BTreeListInsert(BNode *p, int data);
int BTreeListIndex(int *list, int data);
int BTreeKidIndex(BNode **list, BNode *data);
BTree BTreeNodeInsert(int data, BNode *p);
BTree BTreeModify(BNode * p);
BTree BTreeDelete(int data, BTree Tree);
BTreeAns BTreePrior(int data, BNode *node);
BTree BTreeDataRemove(BNode * node, int index, BTree Tree);
void BTreeListRemove(BNode *node, int index);
BTree BTreeNodeAdjust(BNode *node, BTree Tree);
BNode *BTreeLbrother(BNode *node);
BNode *BTreeRbrother(BNode *node);

BTree BTree_gen(int *array, int n, BTree Tree)
{
    Tree->data[0] = array[0];
    Tree->KeyCount = 1;
    Tree->KidCount = 0;
    Tree->parent = NULL;

    int i = 1;
    while (i < n)
    {
        Tree = BTreeInsert(array[i], Tree);
        i += 1;
    }
    return Tree;
}

BTreeAns BTreeQuery(int data, BTree Tree)
{
    BNode *p = Tree;
    BTreeAns ans;
    ans.found = 0;
    ans.index = 0;
    ans.node = p;

    if (data < p->data[0] && p->KidCount)
        return BTreeQuery(data, p->kid[0]);

    for (int i = 0; i < p->KeyCount - 1; i++)
    {
        if (p->data[i] == data)
        { // if found
            ans.index = i;
            ans.found = 1;
            return ans;
        }

        if (p->data[i] < data && p->data[i + 1] > data)
        {// if not found, then go to kids
            if (p->KidCount)
                return BTreeQuery(data, p->kid[i+1]);
            else
                return ans;
        }
    }

    if (p->data[p->KeyCount-1] == data)
    { // if found
        ans.index = p->KeyCount-1;
        ans.found = 1;
        return ans;
    }
    else
    {// if not found, then go to kids
        if (p->KidCount)
            return BTreeQuery(data, p->kid[p->KidCount-1]);
    }
    
    return ans; // if not found, then ans.found=0
}

BTree BTreeInsert(int data, BTree Tree)
{
    BTreeAns ans = BTreeQuery(data, Tree);
    if (ans.found == 1) // if found
        return Tree;

    BNode *tmp = BTreeNodeInsert(data, ans.node);
    if (tmp)
        Tree = tmp;
    return Tree;
}

void BTreeListInsert(BNode *p, int data)
{
    // insert into the data list
    int i = 0;
    while (p->data[i] < data && i < p->KeyCount)
        i++;

    for (int j = 4; j > i; j--)
        p->data[j] = p->data[j - 1];

    p->data[i] = data;
}

int BTreeListIndex(int *list, int data)
{
    for (int i = 0; i < 5; i++)
    {
        if(list[i]==data)
            return i;
    }
}

int BTreeKidIndex(BNode **list, BNode * data)
{
    for (int i = 0; i < 6; i++)
    {
        if(list[i]==data)
            return i;
    }
}

BTree BTreeNodeInsert(int data, BNode *p)
{
    BTreeListInsert(p, data);
    p->KeyCount++;
    return BTreeModify(p);
}

BTree BTreeModify(BNode * p)
{
    if (p->KeyCount < 5)
        return NULL;

    if(p->parent == NULL)
    {
        // root
        BNode *head = (BNode *)malloc(sizeof(BNode));
        BNode *q = (BNode *)malloc(sizeof(BNode));

        // new root
        head->data[0] = p->data[2];
        head->KeyCount = 1;
        head->kid[0] = p;
        head->kid[1] = q;
        head->KidCount = 2;
        head->parent = NULL;

        // new brother
        q->data[0] = p->data[3];
        q->data[1] = p->data[4];
        q->KeyCount = 2;
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
        p->KeyCount = 2;
        if (p->KidCount != 0) // if leaf
            p->KidCount = 3;
        p->parent = head;
        return head;
    }

    if (p->KidCount == 0)
    {   
        // for leaf node
        BNode *parent = p->parent;
        BTreeListInsert(parent, p->data[2]);
        int index = BTreeListIndex(parent->data, p->data[2]);

        BNode *q = (BNode *)malloc(sizeof(BNode));

        // new brother
        q->data[0] = p->data[3];
        q->data[1] = p->data[4];
        q->KeyCount = 2;
        q->KidCount = 0;
        q->parent = parent;

        // modify parent
        parent->kid[index] = p;
        for (int j = 5; j > index + 1; j--)
            parent->kid[j] = parent->kid[j - 1];
        parent->kid[index + 1] = q;
        parent->KeyCount++;
        parent->KidCount++;

        // modify the original node
        p->KeyCount = 2;
        return BTreeModify(parent);
    }

    // for other nodes
    BNode *parent = p->parent;
    BNode *q = (BNode *)malloc(sizeof(BNode));

    BTreeListInsert(parent, p->data[2]);
    int index = BTreeListIndex(parent->data, p->data[2]);

    // new brother
    q->data[0] = p->data[3];
    q->data[1] = p->data[4];
    q->KeyCount = 2;
    q->kid[0] = p->kid[3];
    q->kid[1] = p->kid[4];
    q->kid[2] = p->kid[5];
    q->KidCount = 3;
    q->kid[0]->parent = q;
    q->kid[1]->parent = q;
    q->kid[2]->parent = q;
    q->KidCount = 3;
    q->parent = parent;

    // modify parent
    parent->kid[index] = p;
    for (int j = 5; j > index + 1; j--)
        parent->kid[j] = parent->kid[j - 1];
    parent->kid[index + 1] = q;
    parent->KeyCount++;
    parent->KidCount++;

    p->KeyCount = 2;
    p->KidCount = 3;
    return BTreeModify(parent);
}

BTree BTreeDelete(int data, BTree Tree)
{
    BTreeAns ans = BTreeQuery(data, Tree);
    if (!ans.found)
        return Tree;
    if (ans.node->KidCount != 0)
    {
        // if not leaf
        BTreeAns prior = BTreePrior(data, ans.node);
        ans.node->data[ans.index] = prior.node->data[prior.index];
        return BTreeDataRemove(prior.node, prior.index, Tree);
    }
    else
    {
        return BTreeDataRemove(ans.node, ans.index, Tree);
    }
}

BTreeAns BTreePrior(int data, BNode *node)
{
    BTreeAns ans;
    int index = BTreeListIndex(node->data, data);
    BNode *p;
    p = node->kid[index];

    while (p->KidCount != 0)
        p = p->kid[p->KidCount];
    
    ans.node = p;
    ans.found = 1;
    ans.index = p->KeyCount - 1;
    return ans;
}

BTree BTreeDataRemove(BNode * node, int index, BTree Tree)
{
    BTreeListRemove(node, index);
    return BTreeNodeAdjust(node, Tree);
}

void BTreeListRemove(BNode *node, int index)
{
    for (int i = index; i < node->KeyCount - 1; i++)
        node->data[i] = node->data[i + 1];

    node->KeyCount--;
}

BTree BTreeNodeAdjust(BNode *node, BTree Tree)
{
    if (!node->parent) // if root
        return Tree;

    if (node->KeyCount > 1) // if sufficient
        return Tree;

    BNode *parent = node->parent;
    BNode *Lbrother = BTreeLbrother(node);
    BNode *Rbrother = BTreeRbrother(node);
    if (Lbrother && Lbrother->KeyCount > 2)
    {
        int index = BTreeKidIndex(parent->kid, node);
        BTreeListInsert(node, parent->data[index - 1]); // insert data from parent
        node->KeyCount++;
        parent->data[index - 1] = Lbrother->data[--Lbrother->KeyCount]; // move brother data to parent
        return Tree;
    }
    else if (Rbrother && Rbrother->KeyCount > 2)
    {
        int index = BTreeKidIndex(parent->kid, node);
        BTreeListInsert(node, parent->data[index]); // insert data from parent
        node->KeyCount++;
        parent->data[index] = Rbrother->data[0]; // move brother data to parent
        BTreeListRemove(Rbrother, 0);
        return Tree;
    }
    else
    {
        if (Lbrother)
        {
            // merge with Lbrother
            int index = BTreeKidIndex(parent->kid, node);

            for (int j = 0; j < node->KeyCount; j++) // merge keys
            {
                BTreeListInsert(Lbrother, node->data[j]);
                Lbrother->KeyCount++;
            }
            for (int j = 0; j < node->KidCount; j++) // merge kids
            {
                Lbrother->kid[Lbrother->KidCount + j] = node->kid[j];
                node->kid[j]->parent = Lbrother;
            }
            Lbrother->KidCount += node->KidCount;

            BTreeListInsert(Lbrother, parent->data[index - 1]);
            Lbrother->KeyCount++;
            BTreeListRemove(parent, index - 1);  // remove key in parent
            for (int j = index; j < parent->KidCount - 1; j++)
                parent->data[j] = parent->data[j + 1];  //remove node in parent

            parent->KidCount--;
            if (parent->KidCount == 1)
                return node;

            return BTreeNodeAdjust(parent, Tree); // recursively call
        }
        else
        {
            // merge with Rbrother
            int index = BTreeKidIndex(parent->kid, Rbrother);

            for (int j = 0; j < Rbrother->KeyCount; j++) // merge keys
            {
                BTreeListInsert(node, Rbrother->data[j]);
                node->KeyCount++;
            }
            for (int j = 0; j < Rbrother->KidCount; j++) // merge kids
            {
                node->kid[node->KidCount + j] = Rbrother->kid[j];
                Rbrother->kid[j]->parent = node;
            }
            node->KidCount += Rbrother->KidCount;

            BTreeListInsert(node, parent->data[index - 1]);
            node->KeyCount++;
            BTreeListRemove(parent, index - 1);  // remove key in parent
            for (int j = index; j < parent->KidCount - 1; j++)
                parent->data[j] = parent->data[j + 1];  //remove node in parent

            parent->KidCount--;
            if (parent->KidCount == 1)
                return node;
            
            return BTreeNodeAdjust(parent, Tree); // recursively call
        }
    }
}

BNode *BTreeLbrother(BNode * node)
{
    // return its left brother
    BNode *parent = node->parent;
    int index = BTreeKidIndex(parent->kid, node);
    return index ? parent->kid[index - 1] : NULL;
}

BNode *BTreeRbrother(BNode * node)
{
    // return its left brother
    BNode *parent = node->parent;
    int index = BTreeKidIndex(parent->kid, node);
    return (index != parent->KidCount - 1) ? parent->kid[index + 1] : NULL;
}


