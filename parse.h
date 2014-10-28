#include "globals.h"
#include "scan.h"
#include <malloc.h>

//�﷨��������
//�� �ṹ������   Grammer, SynStack��TreeNode

#define SETOFF 70
#define MAXSYNNUM 200	//����ķ�����
#define MAXLENTH 50		//�ķ���󳤶�		
#define MAXFIRSTNUM 200	//first�ս��������
#define MAXFOLLOWNUM 100
#define MAXELEMENTNUM 200	//ջ

class SynStack;//����ջ��
class grammar//����grammar��,������ݹ飬���ս������,�ս������,first/follow����,Ԥ�������,��
{
public:
	friend SynStack;//����SynStack��,��������Ҫ�������ڶ�����Ԥ�����������������ջ
	grammar()
	{
		count=0;
	}
	///////////////////////////////////////////////////////

	bool testLeftRecursion(char gramprow[][50],int n);
	void removeLeftRecursion(char*,const char*);
	void writeSyntax(char*,char tmpgram[][50],int n,bool,FILE*);
	void Relate();
	void three_to_two();
	void Get_Nonterminal_Terminal();
	int read_txt(const char* rmvleftcsion);
	int Search(char source[][MAXTOKENLEN],char obj[]);
	char three_d_gram[MAXVNNUM][MAXVTNUM][MAXTOKENLEN];
	/////////////////////////////////////////////////////////
	int Get();
	void prepareform();
	void buildform();
	void printform();
	void buildProcess(SynStack& ss); 
	//���� �ʷ����﷨ �ĺ���
private: 
	int count;
	int emptychar;
	int startsym;	//��ʼ����
	char empty[MAXTOKENLEN];
	char Begin[MAXTOKENLEN];
	int gram[MAXSYNNUM][MAXLENTH];
	int first[MAXVTNUM+MAXVNNUM][MAXFIRSTNUM];//��ʱ���з��ս����first����
	int follow[MAXVNNUM][MAXFOLLOWNUM];//�������з��ս����follow����
	int form[MAXVNNUM][MAXVTNUM];//���Ƕ�Ӧ�ķ���Ԥ�������*/
   
	bool addFirst(int,int,int);
	bool addFollow(int,int,int);
	void buildfirst();
	void buildfollow(); 
};

class SynStack//����SynStack��,��������Ҫ�������ڶ�����Ԥ�����������������ջ
{
public:
	friend grammar;	//grammar.buildprocess ʹ��element[];
	SynStack()
	{
		top=-1;
	}
	void Push(int x);
	bool Pop();
	bool getTop(int& x);
	bool IsEmpty();
	bool IsFull();
	void printStack();
private:
	int elements[MAXELEMENTNUM];
	int top;
};

struct TreeNode{	//������������
	char name;
	bool isterm;
	struct TreeNode *parent;
	struct TreeNode *fchild;	//��һ�����ӽ��
	struct TreeNode *sibling;	//�ֵܽ��
};

int search(int *,int);
void layoutCh(int ch);
void printt(string str,FILE* myfile);
void outputblank(FILE *output,int n);

static TreeNode *parseTree = NULL;
void printTree(TreeNode *curNode,int level);
TreeNode *findNode(TreeNode *pTree,char name);


