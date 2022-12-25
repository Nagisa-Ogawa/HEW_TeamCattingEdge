#include <algorithm>
#include "FireBallFactory.h"
#include "FireBall.h"
#include "Block.h"

FireBallFactory::FireBallFactory()
{
	
}

void FireBallFactory::Create(D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power)
{
	m_nowID++;
	m_pFireBallList.push_back(new FireBall(m_nowID,pos, muki, power));
}

void FireBallFactory::Init()
{
	m_pPlayer = GetPlayer();
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Init();
	}
}

void FireBallFactory::Uninit()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Uninit();
	}
}

void FireBallFactory::Update()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Update();
	}
	CollisionPlayerToFireBall();
	CollisionBlockToFireBall();
	// 使い終わった火の玉がないかチェック
	DeleteFireBall();
}

void FireBallFactory::Draw()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Draw();
	}
}

FireBallFactory::~FireBallFactory()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		delete fireBall;
	}

	m_pFireBallList.clear();
}

void FireBallFactory::DeleteFireBall()
{
	while (true)
	{
		auto deleteIt = std::find_if(m_pFireBallList.begin(), m_pFireBallList.end(), ([&](FireBall* fireBall) {return (fireBall->GetIsActive() == false); }));
		// 表示していない爆弾が一個もないなら抜ける
		if (deleteIt == m_pFireBallList.end())
		{
			break;
		}
		// 爆発済みの爆弾は削除
		delete (*deleteIt);
		m_pFireBallList.erase(deleteIt);
	}
}

void FireBallFactory::CollisionPlayerToFireBall()
{
	// プレイヤーが無敵ならスキップ
	if (m_pPlayer->mutekiflag)
	{
		return;
	}
	for (FireBall* fireBall : m_pFireBallList)
	{
		// 死亡していたならスキップ
		if (!fireBall->GetIsActive())
		{
			continue;
		}
		// プレイヤーと当たったなら
		if (HitCheckBox(fireBall->GetPos(), fireBall->GetSize(),
			m_pPlayer->pos, D3DXVECTOR2(m_pPlayer->size, m_pPlayer->size)))
		{
			// プレイヤーのHPを減らす
			PlayerDamage(1);
			fireBall->Hit();
		}
	}
	return;
}

void FireBallFactory::CollisionBlockToFireBall()
{
	auto blocks = GetBlocks();
	for (FireBall* fireBall : m_pFireBallList)
	{
		// 死亡していたならスキップ
		if (!fireBall->GetIsActive())
		{
			continue;
		}
		for (int y = 0; y < blocks->size(); y++)
		{
			for (int x = 0; x < (*blocks)[y].size(); x++)
			{
				if ((*blocks)[y][x] == 1) {
					// プレイヤーと当たったなら
					if (HitCheckBox_Block(D3DXVECTOR2(x*BLOCK_SIZE,y*BLOCK_SIZE),D3DXVECTOR2( BLOCK_SIZE,BLOCK_SIZE),
						fireBall->GetPos(), fireBall->GetSize()))
					{
						fireBall->Hit();
					}
				}
			}
		}
	}
	return;
}

bool FireBallFactory::HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
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

bool FireBallFactory::HitCheckBox_Block(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize, D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize)
{

	float box1Xmin = blockPos.x;
	float box1Xmax = blockPos.x + blockSize.x;
	float box1Ymin = blockPos.y ;
	float box1Ymax = blockPos.y + blockSize.y;

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

