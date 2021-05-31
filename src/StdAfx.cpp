// stdafx.cpp : source file that includes just the standard includes
//	testOpc.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
//#include "SimpleOPCClient_v2.h"

DLL_EXPORT int sum_fun(int a, int b)
{
    return a + b;
}

DLL_EXPORT int __stdcall test_buf(char* buf,
                       int num,
                       char* outbuf)
{
    int i = 0;

    for (i = 0; i < num; ++i)
    {
        outbuf[i] = buf[i] + 1;
    }

    return num;
}

DLL_EXPORT int __stdcall one_turn_rw()
{
    SimpleRW();
	return 0;
}