#pragma once
#include "main.h"

struct CAMERA_2D;

class ThunderBlade
{
public:
	enum STATE_THUNDERBLADE
	{
		IDLE,
		THUNDERBOLT,
		DEAD,
	};
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[1] =
	{
		0.0f,
	};
	const float M_MukiTable[1] =
	{
		0.00f,
	};
private:
	CAMERA_2D* m_pCamera = nullptr;
	int m_ID = -1;
	STATE_THUNDERBLADE m_State = IDLE;
	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_Vel;
	int m_TextureNo = -1;
	int m_WaitFrame = 0;
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 火の玉の表示サイズ
	int m_AnimationPtn = 0;		// アニメーションのパターン番号
	int m_AnimationCounter = 0;	// アニメーションのカウンター
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	int m_Muki = 0;
	bool m_IsActive = false;
	bool m_IsCanHit = false;
	int m_ThunderBoltFrame = 0;
	int m_HitStartFrame = 0;
public:
	ThunderBlade(D3DXVECTOR2 pos, D3DXVECTOR2 size, int textureNo);
	// 火の玉の初期化処理
	void Init();
	// 火の玉の終了処理
	void Uninit();
	// 火の玉の更新処理
	void Update();
	// 火の玉の描画処理
	void Draw();
	~ThunderBlade();

	void ThunderBolt();

	// Get系関数
	bool GetIsActive() { return m_IsActive; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	bool GetIsCanHit() { return m_IsCanHit; };

};

