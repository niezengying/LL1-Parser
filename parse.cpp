#include "parse.h"
#include "globals.h"

int indent = 0;

bool grammar::testLeftRecursion(char gramprow[][50],int n)//检查是否存在左递归
{	
	int i;
	for(i=1;i<n;i++){
		if(i == 1){
			if(!strcmp(gramprow[0],gramprow[1])) return true;
		}
		if(!strcmp(gramprow[i],"|")){
			if(i+1 < n)
				if(!strcmp(gramprow[0],gramprow[ i+1])) return true;
		}
	}
	return false;
}
void grammar::writeSyntax(char *nonterm,char tmpgram[][50],int n,bool hasCur,FILE* file)//写入文件
{
	int i;
	//cout<<endl;
	if(hasCur){
		if(strcmp(nonterm,tmpgram[0])){
			fprintf(file,"%s -> ",nonterm);
			if(strcmp(empty,tmpgram[0])) for(i = 0;i < n;i++) fprintf(file,"%s ",tmpgram[i]);
			fprintf(file,"%s' $\n",nonterm);
		}
		else{
			fprintf(file,"%s' -> ",nonterm);
			for(i = 1;i < n;i++) fprintf(file,"%s ",tmpgram[i]);
			fprintf(file,"%s' $\n",nonterm);
		}
	}
	else{
		fprintf(file,"%s -> ",nonterm);
		for(i = 0;i < n;i++) fprintf(file,"%s ",tmpgram[i]);
		fprintf(file,"$\n");
	}
	//fclose(file);
}
void grammar::removeLeftRecursion(char *gramfilename,const char* gramfilename2)	//消除左递归
{
	char buffer[BUFLEN],buf1[BUFLEN];
	int i,j,k,l,len;
	char gramprow[20][50],strs2[20][50];
	bool firstline = true;
	bool firstsyntax = true;
	bool hasCur;
	FILE *fp;
	FILE* file;

	if(!(fp = fopen(gramfilename,"r"))){
		printf("can not open syntax1");
		system("pause");
		exit(1);
	}
	if(!(file = fopen(gramfilename2,"w"))){
		printf("can not open syntax2");
		system("pause");
		exit(1);
	}
	else{
		fprintf(premoveleft,"消除左递归后的文法表示如下：\n");
	}
	while(fgets(buffer,BUFLEN-1,fp))
	{
		hasCur = false;
		len = strlen(buffer);

		if(firstline){
			buffer[len - 1] = 0;
			strcpy(empty,buffer);
			firstline = false;
			continue;
		}
		
		if(buffer[len-1] != '\n')
		{
			buffer[len] = '\n';
			buffer[len + 1] = 0;
			len = len + 1;
		}
		i = j = k = 0;
		//提取产生式左部;		
		while(i < len){
			if(isspace(buffer[i])){
				buf1[j] = '\0';
				j = 0;
				if(strlen(buf1) > 0) strcpy(gramprow[k++],buf1);
			}
			else if(buffer[i] == '>'){
				i++;
				break;
			}
			else buf1[j++] = buffer[i];
			i++;
		}
		//提取每一个产生式选择;
		j = 0;
		while(i < len){
			if(isspace(buffer[i])){
				buf1[j] = 0;
				if(strlen(buf1) > 0) strcpy(gramprow[k++],buf1);
				j = 0;
			} // end if
			else buf1[j++] = buffer[i];
			i++;
		}		
		strcpy(gramprow[k++],"|");
		if(firstsyntax == true)
		{
			strcpy(Begin,gramprow[0]);
			firstsyntax  = false;
		}
		i = 1;
		hasCur = testLeftRecursion(gramprow,k);
		while(i < k){
				j = i;
				l = 0;
				while(strcmp(gramprow[j],"|")){
					strcpy(strs2[l++],gramprow[j++]);						
				}
				//写入文件;
				writeSyntax(gramprow[0],strs2,l,hasCur,file);
				i = j+1;
			//}
		}// end while
		//
		if(hasCur){
			fprintf(file,"%s' -> ",gramprow[0]);
			fprintf(file,"%s $\n",empty);
		}
	}// end while fgets
	fclose(file);
	count = read_txt(gramfilename2);
	
	Get_Nonterminal_Terminal();
}

int grammar::read_txt(const char* rmvleftcsion)
{
	FILE* myfile=fopen(rmvleftcsion,"r");
	char buff[256];
	int numrow=0,numcol=1;
	memset(three_d_gram,0,sizeof(three_d_gram));
	while(fscanf(myfile,"%s",buff)==1)
	{
		if(strcmp(buff,"->")==0)continue;
		if(strcmp(buff,"$")==0)
		{
			three_d_gram[numrow][0][0] = numcol-1;
			numrow++;
			numcol=1;
			continue;
		}
		strcpy(three_d_gram[numrow][numcol++],buff);
	}
	fclose(myfile);
	return numrow;
}
int grammar::Search(char source[][MAXTOKENLEN],char obj[])
{
	int count = source[0][0];
	for(int i=1;i<=count;i++)
		if(strcmp(source[i],obj)==0)return i;
	return -1;
}
void grammar::Get_Nonterminal_Terminal()
{
	int k = 0;
	memset(Vn,0,sizeof(Vn));
	memset(Vt,0,sizeof(Vt));
	Vn[0][0]=0;
	for(int i=0;i<count;i++)
	{
		if(Search(Vn,three_d_gram[i][1])==-1)
		{
			strcpy(Vn[++k],three_d_gram[i][1]);
			if(!strcmp(Begin,Vn[k])) startsym=k;
			Vn[0][0]++;
		}
	}
	Vt[0][0] = 0;
	for(int i=0;i<count;i++)
	{
		int col = three_d_gram[i][0][0];
		for(int j=1;j<=col;j++)
		{
			if(strcmp(three_d_gram[i][j],"->")==0)continue;
			if(Search(Vn,three_d_gram[i][j])==-1 && Search(Vt,three_d_gram[i][j])==-1)
			{
				int cur = Vt[0][0];
				strcpy(Vt[cur+1],three_d_gram[i][j]);
				if(!strcmp(Vt[cur+1],empty)) emptychar=cur+71;
				++cur;
				Vt[0][0]=cur;
			}
		}
	}
}
void grammar::three_to_two()
{
	for(int i=0;i<count;i++)
	{
		int colmn = gram[i][0]=three_d_gram[i][0][0];
		for(int j=1;j<=colmn;j++)
		{
			int k = Search(Vn,three_d_gram[i][j]);
			if(k!=-1)gram[i][j]=k;
			else
			{
				gram[i][j] = Search(Vt,three_d_gram[i][j]) + 70;
			}
		}
	}
	Relate();
}
void grammar::Relate()
{
	vn[0] = Vn[0][0];
	for(int i=1;i<=vn[0];i++)
	{
		vn[i]=i;
		//cout<<(int)vn[i]<<" ";
	}
	//cout<<endl;
	vt[0] = Vt[0][0];
	for(int i=1;i<=vt[0];i++)
	{
		vt[i]=i+70;
	}
	endchar = vt[0]+71;
	strcpy(Vt[vt[0]+1],"$");
}

bool grammar::addFirst(int indexA,int indexX,int flag)	//在first中的索引
{	
	//flag=0,不加@，flag=1,加@;
	bool changed = false;
	int i;
	int numA = first[indexA][0];
	for(i=1;i<=first[indexX][0];i++)
	{	
		//flag==0时跳过 '@';
		if(flag == 0 && first[indexX][i] == emptychar) continue;

		if(!search(first[indexA],first[indexX][i])){
			numA = first[indexA][0];
			first[indexA][numA+1]=first[indexX][i];
			first[indexA][0]++;;
			changed = true;
		}	
	}
	return changed;
}
bool grammar::addFollow(int indexA,int indexX,int flag)
{
	//flag=0,add first(abcde); flag=1,add follow(A)
	bool changed = false;
	int i;
	int numA = follow[indexA][0];
	if(!flag)
	{
		for(i=1;i<=first[indexX][0];i++)
		{
			if(!search(follow[indexA],first[indexX][i]) && first[indexX][i] != emptychar)//不加@
			{
				int numfollow = follow[indexA][0];
				follow[indexA][numfollow+1]=first[indexX][i];
				follow[indexA][0]++;
				changed=true;
			}
		}//end for
	}
	else
	{
		for(i=1;i<=follow[indexX][0];i++)
		{
			if(!search(follow[indexA],follow[indexX][i]))
			{
				numA = follow[indexA][0];
				follow[indexA][numA+1]=follow[indexX][i];
				follow[indexA][0]++;
				changed = true;
			}
		}//end for
	}
	return changed;
}
void grammar::buildfirst(){//对(非)终结符建立对应的first集合
	int i,k;
	int indexFvt=1;//vt 索引

	int indexA,indexX;
	bool changed;
	bool Continue;
	int numfirst = vt[0]+vn[0];//first集的个数
	int cnt = 0;
	for(i=0;i<numfirst;i++) first[i][0]=0;

	//终结符的fisrt集合;
	//存放在最后vt[0]
	for(i=vn[0];i<vn[0]+vt[0];i++){
		indexFvt=i-vn[0]+1;	//i=vn[0]时，传vt[1]
		first[i][0]=1;
		first[i][1]=vt[indexFvt];
	}

	//非终结符的first集合;
	do{
		//printf("第%d遍\n",++cnt);
		changed = false;
		for(i=0;i<count;i++){
			k=2;	//k: idx of Xi
			Continue = true;
			while(Continue == true && k <= gram[i][0])//此处与书上其实一致，n=gram[i][0]-1
			{							// A -> a1 a2 a3 ...ak...an
				indexA = search(vn,gram[i][1])-1;	//非终结符A在first中的行下标+1;
				if( indexX=search(vt,gram[i][k]) ) 
					indexX=indexX+vn[0]-1;	//Xk为终结符,在first的行下标为search(vt,gram[i][k])+vn[0]-1;
				else indexX = search(vn,gram[i][k])-1;	//Xk为非终结符,在first的行下标为search(vn,gram[i][k])-1;

				changed |= addFirst(indexA,indexX,0);
				if(!search(first[indexX],emptychar)){	//@不在first(Xk)里;
					Continue = false;
				}
				k++;
			}// end while
			if(Continue == true){ //加 @ 到 first(A)，如果@原来不在first(A)中;
				changed |= addFirst(indexA,indexX,1);
			}// end if Continue
		} // end for i = 0 to count-1
	}while(changed);
	//for(i=0;i<first[0][0];i++)
	//cout<<Vt[first[0][i+1]]<<" ";
	//system("PAUSE");
}
void grammar::buildfollow(void)//对指定非终结符建立follow集合
{
	int i,j,k,m,t;
	int indexA,indexX;
	bool changed;
	if(t=search(vn,startsym))//把$字符放入开始符的follow集合
	{					  //默认为t=1
		follow[t-1][1] = endchar;
		follow[t-1][0]=1;
	}
	for(i=1;i<=vn[0];i++)
	{
		if(vn[i]!=startsym)
			follow[i-1][0]=0;
	}
	do
	{
		changed=false;
		for(i=0;i<count;i++)//i==no. of production
		{
			indexA=search(vn,gram[i][1])-1;
			for(j=2;j<=gram[i][0];j++)//for each Xi,where Xi in A -> X1 X2 ... Xn
			{
				if(k=search(vn,gram[i][j]))
				{
					for(m=j+1;m<=gram[i][0];m++)
					{
						if(indexX=search(vt,gram[i][m])) 
							indexX=indexX+vn[0]-1;	//Xk为终结符,在first的行下标;
						else indexX = search(vn,gram[i][m])-1;	//Xk为非终结符,在first的行下标;

						changed |= addFollow(k-1,indexX,0);
						if(!(search(first[indexX],emptychar)))
							break;
					}
					if(m > gram[i][0])//正常退出，非break;
					{				//@在First(Xi+1Xi+2...Xn)
						changed |= addFollow(k-1,indexA,1);
					}
				}
			}
		}//end for
	}while(changed);
}
void grammar::prepareform()
{
	int i,j;
	buildfirst();
	buildfollow();
	fprintf(pfstflw,"各非终结符first集合如下：\n");
	//cout<<endl<<"各非终结符first集合如下："<<endl;
	
	for(i=0;i<vn[0];i++)
	{
		fprintf(pfstflw,"First( %s ) = { ",Vn[i+1]);
		//cout<<Vn[i+1]<<":  ";
		for(j=1;j<=first[i][0];j++)
		{
			fprintf(pfstflw,"%s ",Vt[first[i][j]-SETOFF]);
		}
		fprintf(pfstflw,"}\n");
	}

	fprintf(pfstflw,"\n\n各非终结符follow集合如下：\n");
	for(i=0;i<vn[0];i++)
	{
		fprintf(pfstflw,"Follow ( %s ) = { ",Vn[i+1]);
		for(j=1;j<=follow[i][0];j++)
		{
			fprintf(pfstflw,"%s ",Vt[follow[i][j]-SETOFF]);	
		}
		fprintf(pfstflw,"}\n");
	}
	fprintf(pfstflw,"\n");
}

void grammar::buildform(){
	int i,j,l,n,t;
	bool flag;
	int indexX;

	//初始化form;
	for(i=0;i<MAXVNNUM;i++)//用vn[0]
		for(j=0;j<MAXVTNUM;j++)//vt[0]+1
			form[i][j] = -1;

	for(n=0;n<vn[0];n++){	//for each nonterminal A
		//在gram里找到第一个A-
		for(j=1;j<=first[n][0];j++){	// 对First(A)的每个元素a (=first[n][j])
			if(first[n][j] == emptychar) continue;	//跳过First(A)里的@

				///for each A->X
				//flag = false;
			for(i=0;i<count;i++)
			{
				if(gram[i][1] != vn[n+1])	continue;
				flag = false;
				//for each Xi,where Xi in X=X1 X2 ..Xi ..Xn
				for(l=2;l<=gram[i][0];l++)
				{
					if(gram[i][l] == emptychar) break;
					// 找出 Xi 在 first集合的行下标;
					if(indexX=search(vt,gram[i][l])) indexX=indexX+vn[0]-1;	//Xk为终结符,在first的行下标为l;
					else indexX = search(vn,gram[i][l])-1;	//Xk为非终结符,在first的行下标为l;
					//printf("Xi = %c indexX = %d\n",gram[k][l],indexX);

					//如果a在Xi的First集合里,则添加A->X到 form里;
					if(search(first[indexX],first[n][j]))//A->X1X2..  判断first(A)与first(X1)是否有交集
					{
						int idx_a = search(vt,first[n][j])-1;	//终结符a在form的列下标;
						form[n][idx_a] = i;
						flag = true;
						break;
					}
					else if(search(first[indexX],emptychar))	//a不在First(Xi)集合,但 @ 在First(Xi);
					{
						continue;
					}
					else	//a和@都不在First(Xi)里
					{
						break;
					}
				}
				if(flag == true) break;
			}// end for k
		} // end for a in First(A)
		if(search(first[n],emptychar))	//如果@在First(A),则考虑Follow(A)的元素;
		{
			for(j=1;j<=follow[n][0];j++)	//对Follow(A)中的元素b
			{
				int idx_b = search(vt,follow[n][j])-1;
				if(idx_b >= 0)	// b 是终结符;
				{
					if(form[n][idx_b]==-1)	//就近原则
						form[n][idx_b] = -2;
				}
				else	//b 是 $
				{
					idx_b = vt[0];	//	放$在最后一列;
					form[n][idx_b] = -2;
				}
			}
		}
	}//end for n
	for(n=0;n<vn[0];n++)		//错误处理，当有误时弹出错误结点
	{
		if(form[n][vt[0]] == -1)		//$列赋值
			form[n][vt[0]]=-3;
		for(t=0;t<vt[0];t++)
		{
			if(search(follow[n],vt[t+1]) && form[n][t] == -1)
			{
				form[n][t] = -3;
			}
		}
	}
}
void grammar::printform()
{
	for(int i=0;i<vn[0];i++)
	{
		fprintf(ptable,"%d\n",i+1);
		for(int j=0;j<=vt[0];j++)
		{
			fprintf(ptable,"  Vn: %s",Vn[i+1]);
			printt(Vn[i+1],ptable);
			fprintf(ptable,"Vt: %s",Vt[j+1]);
			printt(Vt[j+1],ptable);			
			if(form[i][j]==-2)
			{
				fprintf(ptable,"Action: %s -> %s\n",Vn[i+1],empty);
			}
			else if(form[i][j]==-1)
			{
				fprintf(ptable,"Action: scan\n");
			}
			else if(form[i][j]==-3)
			{
				fprintf(ptable,"Action: pop\n");
			}
			else
			{
				int curindex = form[i][j];
				int tmpcount = three_d_gram[curindex][0][0];
				for(int k=1;k<=tmpcount;k++)
				{
					if(k==1)
					{fprintf(ptable,"Action: %s -> ",three_d_gram[curindex][k]);}//cout<<three_d_gram[curindex][k]<<" -> ";}
					else 
					{fprintf(ptable,"  %s ",three_d_gram[curindex][k]);}//cout<<three_d_gram[curindex][k]<<" ";}
				}
				fprintf(ptable,"\n");
			}
		}
	}
}
void grammar::buildProcess(SynStack& ss)
{
	int A,ch;
	int i,j,m,n;
	bool correct = true;

	//初始化分析树
	parseTree = (TreeNode *)malloc(sizeof(TreeNode));
	parseTree ->name = startsym;
	parseTree ->fchild = NULL;
	parseTree ->parent  = NULL;
	parseTree ->sibling = NULL;
	parseTree ->isterm = false;	//startsym为Vn

	TreeNode *pch = NULL;	//指向根结点的指针
	TreeNode *curNode = NULL;	//游标结点指针
	curNode=parseTree;

	fprintf(poutput,"LL(1)预测分析过程如下所示:\n\n");
	cout<<endl<<"LL(1)预测分析:\n";
	fprintf(ptree,"LL(1)分析树如下所示：\n\n");

    ss.Push(endchar);
	ss.Push(startsym);
	ss.printStack();
	ch = Get();	//获取终结符
	layoutCh(ch);

	while(ss.getTop(A) && A != endchar)
	{
		if(m =search(vt,A) && ch == A) 
		{	//Mactch;
			fprintf(poutput,"Action: Match\n");
			ss.Pop();
			ss.printStack();
			ch = Get();
			layoutCh(ch);
		}		
			
		else if(m=search(vn,A))
		{
			n = search(vt,ch);
			if(n == 0)
			{//ch == $
				n = vt[0]+1;
			}
			if((i = form[m-1][n-1]) != -1 && i != -3)	//产生
			{//Generate;
				if(i == -2){	//A->@
					fprintf(poutput,"Action: %s -> %s\n",Vn[m],empty);
					ss.Pop();
				}	
				else{
					fprintf(poutput,"Action: %s ->",three_d_gram[i][1]);
					ss.Pop();
					for(j = gram[i][0];j>=2;j--){
						if(gram[i][j] != emptychar) 
						{
							ss.Push(gram[i][j]);
						}
						fprintf(poutput," %s",three_d_gram[i][gram[i][0]-j+2]);
					}
					///////////////////////////////////////////////////////////////////////////////

					if(gram[i][2] != emptychar){	
						curNode = findNode(curNode,gram[i][1]);

						for(j = 2;j <= gram[i][0];j++)
						{
							if(curNode->fchild == NULL)
							{
								pch = (TreeNode *)malloc(sizeof(TreeNode));
								curNode->fchild = pch;
							}
							else
							{
								pch->sibling = (TreeNode *)malloc(sizeof(TreeNode));
								pch = pch->sibling;
							}
							pch->fchild = NULL;
							pch->name = gram[i][j];
							if(search(vt,gram[i][j])) pch->isterm = true;
							else pch->isterm = false;//changed;
							pch->parent = curNode;
							pch->sibling = NULL;
						}
						curNode = curNode->fchild;
					}// end if

					/////////////////////////////////////////////////////////////////////////////////////
					fprintf(poutput,"\n");
				}
				ss.printStack();
				layoutCh(ch);
			}
			else if(i == -1)
			{
				correct = false;
				cout<<"Syntax error :  "<<"Lineno:"<<lineno<<"  SCAN unpredicted token -> "<<Vt[ch-70]<<endl;
				fprintf(poutput,"\tSyntax error :  %d  :  SCAN unpredicted token -> %s\n\t",lineno,Vt[ch-70]);
				ch = Get();
			}
			else if(i == -3)
			{
				correct = false;
				cout<<"Syntax error :  "<<"Lineno:"<<lineno<<"  POP unpredicted token -> "<<Vt[ch-70]<<endl;
				fprintf(poutput,"\tSyntax error :  %d  :  POP unpredicted token -> %s\n\t",lineno,Vt[ch-70]);
				ss.Pop();
			}
			else{//error;
				cout<<"Syntax error : Lineno:"<<lineno<<"error terminal！"<<endl;
				//fprintf(poutput,"Syntax error :  "<<"Lineno:"<<lineno<<" unpredicted token \n");
				system("pause");
				exit(1);
			}
		}

		else {
			cout<<"Syntax error :  Lineno:"<<lineno<<" error token！"<<endl;
			system("pause");
			exit(1);
		}
	}//end while
	if(ss.getTop(A) && A == endchar && ch == endchar)
	{
		fprintf(poutput,"Accept\n");
		printTree(parseTree,1);
	}
	if(ss.getTop(A) && A == endchar && ch == endchar && correct)
	{
		cout<<endl<<"Syntax Correct !"<<endl;
	}
	else{
		//cout<<"Eorror3:非法结束符！"<<endl;
		//system("pause");
	}
}
int grammar::Get()
{
	//char reChar='0';
	int reChar = 0;
	TokenType token  = getToken();
	if(Select == '1')
	{
	switch( token ){
	case ID:
		reChar=Search(Vt,"ID");
		break;
	case NUM:	
		reChar =Search(Vt,"NUM");
		break;
	case ENDFILE:
		reChar = vt[0]+1;
		break;
	default:
		reChar =Search(Vt,tokenString);
		break;	
	} // end switch
	 
	}//end if
	else{
		if(token == ENDFILE) 
			reChar = vt[0]+1;
		else 
			reChar =Search(Vt,tokenString);
		//cout<<reChar<<endl;
	}
	return reChar+SETOFF;
}

bool SynStack::getTop(int &x)
{
	if(IsEmpty()==true)return false;
	x=elements[top];
	return true;
}
void SynStack::Push(int x)
{
	if(IsFull())
	{
		cout<<"栈满越界!"<<endl;
		system("pause");
		exit(1);
	}
	elements[++top]=x;
}
bool SynStack::Pop()
{
	if(IsEmpty())
		return false;
	top--;
	return true;
}
bool SynStack::IsEmpty()
{
	return (top==-1) ? true : false;
}
bool SynStack::IsFull()
{
	return (top == MAXELEMENTNUM-1)?true:false;
}
void SynStack::printStack(){
	int i;
	fprintf(poutput,"\n");
	for(i = 0;i<=top;i++)
	{
		int j;
		if(j = search(vn,elements[i])) 
		{
			fprintf(poutput,"%s ",Vn[j]);
		}
		else if(j = search(vt,elements[i]))
		{
			fprintf(poutput,"%s ",Vt[j]);
		}
		else 
		{
			fprintf(poutput,"$ ");
		}
	}
	outputblank(poutput,0);
	fprintf(poutput,"\n");
}

int search(int *a,int ch)//搜索函数,用于在指定数组中对指定字符进行搜寻,若存在输出对应的序号,若不在则输出0
{
	for(int i=1;i<=a[0];i++)
	{
		if(ch==a[i])
			return i;
	}
	return 0;
}
void layoutCh(int ch)
{
	outputblank(poutput,5);
	fprintf(poutput,"Input: %s\n",Vt[ch-70]);
	outputblank(poutput,5);
}
void printt(string str,FILE* myfile)
{
	int mycount;
	if(str.size()<4)mycount=4;
	else if(str.size()<=9)mycount = 3;
	else if(str.size()<18)mycount = 2;
	else mycount = 1;
	for(int i=0;i<mycount;i++)
		fprintf(myfile,"\t");
}
void outputblank(FILE *output,int n){
	int i;
	for(i=0;i<n;i++) fprintf(output," ");
}

void printTree(TreeNode *curNode,int level){
	TreeNode *pch;
	int k;
	if(curNode == parseTree) 
	{
		fprintf(ptree,"%d ",level);
		if(k=search(vn,curNode->name)) 
		{
			fprintf(ptree,"%s \n",Vn[k]);
		}
		else
		{
			k = search(vt,curNode->name);
			fprintf(ptree,"%s \n",Vt[k]);
		}
	}
	indent += 3;
	for(pch = curNode->fchild;pch != NULL;pch = pch->sibling)
	{
		outputblank(ptree,indent);
		fprintf(ptree,"%d ",level+1);
		if(k=search(vn,pch->name)) 
		{
			fprintf(ptree,"%s \n",Vn[k]);
		}
		else
		{
			k = search(vt,pch->name);
			fprintf(ptree,"%s \n",Vt[k]);
		}
		if(pch->isterm == false)
		{
			printTree(pch,level+1);
		}	
	}
	indent -= 3;
}
TreeNode *findNode(TreeNode *curNode,char name){
	TreeNode *tmpPt = curNode;

	if(curNode->isterm){	//Need to back track;
		while(curNode != NULL){
			for(tmpPt = curNode->sibling;tmpPt != NULL;tmpPt = tmpPt->sibling)
			{
				if(tmpPt->name == name)
					break;
			}
			if(tmpPt == NULL)
			{
				curNode=curNode->parent;
			}
			else break;
		}// end while

		if(curNode == NULL){
			system("PAUSE");
			exit(1);
		}
	}
	return tmpPt;
}