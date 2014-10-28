#include "globals.h"
#include "scan.h"
#include <malloc.h>

//语法分析程序
//类 结构体声明   Grammer, SynStack，TreeNode

#define SETOFF 70
#define MAXSYNNUM 200	//最大文法个数
#define MAXLENTH 50		//文法最大长度		
#define MAXFIRSTNUM 200	//first终结符最大个数
#define MAXFOLLOWNUM 100
#define MAXELEMENTNUM 200	//栈

class SynStack;//声明栈类
class grammar//定义grammar类,消除左递归，非终结符集合,终结符集合,first/follow集合,预测分析表,等
{
public:
	friend SynStack;//定义SynStack类,这个类的主要作用是在对语句的预测分析过程中作符号栈
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
	//连接 词法与语法 的函数
private: 
	int count;
	int emptychar;
	int startsym;	//开始符号
	char empty[MAXTOKENLEN];
	char Begin[MAXTOKENLEN];
	int gram[MAXSYNNUM][MAXLENTH];
	int first[MAXVTNUM+MAXVNNUM][MAXFIRSTNUM];//这时所有非终结符的first集合
	int follow[MAXVNNUM][MAXFOLLOWNUM];//这是所有非终结符的follow集合
	int form[MAXVNNUM][MAXVTNUM];//这是对应文法的预测分析表*/
   
	bool addFirst(int,int,int);
	bool addFollow(int,int,int);
	void buildfirst();
	void buildfollow(); 
};

class SynStack//定义SynStack类,这个类的主要作用是在对语句的预测分析过程中作符号栈
{
public:
	friend grammar;	//grammar.buildprocess 使用element[];
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

struct TreeNode{	//分析树的生成
	char name;
	bool isterm;
	struct TreeNode *parent;
	struct TreeNode *fchild;	//第一个孩子结点
	struct TreeNode *sibling;	//兄弟结点
};

int search(int *,int);
void layoutCh(int ch);
void printt(string str,FILE* myfile);
void outputblank(FILE *output,int n);

static TreeNode *parseTree = NULL;
void printTree(TreeNode *curNode,int level);
TreeNode *findNode(TreeNode *pTree,char name);


