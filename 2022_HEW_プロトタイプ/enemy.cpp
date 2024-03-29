/*==============================================================================

   敵の処理 [enemy.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "enemy.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "player.h"
#include "collision.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_SPEED (6.0f)	//敵の速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

void Enemy::HitCheckPlayerAttack(D3DXVECTOR2 AttackPos)
{
	if (m_IsDead) {
		return;
	}
	if (m_IsInvincible)
	{
		return;
	}
	if (!m_IsStealth)
	{
		return;
	}
	if (HitCheckBox(AttackPos,120.0f,120.0f,m_Pos,m_CollisionSize.x, m_CollisionSize.y))
	{
		Damege(1);
	}
}

void Enemy::Damege(int damage)
{
	if (m_IsDead)
	{
		return;
	}

	m_HP -= damage;

	if (m_HP <= 0)
	{
		m_IsDie = true;
		m_IsDead = true;
	}
	else
	{
		m_IsInvincible = true;
	}
}
