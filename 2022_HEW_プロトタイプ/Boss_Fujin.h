#pragma once
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

class ExplosionFactory;
struct PLAYER;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class Boss_Fujin:public Enemy
{
public:
	enum STATE_ENEMY_FUJIN
	{
		IDLE,		// 待機
		ATTACK,		// 近接攻撃
		INHALE,	// 吸い込み
		BULLET_X,	// X弾
		WINDBLADE,	// 風の屋いな
		AVATOR,		// 分身
		WIT,		// 待機
		DEAD,		// 死亡
	};
private:
	const float m_AnimeTable[6] =
	{
		0.000000f,
		0.166667f,
		0.333334f,
		0.500000f,
		0.666667f,
		0.833334f
	};
	const float M_MukiTable[10] =
	{
		0.0f,	// 待機（左）
		0.1f,	// 待機（右）
		0.2f,	// やられ（左）
		0.3f,	// やられ（右）
		0.4f,	// ブレス（左）
		0.5f,	// ブレス（右）
		0.6f,	// 移動（左）
		0.7f,	// 移動（右）
		0.8f,	// 停止（左）
		0.9f,	// 停止（右）
	};

	STATE_ENEMY_FUJIN m_State = IDLE;
	// 待機フレーム
	int m_WaitFrame = 0;
	PLAYER* m_pPlayer = nullptr;
	// 近接攻撃用変数宣言
	int m_AttackTextureNo = -1;
	D3DXVECTOR2 m_AttackCollisionSize = D3DXVECTOR2(0.0f, 0.0f);
	// 吸い込み攻撃用変数宣言
	D3DXVECTOR2 m_InHalePower = D3DXVECTOR2(0.0f, 0.0f);
	float m_NowDistance = 0.0f;
	float m_AttackDistance = 0.0f;

public:
	Boss_Fujin(D3DXVECTOR2 pos, int ID, int textureNo);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;

	void InHale();

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Boss_Fujin();
};

