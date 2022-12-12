#include "Boss_Tengu.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"
#include "Block.h"
#include "player.h"
#include "BombFactory.h"
#include "ShockWaveFactory.h"

#define TENGU_WAITFRAME_SETUP (120)
#define TENGU_WAITFRAME_AFTERDROP (20)
#define TENGU_WAITFRAME_WAIT (60)

Boss_Tengu::Boss_Tengu(D3DXVECTOR2 pos, int ID):Enemy(pos,ID,D3DXVECTOR2(480.0f,480.0f),D3DXVECTOR2(6.0f,4.0f))
{
	// 敵のサイズを設定
	m_Gravity = 4.0f;
	m_HP = 10;
	// ヒットドロップ系変数初期化
	m_DropPower = 0.98f;
	m_ActiveRad_Jump = 600.0f;
	m_JumpPowerMax = D3DXVECTOR2(5.0f, -15.0f);
	m_JumpAttenuation = D3DXVECTOR2(0.2f, 0.2f);
	m_ChangeGlidPowerY = -4.0f;
	m_ChangeDropPowerY = 3.0f;
	m_AddDropPower = 0.98f;
	// 衝撃波系変数初期化
	m_pShockWaveFactory = GetShockWaveFactory();
	m_ShockWavePower = D3DXVECTOR2(6.0f, 0.0f);
	// 爆弾系変数初期化
	m_ActiveRad_Throw = 800.0f;
	m_pBombFactory = GetBombFactory();
	m_ThrowDistance = 200.0f;
	m_ThrowOffset = 300.0f;
}

void Boss_Tengu::Init()
{
	// 敵のテクスチャを読み込み
	m_EnemyTextureNo = LoadTexture((char*)"data/TEXTURE/Boss_Tengu.png");
}

void Boss_Tengu::Uninit()
{
}

void Boss_Tengu::Update()
{
	if (!m_IsActive)
	{
		return;
	}

	m_OldPos = m_Pos;
	DWORD result = 0;
	PLAYER* pPlayer = GetPlayer();

	switch (m_State)
	{
	case Boss_Tengu::IDLE:
	{
		D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
		float len = D3DXVec2Length(&pVec);
		// プレイヤーが範囲内に入ったなら攻撃準備へ
		if (len < m_ActiveRad_Jump&&m_IsGround)
		{
			ChangeSetUp();
			m_ActiveRad_Jump = 0.0f;
		}
		if (len < m_ActiveRad_Throw)
		{
			m_State = Boss_Tengu::THROW;
			m_ActiveRad_Throw = 0.0f;
		}
		break;
	}
	case Boss_Tengu::SETUP:
		// 待機
		if (m_WaitFrame >= TENGU_WAITFRAME_SETUP)
		{
			m_State = Boss_Tengu::JUMP;
			m_WaitFrame = 0;
			// ジャンプアニメーションにする
			m_AnimationPtn++;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Boss_Tengu::JUMP:
		// ジャンプ処理
		Jump();
		break;
	case Boss_Tengu::GLID:
		Glid();
	case Boss_Tengu::DROP:
		m_DropPower += m_AddDropPower;
		if (m_IsGround)
		{
			// 衝撃波を左右に作成
			ShockWave();
			// 落下後の待機アニメーションへ
			m_DropPower = m_AddDropPower;
			m_State = Boss_Tengu::AFTERDROP;
		}
		break;
	case Boss_Tengu::AFTERDROP:
		if (m_WaitFrame >= TENGU_WAITFRAME_AFTERDROP)
		{
			m_State = Boss_Tengu::WAIT;
			m_WaitFrame = 0;
			// 待機アニメーションにする
			m_AnimationPtn = 0;
			m_ActiveRad_Throw = 800.0f;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	case Boss_Tengu::THROW:
		Throw();
		m_ActiveRad_Jump = 600.0f;
		break;
	case Boss_Tengu::WAIT:
		if (m_WaitFrame >= TENGU_WAITFRAME_WAIT)
		{
			m_State = Boss_Tengu::IDLE;
			m_WaitFrame = 0;
		}
		else
		{
			m_WaitFrame++;
		}
		break;
	default:
		break;
	}

	result = HitChackEnemy_Block(m_Pos, m_Size, m_Vel);
	//当たり判定処理
	if (result & HIT_LEFT)
	{
		if (m_Vel.x > 0.0)
			m_Vel.x = 0.0f;
	}
	if (result & HIT_RIGHT)
	{
		if (m_Vel.x < 0.0)
			m_Vel.x = 0.0f;
	}

	m_Pos.x += m_Vel.x;

	if (m_State == Boss_Tengu::JUMP||m_State==Boss_Tengu::GLID)
	{
		m_Vel.y += m_JumpPower.y;
	}
	else if (m_State == Boss_Tengu::DROP)
	{
		m_Vel.y += m_DropPower;
	}
	else
	{
		m_Vel.y += m_Gravity;
	}

	result = HitChackEnemy_Block(m_Pos, m_Size, m_Vel);
	//落下させるか？処理
	if ((result & HIT_UP) == 0 && m_IsGround == true)
	{
		m_IsGround = false;
	}

	//落下処理
	if (m_IsGround == false)
	{
		if (result & HIT_UP)
		{
			m_IsGround = true;
			m_Pos.y = GetBlockHeight() - (m_Size.y / 2);
			m_Vel.y = 0.0f;
		}
	}
	else // 最終的に地面に触れている
	{
		m_Vel.y = 0.0f;
	}

	m_Pos.y += m_Vel.y;
	// プレイヤーの方を向く関数
	LookPlayer();

	m_Vel = D3DXVECTOR2(0.0f, 0.0f);

	////アニメーションカウンターをカウントアップして、ウエイト値を超えたら
	//if (m_AnimationCounter > 20)
	//{
	//	//アニメーションパターンを切り替える
	//	m_AnimationPtn++;
	//	//最後のアニメーションパターンを表示したらリセットする
	//	if (m_AnimationPtn >= m_divid.x)
	//	{
	//		m_AnimationPtn = 0;
	//	}
	//	//アニメーションカウンターのリセット
	//	m_AnimationCounter = 0;
	//}
	//m_AnimationCounter++;
}

void Boss_Tengu::Draw()
{
	if (!m_IsActive)
	{
		return;
	}
	D3DXVECTOR2 basePos = GetBase();
	DrawSprite(m_EnemyTextureNo, basePos.x + m_Pos.x, basePos.y + m_Pos.y, m_Size.x, m_Size.y,
		m_AnimeTable[m_AnimationPtn], M_MukiTable[m_Muki], m_pttern.x, m_pttern.y);
}

Boss_Tengu::~Boss_Tengu()
{
}

void Boss_Tengu::ChangeSetUp()
{
	m_State = Boss_Tengu::SETUP;
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	// プレイヤーと敵の位置から飛ぶ方向と落下予定座標を決定
	if (pVec.x > 0)
	{
		m_JumpPower = m_JumpPowerMax;
		m_JumpPower.x *= -1.0f;
		m_JumpAttenuation.x *= -1.0f;
	}
	else
	{
		m_JumpPower = m_JumpPowerMax;
	}
	m_DropPosX = pPlayer->pos.x;
	// 準備アニメーションにする
	m_AnimationPtn++;
}

void Boss_Tengu::Jump()
{
	// ジャンプ
	m_Vel.x += m_JumpPower.x;
	// 減衰
	m_JumpPower.y += m_JumpAttenuation.y;
	// 移動中にプレイヤーがいた座標まで来たならそこに落下
	// 敵の向きによって変える
	D3DXVECTOR2 enemyPos = m_Pos + m_Vel;
	if (m_Muki == 0)
	{
		if (enemyPos.x >= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn += 2;
		}
	}
	else
	{
		if (enemyPos.x <= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn += 2;
		}
	}
	// Y方向の移動量が弱くなり始めたら滑空へ
	if (m_JumpPower.y >= m_ChangeGlidPowerY)
	{
		m_State = Boss_Tengu::GLID;
		m_AnimationPtn++;
	}
}

void Boss_Tengu::Glid()
{
	// ジャンプ
	m_Vel.x += m_JumpPower.x;
	// 減衰
	m_JumpPower.y += m_JumpAttenuation.y;
	// 移動中にプレイヤーがいた座標まで来たならそこに落下
	// 敵の向きによって変える
	D3DXVECTOR2 enemyPos = m_Pos + m_Vel;
	if (m_Muki == 0)
	{
		if (enemyPos.x >= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn++;
		}
	}
	else
	{
		if (enemyPos.x <= m_DropPosX)
		{
			m_State = Boss_Tengu::DROP;
			m_AnimationPtn++;
		}
	}
	// 下に落ち始めたらドロップへ
	if (m_JumpPower.y >= m_ChangeDropPowerY)
	{
		m_State = Boss_Tengu::DROP;
		m_AnimationPtn++;
	}
}

void Boss_Tengu::ShockWave()
{
	// 一度ブロックとの当たり判定を確認
	HitChackEnemy_Block(m_Pos, m_Size, m_Vel);
	// 触れているブロックのすぐ上に衝撃波の位置を決定
	D3DXVECTOR2 pos;
	pos.x = m_Pos.x;
	pos.y = GetBlockHeight();
	// 左右に衝撃波を作成
	m_pShockWaveFactory->Create(pos, m_ShockWavePower, 0);
	m_pShockWaveFactory->Create(pos, D3DXVECTOR2(m_ShockWavePower.x*-1, m_ShockWavePower.y), 1);
}

void Boss_Tengu::Throw()
{
	// 爆弾を投げる場所を決定
	PLAYER* pPlayer = GetPlayer();
	// 始点と終点と開始ベクトルと終了ベクトルを決定
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	// 距離から移動ベクトルの量を変化
	D3DXVECTOR2 startVec = D3DXVECTOR2(0.0f, -800.0f);
	D3DXVECTOR2 endVec = D3DXVECTOR2(0.0f, 800.0f);
	float distance = m_ThrowDistance;
	float offset = m_ThrowOffset;
	if (pVec.x < 0)
	{
		startVec.x = -5.0f;
		endVec.x = -5.0f;
	}
	else
	{
		startVec.x = 5.0f;
		endVec.x = 5.0f;
		distance *= -1.0f;
		offset *= -1.0f;
	}
	// 3つ爆弾を生成
	for (int i = 0; i < 3; i++)
	{
		m_pBombFactory->CreateInstallationBomb(m_Pos, D3DXVECTOR2(m_Pos.x+(i*distance)+offset , pPlayer->pos.y), startVec, endVec);
	}
	m_State = Boss_Tengu::WAIT;
}

void Boss_Tengu::LookPlayer()
{
	// 向きを変えてはいけない状態なら変えない
	if (m_State== Boss_Tengu::JUMP || m_State == Boss_Tengu::GLID || m_State == Boss_Tengu::DROP||m_State==Boss_Tengu::SETUP)
	{
		return;
	}
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR2 pVec = m_Pos - pPlayer->pos;
	if (pVec.x > 0)
	{
		m_Muki = 1;
	}
	else
	{
		m_Muki = 0;
	}
}
