/* ��׼ͷ�ļ� */
#ifndef __STDEFINE_H__
#define __STDEFINE_H__

#if defined(WIN32)
#include <windows.h>
#else
#define TRUE     1
#define FALSE    0
typedef int            BOOL;
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#endif

#endif


