#ifndef Engine_Define_h__
#define Engine_Define_h__

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

//#include <iostream>
using namespace std;

#pragma warning(disable : 4251)

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Engine_Enum.h"

#endif // Engine_Define_h__
