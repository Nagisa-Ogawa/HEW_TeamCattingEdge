#include "Enemy_Sub_ThrowBomb.h"
#include "player.h"
#include "game.h"
#include "sound.h"

static int	g_SE_dead;	// SEの識別子
static int	g_SE_throw;		// SEの識別子


Enemy_Sub_ThrowBomb::Enemy_Sub_ThrowBomb(D3DXVECTOR2 pos, int ID, int textureNo):Enemy_ThrowBomb::Enemy_ThrowBomb(pos, ID, textureNo)
{
	m_Size.x = 240.0f;
	m_Size.y = 240.0f;
	m_ActiveRange = 800.0f;

	g_SE_throw = LoadSound((char*)"data/SE/Tengu_throw.wav");
	SetVolume(g_SE_throw, 1.0f);
	g_SE_dead = LoadSound((char*)"data/SE/Tengu_dead.wav");
	SetVolume(g_SE_dead, 0.5f);
}

Enemy_Sub_ThrowBomb::~Enemy_Sub_ThrowBomb()
{
	SetGameScene(GAMESCENE_PICTURE_STAGE01_BOSS);
}

void Enemy_Sub_ThrowBomb::Uninit(void)
{
	SetGameScene(GAMESCENE_PICTURE_STAGE01_BOSS);
}

void Enemy_Sub_ThrowBomb::Update(void)
{
	// 死亡していたなら死亡状態へ
	if (m_IsDie) {
		m_IsDie = false;
		m_AnimationPtn = 0;
		m_Muki += 2;
		m_WaitFrame = 0;
		m_State = DEAD;
		PlaySound(g_SE_dead, 0);
	}
	m_OldPos = m_Pos;
	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();
	switch (m_State)
	{
	case Enemy_ThrowBomb::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// プレイヤーが範囲内に入ったなら爆弾を投げる
		if (len < m_ActiveRange)
		{
			m_AnimationPtn++;
			m_State = STATE_ENEMY_THROWBOMB::THROW;
		}
		break;
	}
	case Enemy_ThrowBomb::THROW:
		Throw();
		break;
	case Enemy_ThrowBomb::WAIT:
		if (m_WaitFrame >= m_WaitTime)
		{
			m_State = STATE_ENEMY_THROWBOMB::COOLDOWN;
			m_AnimationPtn = 0;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Enemy_ThrowBomb::COOLDOWN:
		if (m_WaitFrame >= m_CooldownTime)
		{
			m_State = STATE_ENEMY_THROWBOMB::IDLE;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Enemy_ThrowBomb::DEAD:
		if (m_WaitFrame >= m_DeadAnimeFrame)
		{
			m_AnimationPtn++;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		if (m_AnimationPtn > m_DeadAnimeNum - 1) {
			m_IsActive = false;
		}
		break;
	default:
		break;
	}
}
