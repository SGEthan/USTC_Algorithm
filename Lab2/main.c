#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<math.h>
#include "AVLTree.h"
#include "RBTree.h"
#include "BTree.h"
#include "BpTree.h"
#include "BSTree.h"
#define N 5000

void time_test(int *array, int n);
void rand_gen(int *array, int n);

int main()
{
    int k[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    rand_gen(k, N);
    time_test(k, N);
    // AVLTree Tree = (AVLTree)malloc(sizeof(AVLNode));
    // Tree = AVLTree_gen(k, N, Tree);
    system("pause");

    // RBTree Tree = (RBTree)malloc(sizeof(RBNode));
    // Tree = RBTree_gen(k, N, Tree);
    // Tree = RBTree_Delete(6, Tree);

    // BTree Tree = (BTree)malloc(sizeof(BNode));
    // Tree = BTree_gen(k, N, Tree);
    // Tree = BTreeDelete(8, Tree);
    // Tree = BTreeDelete(16, Tree);
    // Tree = BTreeDelete(15, Tree);
    // Tree = BTreeDelete(4, Tree);
    // Tree = BTreeDelete(8, Tree);

    // BpTree Tree = (BpTree)malloc(sizeof(BpNode));
    // Tree = BpTree_gen(k, N, Tree);

    // system("pause");
}

void rand_gen(int *array, int n)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++)
    {
        array[i] = rand();
    }
}

void time_test(int *array, int n)
{
    clock_t start_t, end_t;
    LARGE_INTEGER time_start, time_end;
    double dqFreq;
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
    double time_waste, run_time;

    // gen
    printf("generation time\n");
    printf("Tree_Kind\t\ttime_used(microseconds)\n");


    QueryPerformanceCounter(&time_start);
    BST Tree1 = (BST)malloc(sizeof(TNode));
    BST_gen(array, n, Tree1);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("BSTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    AVLTree Tree2 = (AVLTree)malloc(sizeof(AVLNode));
    Tree2 = AVLTree_gen(array, n, Tree2);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("AVLTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    RBTree Tree3 = (RBTree)malloc(sizeof(RBNode));
    Tree3 = RBTree_gen(array, n, Tree3);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("RBTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    BTree Tree4 = (BTree)malloc(sizeof(BNode));
    Tree4 = BTree_gen(array, n, Tree4);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("BTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    BpTree Tree5 = (BpTree)malloc(sizeof(BNode));
    Tree5 = BpTree_gen(array, n, Tree5);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("B+Tree\t\t %lf\n", run_time);

    // AVL_print(Tree2);
    // RB_print(Tree3);
    // system("pause");
    // return 0;

    //query
    int query[200];
    rand_gen(query, 200);
    printf("\nquery time:\n");
    printf("Tree_Kind\ttime_used(microseconds)\n", time_waste);

    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
        BST_search(array[query[i]%2000], Tree1);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("BSTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
        AVL_search(array[query[i]%2000], Tree2);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("AVLTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
        RBTree_search(array[query[i]%2000], Tree3);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("RBTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
        BTreeQuery(array[query[i]%2000], Tree4);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("BTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
        BpTreeQuery(array[query[i]%2000], Tree5);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("B+Tree\t\t %lf\n", run_time);


    //delete
    printf("\ndelete time:\n");
    printf("Tree_Kind\ttime_used(microseconds)\n", time_waste);

    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
        BST_delete(query[i], Tree1);
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("BSTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
    {
        AVLTree tmp = AVL_Delete(query[i], Tree2);
        if(tmp)
            Tree2 = tmp;
    }
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("AVLTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
    {
        RBTree tmp = RBTree_Delete(query[i], Tree3);
        if(tmp)
            Tree3 = tmp;
    }
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("RBTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
    {
        BTree tmp = BTreeDelete(query[i], Tree4);
        if(tmp)
            Tree4 = tmp;
    }
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("BTree\t\t %lf\n", run_time);


    QueryPerformanceCounter(&time_start);
    for (int i = 0; i < 200; i++)
    {
        BpTree tmp = BpTreeDelete(query[i], Tree5);
        if(tmp)
            Tree5 = tmp;
    }
    QueryPerformanceCounter(&time_end);
    run_time=1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq;
    printf("B+Tree\t\t %lf\n", run_time);
    
}
