#include "Explosion.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"

Explosion::Explosion(D3DXVECTOR2 pos,D3DXVECTOR2 size):m_Pos(pos),m_Size(size)
{
	m_divid = D3DXVECTOR2(7.0f, 1.0f);
	m_pttern = D3DXVECTOR2(1.0f / m_divid.x, 1.0f / m_divid.y);
	// 敵のテクスチャを読み込み
	m_TextureNo = LoadTexture((char*)"data/TEXTURE/explosion.png");
	m_IsActive = true;
	m_CollisionRad = m_Size.x / 2.0f;
};

void Explosion::Init()
{
}

void Explosion::Uninit()
{
}

void Explosion::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	//アニメーションカウンターをカウントアップして、ウエイト値を超えたら
	if (m_AnimationCounter > 6)
	{
		//アニメーションパターンを切り替える
		m_AnimationPtn++;
		//最後のアニメーションパターンを表示したらリセットする
		if (m_AnimationPtn >= m_divid.x)
		{
			m_IsActive = false;
		}
		//アニメーションカウンターのリセット
		m_AnimationCounter = 0;
	}
	m_AnimationCounter++;
}

void Explosion::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_TextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[0], m_pttern.x, m_pttern.y);
}

Explosion::~Explosion()
{
}
