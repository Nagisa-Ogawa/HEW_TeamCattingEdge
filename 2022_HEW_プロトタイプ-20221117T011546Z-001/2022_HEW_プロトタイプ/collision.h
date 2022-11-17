#pragma once

#include "main.h"

bool HitCheckBox(D3DXVECTOR2 box1pos, float box1width, float box1height,
	D3DXVECTOR2 box2pos, float box2width, float box2height);

void Collision_Bullet_Enemy(void);
