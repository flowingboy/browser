// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

// 链接错误"unresolved external symbol _main", 需要注释掉下面的包含文件
// 或者在Project=>setting=>C/C++ Preprocessor中去掉_ATL_MIN_CRT，这个预处理符号的
// 意思是服务器不链接标准的C/C++运行库

//#include <atlimpl.cpp>
