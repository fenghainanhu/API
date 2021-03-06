#include "stdio.h"
//链队列的类型定义：
 typedef  struct Qnode
{
    int  data;
    struct Qnode *next;
 }Qnode;

Qnode  *front;                   //队头指针
Qnode  *rear;                    //队尾指针

int EnQueue( int *e)
{
    Qnode *p;
    p=(Qnode *)malloc(sizeof(Qnode));

    p->data=*e;         //将数据加入到队列中
    p->next=NULL;

    rear->next=p;       //调整队尾指针位置，为下一次压栈做准备
    rear=p;
    return  1;
}



int DeQueue( int *e)
{
    Qnode *p;

    if(front==rear)
        return 0;   	    //队空

    p=front->next;          //获得出队指针
    *e=p->data;             //将待出队节点的数据取出
    front->next= p->next;   //调整对头指针，为下次出队做准备

    if(p == rear)           //若队头指针追上队尾指针，调整队头指针，此时队空
        rear = front; 
        
    free(p);                //释放已出队数据
    return 1;
}

int main()
{
    int a = 11;
    int b = 0;
    rear=front=(Qnode *)malloc(sizeof(Qnode));
    EnQueue(&a);        //插入队列
    printf("%d\n",a);

    DeQueue(&b);        //弹出队列
    printf("%d\n",b);
}

