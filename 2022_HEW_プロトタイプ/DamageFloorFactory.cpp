#include "DamageFloorFactory.h"
#include "DamageFloor.h"
#include "texture.h"
#include "player.h"
#include "EnemyFactory.h"
#include "game.h"
#include <vector>
#include "Block.h"

bool HitCheckBox_DamageFloor(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
	D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize);

DamageFloorFactory::DamageFloorFactory()
{
}

void DamageFloorFactory::Init()
{
	m_DamageFloorTextureNo = LoadTexture((char*)"data/TEXTURE/DamageFloor.png");
	m_pEnemyFactory = GetEnemyFactory();
	m_pCamera = GetCamera();
	Set();
	// Create(D3DXVECTOR2(960.0f, 300.0f));
}

void DamageFloorFactory::Uninit()
{
	for (DamageFloor* pDamageFloor : m_pDamageFloorList)
	{
		delete pDamageFloor;
	}
	m_pDamageFloorList.clear();
}

void DamageFloorFactory::Update()
{
	for (DamageFloor* pDamageFloor : m_pDamageFloorList)
	{
		pDamageFloor->Update();
	}
	CollisionDamageFloorToEnemy();
	CollisionDamageFloorToPlayer();
}

void DamageFloorFactory::Draw()
{
	for (DamageFloor* pDamageFloor : m_pDamageFloorList)
	{
		pDamageFloor->Draw();
	}
}

DamageFloorFactory::~DamageFloorFactory()
{
	for (DamageFloor* pDamageFloor : m_pDamageFloorList)
	{
		delete pDamageFloor;
	}
	m_pDamageFloorList.clear();
}

void DamageFloorFactory::Create(D3DXVECTOR2 pos)
{
	m_pDamageFloorList.push_back(new DamageFloor(pos, m_DamageFloorTextureNo));
}

void DamageFloorFactory::Set()
{
	auto pMaps = GetBlocks();
	for (int y = 0; y < pMaps->size(); y++)
	{
		for (int x = 0; x < (*pMaps)[0].size(); x++)
		{
			D3DXVECTOR2 pos = D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE / 2, BLOCK_SIZE * y);
			if((*pMaps)[y][x]==45)
			{
				pos.y += BLOCK_SIZE / 2.0f;
				Create(pos);
			}
		}
	}
}

void DamageFloorFactory::CollisionDamageFloorToEnemy()
{
	std::list<Enemy*>* pEnemyList = m_pEnemyFactory->GetEnemyList();
	for (Enemy* pEnemy : *pEnemyList) {
		if (pEnemy->GetPos().x - m_pCamera->pos.x >= -120.0f && pEnemy->GetPos().x - m_pCamera->pos.x <= SCREEN_WIDTH + 120.0f) {
			if (pEnemy->GetIsDead()) {
				continue;
			}
			if (!pEnemy->GetIsActive()) {
				continue;
			}
			for (DamageFloor* pDamageFloor : m_pDamageFloorList) {
				if ((pDamageFloor->GetPos().x + (pDamageFloor->GetSize().x / 2.0f)) - m_pCamera->pos.x >= -120.0f && (pDamageFloor->GetPos().x - (pDamageFloor->GetSize().x / 2.0f)) - m_pCamera->pos.x <= SCREEN_WIDTH + 120.0f) {

					if (!pDamageFloor->GetIsActive()) {
						continue;
					}
					if(HitCheckBox_DamageFloor(pEnemy->GetPos(), pEnemy->GetSize(), pDamageFloor->GetPos(), pDamageFloor->GetSize())) {
						pEnemy->Damege(1);
					}
				}
			}
		}
	}
}

void DamageFloorFactory::CollisionDamageFloorToPlayer()
{
	PLAYER* pPlayer = GetPlayer();
	if (pPlayer->mutekiflag) {
		return;
	}
	for (DamageFloor* pDamageFloor : m_pDamageFloorList) {
		if ((pDamageFloor->GetPos().x+(pDamageFloor->GetSize().x/2.0f)) - m_pCamera->pos.x >= -120.0f && (pDamageFloor->GetPos().x - (pDamageFloor->GetSize().x / 2.0f)) - m_pCamera->pos.x <= SCREEN_WIDTH + 120.0f) {

			if (!pDamageFloor->GetIsActive()) {
				continue;
			}
			if (HitCheckBox_DamageFloor(pPlayer->pos, D3DXVECTOR2( pPlayer->size,pPlayer->size), pDamageFloor->GetPos(), pDamageFloor->GetSize())) {
				PlayerDamage(1);
			}
		}
	}
}


bool HitCheckBox_DamageFloor(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
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
