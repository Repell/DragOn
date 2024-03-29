#ifndef FrameMgr_h__
#define FrameMgr_h__

class CFrameMgr
{
public:
	CFrameMgr();
	~CFrameMgr();

public:
	void InitFrm(_double Frm);
	bool LockFrm();
	void RenderFrm();

private:
	LARGE_INTEGER m_OldTime;		//측정 시작 시간(Time)
	LARGE_INTEGER m_CurTime;		//측정 후 지나간 시간(Time)
	LARGE_INTEGER m_CpuClock;	//CPU 진동수(Frequency)

	_double m_DeletaTime;		//델타 타임(1초 / 재생횟수)
	_double m_SecForFrm;		//초당 재생프레임 수

	int m_iFPS;						//FPS
	TCHAR szFPS[128];					//FPS 출력 문자열
	_double m_fpsTime;			//


};

#endif // FrameMgr_h__
