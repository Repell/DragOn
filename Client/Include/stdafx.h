// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
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
#include <algorithm>	  //�����̳� �󿡼� �����ϴ� �˰���
#include <functional> //�˰��� ����� �Բ� ����ϴ� �Լ� ���
#include <ctime>		 // C ��Ÿ�� ��¥ �� �ð� ��ƿ��Ƽ
#include <process.h> //��Ƽ������ �� ���μ��� �۾�
#include <iostream>

using namespace std;

#include "MyDefine.h"
//#include "Export_Function.h"

//#include "vld.h"

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#include <crtdbg.h>



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
