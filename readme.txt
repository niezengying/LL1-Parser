
*****注意：请保证所有文件名 不含有空格******
操作步骤：

	打开程序"语法分析手工构造.exe"
输入：	根据提示 

	若选择“1”：文法名 请输入“C-文法.txt”。
		     源文件名 可选“C-错误程序.txt”或“C-正确程序.txt”

	若选择“2”：文法名 请输入“Demo文法.txt”。
		     源文件名 可选“Demo错误程序.txt”或“Demo正确程序.txt”
		
		//S文法中ABC Follow ( ABC' ) = { } 是因为没有文法用到ABC。
		//按书上的方式提取 结果正确。

输出：	消除左递归后的文法文件：SynRmvlrec.txt
	First/Follow 集存于文件：SynFstFlw.txt
	分析表 信息存于文件：SynTable.txt
	分析栈 信息存于文件:SynProcess.txt
	分析树 信息存于文件:SynTree.txt

