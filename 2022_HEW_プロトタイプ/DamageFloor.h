#pragma once
#include "main.h"


class DamageFloor
{
public:
	const float m_AnimeTable[4] =
	{
		0.0f,
		0.25f,
		0.50f,
		0.75f,
	};
	const float M_MukiTable[2] =
	{
		0.0f,
		0.5f,	//左向き待機
	};
private:
	int m_ID = -99999;	// 識別するためのID
	D3DXVECTOR2 m_Pos = D3DXVECTOR2(0.0f, 0.0f);	// 位置座標
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 表示サイズ
	int m_AnimationPtn = 0;		// アニメーションのパターン番号
	int m_AnimationCounter = 0;	// アニメーションのカウンター
	int m_Muki = 0;
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;	// 表示フラグ
	int m_TextureNo;		// 使用するテクスチャの番号
	int m_WaitFrame = 0;
public:
	DamageFloor(D3DXVECTOR2 pos, int textureNo);
	// エネミーの初期化処理
	void Init();
	// エネミーの終了処理
	void Uninit();
	// エネミーの更新処理
	void Update();
	// エネミーの描画処理
	void Draw();

	bool GetIsActive() { return m_IsActive; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	~DamageFloor();

};

