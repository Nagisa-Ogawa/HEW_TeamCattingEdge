#define _USE_MATH_DEFINES

#include "FireBall.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include <math.h>

FireBall::FireBall(int ID,D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power,FireBall::MODE mode,int textureNo):
	m_ID(ID), m_Pos(pos),m_Muki(muki),m_ThrowPower(power),m_Mode(mode)
{
	m_DistanceMax = 600.0f;
	m_StartPos = m_Pos;
	m_divid = D3DXVECTOR2(3.0f, 4.0f);
	m_pttern.x = 1.0f / m_divid.x;
	m_pttern.y = 1.0f / m_divid.y;
	m_IsActive = true;
	m_Size = D3DXVECTOR2(160.0f, 160.0f);
	m_StartChangeSizeFrame = 3;
	m_TextureNo = textureNo;
	switch (m_Mode)
	{
	case FireBall::GHOSTFIRE:
		m_BurnsOutCount = 0;
		break;
	case FireBall::KASYA_ONESHOT:
		m_BurnsOutCount = 9;
		RotToVecotr(m_ThrowPower);
		break;
	case FireBall::KASYA_THREESHOT:
		m_BurnsOutCount = 1;
		RotToVecotr(m_ThrowPower);
		break;
	default:
		break;
	}
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
	case FireBall::START:
		// ‚¾‚ñ‚¾‚ñ‘å‚«‚­‚È‚é
		if (m_WaitFrame >= m_StartChangeSizeFrame)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn >= 3) {
				m_AnimationPtn = 0;
				m_Muki += 2;
				m_State = FireBall::MOVE;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		switch (m_Mode)
		{
		case FireBall::GHOSTFIRE:
			Move();
			break;
		case FireBall::KASYA_ONESHOT:
			m_Vel += m_ThrowPower;
			break;
		case FireBall::KASYA_THREESHOT:
			m_Vel += m_ThrowPower;
			break;
		default:
			break;
		}
		break;
	case FireBall::MOVE:
		switch (m_Mode)
		{
		case FireBall::GHOSTFIRE:
			Move();
			break;
		case FireBall::KASYA_ONESHOT:
			m_Vel += m_ThrowPower;
			break;
		case FireBall::KASYA_THREESHOT:
			m_Vel += m_ThrowPower;
			break;
		default:
			break;
		}
		// ‚¾‚ñ‚¾‚ñ‘å‚«‚­‚È‚é
		if (m_WaitFrame >= 5)
		{
			m_WaitFrame = 0;
			m_AnimationPtn++;
			if (m_AnimationPtn > 3) {
				m_AnimationPtn = 0;
			}
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case FireBall::BURNSOUT:
		// ‚¾‚ñ‚¾‚ñ¬‚³‚­‚È‚é
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
		switch (m_Mode)
		{
		case FireBall::GHOSTFIRE:
			DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
				m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
			break;
		case FireBall::KASYA_ONESHOT:
			DrawSpriteColorRotate(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
				m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y,
				D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),m_Rot);
			break;
		case FireBall::KASYA_THREESHOT:
			DrawSpriteColorRotate(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
				m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_Rot);
			break;
		default:
			break;
		}
	}
}

FireBall::~FireBall()
{
}

void FireBall::Move() {
	if (m_Muki % 2 == 0) {
		m_Vel.x += m_ThrowPower.x * -1.0f;
		m_Vel.y += m_ThrowPower.y;
	}
	else {
		m_Vel.x += m_ThrowPower.x;
		m_Vel.y += m_ThrowPower.y;
	}
	// ˆÚ“®‹——£‚ªÅ‘å‹——£‚Ü‚Ås‚Á‚½‚È‚çÁ‚¦‚é
	D3DXVECTOR2 pos = m_Pos + m_Vel;
	D3DXVECTOR2 vec = pos - m_StartPos;
	float distance = D3DXVec2Length(&vec);
	if (distance >= m_DistanceMax) {
		m_State = FIRE::BURNSOUT;
	}
}

void FireBall::Hit()
{
	m_State = FIRE::BURNSOUT;
}

void FireBall::RotToVecotr(D3DXVECTOR2 vector)
{
	float rot = 0;
	if (vector.x < 0) {
		rot = atan2(vector.y, vector.x);
	}
	else {
		rot = atan2(vector.y, vector.x); //+ M_PI;
	}
	m_Rot = rot;
}
