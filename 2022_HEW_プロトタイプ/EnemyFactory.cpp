#include <algorithm>
#include <vector>
#include "EnemyFactory.h"
#include "Enemy_HitDrop.h"
#include "Enemy_SelfDestruct.h"
#include "Enemy_ThrowBomb.h"
#include "Enemy_GhostFire.h"
#include "Enemy_Rush.h"
#include "Enemy_ExplosionGas.h"
#include "Boss_Tengu.h"
#include "Boss_Kasya.h"
#include "Block.h"
#include "camera.h"
#include "texture.h"

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
	m_pEnemyList.insert(enemyIt, new Enemy_HitDrop(pos, m_nowID,m_EnemyHitDropNo));
}

void EnemyFactory::Create_SelfDestruction(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_SelfDestruct(pos, m_nowID,m_EnemySelfDestructNo));
}

void EnemyFactory::Create_ThrowBomb(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_ThrowBomb(pos, m_nowID,m_EnemyThrowBombNo));
}

void EnemyFactory::Create_GhostFire(D3DXVECTOR2 pos, int muki)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_GhostFire(pos,muki, m_nowID,m_EnemyGhostFireNo));
}

void EnemyFactory::Create_Rush(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_Rush(pos, m_nowID,m_EnemyRushNo));
}

void EnemyFactory::Create_ExplosionGas(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_ExplosionGas(pos, m_nowID,m_EnemyExplosionGasNo));
}

void EnemyFactory::Create_Boss_Tengu(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Boss_Tengu(pos, m_nowID,m_EnemyTenguNo));
}

void EnemyFactory::Create_Boss_Kasya(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Boss_Kasya(pos, m_nowID, m_EnemyKasyaNo));
}

void EnemyFactory::Init()
{
	// 敵のテクスチャをロード
	m_EnemyHitDropNo = LoadTexture((char*)"data/TEXTURE/Enemy_HitDrop.png");
	m_EnemySelfDestructNo = LoadTexture((char*)"data/TEXTURE/enemy_selfdestruct.png");
	m_EnemyThrowBombNo = LoadTexture((char*)"data/TEXTURE/Enemy_ThrowBomb.png");
	m_EnemyTenguNo = LoadTexture((char*)"data/TEXTURE/Boss_Tengu.png");

	m_EnemyGhostFireNo = LoadTexture((char*)"data/TEXTURE/Enemy_GhostFire.png");
	m_EnemyRushNo = LoadTexture((char*)"data/TEXTURE/enemy_selfdestruct.png");
	m_EnemyExplosionGasNo = LoadTexture((char*)"data/TEXTURE/Enemy_ExplosionGas.png");
	m_EnemyKasyaNo = LoadTexture((char*)"data/TEXTURE/Boss_Kasya.png");

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
	CAMERA_2D* pCamera = GetCamera();
	D3DXVECTOR2 CameraPos = GetBase();

	for (Enemy* pEnemy : m_pEnemyList)
	{
		if (pEnemy->GetPos().x - pCamera->pos.x >= -120.0f && pEnemy->GetPos().x - pCamera->pos.x <= SCREEN_WIDTH + 120.0f)
		{
			pEnemy->Update();
		}

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
	for (int y = 0; y < pMaps->size(); y++)
	{
		for (int x = 0; x < (*pMaps)[0].size(); x++)
		{
			D3DXVECTOR2 pos = D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE / 2, BLOCK_SIZE * y);

			switch ((*pMaps)[y][x])
			{
				//天狗エネミー
			case 30:
				// ボス作成
				Create_Boss_Tengu(pos);
				break;
			case 31:
				Create_ThrowBomb(pos);
				break;
			case 32:
				break;
			case 33:
				Create_HitDrop(pos);
				break;
			case 34:
				break;
			case 35:
				break;
			case 36:
				Create_SelfDestruction(pos);
				break;
			case 37:
				break;
			case 40:
				Create_Boss_Kasya(pos);
				break;
			case 41:
				Create_GhostFire(pos, 0);
				break;
			case 42:
				Create_GhostFire(pos, 1);
				break;
			case 43:
				Create_Rush(pos);
				break;
			case 44:
				Create_ExplosionGas(pos);
				break;
				//エネミー以外
			default:
				continue;
				break;
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

