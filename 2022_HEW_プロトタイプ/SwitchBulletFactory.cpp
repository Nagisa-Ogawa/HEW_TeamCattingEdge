#include "SwitchBulletFactory.h"
#include "player.h"
#include "camera.h"
#include "SwitchBullet.h"
#include "texture.h"
#include "Block.h"
#include <algorithm>

SwitchBulletFactory::SwitchBulletFactory()
{
}

void SwitchBulletFactory::Init()
{
	m_pPlayer = GetPlayer();
	m_pCamera = GetCamera();
	m_SwitchBulletNo = LoadTexture((char*)"data/TEXTURE/TBullet.png");
}

void SwitchBulletFactory::Uninit()
{
	for (SwitchBullet* pThunderBlade : m_pSwitchBulletList)
	{
		pThunderBlade->Uninit();
	}
	for (SwitchBullet* pThunderBlade : m_pSwitchBulletList)
	{
		delete pThunderBlade;
	}
	m_pSwitchBulletList.clear();
}

void SwitchBulletFactory::Update()
{
	for (SwitchBullet* pThunderBlade : m_pSwitchBulletList)
	{
		pThunderBlade->Update();
	}
	CollisionSwitchBulletToPlayer();
	CollisionSwitchBulletToBlock();
	DeleteSwitchBullet();
}

void SwitchBulletFactory::Draw()
{
	for (SwitchBullet* pThunderBlade : m_pSwitchBulletList)
	{
		pThunderBlade->Draw();
	}
}

void SwitchBulletFactory::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size,D3DXVECTOR2 targetPos,int muki,int mode)
{
	m_pSwitchBulletList.push_back(new SwitchBullet(pos, size, m_SwitchBulletNo, targetPos,muki,(SwitchBullet::BULLET_MODE)mode));
}

void SwitchBulletFactory::CollisionSwitchBulletToPlayer()
{
	if (m_pPlayer->mutekiflag)
	{
		return;
	}
	// 全ての風の刃をチェック
	for (SwitchBullet* pWindBlade : m_pSwitchBulletList)
	{
		if (!pWindBlade->GetIsActive())
		{
			continue;
		}
		if (pWindBlade->GetState() != SwitchBullet::STATE_SWITCHBULLET::MOVE) {
			continue;
		}
		// プレイヤーと風の刃の当たり判定
		if (HitCheckBox(m_pPlayer->pos, D3DXVECTOR2(m_pPlayer->size, m_pPlayer->size),
			pWindBlade->GetPos(), pWindBlade->GetSize()))
		{
			// プレイヤーへダメージを与える
			PlayerDamage(1);
		}
	}
}

void SwitchBulletFactory::CollisionSwitchBulletToBlock()
{
	auto g_Stage = GetBlocks();
	// 全ての風の刃をチェック
	for (SwitchBullet* pWindBlade : m_pSwitchBulletList)
	{
		if (!pWindBlade->GetIsActive())
		{
			continue;
		}
		if (pWindBlade->GetState() != SwitchBullet::STATE_SWITCHBULLET::MOVE) {
			continue;
		}
		if (pWindBlade->GetMode() != SwitchBullet::BULLET_MODE::ONCE) {
			continue;
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
				if (HitCheckBox_Block(BlockPos, BLOCK_SIZE, BLOCK_SIZE,
					pWindBlade->GetPos(), pWindBlade->GetSize().x, pWindBlade->GetSize().y))
				{
					D3DXVECTOR2 pos = D3DXVECTOR2(0.0f, 0.0f);
					// 座標を作成
					if (x == 0) {
						pos.x = BLOCK_SIZE * 1.0f + 240.0f;
						pos.y = BLOCK_SIZE * 18.0f - 240.0f;
					}
					else if (x == ((*g_Stage)[0].size() - 1)) {
						pos.x = BLOCK_SIZE * 31.0f - 240.0f;
						pos.y = BLOCK_SIZE * 18.0f - 240.0f;
					}
					pWindBlade->Switch(pos);
				}
			}
		}
	}
}

bool SwitchBulletFactory::HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize, D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize)
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

bool SwitchBulletFactory::HitCheckBox_Block(D3DXVECTOR2 box1pos, float box1width, float box1height, D3DXVECTOR2 box2pos, float box2width, float box2height)
{
	float box1Xmin = box1pos.x;
	float box1Xmax = box1pos.x + box1width;
	float box1Ymin = box1pos.y;
	float box1Ymax = box1pos.y + box1height;

	float box2Xmin = box2pos.x - (box2width / 2);
	float box2Xmax = box2pos.x + (box2width / 2);
	float box2Ymin = box2pos.y - (box2height / 2);
	float box2Ymax = box2pos.y + (box2height / 2);

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

void SwitchBulletFactory::DeleteSwitchBullet()
{
	while (true)
	{
		auto deleteIt = std::find_if(m_pSwitchBulletList.begin(), m_pSwitchBulletList.end(), ([&](SwitchBullet* enemy) {return (enemy->GetIsActive() == false); }));
		// 表示していない爆弾が一個もないなら抜ける
		if (deleteIt == m_pSwitchBulletList.end())
		{
			break;
		}
		(*deleteIt)->Uninit();
		// 爆発済みの爆弾は削除
		delete (*deleteIt);
		m_pSwitchBulletList.erase(deleteIt);
	}
}

bool SwitchBulletFactory::CheckSwitchFlag()
{
	bool flag = false;
	for (SwitchBullet* pSwitchBullet : m_pSwitchBulletList)
	{
		if (pSwitchBullet->GetState() != SwitchBullet::STATE_SWITCHBULLET::SWITCH) {
			continue;
		}
		if (pSwitchBullet->GetIsSwitch()) {
			flag = true;
			return flag;
		}
	}
	return flag;
}

D3DXVECTOR2 SwitchBulletFactory::GetSwitchPos()
{
	for (SwitchBullet* pSwitchBullet : m_pSwitchBulletList)
	{
		if (pSwitchBullet->GetIsSwitch()) {
			pSwitchBullet->Died();
			return pSwitchBullet->GetSwitchPos();
		}
	}
	return D3DXVECTOR2(0.0f, 0.0f);
}

SwitchBulletFactory::~SwitchBulletFactory()
{
}
