#pragma once

#include "main.h"

class ShockWave
{
public:
public:
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[1] =
	{
		0.0f,
	};
	const float M_MukiTable[2] =
	{
		0.0f,	//右向き
		0.5f,
	};
	enum STATE_SHOCKWAVE
	{
		MOVE,
	};
private:

	D3DXVECTOR2 m_Pos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_Vel = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	int m_Muki = -1;	// 衝撃波の向き

	int m_TextureNo = -1;	// 爆弾が使用するテクスチャの番号

	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 爆弾の表示サイズ
	int m_AnimationPtn = 0;		// 爆弾のアニメーションのパターン番号
	int m_AnimationCounter = 0;	// 爆弾のアニメーションのカウンター
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;

	STATE_SHOCKWAVE m_State = STATE_SHOCKWAVE::MOVE;
public:
	ShockWave(D3DXVECTOR2 pos,D3DXVECTOR2 moveVec,int muki);
	// 爆弾の初期化処理
	void Init();
	// 爆弾の終了処理
	void Uninit();
	// 爆弾の更新処理
	void Update();
	// 爆弾の描画処理
	void Draw();
	~ShockWave();

	// Get系関数
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	bool GetIsActive() { return m_IsActive; };
};

