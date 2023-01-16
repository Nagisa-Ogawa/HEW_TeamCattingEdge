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
	// �G�̃e�N�X�`�������[�h
	m_EnemyHitDropNo = LoadTexture((char*)"data/TEXTURE/Enemy_HitDrop.png");
	m_EnemySelfDestructNo = LoadTexture((char*)"data/TEXTURE/enemy_selfdestruct.png");
	m_EnemyThrowBombNo = LoadTexture((char*)"data/TEXTURE/Enemy_ThrowBomb.png");
	m_EnemyTenguNo = LoadTexture((char*)"data/TEXTURE/Boss_Tengu.png");

	m_EnemyGhostFireNo = LoadTexture((char*)"data/TEXTURE/Enemy_GhostFire.png");
	m_EnemyRushNo = LoadTexture((char*)"data/TEXTURE/enemy_selfdestruct.png");
	m_EnemyExplosionGasNo = LoadTexture((char*)"data/TEXTURE/Enemy_ExplosionGas.png");
	m_EnemyKasyaNo = LoadTexture((char*)"data/TEXTURE/Boss_Kasya01.png");

	// �}�b�v��񂩂�G�l�~�[�𐶐�
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
			// CollisionWallToEnemy();
			
		}
		if (pEnemy->GetPos().y >= SCREEN_HEIGHT + (BLOCK_SIZE * 2))
		{
			pEnemy->Damege(1);
		}

	}
	// �v���C���[�ƃG�l�~�[�̓����蔻��
	CollisionPlayerToEnemy();
	// �v���C���[�ƃ{�X�̓����蔻��
	// CollisionPlayerToBoss();
	// �\�����Ă��Ȃ��G�͍폜
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
				//�V��G�l�~�[
			case 30:
				// �{�X�쐬
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
				//�G�l�~�[�ȊO
			default:
				continue;
				break;
			}
		}
	}
}

void EnemyFactory::CollisionWallToEnemy()
{
	//DWORD result = 0;
	//CAMERA_2D* pCamera = GetCamera();
	//std::vector<std::vector<int>>* g_Stage = GetBlocks();

	//float block_length = 99999;//��ԋ߂��u���b�N�̋���
	//int block_index[2]{ -99999,-99999 };//1��ԋ߂��u���b�N�̓Y����

	////�G�o�b�t�@�̂��ׂĂ��`�F�b�N����
	//for (int x = 0; x < (*g_NowStage)[0].size(); x++)
	//{
	//	for (int y = 0; y < g_NowStage->size(); y++)
	//	{
	//		//�G�̉��t���O���I�t�̏ꍇ�̓X�L�b�v����
	//		if ((*g_NowStage)[y][x] != 1)
	//		{
	//			continue;
	//		}



	//		//�q�b�g���Ă��邩�𔻒肷��
	//		D3DXVECTOR2 BlockPos = D3DXVECTOR2(x * BLOCK_SIZE, y * BLOCK_SIZE);

	//		if (pCamera->pos.x <= BlockPos.x)
	//		{
	//			if (pCamera->pos.x + SCREEN_WIDTH >= BlockPos.x)
	//			{
	//				if (HitCheckBox_Block(BlockPos, BLOCK_SIZE, BLOCK_SIZE, vEnemyPos, size.x, size.y))
	//				{
	//					//�����ɓ������Ă��钆�ň�ԋ߂��u���b�N��T��
	//					D3DXVECTOR2 vLength = BlockPos - vEnemyPos;
	//					float length = D3DXVec2Length(&(vLength));
	//					if (block_length > length)
	//					{
	//						//���߂��u���b�N�̏��ɍX�V����
	//						block_length = length;
	//						block_index[0] = x;
	//						block_index[1] = y;
	//					}
	//				}
	//			}
	//		}

	//		if (HitCheckBox_Block(BlockPos, BLOCK_SIZE, BLOCK_SIZE, vEnemyPos, size.x, size.y))
	//		{
	//			//�����ɓ������Ă��钆�ň�ԋ߂��u���b�N��T��
	//			D3DXVECTOR2 vLength = BlockPos - vEnemyPos;
	//			float length = D3DXVec2Length(&(vLength));
	//			if (block_length > length)
	//			{
	//				//���߂��u���b�N�̏��ɍX�V����
	//				block_length = length;
	//				block_index[0] = x;
	//				block_index[1] = y;
	//			}
	//		}
	//	}
	//}

	////�u���b�N�̃q�b�g���������𒲂ׂ�
	//if (block_length != 99999)
	//{
	//	D3DXVECTOR2 vY(0.0f, -1.0f);

	//	//�u���b�N�̍��W(���S)
	//	D3DXVECTOR2 BlockPos = D3DXVECTOR2((block_index[0] * BLOCK_SIZE) + 30.0f, (block_index[1] * BLOCK_SIZE) + 30.0f);

	//	D3DXVECTOR2 vDist = vEnemyPos - BlockPos;
	//	D3DXVec2Normalize(&vDist, &vDist);

	//	//�q�b�g�����̔���
	//	float hit_vartical = D3DXVec2Dot(&vY, &vDist);

	//	if (hit_vartical < 0)
	//	{
	//		result |= HIT_DOWN;
	//	}
	//	else if (hit_vartical > 0)
	//	{
	//		result |= HIT_UP;
	//	}

	//	D3DXVECTOR2 vX(-1.0f, 0.0f);
	//	float hit_horizontal = D3DXVec2Dot(&vX, &vDist);
	//	if (hit_horizontal <= 0)
	//	{
	//		result |= HIT_RIGHT;
	//	}
	//	else if (hit_horizontal > 0)
	//	{
	//		result |= HIT_LEFT;
	//	}

	//	//�u���b�N�̏���W���X�V����
	//	g_Block_Height = BlockPos.y - 30.0f;
	//}

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

