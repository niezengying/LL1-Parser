#include "parse.h"
#include "scan.h"

FILE * source;
FILE * listing;

FILE * pfstflw;
FILE * ptable;
FILE * poutput;
FILE * ptree;
FILE * premoveleft;

int lineno=0;
int endchar;
bool EchoSource = true;
bool TraceScan = false;
char Select = '1';

int vt[MAXVTNUM];
int vn[MAXVNNUM];
char Vn[MAXVNNUM][MAXTOKENLEN];
char Vt[MAXVTNUM][MAXTOKENLEN];
char tokenString[MAXTOKENLEN+1];

const char *pfstflwfile = "./SynFstFlw.txt";
const char *ptablefile = "./SynTable.txt";
const char *poutputfile = "./SynProcess.txt";
const char *ptreefile = "./SynTree.txt";
const char *premoveleftfile = "./SynRmvlrec.txt";

void hint(){
	while(true){
	cout<<"请选择文法类型:"<<endl<<"\t1-C_Minus文法；2-通用文法；3-退出程序。"<<endl;
	cout<<"请选择：";
	cin>>Select;
	if(Select == '1'|| Select == '2' || Select == '3') break;
	}
	if(Select == '3') exit(1);
}
void openwtfile()
{
	if(!(pfstflw = fopen(pfstflwfile,"w") ) ) {
		cout<<"无法打开First/Follow输出文件 "<<pfstflwfile<<endl;
		exit(1);
	}
	if(!(ptable = fopen(ptablefile,"w") ) ) {
		cout<<"无法打开分析表输出文件 "<<ptablefile<<endl;
		exit(1);
	}
	else{
		fprintf(ptable,"LL(1)分析表如下：\n");
	}
	if(!(poutput = fopen(poutputfile,"w") ) ) {
		cout<<"无法打开分析过程输出文件 "<<poutputfile<<endl;
		exit(1);
	}
	if(!(ptree = fopen(ptreefile,"w") ) ) {
		cout<<"无法打开分析过程输出文件 "<<ptreefile<<endl;
		exit(1);
	}
	if(!(premoveleft=fopen(premoveleftfile,"w"))){
		cout<<"无法打开左递归消除输出文件 "<<pfstflwfile<<endl;
		exit(1);
	}
}

int main()
{
	char filename[200];
	char filename2[200];
	cout<<"\t\t\t欢迎来到本LL(1)文法分析程序"<<endl;
	cout<<"*******************************************************************************"<<endl;
	//cout<<"\t\t消除左递归后文法 存于文件："<<ptablefile<<endl;
	cout<<"\t\tFirst/Follow 集存于文件："<<ptablefile<<endl;
	cout<<"\t\t分析表 信息存于文件："<<ptablefile<<endl;
	cout<<"\t\t分析栈 信息存于文件:"<<poutputfile<<endl;
	cout<<"\t\t分析树 信息存于文件:"<<ptreefile<<endl;
	cout<<"*******************************************************************************"<<endl;
	cout<<endl;
		lineno = 0;
		hint();
		grammar ga;
		SynStack ss;
		openwtfile();
		cout<<"\n请输入文法文件名:  ";
		cin>>filename;
		ga.removeLeftRecursion(filename,premoveleftfile);
		ga.three_to_two();

		ga.prepareform();
		fclose(pfstflw);

		ga.buildform();
		ga.printform();
		fclose(ptable);

		cout<<endl<<"请输入源程序文件名:";
		cin>>filename2;
		if(!(source = fopen(filename2,"r") )){
			cout<<"无法打开源程序文件 "<<filename2<<endl;
			exit(1);
		}

		listing = stdout;
		ga.buildProcess(ss);
		
		fclose(poutput);
		fclose(ptree);

	system("pause");
	return 0;
}