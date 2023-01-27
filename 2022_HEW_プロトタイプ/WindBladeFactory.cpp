#include "WindBladeFactory.h"
#include "player.h"
#include "camera.h"
#include "WindBlade.h"
#include "texture.h"
#include "Block.h"

WindBladeFactory::WindBladeFactory()
{
}

void WindBladeFactory::Init()
{
	m_pPlayer = GetPlayer();
	m_pCamera = GetCamera();
	m_WindBladeNo = LoadTexture((char*)"data/TEXTURE/WindBlade.png");
}

void WindBladeFactory::Uninit()
{
	for (WindBlade* pWindBlade : m_pWindBladeList)
	{
		pWindBlade->Uninit();
	}
	for (WindBlade* pWindBlade : m_pWindBladeList)
	{
		delete pWindBlade;
	}
	m_pWindBladeList.clear();
}

void WindBladeFactory::Update()
{
	for (WindBlade* pWindBlade : m_pWindBladeList)
	{
		pWindBlade->Update();
	}
	CollisionWindBladeToPlayer();
}

void WindBladeFactory::Draw()
{
	for (WindBlade* pWindBlade : m_pWindBladeList)
	{
		pWindBlade->Draw();
	}
}

void WindBladeFactory::Create(D3DXVECTOR2 pos,D3DXVECTOR2 size, int muki,bool isDuo)
{
	m_pWindBladeList.push_back(new WindBlade(pos,size, muki,m_WindBladeNo,isDuo));
}

void WindBladeFactory::CollisionWindBladeToPlayer()
{
	if (m_pPlayer->mutekiflag)
	{
		return;
	}
	// 全ての風の刃をチェック
	for (WindBlade* pWindBlade : m_pWindBladeList)
	{
		if (!pWindBlade->GetIsActive())
		{
			continue;
		}
		// プレイヤーと風の刃の当たり判定
		if (HitCheckBox(m_pPlayer->pos, D3DXVECTOR2(m_pPlayer->size, m_pPlayer->size),
			pWindBlade->GetPos(), pWindBlade->GetSize()))
		{
			if (pWindBlade->GetIsDuo())
			{
				// プレイヤーへダメージを与える
				PlayerDamage(1);
			}
			else
			{
				// プレイヤーへダメージを与える
				PlayerDamage(1);
			}
		}
	}
}

WindBladeFactory::~WindBladeFactory()
{
}

bool WindBladeFactory::HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
	D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize)
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

	return false;
}
