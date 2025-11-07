#include<bits/stdc++.h>
using namespace std;

const int MAXSIZE=1000;

//定义出二叉树的结构
typedef char ElemType;
typedef struct BiNode{
    int flag; //0表示左右子树均未被访问，1表示左子树已经被访问过了，2表示左右子树均访问过了
    ElemType data;
    struct BiNode *lchild,*rchild;
}BiNode,*BiTree; 

//定义出栈结构，因为要用于回溯，所以栈的类型为BiTree(后面主要是用BiTree类型，所以不用BiNode)
typedef struct Stack{
    BiTree data[MAXSIZE];
    int top;
}BiStack;

//初始化栈
void InitBiStack(BiStack &S)//不引用就毁了
{
    S.top=-1;
}

//检验栈是否空
bool EmptyBiStack(BiStack S)
{
    if(S.top==-1) return true;
    else return false;
}

//入栈操作
void PushBiStack(BiStack &S,BiTree e)
{
    if(S.top==MAXSIZE-1) return;
    S.data[++S.top]=e;
}

//出栈操作
BiTree PopBiStack(BiStack &S)
{
    if(EmptyBiStack(S)) return NULL;
    return S.data[S.top--];
}

//获取栈顶元素
BiTree GetTopBiStack(BiStack &S)
{
    if(EmptyBiStack(S)) return NULL;
    return S.data[S.top];
}

//用栈来创建二叉树
void CreateBiTreeByStack(BiTree &T,string Inse,int &index)//这种跟初始化的操作有关的建议都加上引用
{
    BiTree p=T;
    BiStack S;
    InitBiStack(S);
    p->data=Inse[index];//先填充根节点，让循环可以进行下去
    p->flag=0;
    PushBiStack(S,p);
    while(!EmptyBiStack(S))
    {
        index++;
        p=GetTopBiStack(S);//获取栈顶元素，每次都要用栈顶元素来进行操作
        if(Inse[index]=='@')//如果为@，则说明指向了NULL，此时借助flag判断具体是指向左孩子空还是右孩子空
        {
            if(p->flag==0)
            {
                p->flag=1;
                p->lchild=NULL;
            }
            else if(p->flag==1)
            {
                p->flag=2;
                p->rchild=NULL;
            }
        }
        else//否则说明是指向了一个具体节点的，借用temp作寄存器
        {
            BiTree temp=new BiNode;
            temp->data=Inse[index];
            temp->flag=0;
            if(p->flag==0)
            {
                p->flag=1;
                p->lchild=temp;
                PushBiStack(S,temp);
            }
            else if(p->flag==1)
            {
                p->flag=2;
                p->rchild=temp;
                PushBiStack(S,temp);
            }
        }
        while(!EmptyBiStack(S)&&GetTopBiStack(S)->flag==2)
        //如果不取栈顶元素取p的话你就有福了
        //因为如果在上个循环的时候p的flag==1,而且这次循环中输入序列不为@(即不为NULL)
        //那么p的flag会被更新为2然后再压栈顶元素，此时p未更新
        //最后到这一步你刚加进去的栈顶元素就嘎嘣一下被弹出来了。
        //最后记得加上空判定，不然在进行到最后弹根节点的时候会取了个寂寞，直接报段错误
        {
            p=PopBiStack(S);
        }
    }
}

//使用递归的方法创建子树，相对于用栈来说会简单很多
void CreateBiTree(BiTree &T,string Inse,int &index)//这里的index一定要加上引用，否则的话递归不改值
{
    if(Inse[index]=='@')//你就设置好出口，剩下的计算机帮你做
    {
        T=NULL;
        index++;
        return;
    }
    else
    {
        T=new BiNode;
        T->data=Inse[index];
        index++;
        CreateBiTree(T->lchild,Inse,index);//沿着左子树往下走
        CreateBiTree(T->rchild,Inse,index);//沿着右子树往下走
    }
}

// 对于遍历二叉树来说，如果用递归的方法，差别就在于什么时候打印节点

//用递归来先序遍历
void PreOrder(BiTree T)
{
    if(T==NULL) return;//递归出口
    printf("%c",T->data);
    PreOrder(T->lchild);
    PreOrder(T->rchild);
}

//用递归来中序遍历
void InOrder(BiTree T)
{
    if(T==NULL) return;//递归出口
    InOrder(T->lchild);
    printf("%c",T->data);
    InOrder(T->rchild);
}

//用递归来后序遍历
void PostOrder(BiTree T)
{
    if(T==NULL) return;//递归出口
    PostOrder(T->lchild);
    PostOrder(T->rchild);
    printf("%c",T->data);
}

//用栈遍历二叉树会更加麻烦，尤其是在后序遍历

//用栈来先序遍历
void PreOrderByStack(BiTree T)
{
    BiStack S;
    InitBiStack(S);
    if(T==NULL) return;
    BiTree p=T;//类似于链表，尽量不动根节点
    while(p!=NULL||!EmptyBiStack(S))
    {
        if(p!=NULL)
        {
            printf("%c",p->data);//进栈则直接输出
            PushBiStack(S,p);
            p=p->lchild;
        }
        else
        {
            p=PopBiStack(S);//发动败者食尘回溯
            p=p->rchild;
        }
    }
}

//用栈来中序遍历，基本与先序遍历相同
void InOrderByStack(BiTree T)
{
    BiStack S;
    InitBiStack(S);
    if(T==NULL) return;
    BiTree p=T;//类似于链表，尽量不动根节点
    while(p!=NULL||!EmptyBiStack(S))
    {
        if(p!=NULL)
        {
            PushBiStack(S,p);
            p=p->lchild;
        }
        else
        {
            p=PopBiStack(S);//发动败者食尘回溯
            printf("%c",p->data);//先等节点的左子树没了再输出，抓个滚
            p=p->rchild;
        }
    }
}

//用栈来后序遍历，这个究极大慢刀尤其的麻烦
void PostOrderByStack(BiTree T)
{
    BiStack S;
    InitBiStack(S);
    if(T==NULL) return;
    BiTree p=T;
    p->flag=1;
    PushBiStack(S,p);
    p=p->lchild;//先填充一个进栈，不然用do-while也可以的
    while(!EmptyBiStack(S))//千万不要跟前面一样或上一个p!=NULL，后序遍历最后一个是根节点，他肯定不是空的，那你p回到根节点了不就死循环了吗
    {
        if(p!=NULL)
        {
            p->flag=1;//你已经不是纯良了
            PushBiStack(S,p);
            p=p->lchild;
        }
        else
        {
            while(!EmptyBiStack(S)&&GetTopBiStack(S)->flag==2)//通过栈顶元素判断是否已经遍历过两次了，不能用p，原理和上面用栈创建树差不多
            {
                p=PopBiStack(S);
                printf("%c",p->data);
            }
            if(!EmptyBiStack(S))
            {
                p=GetTopBiStack(S);//不能弹，只能取，不然会出BUG，原因和上面栈创建树差不多
                p->flag=2;
                p=p->rchild;
            }
        }
    }
}

int main()
{
    BiTree T;
    T=new BiNode;
    string Inse;
    printf("输入先序拓展遍历序列:");
    cin>>Inse;
    int index=0;
    CreateBiTree(T,Inse,index);//用递归来创建二叉树
    printf("\n用递归先序中序后序遍历的结果分别为:\n");
    PreOrder(T);
    printf("\n");
    InOrder(T);
    printf("\n");
    PostOrder(T);
    printf("\n用栈先序中序后序遍历的结果分别为:\n");
    PreOrderByStack(T);
    printf("\n");
    InOrderByStack(T);
    printf("\n");
    PostOrderByStack(T);
    printf("\n输出完毕\n");
    return 0;
}