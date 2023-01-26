#pragma once
#include "enemy.h"

class WindBladeFactory;
class ThunderBladeFactory;
class FlashFactory;
class EnemyFactory;

class Enemy_BossAvator :public Enemy
{
public:
	enum STATE_ENEMY_FUJINAVATOR
	{
		IDLE,		// 待機
		WINDBLADE,	// 風の刃
		THUNDERBLADE,
		MOVE,
		DEAD,		// 死亡
	};
private:
	const float m_AnimeFujinTable[6] =
	{
		0.000000f,
		0.166667f,
		0.333334f,
		0.500000f,
		0.666667f,
		0.833334f
	};
	const float m_AnimeRaijinTable[8] =
	{
		0.000f,
		0.125f,
		0.250f,
		0.375f,
		0.500f,
		0.625f,
		0.750f,
		0.875f,
	};
	const float M_MukiFujinTable[12] =
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

	const float M_MukiRaijinTable[8] =
	{
		0.000f,
		0.125f,
		0.250f,
		0.375f,
		0.500f,
		0.625f,
		0.750f,
		0.875f,
	};

	STATE_ENEMY_FUJINAVATOR m_State = IDLE;
	STATE_ENEMY_FUJINAVATOR m_BeforeState = IDLE;
	// 待機フレーム
	int m_WaitFrame = 0;
	WindBladeFactory* m_pWindBladeFactory = nullptr;
	ThunderBladeFactory* m_pThunderBladeFactory = nullptr;
	FlashFactory* m_pFlashFactory = nullptr;
	EnemyFactory* m_pEnemyFactory = nullptr;
	// 移動用変数宣言
	float m_NowDistance = 0.0f;
	D3DXVECTOR2 m_MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_StartPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_EndPos = D3DXVECTOR2(0.0f, 0.0f);
	float m_MoveDistance = 0.0f;
	int m_BeforeMuki = -1;
	int m_MoveCount = 0;
	D3DXVECTOR2 m_TargetPos = D3DXVECTOR2(0.0f, 0.0f);
public:
	Enemy_BossAvator(D3DXVECTOR2 pos, int ID, int textureNo,D3DXVECTOR2 targetPos,D3DXVECTOR2 divid, Enemy::ENEMY_TYPE mode,int muki);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;

	void WindBlade();
	void ThunderBlade();
	void SetMove_Fujin(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJINAVATOR state, int muki);
	void SetMove_Raijin(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJINAVATOR state, int muki);
	void Move_Fujin();
	void Move_Raijin();

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;


	~Enemy_BossAvator();

};

