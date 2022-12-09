#include "EnemyFactory.h"
#include "Enemy_HitDrop.h"
#include "Enemy_SelfDestruct.h"
#include "Enemy_ThrowBomb.h"

bool HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
	D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);

EnemyFactory::EnemyFactory()
{
	m_pPlayer = GetPlayer();
	auto enemyIt = m_pEnemyList.begin();
	//m_pEnemyList.insert(enemyIt, new Enemy_HitDrop(D3DXVECTOR2(700.0f, 100.0f), 0));
	//m_pEnemyList.insert(enemyIt, new Enemy_HitDrop(D3DXVECTOR2(900.0f, 200.0f), 0));
	m_pEnemyList.insert(enemyIt, new Enemy_SelfDestruct(D3DXVECTOR2(1100.0f, 100.0f), 0));
	m_pEnemyList.insert(enemyIt, new Enemy_SelfDestruct(D3DXVECTOR2(1300.0f, 200.0f), 0));
	//m_pEnemyList.insert(enemyIt, new Enemy_ThrowBomb(D3DXVECTOR2(700.0f, 100.0f), 0));
	// m_pEnemyList.insert(enemyIt, new Enemy_ThrowBomb(D3DXVECTOR2(900.0f, 200.0f), 0));
}

void EnemyFactory::Init()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Init();
	}
}

void EnemyFactory::Uninit()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Uninit();
	}
}

void EnemyFactory::Update()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Update();
	}
	// プレイヤーとエネミーの当たり判定
	CollisionPlayerToEnemy();
}

void EnemyFactory::Draw()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Draw();
	}
}

void EnemyFactory::CollisionPlayerToEnemy()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		// 画面外ならスキップ
		if (!pEnemy->GetIsActive())
		{
			continue;
		}
		// 敵と当たったなら
		if (HitCheckBox(pEnemy->GetPos(), pEnemy->GetSize(),
			m_pPlayer->pos, D3DXVECTOR2(m_pPlayer->size, m_pPlayer->size)))
		{
			// プレイヤーのHPを減らす
			PlayerDamage(1);
		}
	}
	return ;
}

EnemyFactory::~EnemyFactory()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		delete pEnemy;
	}
	m_pEnemyList.clear();
}

bool HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
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

