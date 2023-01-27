#pragma once
#include <list>
#include "RayInterface.h"

class RayFactory
{
private:
	std::list<RayInterface*> m_pRayList;
public:
	void Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);

	void CreateXRay(D3DXVECTOR2 pos, D3DXVECTOR2 playerpos,bool isDuo);
	void CreateTRay(D3DXVECTOR2 pos, D3DXVECTOR2 playerpos,bool isDuo);
};

