#pragma once

#include "enemy.h"
#include "game.h"

class Boss_Tengu:public Enemy
{
public:
	enum STATE_BOSS_TENGU
	{
		IDLE,
		SETUP,
		JUMP,
		GLID,
		DROP,
		AFTERDROP,
		THROW,
		AFTERTHROW,
		WAIT,
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
	const float M_MukiTable[4] =
	{
		0.0f,	// 右向きのしかかり攻撃
		0.25f,	// 左向きのしかかり攻撃
		0.50f,	// 右向き爆弾投げ攻撃
		0.75f,	// 左向き爆弾なげ攻撃
	};

	STATE_BOSS_TENGU m_State = IDLE;

	// ジャンプ系
	float m_ActiveRad_Jump = 0.0f;
	D3DXVECTOR2 m_JumpPowerMax = D3DXVECTOR2(0.0f, 0.0f);	// 敵のジャンプの最大パワー
	D3DXVECTOR2 m_JumpPower = D3DXVECTOR2(0.0f, 0.0f);	// 敵のジャンプのパワー
	D3DXVECTOR2 m_JumpAttenuation = D3DXVECTOR2(0.0f, 0.0f);	// ジャンプ中の減衰力
	float m_DropPower = 0.0f;	// ドロップ中のパワー
	float m_DropPosX = 0.0f;	// プレイヤーがいた座標
	float m_ChangeGlidPowerY = 0.0f;	// ジャンプから滑空へ移行するときの基準値
	float m_ChangeDropPowerY = 0.0f;	// 滑空からドロップへ移行するときの基準値
	float m_AddDropPower = 0.0f;		// 落下パワーに毎フレーム足す値

	// 衝撃波系
	ShockWaveFactory* m_pShockWaveFactory = nullptr;
	D3DXVECTOR2 m_ShockWavePower = D3DXVECTOR2(0.0f, 0.0f);

	// 爆弾系
	BombFactory* m_pBombFactory = nullptr;
	float m_ActiveRad_Throw = 0.0f;	// 爆弾を投げ始める範囲
	int m_WaitTime = 0;	// 爆弾を投げた後の待機時間
	int m_CooldownTime = 0;	// 爆弾を投げ終わった後のクールタイム時間
	float m_ThrowOffset = 0.0f;	// 爆弾を投げるときのボスからのオフセット
	float m_ThrowDistance = 0.0f;	// 爆弾同士の間隔
	// 待機フレーム
	int m_WaitFrame = 0;
public:
	Boss_Tengu(D3DXVECTOR2 pos, int ID);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;
	~Boss_Tengu();

	// ジャンプの準備をする関数
	void ChangeSetUp();
	// ジャンプ
	void Jump();
	// 滑空
	void Glid();
	// 衝撃波を発生させる関数
	void ShockWave();
	// 爆弾を投げる
	void Throw();
	// プレイヤーの方を向く関数
	void LookPlayer();

};

