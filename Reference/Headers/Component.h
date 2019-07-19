#ifndef Component_h__
#define Component_h__

#include "Base.h"
#include "Engine_Define.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CComponent :
	public CBase
{
protected:
	explicit CComponent(void);
	virtual ~CComponent(void);

public:
	virtual _int Update_Component(const _double& TimeDelta) { return 0; };
	virtual void Late_Update_Component() {};
	virtual CComponent* Clone() { return nullptr; }

protected:
	virtual void Free()PURE;

};

NS_END
#endif // Component_h__
