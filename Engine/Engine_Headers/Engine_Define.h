#ifndef Engine_Define_h__
#define Engine_Define_h__

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
