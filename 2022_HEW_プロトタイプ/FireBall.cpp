#include "FireBall.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"

FireBall::FireBall(int ID,D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power):
	m_ID(ID), m_Pos(pos),m_Muki(muki),m_ThrowPower(power)
{
	m_DistanceMax = 600.0f;
	m_StartPos = m_Pos;
	m_divid = D3DXVECTOR2(1.0f, 1.0f);
	m_pttern.x = 1.0f / m_divid.x;
	m_pttern.y = 1.0f / m_divid.y;
	// 敵のテクスチャを読み込み
	m_TextureNo = LoadTexture((char*)"data/TEXTURE/Fire.png");
	m_IsActive = true;
	m_Size = D3DXVECTOR2(60.0f, 60.0f);
}

void FireBall::Init()
{
}

void FireBall::Uninit()
{
}

void FireBall::Update()
{
	if (!m_IsActive) {
		return;
	}
	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
	
	switch (m_State)
	{
	case FireBall::MOVE:
		Move();
		break;
	case FireBall::BURNSOUT:
		m_IsActive = false;
		break;
	default:
		break;
	}

	m_Pos += m_Vel;
}

void FireBall::Draw()
{
	if (m_IsActive)
	{
		D3DXVECTOR2 basePos = GetBase();
		DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
			m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
	}
}

FireBall::~FireBall()
{
}

void FireBall::Move() {
	if (m_Muki == 0) {
		m_Vel.x += m_ThrowPower.x;
		m_Vel.y += m_ThrowPower.y;
	}
	else {
		m_Vel.x += m_ThrowPower.x * -1.0f;
		m_Vel.y += m_ThrowPower.y;
	}
	// 移動距離が最大距離まで行ったなら消える
	D3DXVECTOR2 pos = m_Pos + m_Vel;
	D3DXVECTOR2 vec = pos - m_StartPos;
	float distance = D3DXVec2Length(&vec);
	if (distance >= m_DistanceMax) {
		m_State = FIRE::BURNSOUT;
	}
}

void FireBall::Hit()
{
	m_IsActive = false;
}
