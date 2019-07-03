// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//Add Headers
#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>	  //컨테이너 상에서 동작하는 알고리즘
#include <functional> //알고리즘 헤더와 함께 사용하는 함수 헤더
#include <ctime>		 // C 스타일 날짜 및 시간 유틸리티
#include <process.h> //멀티스레드 및 프로세스 작업
#include <iostream>

using namespace std;

#include "MyDefine.h"
//#include "Export_Function.h"

//#include "vld.h"

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#include <crtdbg.h>



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
