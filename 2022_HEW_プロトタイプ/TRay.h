#pragma once
#include "main.h"
#include "RayInterface.h"


class TRay :public RayInterface
{
public:
	TRay(D3DXVECTOR2 pos, D3DXVECTOR2 playerpos);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
};

