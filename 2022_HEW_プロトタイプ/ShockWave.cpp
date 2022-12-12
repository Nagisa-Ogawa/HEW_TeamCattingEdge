#include "ShockWave.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"

ShockWave::ShockWave(D3DXVECTOR2 pos, D3DXVECTOR2 moveVec,int muki):m_MoveVec(moveVec),m_Muki(muki)
{
	m_Size = D3DXVECTOR2(60.0f, 120.0f);
	m_Pos = D3DXVECTOR2(pos.x, pos.y - m_Size.y / 2);
	m_divid = D3DXVECTOR2(1.0f, 2.0f);
	m_pttern = D3DXVECTOR2(1.0f / m_divid.x, 1.0f / m_divid.y);
	m_IsActive = true;
	// 敵のテクスチャを読み込み
	m_TextureNo = LoadTexture((char*)"data/TEXTURE/ShockWave.png");
}

void ShockWave::Init()
{
}

void ShockWave::Uninit()
{
}

void ShockWave::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	// 決まった方向に進む
	m_Vel += m_MoveVec;
	// 壁との当たり判定
	// 画面外チェック
	m_Pos += m_Vel;


	m_Vel = D3DXVECTOR2(0.0f, 0.0f);
}

void ShockWave::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

ShockWave::~ShockWave()
{
}
