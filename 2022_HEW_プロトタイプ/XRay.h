#pragma once
#include "main.h"
#include "RayInterface.h"

class XRay:public RayInterface
{
public:
	XRay(D3DXVECTOR2 pos, D3DXVECTOR2 playerpos)
		:RayInterface(pos, playerpos) {};
	void Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
};

