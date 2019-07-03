#ifndef FrameMgr_h__
#define FrameMgr_h__

class CFrameMgr
{
public:
	CFrameMgr();
	~CFrameMgr();

public:
	void InitFrm(float Frm);
	bool LockFrm();
	void RenderFrm();

private:
	LARGE_INTEGER m_OldTime;		//���� ���� �ð�(Time)
	LARGE_INTEGER m_CurTime;		//���� �� ������ �ð�(Time)
	LARGE_INTEGER m_CpuClock;	//CPU ������(Frequency)

	float m_fDeletaTime;		//��Ÿ Ÿ��(1�� / ���Ƚ��)
	float m_fSecForFrm;		//�ʴ� ��������� ��

	int m_iFPS;						//FPS
	TCHAR szFPS[128];					//FPS ��� ���ڿ�
	float m_fpsTime;			//


};

#endif // FrameMgr_h__
