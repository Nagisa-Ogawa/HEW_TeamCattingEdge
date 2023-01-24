#include <algorithm>
#include <vector>
#include "enemy.h"
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
#include "texture.h"
#include "Enemy_Sub_ThrowBomb.h"
#include "Boss_Fujin.h"
#include "Enemy_BossAvator.h"
#include "Boss_Raijin.h"

bool HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
	D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);

EnemyFactory::EnemyFactory()
{
	m_pPlayer = GetPlayer();
	m_pCamera = GetCamera();
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

void EnemyFactory::Create_SubThrowBomb(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_Sub_ThrowBomb(pos, m_nowID, m_EnemyThrowBombNo));
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

void EnemyFactory::Create_BossAvator(D3DXVECTOR2 pos, D3DXVECTOR2 targetPos, int mode)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	if (mode == Enemy_BossAvator::AVATOR_MODE::FUJIN) {
		m_pEnemyList.insert(enemyIt, new Enemy_BossAvator(pos, m_nowID, m_EnemyKasyaNo, targetPos, Enemy_BossAvator::AVATOR_MODE::FUJIN));
	}
	else {
		m_pEnemyList.insert(enemyIt, new Enemy_BossAvator(pos, m_nowID, m_EnemyKasyaNo, targetPos, Enemy_BossAvator::AVATOR_MODE::RAIJIN));
	}
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

void EnemyFactory::Create_Boss_Fujin(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Boss_Fujin(pos, m_nowID, m_EnemyFujinNo));
}

void EnemyFactory::Create_Boss_Raijin(D3DXVECTOR2 pos)
{
	m_nowID++;
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Boss_Raijin(pos, m_nowID, m_EnemyRaijinNo));
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
	m_EnemyKasyaNo = LoadTexture((char*)"data/TEXTURE/Boss_Kasya01.png");

	m_EnemyFujinNo= LoadTexture((char*)"data/TEXTURE/Fujin.png");
	m_EnemyRaijinNo = LoadTexture((char*)"data/TEXTURE/Raijin.png");

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
		delete pEnemy;
	}
	m_pEnemyList.clear();
}

void EnemyFactory::Update()
{
	CAMERA_2D* pCamera = GetCamera();
	D3DXVECTOR2 CameraPos = GetBase();

	for (Enemy* pEnemy : m_pEnemyList)
	{
		if (pEnemy->GetPos().x - pCamera->pos.x >= -60.0f && pEnemy->GetPos().x - pCamera->pos.x <= SCREEN_WIDTH + 60.0f)
		{
			pEnemy->Update();	
		}
		// ボス風神の場合は画面外でもupdateを行う
		else if (pEnemy->GetEnemyType() == Enemy::BOSS_FUJIN|| pEnemy->GetEnemyType() == Enemy::BOSS_RAIJIN || pEnemy->GetEnemyType() == Enemy::FUFINAVATOR)
		{
			pEnemy->Update();
		}
		if (pEnemy->GetPos().y >= SCREEN_HEIGHT + (BLOCK_SIZE * 2))
		{
			pEnemy->Damege(1);
		}

	}
	// ブロックとエネミーの当たり判定
	CollisionWallToEnemy();
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
				Create_SubThrowBomb(pos);
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
			case 50:
				Create_Boss_Fujin(pos);
				break;
			case 51:
				Create_Boss_Raijin(pos);
				break;
				//エネミー以外
			default:
				continue;
				break;
			}
		}
	}
}

void EnemyFactory::CollisionWallToEnemy()
{
	std::vector<std::vector<int>>* g_Stage = GetBlocks();


	HitCheckWallToEnemy(g_Stage);

	// すべての敵をチェック
	for (Enemy* pEnemy : m_pEnemyList)
	{
		if ((pEnemy->GetPos().x - m_pCamera->pos.x >= -60.0f && pEnemy->GetPos().x - m_pCamera->pos.x <= SCREEN_WIDTH + 60.0f)|| pEnemy->GetEnemyType() == Enemy::BOSS_FUJIN || pEnemy->GetEnemyType() == Enemy::BOSS_RAIJIN || pEnemy->GetEnemyType() == Enemy::FUFINAVATOR) {

			m_Result = 0;
			if (!pEnemy->GetIsActive()) {
				continue;
			}
			//ブロックのヒットした向きを調べる
			if (pEnemy->GetBlockLength() != 99999)
			{
				D3DXVECTOR2 vY(0.0f, -1.0f);
				D3DXVECTOR2 vEnemyPos = pEnemy->GetPos() + pEnemy->GetVel();

				//ブロックの座標(中心)
				D3DXVECTOR2 BlockPos = D3DXVECTOR2((pEnemy->GetBlockIndexX() * BLOCK_SIZE) + 30.0f, (pEnemy->GetBlockIndexY() * BLOCK_SIZE) + 30.0f);

				D3DXVECTOR2 vDist = vEnemyPos - BlockPos;
				D3DXVec2Normalize(&vDist, &vDist);

				//ヒット方向の判定
				float hit_vartical = D3DXVec2Dot(&vY, &vDist);

				if (hit_vartical < 0)
				{
					m_Result |= HIT_DOWN;
				}
				else if (hit_vartical > 0)
				{
					m_Result |= HIT_UP;
				}

				D3DXVECTOR2 vX(-1.0f, 0.0f);
				float hit_horizontal = D3DXVec2Dot(&vX, &vDist);
				if (hit_horizontal <= 0)
				{
					m_Result |= HIT_RIGHT;
				}
				else if (hit_horizontal > 0)
				{
					m_Result |= HIT_LEFT;
				}

				//ブロックの上座標を更新する
				pEnemy->SetBlockHeight(BlockPos.y - 30.0f);
			}
			// 当たり判定後のエネミーの処理
			pEnemy->AfterHitCheckBlockX(m_Result);
		}
	}

	HitCheckWallToEnemy(g_Stage);

	// すべての敵をチェック
	for (Enemy* pEnemy : m_pEnemyList)
	{
		if ((pEnemy->GetPos().x - m_pCamera->pos.x >= -60.0f && pEnemy->GetPos().x - m_pCamera->pos.x <= SCREEN_WIDTH + 60.0f) || pEnemy->GetEnemyType() == Enemy::BOSS_FUJIN || pEnemy->GetEnemyType() == Enemy::BOSS_RAIJIN || pEnemy->GetEnemyType() == Enemy::FUFINAVATOR) {
			m_Result = 0;
			if (!pEnemy->GetIsActive()) {
				continue;
			}
			//ブロックのヒットした向きを調べる
			if (pEnemy->GetBlockLength() != 99999)
			{
				D3DXVECTOR2 vY(0.0f, -1.0f);
				D3DXVECTOR2 vEnemyPos = pEnemy->GetPos() + pEnemy->GetVel();

				//ブロックの座標(中心)
				D3DXVECTOR2 BlockPos = D3DXVECTOR2((pEnemy->GetBlockIndexX() * BLOCK_SIZE) + 30.0f, (pEnemy->GetBlockIndexY() * BLOCK_SIZE) + 30.0f);

				D3DXVECTOR2 vDist = vEnemyPos - BlockPos;
				D3DXVec2Normalize(&vDist, &vDist);

				//ヒット方向の判定
				float hit_vartical = D3DXVec2Dot(&vY, &vDist);

				if (hit_vartical < 0)
				{
					m_Result |= HIT_DOWN;
				}
				else if (hit_vartical > 0)
				{
					m_Result |= HIT_UP;
				}

				D3DXVECTOR2 vX(-1.0f, 0.0f);
				float hit_horizontal = D3DXVec2Dot(&vX, &vDist);
				if (hit_horizontal <= 0)
				{
					m_Result |= HIT_RIGHT;
				}
				else if (hit_horizontal > 0)
				{
					m_Result |= HIT_LEFT;
				}

				//ブロックの上座標を更新する
				pEnemy->SetBlockHeight(BlockPos.y - 30.0f);
			}
			// 当たり判定後のエネミーの処理
			pEnemy->AfterHitCheckBlockY(m_Result);
		}
	}
}

void EnemyFactory::HitCheckWallToEnemy(std::vector<std::vector<int>>* g_Stage)
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->SetBlockLength(99999);
		pEnemy->SetBlockIndexX(-99999);
		pEnemy->SetBlockIndexY(-99999);
	}
	//敵バッファのすべてをチェックする
	for (int x = 0; x < (*g_Stage)[0].size(); x++)
	{
		for (int y = 0; y < g_Stage->size(); y++)
		{
			//敵の可視フラグがオフの場合はスキップする
			if ((*g_Stage)[y][x] != 1)
			{
				continue;
			}

			//ヒットしているかを判定する
			D3DXVECTOR2 BlockPos = D3DXVECTOR2(x * BLOCK_SIZE, y * BLOCK_SIZE);

			if (m_pCamera->pos.x-120.0f <= BlockPos.x)
			{
				if (m_pCamera->pos.x + SCREEN_WIDTH >= BlockPos.x)
				{
					// すべての敵をチェック
					for (Enemy* pEnemy : m_pEnemyList)
					{
						if (!pEnemy->GetIsActive()) {
							continue;
						}
						D3DXVECTOR2 vEnemyPos = pEnemy->GetPos() + pEnemy->GetVel();
						if (vEnemyPos.x - m_pCamera->pos.x >= -60.0f && vEnemyPos.x - m_pCamera->pos.x <= SCREEN_WIDTH + 60.0f) {

							if (HitCheckBox_Block(BlockPos, BLOCK_SIZE, BLOCK_SIZE,
								vEnemyPos, pEnemy->GetSize().x, pEnemy->GetSize().y))
							{
								//自分に当たっている中で一番近いブロックを探す
								D3DXVECTOR2 vLength = BlockPos - vEnemyPos;
								float length = D3DXVec2Length(&(vLength));
								if (pEnemy->GetBlockLength() > length)
								{
									//より近いブロックの情報に更新する
									pEnemy->SetBlockLength(length);
									pEnemy->SetBlockIndexX(x);
									pEnemy->SetBlockIndexY(y);
								}
							}
						}
					}
				}
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
		if (pEnemy->GetIsDead()) {
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

bool EnemyFactory::CheckFujinAvatorSetEnd()
{
	bool flag = true;
	for (Enemy* pEnemy : m_pEnemyList)
	{
		if (pEnemy->GetEnemyType() != Enemy::FUFINAVATOR)
		{
			continue;
		}
		if (!pEnemy->GetIsActive())
		{
			continue;
		}
		if (pEnemy->GetIsDead())
		{
			continue;
		}
		if (!pEnemy->GetIsEndSetUp())
		{
			flag = false;
		}
	}
	return flag;
}

bool EnemyFactory::CheckAliveFujinAvator()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		if (pEnemy->GetEnemyType() != Enemy::FUFINAVATOR)
		{
			continue;
		}
		if (!pEnemy->GetIsActive())
		{
			continue;
		}
		if (pEnemy->GetIsDead())
		{
			continue;
		}
		return false;
	}
	return true;
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
		(*deleteIt)->Uninit();
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

