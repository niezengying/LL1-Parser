#include<fstream>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>	//isspace
#include <string.h>
using namespace std;

//ȫ�ֱ�������

#define MAXTOKENLEN 50
#define MAXVTNUM 100
#define MAXVNNUM 100
#define BUFLEN 256

extern int endchar;
extern FILE * source;	//source = fopen(yuanchengxu,"r")
extern FILE * listing;	//listing = stdout;

extern FILE * pfstflw;
extern FILE * ptable;
extern FILE * poutput;
extern FILE * ptree;
extern FILE* premoveleft;

extern char tokenString[MAXTOKENLEN+1];
extern int lineno;
extern bool EchoSource;	//Դ�ļ�
extern bool TraceScan;	//��������
extern char Select;
extern int indent;

extern int search(int *,int);
extern char Vn[MAXVNNUM][MAXTOKENLEN];
extern char Vt[MAXVTNUM][MAXTOKENLEN];
extern int vt[MAXVTNUM];
extern int vn[MAXVNNUM];
