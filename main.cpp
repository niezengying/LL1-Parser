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
	cout<<"��ѡ���ķ�����:"<<endl<<"\t1-C_Minus�ķ���2-ͨ���ķ���3-�˳�����"<<endl;
	cout<<"��ѡ��";
	cin>>Select;
	if(Select == '1'|| Select == '2' || Select == '3') break;
	}
	if(Select == '3') exit(1);
}
void openwtfile()
{
	if(!(pfstflw = fopen(pfstflwfile,"w") ) ) {
		cout<<"�޷���First/Follow����ļ� "<<pfstflwfile<<endl;
		exit(1);
	}
	if(!(ptable = fopen(ptablefile,"w") ) ) {
		cout<<"�޷��򿪷���������ļ� "<<ptablefile<<endl;
		exit(1);
	}
	else{
		fprintf(ptable,"LL(1)���������£�\n");
	}
	if(!(poutput = fopen(poutputfile,"w") ) ) {
		cout<<"�޷��򿪷�����������ļ� "<<poutputfile<<endl;
		exit(1);
	}
	if(!(ptree = fopen(ptreefile,"w") ) ) {
		cout<<"�޷��򿪷�����������ļ� "<<ptreefile<<endl;
		exit(1);
	}
	if(!(premoveleft=fopen(premoveleftfile,"w"))){
		cout<<"�޷�����ݹ���������ļ� "<<pfstflwfile<<endl;
		exit(1);
	}
}

int main()
{
	char filename[200];
	char filename2[200];
	cout<<"\t\t\t��ӭ������LL(1)�ķ���������"<<endl;
	cout<<"*******************************************************************************"<<endl;
	//cout<<"\t\t������ݹ���ķ� �����ļ���"<<ptablefile<<endl;
	cout<<"\t\tFirst/Follow �������ļ���"<<ptablefile<<endl;
	cout<<"\t\t������ ��Ϣ�����ļ���"<<ptablefile<<endl;
	cout<<"\t\t����ջ ��Ϣ�����ļ�:"<<poutputfile<<endl;
	cout<<"\t\t������ ��Ϣ�����ļ�:"<<ptreefile<<endl;
	cout<<"*******************************************************************************"<<endl;
	cout<<endl;
		lineno = 0;
		hint();
		grammar ga;
		SynStack ss;
		openwtfile();
		cout<<"\n�������ķ��ļ���:  ";
		cin>>filename;
		ga.removeLeftRecursion(filename,premoveleftfile);
		ga.three_to_two();

		ga.prepareform();
		fclose(pfstflw);

		ga.buildform();
		ga.printform();
		fclose(ptable);

		cout<<endl<<"������Դ�����ļ���:";
		cin>>filename2;
		if(!(source = fopen(filename2,"r") )){
			cout<<"�޷���Դ�����ļ� "<<filename2<<endl;
			exit(1);
		}

		listing = stdout;
		ga.buildProcess(ss);
		
		fclose(poutput);
		fclose(ptree);

	system("pause");
	return 0;
}