#pragma once

#include "main.h"

class FireBallFactory;

class FireBall
{
public:
	enum FIRE
	{
		MOVE,
		BURNSOUT,
	};
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[1] =
	{
		0.0f,
	};
	const float M_MukiTable[1] =
	{
		0.0f,	//右向き
	};

private:
	int m_ID = -1;
	FIRE m_State = MOVE;

	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_StartPos;
	D3DXVECTOR2 m_Vel;
	D3DXVECTOR2 m_ThrowPower = D3DXVECTOR2(0.0f, 0.0f);
	FireBallFactory* m_pFireBallFactory = nullptr;
	float m_DistanceMax = 0.0f;

	int m_TextureNo = -1;
	int m_WaitFrame = 0;
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 火の玉の表示サイズ
	int m_AnimationPtn = 0;		// 火の玉のアニメーションのパターン番号
	int m_AnimationCounter = 0;	// 火の玉のアニメーションのカウンター
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	int m_Muki = 0;
	bool m_IsActive = false;

public:
	FireBall(int ID,D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power);
	// 火の玉の初期化処理
	void Init();
	// 火の玉の終了処理
	void Uninit();
	// 火の玉の更新処理
	void Update();
	// 火の玉の描画処理
	void Draw();
	~FireBall();

	// Get系関数
	bool GetIsActive() { return m_IsActive; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };

	void Move();
	void Hit();
};

