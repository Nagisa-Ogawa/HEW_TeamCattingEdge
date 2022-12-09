#include "ExplosionFactory.h"
#include "player.h"

ExplosionFactory::ExplosionFactory()
{
}

void ExplosionFactory::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	m_pExplosion.push_back(new Explosion(pos, size, (size.x / 2.0f), (size.x / 2.0f) + 50.0f));
}

void ExplosionFactory::Init()
{
	// Create(D3DXVECTOR2(300.0f, 300.0f), D3DXVECTOR2(120.0f, 120.0f));
	for (Explosion* pExplosion : m_pExplosion)
	{
		pExplosion->Init();
	}
}

void ExplosionFactory::Uninit()
{
	for (Explosion* pExplosion : m_pExplosion)
	{
		pExplosion->Uninit();
	}
}

void ExplosionFactory::Update()
{
	for (Explosion* pExplosion : m_pExplosion)
	{
		pExplosion->Update();
	}
	CollisionBlastToPlayer();
}

void ExplosionFactory::Draw()
{
	for (Explosion* pExplosion : m_pExplosion)
	{
		pExplosion->Draw();
	}
}

ExplosionFactory::~ExplosionFactory()
{
	for (Explosion* pExplosion : m_pExplosion)
	{
		delete pExplosion;
	}
	m_pExplosion.clear();
}

void ExplosionFactory::CollisionBlastToPlayer()
{
	// プレイヤーを取得
	PLAYER* pPlayer = GetPlayer();
	// プレイヤーが無敵なら当たらない
	if (pPlayer->mutekiflag)
	{
		return;
	}
	// 全ての爆風をチェック
	for (Explosion* pExplosion : m_pExplosion)
	{
		// 爆風が画面外なら当たらない
		if (!pExplosion->GetIsActive())
		{
			continue;
		}
		// プレイヤーと爆弾のベクトルを計算
		D3DXVECTOR2 vec = pExplosion->GetPos() - pPlayer->pos;
		// ベクトルの長さを計算
		float len = D3DXVec2Length(&vec);
		// プレイヤーと爆弾の当たり判定の半径を足した数値よりも
		// ベクトルの方が短いなら当たっている
		if (len <= pExplosion->GetCollsionRad() + pPlayer->size / 2.0f)
		{
			PlayerDamage(10);
		}
		return;
	}
}
