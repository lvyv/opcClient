// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7B71F903_8400_46ED_8500_5E82675A26B3__INCLUDED_)
#define AFX_STDAFX_H__7B71F903_8400_46ED_8500_5E82675A26B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#define DLL_EXPORT extern "C" __declspec(dllexport)

void SimpleRW();

DLL_EXPORT int sum_fun(int, int);
DLL_EXPORT int __stdcall test_buf(char* buf, int num, char* outbuf);
DLL_EXPORT int __stdcall one_turn_rw();

#endif // !defined(AFX_STDAFX_H__7B71F903_8400_46ED_8500_5E82675A26B3__INCLUDED_)
