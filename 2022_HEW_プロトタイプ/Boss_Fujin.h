#pragma once
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

class ExplosionFactory;
class WindBladeFactory;
class RayFactory;
class FlashFactory;
class EnemyFactory;
struct PLAYER;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class Boss_Fujin:public Enemy
{
public:
	enum STATE_ENEMY_FUJIN
	{
		INHALE,		// 吸い込み
		BULLET_X,	// X弾
		WINDBLADE,	// 風の刃
		AVATOR,		// 分身
		IDLE,		// 待機
		ATTACK,		// 近接攻撃
		MOVE,
		WAIT,		// 待機
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
	const float M_MukiTable[12] =
	{
		0.000000f,	// 移動（左）
		0.083333f,	// 移動（右）
		0.166666f,	// 近接（左）
		0.249999f,	// 近接（右）
		0.333332f,	// やられ（右）
		0.416665f,	// ブレス（左）
		0.500000f,	// ブレス（右）
		0.583333f,	// 移動（左）
		0.666666f,	// 移動（右）
		0.749999f,	// 停止（左）
		0.833332f,	// 停止（右）
		0.916665f,
	};

	STATE_ENEMY_FUJIN m_State = IDLE;
	STATE_ENEMY_FUJIN m_BeforeState = IDLE;
	STATE_ENEMY_FUJIN m_LastTimeState = IDLE;
	int m_StateCount = 0;

	// 待機フレーム
	int m_WaitFrame = 0;
	int m_AnimeFrame = 0;
	PLAYER* m_pPlayer = nullptr;
	WindBladeFactory* m_pWindBladeFactory = nullptr;
	FlashFactory* m_pFlashFactory = nullptr;
	EnemyFactory* m_pEnemyFactory = nullptr;
	RayFactory* m_pRayFactory = nullptr;
	// 近接攻撃用変数宣言
	int m_AttackTextureNo = -1;
	D3DXVECTOR2 m_AttackCollisionSize = D3DXVECTOR2(0.0f, 0.0f);
	// 吸い込み攻撃用変数宣言
	D3DXVECTOR2 m_InHalePower = D3DXVECTOR2(0.0f, 0.0f);
	float m_NowDistance = 0.0f;
	float m_AttackDistance = 0.0f;
	// X弾攻撃用変数宣言
	D3DXVECTOR2 m_BeforeShotPos = D3DXVECTOR2(0.0f, 0.0f);
	// 移動用変数宣言
	D3DXVECTOR2 m_MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_StartPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_EndPos = D3DXVECTOR2(0.0f, 0.0f);
	float m_MoveDistance = 0.0f;
	int m_BeforeMuki = -1;
	int m_MoveCount = 0;

public:
	Boss_Fujin(D3DXVECTOR2 pos, int ID, int textureNo,bool isDuo);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;

	void InHale();
	void ShotBullet_X();
	void Avator();

	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJIN state,int muki);
	void Move();

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Boss_Fujin();
};

