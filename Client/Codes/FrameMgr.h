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
	LARGE_INTEGER m_OldTime;		//���� ���� �ð�(Time)
	LARGE_INTEGER m_CurTime;		//���� �� ������ �ð�(Time)
	LARGE_INTEGER m_CpuClock;	//CPU ������(Frequency)

	_double m_DeletaTime;		//��Ÿ Ÿ��(1�� / ���Ƚ��)
	_double m_SecForFrm;		//�ʴ� ��������� ��

	int m_iFPS;						//FPS
	TCHAR szFPS[128];					//FPS ��� ���ڿ�
	_double m_fpsTime;			//


};

#endif // FrameMgr_h__
