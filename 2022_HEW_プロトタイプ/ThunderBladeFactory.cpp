#include "ThunderBladeFactory.h"
#include "player.h"
#include "camera.h"
#include "ThunderBlade.h"
#include "texture.h"
#include "Block.h"

ThunderBladeFactory::ThunderBladeFactory()
{
}

void ThunderBladeFactory::Init()
{
	m_pPlayer = GetPlayer();
	m_pCamera = GetCamera();
	m_ThunderBladeNo = LoadTexture((char*)"data/TEXTURE/ThunderBlade.png");
}

void ThunderBladeFactory::Uninit()
{
	for (ThunderBlade* pThunderBlade : m_pThunderBladeList)
	{
		pThunderBlade->Uninit();
	}
	for (ThunderBlade* pThunderBlade : m_pThunderBladeList)
	{
		delete pThunderBlade;
	}
	m_pThunderBladeList.clear();
}

void ThunderBladeFactory::Update()
{
	for (ThunderBlade* pThunderBlade : m_pThunderBladeList)
	{
		pThunderBlade->Update();
	}
}

void ThunderBladeFactory::Draw()
{
	for (ThunderBlade* pThunderBlade : m_pThunderBladeList)
	{
		pThunderBlade->Draw();
	}
}

void ThunderBladeFactory::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	m_pThunderBladeList.push_back(new ThunderBlade(pos, size, m_ThunderBladeNo));
}

void ThunderBladeFactory::CollisionThunderBladeToPlayer()
{
}

bool ThunderBladeFactory::HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize, D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize)
{
	float box1Xmin = enemyPos.x - (enemySize.x / 2);
	float box1Xmax = enemyPos.x + (enemySize.x / 2);
	float box1Ymin = enemyPos.y - (enemySize.y / 2);
	float box1Ymax = enemyPos.y + (enemySize.y / 2);

	float box2Xmin = playerPos.x - (playerSize.x / 2);
	float box2Xmax = playerPos.x + (playerSize.x / 2);
	float box2Ymin = playerPos.y - (playerSize.y / 2);
	float box2Ymax = playerPos.y + (playerSize.y / 2);

	if (box1Xmin < box2Xmax)
	{
		if (box1Xmax > box2Xmin)
		{
			if (box1Ymin < box2Ymax)
			{
				if (box1Ymax > box2Ymin)
				{
					return true;
				}
			}
		}
	}

	return false;}

ThunderBladeFactory::~ThunderBladeFactory()
{
}
