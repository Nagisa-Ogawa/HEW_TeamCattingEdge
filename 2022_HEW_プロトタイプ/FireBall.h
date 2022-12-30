#pragma once

#include "main.h"

class FireBallFactory;

class FireBall
{
public:
	enum MODE
	{
		GHOSTFIRE,
		KASYA_ONESHOT,
		KASYA_THREESHOT,
	};
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
	MODE m_Mode = GHOSTFIRE;

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

	//OneShotモード用変数
	bool m_isHittingWall = false;
	bool m_isPassThrough = false;
	bool m_isReflect = false;
	int m_BurnsOutCount = 5;

public:
	FireBall(int ID,D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power,MODE mode);
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
	D3DXVECTOR2 GetPower() { return m_ThrowPower; };
	bool GetHittingFlag() { return m_isHittingWall; };
	bool GetPassThroughFlag() { return m_isPassThrough; };
	int GetBurnsOutCount() { return m_BurnsOutCount; };
	bool GetIsReflectFlag() { return m_isReflect; };
	MODE GetMode() { return m_Mode; };

	// Set系関数
	void SetThrowPower(D3DXVECTOR2 throwPower) { m_ThrowPower = throwPower; };
	void SetHittingFlag(bool flag) { m_isHittingWall = flag; };
	void SetPassThroughFlag(bool flag) { m_isPassThrough = flag; };
	void SetReflect(bool flag) { m_isReflect = flag; };


	void Move();
	void Hit();
};

