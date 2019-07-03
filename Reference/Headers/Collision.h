#ifndef Collision_h__
#define Collision_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CCollision :
	public CComponent
{
	enum COLLTYPE
	{
		COL_TERRAIN,
		COL_MOUSE,
		COL_SPHERE,
		COL_END
	};

protected:
	explicit CCollision();
	virtual ~CCollision();


};

NS_END

#endif // Collision_h__
