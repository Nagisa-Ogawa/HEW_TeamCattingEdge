#include <algorithm>
#include <vector>
#include "EnemyFactory.h"
#include "Enemy_HitDrop.h"
#include "Enemy_SelfDestruct.h"
#include "Enemy_ThrowBomb.h"
#include "Boss_Tengu.h"
#include "Block.h"

bool HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
	D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);

EnemyFactory::EnemyFactory()
{
	m_pPlayer = GetPlayer();
}

void EnemyFactory::Create_HitDrop(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_HitDrop(pos, m_nowID));
}

void EnemyFactory::Create_SelfDestruction(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_SelfDestruct(pos, m_nowID));
}

void EnemyFactory::Create_ThrowBomb(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_ThrowBomb(pos, m_nowID));
}

void EnemyFactory::Create_Boss(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Boss_Tengu(pos, m_nowID));
}

void EnemyFactory::Init()
{
	// マップ情報からエネミーを生成
	SetEnemy();
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
	// プレイヤーとボスの当たり判定
	// CollisionPlayerToBoss();
	// 表示していない敵は削除
	DeleteEnemy();
}

void EnemyFactory::Draw()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Draw();
	}
}


void EnemyFactory::SetEnemy()
{
	auto pMaps = GetBlocks();
	for (int y = 0; y < pMaps.size(); y++)
	{
		for (int x = 0; x < pMaps[0].size(); x++)
		{
			if (pMaps[y][x] == 0 || pMaps[y][x] == 1)
			{
				continue;
			}
			D3DXVECTOR2 pos = D3DXVECTOR2(BLOCK_SIZE*x + BLOCK_SIZE / 2, BLOCK_SIZE*y + BLOCK_SIZE / 2);
			pos.y -= 100.0f;
			if (pMaps[y][x] == 100)
			{
				Create_HitDrop(pos);
			}
			if (pMaps[y][x] == 101)
			{
				Create_SelfDestruction(pos);
			}
			if (pMaps[y][x] == 102)
			{
				Create_ThrowBomb(pos);
			}
			if (pMaps[y][x] == 200)
			{
				// ボス作成
				Create_Boss(pos);
			}
		}
	}
}

void EnemyFactory::CollisionPlayerToEnemy()
{
	// プレイヤーが無敵ならスキップ
	if (m_pPlayer->mutekiflag)
	{
		return;
	}
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


void EnemyFactory::CollisoinAttacktoEnemy(D3DXVECTOR2 AttackPos)
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->HitCheckPlayerAttack(AttackPos);
	}
}

void EnemyFactory::DeleteEnemy()
{
	while (true)
	{
		auto deleteIt = std::find_if(m_pEnemyList.begin(), m_pEnemyList.end(), ([&](Enemy* enemy) {return (enemy->GetIsActive() == false); }));
		// 表示していない爆弾が一個もないなら抜ける
		if (deleteIt == m_pEnemyList.end())
		{
			break;
		}
		// 爆発済みの爆弾は削除
		delete (*deleteIt);
		m_pEnemyList.erase(deleteIt);
	}
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

