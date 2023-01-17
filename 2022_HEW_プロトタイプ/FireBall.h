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
		START,
		MOVE,
		BURNSOUT,
	};
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[3] =
	{
		0.000f,
		0.333f,
		0.666f,
	};
	const float M_MukiTable[4] =
	{
		0.00f,
		0.25f,
		0.50f,
		0.75f,
	};


private:
	int m_ID = -1;
	FIRE m_State = START;
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
	float m_Rot = 0;

	//サイズ変更系変数
	int m_StartChangeSizeFrame = 0;

	//OneShotモード用変数
	bool m_isHittingWall = false;
	bool m_isPassThrough = false;
	bool m_isReflect = false;
	int m_BurnsOutCount = 0;
	const int m_ReflectWallCounts[4]
	{
		1,
		3,
		2,
		3
	};
	int m_NowReflectWallCount = 0;
	int m_NowReflectCount = 0;
	int m_NowReflectAllCount = 0;

	float m_BlockLength = 99999;//一番近いブロックの距離
	int m_BlockIndex[2]{ -99999,-99999 };//1一番近いブロックの添え字
	float m_BlockHeight = 0.0f;

	//ThreeShotモード用変数

public:
	FireBall(int ID,D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power,MODE mode,int textureNo);
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
	FIRE GetState() { return m_State; };
	int GetReflectWallCounts() { return m_ReflectWallCounts[m_NowReflectWallCount]; };
	int GetNowReflectCount() { return m_NowReflectCount; };
	int GetNowReflectAllCount() { return m_NowReflectAllCount; };
	int GetNowReflectWallCount() { return m_NowReflectWallCount; };

	int GetBlockIndexX() { return m_BlockIndex[0]; };
	int GetBlockIndexY() { return m_BlockIndex[1]; };
	float GetBlockLength() { return m_BlockLength; };
	float GetBlockHeight() { return m_BlockHeight; };

	// Set系関数
	void SetThrowPower(D3DXVECTOR2 throwPower) { m_ThrowPower = throwPower; };
	void SetHittingFlag(bool flag) { m_isHittingWall = flag; };
	void SetPassThroughFlag(bool flag) { m_isPassThrough = flag; };
	void SetReflect(bool flag) { m_isReflect = flag; };
	void SetNowReflectCount(int num) { m_NowReflectCount = num; };
	void SetNowReflectAllCount(int num) { m_NowReflectAllCount = num; };
	void SetNowReflectWallCount(int num) { m_NowReflectWallCount = num; };
	void SetBlockIndexX(int num) { m_BlockIndex[0] = num; };
	void SetBlockIndexY(int num) { m_BlockIndex[1] = num; };
	void SetBlockLength(float num) { m_BlockLength = num; };
	void SetBlockHeight(float num) { m_BlockHeight = num; };

	void Move();
	void Hit();
	void RotToVecotr(D3DXVECTOR2 vector);
};

