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

void EnemyFactory::Init()
{
	// �}�b�v��񂩂�G�l�~�[�𐶐�
	SetEnemy();
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Init();
	}
	if (m_pBoss != nullptr)
	{
		m_pBoss->Init();
	}
}

void EnemyFactory::Uninit()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Uninit();
	}
	if (m_pBoss != nullptr)
	{
		delete m_pBoss;
		m_pBoss = nullptr;
	}
	if (m_pBoss != nullptr)
	{
		m_pBoss->Uninit();
	}
}

void EnemyFactory::Update()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Update();
	}
	if (m_pBoss != nullptr)
	{
		m_pBoss->Update();
	}
	// �v���C���[�ƃG�l�~�[�̓����蔻��
	CollisionPlayerToEnemy();
	// �v���C���[�ƃ{�X�̓����蔻��
	CollisionPlayerToBoss();
	// �\�����Ă��Ȃ��G�͍폜
	DeleteEnemy();
}

void EnemyFactory::Draw()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Draw();
	}
	if (m_pBoss != nullptr)
	{
		m_pBoss->Draw();
	}
}


void EnemyFactory::SetEnemy()
{
	auto pMaps = GetBlocks();
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (pMaps[y][x] == 0 || pMaps[y][x] == 1)
			{
				continue;
			}
			D3DXVECTOR2 pos = D3DXVECTOR2(BLOCK_SIZE*x + BLOCK_SIZE / 2, BLOCK_SIZE*y + BLOCK_SIZE / 2);
			pos.y -= 100.0f;
			if (pMaps[y][x] == 2)
			{
				Create_HitDrop(pos);
			}
			if (pMaps[y][x] == 3)
			{
				Create_SelfDestruction(pos);
			}
			if (pMaps[y][x] == 4)
			{
				Create_ThrowBomb(pos);
			}
			if (pMaps[y][x] == 5)
			{
				// �{�X�쐬
				m_pBoss = new Boss_Tengu(pos, 0);
			}
		}
	}
}

void EnemyFactory::CollisionPlayerToEnemy()
{
	// �v���C���[�����G�Ȃ�X�L�b�v
	if (m_pPlayer->mutekiflag)
	{
		return;
	}
	for (Enemy* pEnemy : m_pEnemyList)
	{
		// ��ʊO�Ȃ�X�L�b�v
		if (!pEnemy->GetIsActive())
		{
			continue;
		}
		// �G�Ɠ��������Ȃ�
		if (HitCheckBox(pEnemy->GetPos(), pEnemy->GetSize(),
			m_pPlayer->pos, D3DXVECTOR2(m_pPlayer->size, m_pPlayer->size)))
		{
			// �v���C���[��HP�����炷
			PlayerDamage(1);
		}
	}
	return ;
}

void EnemyFactory::CollisionPlayerToBoss()
{
	// �v���C���[�����G�Ȃ�X�L�b�v
	if (m_pPlayer->mutekiflag)
	{
		return;
	}
	// ��ʊO�Ȃ�X�L�b�v
	if (!(m_pBoss->GetIsActive()))
	{
		return;
	}
	// �G�Ɠ��������Ȃ�
	if (HitCheckBox(m_pBoss->GetPos(), m_pBoss->GetSize(),
		m_pPlayer->pos, D3DXVECTOR2(m_pPlayer->size, m_pPlayer->size)))
	{
		// �v���C���[��HP�����炷
		PlayerDamage(1);
	}
	return;
}

void EnemyFactory::DeleteEnemy()
{
	while (true)
	{
		auto deleteIt = std::find_if(m_pEnemyList.begin(), m_pEnemyList.end(), ([&](Enemy* enemy) {return (enemy->GetIsActive() == false); }));
		// �\�����Ă��Ȃ����e������Ȃ��Ȃ甲����
		if (deleteIt == m_pEnemyList.end())
		{
			break;
		}
		// �����ς݂̔��e�͍폜
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

