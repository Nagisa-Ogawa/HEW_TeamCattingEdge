#pragma once
#include "main.h"

struct CAMERA_2D;

class SwitchBullet
{
public:
	enum STATE_SWITCHBULLET
	{
		IDLE,
		MOVE,
		SWITCH,
		DEAD,
	};
	enum BULLET_MODE
	{
		ONCE,
		TWICE,
	};
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[6] =
	{
		0.000000f,
		0.166667f,
		0.333334f,
		0.500000f,
		0.666667f,
		0.833334f
	};
	const float M_MukiTable[2] =
	{
		0.00f,
		0.50f,
	};
private:
	CAMERA_2D* m_pCamera = nullptr;
	int m_ID = -1;
	STATE_SWITCHBULLET m_State = IDLE;
	BULLET_MODE m_mode;
	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_Vel;
	int m_TextureNo = -1;
	int m_WaitFrame = 0;
	D3DXVECTOR2 m_Power = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 火の玉の表示サイズ
	int m_AnimationPtn = 0;		// アニメーションのパターン番号
	int m_AnimationCounter = 0;	// アニメーションのカウンター
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	int m_Muki = 0;
	bool m_IsActive = false;
	D3DXVECTOR2 m_TargetPos = D3DXVECTOR2(0.0f, 0.0f);
	float m_TargetDistance = 0.0f;
	bool m_IsSwitch = false;
	D3DXVECTOR2 m_SwitchPos = D3DXVECTOR2(0.0f, 0.0f);
public:
	SwitchBullet(D3DXVECTOR2 pos, D3DXVECTOR2 size, int textureNo,D3DXVECTOR2 targetPos,int muki,BULLET_MODE mode);
	// 火の玉の初期化処理
	void Init();
	// 火の玉の終了処理
	void Uninit();
	// 火の玉の更新処理
	void Update();
	// 火の玉の描画処理
	void Draw();
	~SwitchBullet();

	void MoveBullet();
	void Switch(D3DXVECTOR2 pos);
	void Died();

	// Get系関数
	bool GetIsActive() { return m_IsActive; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	bool GetIsSwitch() { return m_IsSwitch; };
	STATE_SWITCHBULLET GetState() { return m_State; };
	BULLET_MODE GetMode() { return m_mode; };
	D3DXVECTOR2 GetSwitchPos() { return m_SwitchPos; };
};

