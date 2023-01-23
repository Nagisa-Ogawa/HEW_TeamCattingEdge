#pragma once
#include "enemy.h"

class WindBladeFactory;
class EnemyFactory;

class Enemy_BossAvator :public Enemy
{
public:
	enum STATE_ENEMY_FUJINAVATOR
	{
		IDLE,		// 待機
		WINDBLADE,	// 風の刃
		MOVE,
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

	STATE_ENEMY_FUJINAVATOR m_State = IDLE;
	STATE_ENEMY_FUJINAVATOR m_BeforeState = IDLE;
	// 待機フレーム
	int m_WaitFrame = 0;
	WindBladeFactory* m_pWindBladeFactory = nullptr;
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
	Enemy_BossAvator(D3DXVECTOR2 pos, int ID, int textureNo,D3DXVECTOR2 targetPos);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;

	void WindBlade();
	void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_FUJINAVATOR state, int muki);
	void Move();

	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;


	~Enemy_BossAvator();

};

