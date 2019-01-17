#include "WindowsPath.hpp"

// Code from https://blog.csdn.net/zuishikonghuan/article/details/47441163

#include<windows.h>
//打开保存文件对话框
#include<Commdlg.h>
//获取“我的文档”路径
#include <ShlObj.h>

inline string GetDocumentPath()
{
	char homePath[1024] = { 0 };
	SHGetSpecialFolderPath(nullptr, homePath, 5, false);
	return homePath;
}

inline void InitOFN(OPENFILENAME &ofn, string filter, string title)
{
	ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
	ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
	ofn.lpstrFilter = TEXT(filter.c_str());//设置过滤
	ofn.nFilterIndex = 1;//过滤器索引
	ofn.lpstrTitle = TEXT(title.c_str());//使用系统默认标题留空即可
	ofn.lpstrInitialDir = GetDocumentPath().c_str();//初始目录为我的文档
}

string OpenFile(string filter)
{
	OPENFILENAME ofn = { 0 };
	InitOFN(ofn, filter, "请选择一个文件");

	TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名
	ofn.lpstrFile = strFilename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(strFilename);//缓冲区长度

	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项

	while (!GetOpenFileName(&ofn))
		MessageBox(NULL, TEXT("请选择一个文件"), NULL, MB_ICONERROR);
	
	//MessageBox(NULL, strFilename, TEXT("选择的文件"), 0);
	return strFilename;
}

string SaveFile(string filter)
{
	OPENFILENAME ofn = { 0 };
	InitOFN(ofn, filter, "保存到");
	ofn.lpstrDefExt = TEXT("cpp");//默认追加的扩展名

	TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名
	ofn.lpstrFile = strFilename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(strFilename);//缓冲区长度

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//目录必须存在，覆盖文件前发出警告

	while (!GetSaveFileName(&ofn))
		MessageBox(NULL, TEXT("请输入一个文件名"), NULL, MB_ICONERROR);
	
	//MessageBox(NULL, strFilename, TEXT("保存到"), 0);
	return strFilename;
}