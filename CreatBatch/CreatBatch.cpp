// CreatBatch.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <stdlib.h>
#include <memory.h>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <atlstr.h>
#pragma comment(lib, "User32.lib")

void DisplayErrorBox(LPTSTR lpszFunction);

_TCHAR *get_filename_extT(_TCHAR* filename) {
	_TCHAR *dot = _tcsrchr(filename, _T('.'));
	if (!dot || dot == filename) return NULL;
	return dot + 1;
}
int get_filename_extT_(_TCHAR* filename) {
	_TCHAR *dot = _tcsrchr(filename, _T('.'));
	if (!dot || dot == filename) return NULL;
	return (dot - filename);
}

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *pfile;
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	DWORD currentPahtlength = 0;
	PWSTR pstr1 = _T("img_cvt -i \"");
	PWSTR pstr2 = _T("\" -f 7\r\n");
	CString cstr1 = _T("img_cvt -i \"");
	CString cstr2 = _T("\" -f 7\r\n");

	// If the directory is not specified as a command-line argument,
	// print usage.

#if 0//[.4test.
	argc = 2;
	argv[0] = _T("CreadBatch.exe");
	argv[1] = _T("E:\\n\\LCD\\img_cvt_0.6");
	if (argc != 2)
	{
		_tprintf(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
		return (-1);
	}
	LPWSTR pBuf = new TCHAR[100];
	pBuf = argv[1];
#else
	if (argc == 2 && (!_tcscmp(argv[1], _T("-?")) || !_tcscmp(argv[1], _T("-h")) || !_tcscmp(argv[1], _T("-help"))))
	{
		printf("Version 1.00\r\n");
		_tprintf(TEXT("\nUsage: %s\n"), argv[0]);
		printf("This program built for Win8.1x64-Win32\n");
		printf("Report error to kai.cheng.wang@gmail.com\n");
		return 0;
	}
	if (argc != 1)
	{
		_tprintf(TEXT("\nUsage: %s\n"), argv[0]);
		return (-1);
	}
	currentPahtlength = GetCurrentDirectory(0, NULL);
	currentPahtlength += 1;
	//unsigned char *pbuf = new unsigned char[currentPahtlength];
	LPWSTR pBuf = new TCHAR[currentPahtlength];
	dwError = GetCurrentDirectory(currentPahtlength, pBuf);
	if (dwError == 0)
	{
		dwError = GetLastError();
		if (pBuf) delete[] pBuf;
		return dwError;
	}
#endif//].

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(pBuf, MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		if (pBuf) delete[] pBuf;
		return (-1);
	}

	_tprintf(TEXT("\nTarget directory is %s\n\n"), pBuf);

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, pBuf);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
		if (pBuf) delete[] pBuf;
		return dwError;
	}

	dwError = fopen_s(&pfile, "batch1.bat", "w+b");
	if (pfile == NULL)
	{
		if (pBuf) delete[] pBuf;
		return dwError;
	}
	
	// List all the files in the directory with some info about them.
	CString csFname_R;
	CString csFname;
	CHAR charName[260];
	CHAR *charSource;
	int length2;
	int i, j;
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			//if (ffd.cFileName )
			if (_tcsstr(ffd.cFileName, _T(".jpg")))
			{
				csFname_R.Format(_T("%s"), ffd.cFileName);
				csFname_R.Delete(get_filename_extT_(ffd.cFileName), 4);

				_tprintf(TEXT("Get \"%s\"\n"), ffd.cFileName);
				//dwError = fwrite(cstr1, 1, cstr1.GetLength(), pfile); // return size
				//csFname.SetString(ffd.cFileName);
				csFname.Format(_T("img_cvt -i \"%s\" -f 7\r\nswap \"%s_RGB565\\%s.raw\" \"2\"\r\n"), ffd.cFileName, csFname_R, csFname_R);
				charSource = (char*)csFname.GetBuffer(csFname.GetLength());
				length2 = csFname.GetLength() * 2;
				for (i = 0, j = 0; i < length2; i += 2, j++)
				{
					charName[j] = charSource[i];
				}
				//memcpy(charName, csFname, csFname.GetLength());
				dwError = fwrite(charName, 1, csFname.GetLength(), pfile); // return size
				//dwError = fwrite(&cstr2, 1, cstr2.GetLength(), pfile); // return size

			//	_swprintf(pFName, _T("img_cvt -i \"%s\" -f 7\r\n"), ffd.cFileName);
			//	dwError = fwrite(pFName, sizeof(TCHAR), _tcslen(pFName), pfile); // return size
				//pFName = _T("img_cvt -i \"%s\" -f 7\r\n", ffd.cFileName);//(const void *)
				//dwError = fwrite(pFName, sizeof(TCHAR), _tcslen(pFName), pfile); // return size
				//"img_cvt -i \"%s\" -f 7\r\n"
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	fclose(pfile);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);

	if (pBuf) delete[] pBuf;
	return dwError;

#if 0
	FILE *pfile;
	errno_t err;
	size_t size;
	unsigned char *pbuf = NULL, *pcomp = NULL;
	unsigned long  datasize;

#if 0//[.4test.
	argc = 1;
	argv[0] = "CreadBatch.exe";
#endif//].
	if (argc > 1)
		return 0;

	err = fopen_s(&pfile, "batch1.bat", "w+");

	if (pfile == NULL)
		return 0;
#endif
#if 0
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("c:\\src\\")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			printf("%s\n", ent->d_name);
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
		return EXIT_FAILURE;
	}
#endif
    return 0;
}

void DisplayErrorBox(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and clean up

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40)*sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

