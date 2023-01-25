
#include <vector>
#include "Bomb.h"
#include "game.h"
#include "ExplosionFactory.h"
#include "Block.h"

Bomb::Bomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos,Bomb::BOMB_TYPE type)
		: m_ID(ID),m_Pos(pos), m_StartPos(pos), m_EndPos(endPos),m_BombType(type)
{
	m_divid = D3DXVECTOR2(2.0f, 1.0f);
	m_pttern = D3DXVECTOR2(1.0f / m_divid.x, 1.0f / m_divid.y);
	m_Size = D3DXVECTOR2(60.0f, 60.0f);
	m_IsActive = true;
	m_pPlayer = GetPlayer();
	m_CollisionRad = m_Size.x / 2.0f;
	m_pExplosionFactory = GetExplosionFactory();

	// 開始点と終了点からベクトルを作成
	D3DXVECTOR2 startVec = D3DXVECTOR2(0.0f, -500.0f);
	D3DXVECTOR2 endVec = D3DXVECTOR2( 0.0f,  500.0f);
	D3DXVECTOR2 pVec = m_EndPos - m_StartPos;
	if (pVec.x < 0)
	{
		startVec.x = -5.0f;
		endVec.x =   -5.0f;
	}
	else
	{
		startVec.x = 5.0f;
		endVec.x = 5.0f;
	}
	m_StartVec = startVec;
	m_EndVec = endVec;

	m_ThrowPower = D3DXVECTOR2(0.1f, -500.0f);
	if (m_BombType == BOMB_TYPE::BOSS_CONTACT)
	{
		m_ThrowFrame = 100;
	}
	else
	{
		m_ThrowFrame = 70;
	}
	//D3DXVECTOR2 pVec = m_EndPos - m_StartPos;
	//float distance = D3DXVec2Length(&pVec);
	//m_ThrowFrame = distance/5.0f;
	//m_StartVec = D3DXVECTOR2(distance*m_ThrowPower.x, m_ThrowPower.y);
	//m_EndVec = D3DXVECTOR2(distance*m_ThrowPower.x, -1.0f*m_ThrowPower.y);
	//if (pVec.x < 0)
	//{
	//	m_StartVec.x *= -1.0f;
	//	m_EndVec.x *= -1.0f;
	//}
}

Bomb::~Bomb()
{
}

void Bomb::Throw()
{
	double t = (double)m_NowFrame / (double)m_ThrowFrame;	// 現在の経過割合
	// エルミート曲線を作成
	m_NextPos.x = (2.0f*m_StartPos.x + (-2.0f)*m_EndPos.x + m_StartVec.x + m_EndVec.x)*(t*t*t) +
		((-3.0f)*m_StartPos.x + 3.0f*m_EndPos.x + (-2.0f)*m_StartVec.x + (-1.0f)*m_EndVec.x)*(t*t) +
		(m_StartVec.x*t) + m_StartPos.x;
	m_NextPos.y = (2.0f*m_StartPos.y + (-2.0f)*m_EndPos.y + m_StartVec.y + m_EndVec.y)*(t*t*t) +
		((-3.0f)*m_StartPos.y + 3.0f*m_EndPos.y + (-2.0f)*m_StartVec.y + (-1.0f)*m_EndVec.y)*(t*t) +
		(m_StartVec.y*t) + m_StartPos.y;
}

bool Bomb::HitCheckCircle()
{
	// プレイヤーと爆弾のベクトルを計算
	D3DXVECTOR2 vec = m_Pos - m_pPlayer->pos;
	// ベクトルの長さを計算
	float len = D3DXVec2Length(&vec);
	// プレイヤーと爆弾の当たり判定の半径を足した数値よりも
	// ベクトルの方が短いなら当たっている
	if (len <= m_CollisionRad + m_pPlayer->size / 2.0f)
	{
		return true;
	}
	return false;
}

bool Bomb::HitCheckBlock(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize)
{
	float box1Xmin = m_NextPos.x - m_Size.x / 2;
	float box1Xmax = m_NextPos.x + m_Size.x / 2;
	float box1Ymin = m_NextPos.y - m_Size.y / 2;
	float box1Ymax = m_NextPos.y + m_Size.y / 2;

	float box2Xmin = blockPos.x;
	float box2Xmax = blockPos.x + blockSize.x;
	float box2Ymin = blockPos.y;
	float box2Ymax = blockPos.y + blockSize.y;

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

bool Bomb::CollisionBombToBlock()
{
	std::vector<std::vector<int>>* blocks = GetBlocks();
	for (int y = 0; y < blocks->size(); y++)
	{
		for (int x = 0; x < (*blocks)[0].size(); x++)
		{
			if ((*blocks)[y][x] != 1)
			{
				continue;
			}
			if (HitCheckBlock(D3DXVECTOR2(x*BLOCK_SIZE, y*BLOCK_SIZE), D3DXVECTOR2(BLOCK_SIZE, BLOCK_SIZE)))
			{
				return true;
			}
		}
	}
	return false;
}
