#include "XRay.h"
#include "texture.h"
#include "sprite.h"
#include "player.h"
#include "Block.h"

bool HitCheckBlockToXRay(D3DXVECTOR2 raypos, float size);
bool HitCheckPlayerToXRay(D3DXVECTOR2 raypos, float raysize, D3DXVECTOR2 playerpos);
bool HitCheckXRayLine(D3DXVECTOR2 startA, D3DXVECTOR2 endA, D3DXVECTOR2 startB, D3DXVECTOR2 endB);
float crossX(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2);

XRay::XRay(D3DXVECTOR2 pos, D3DXVECTOR2 playerpos)
	:RayInterface(pos, playerpos)
{
	m_texture = LoadTexture((char*)"data/TEXTURE/XRay.png");
	m_use = true;
	m_pPlayer = GetPlayer();
	m_state = Normal;
	m_goalpos = m_pPlayer->pos;

	m_vec = m_goalpos - m_pos;
	D3DXVec2Normalize(&m_vec, &m_vec);
}

void XRay::Init(void)
{
}

void XRay::Update(void)
{
	switch (m_state)
	{
	case Normal:
		m_pos += m_vec * RAYSPEED;

		if (HitCheckBlockToXRay(m_pos, m_size))
			m_state = Expo;
		break;
	case Expo:
		if (m_size <= MAXSIZE)
		{
			m_size += 3.0f;
		}
		else
		{
			m_use = false;
		}
		break;
	}

	if (HitCheckPlayerToXRay(m_pos, m_size, m_pPlayer->pos))
	{
		PlayerDamage(1);
	}
}

void XRay::Draw(void)
{
	DrawSpriteColor(m_texture, m_pos.x, m_pos.y, m_size*0.71f, m_size*0.71f, 0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

void XRay::Uninit(void)
{
	
}

bool HitCheckBlockToXRay(D3DXVECTOR2 raypos,float size)
{
	std::vector<std::vector<int>>* g_Stage = GetBlocks();

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
			D3DXVECTOR2 BlockPos = D3DXVECTOR2(x * BLOCK_SIZE, (y-1) * BLOCK_SIZE);

			if (HitCheckBox_Block(BlockPos, BLOCK_SIZE, BLOCK_SIZE, raypos, size , size))
			{
				return true;
			}
		}
	}

	return false;
}

bool HitCheckPlayerToXRay(D3DXVECTOR2 raypos, float raysize, D3DXVECTOR2 playerpos)
{
	PLAYER* pPlayer = GetPlayer();
	if (pPlayer->mutekiflag)
	{
		return false;
	}

	float size = raysize * 0.5f;
	//頂点の作成
	//外側４点
	D3DXVECTOR2 out[4] = {
		D3DXVECTOR2(raypos.x - size, raypos.y - size),//outLT
		D3DXVECTOR2(raypos.x + size, raypos.y - size),//outRT
		D3DXVECTOR2(raypos.x + size, raypos.y + size),//outRB
		D3DXVECTOR2(raypos.x - size, raypos.y + size),//outLB
	};
	//内側４点
	D3DXVECTOR2 in[4] = {
		D3DXVECTOR2(raypos.x, raypos.y - (size * 0.28f)), //inT
		D3DXVECTOR2(raypos.x + (size * 0.28f), raypos.y), //inR
		D3DXVECTOR2(raypos.x, raypos.y + (size * 0.28f)), //inB
		D3DXVECTOR2(raypos.x - (size * 0.28f), raypos.y), //inL
	};
	
	//プレイヤー４点
	D3DXVECTOR2 Player[5] = {
		D3DXVECTOR2(playerpos.x - 60.0f, playerpos.y - 60.0f), // PlayerLT
		D3DXVECTOR2(playerpos.x + 60.0f, playerpos.y - 60.0f), // PlayerRT
		D3DXVECTOR2(playerpos.x + 60.0f, playerpos.y + 60.0f), // PlayerRB
		D3DXVECTOR2(playerpos.x - 60.0f, playerpos.y + 60.0f), // PlayerLB
		D3DXVECTOR2(playerpos.x - 55.0f, playerpos.y - 55.0f), // PlayerLT <- もう一回
	};
	
	
	for (int player = 0; player < 4; player++)
	{
		if (HitCheckXRayLine(out[0], in[0], Player[player], Player[player + 1]))
			return true;

		if (HitCheckXRayLine(out[1], in[0], Player[player], Player[player + 1]))
			return true;

		if (HitCheckXRayLine(out[1], in[1], Player[player], Player[player + 1]))
			return true;

		if (HitCheckXRayLine(out[2], in[1], Player[player], Player[player + 1]))
			return true;

		if (HitCheckXRayLine(out[2], in[2], Player[player], Player[player + 1]))
			return true;

		if (HitCheckXRayLine(out[3], in[2], Player[player], Player[player + 1]))
			return true;

		if (HitCheckXRayLine(out[3], in[3], Player[player], Player[player + 1]))
			return true;

		if (HitCheckXRayLine(out[0], in[3], Player[player], Player[player + 1]))
			return true;
	}

	return false;
}

// 線と線の当たり判定
bool HitCheckXRayLine(D3DXVECTOR2 startA, D3DXVECTOR2 endA, D3DXVECTOR2 startB, D3DXVECTOR2 endB)
{
	// ２つの線分のベクトルをABとCDとする
	// ３つのベクトルからなる二つのグループを作成

	// グループ１
	D3DXVECTOR2 AB = endA - startA;
	D3DXVECTOR2 AC = startB - startA;
	D3DXVECTOR2 AD = endB - startA;

	// グループ２
	D3DXVECTOR2 CD = endB - startB;
	D3DXVECTOR2 CA = startA - startB;
	D3DXVECTOR2 CB = endA - startB;

	// 正負、負正の組み合わせなら当たっている
	float cross_AB_AC = crossX(AB, AC);
	float cross_AB_AD = crossX(AB, AD);
	if (cross_AB_AC*cross_AB_AD > 0.0f)
	{
		return false;
	}

	// 正負、負正の組み合わせなら当たっている
	float cross_CD_CA = crossX(CD, CA);
	float cross_CD_CB = crossX(CD, CB);
	if (cross_CD_CA*cross_CD_CB > 0.0f)
	{
		return false;

	}
	return true;
}

// 外積
float crossX(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2)
{
	return (vec1.x*vec2.y) - (vec2.x*vec1.y);
}