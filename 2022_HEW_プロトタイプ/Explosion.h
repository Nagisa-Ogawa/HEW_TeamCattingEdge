#pragma once

#include "main.h"

class Explosion
{
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[7] =
	{
		0.0f,
		0.142857f,
		0.285714f,
		0.428571f,
		0.571428f,
		0.714285f,
		0.857142f
	};
	const float M_MukiTable[1] =
	{
		0.0f,	//右向き
	};
	enum STATE_EXPLOSION
	{
		THROW,
		EXPLOSION,
	};
private:
	D3DXVECTOR2 m_Pos;

	float m_CollisionRad = 0.0f;	// 爆弾の当たり判定用の半径

	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 爆弾の表示サイズ
	int m_AnimationPtn = 0;		// 爆弾のアニメーションのパターン番号
	int m_AnimationCounter = 0;	// 爆弾のアニメーションのカウンター
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
	STATE_EXPLOSION m_State;
	int m_TextureNo = 0;
public:
	Explosion(D3DXVECTOR2 pos,D3DXVECTOR2 size);
	// 爆発の初期化処理
	void Init();
	// 爆発の終了処理
	void Uninit();
	// 爆発の更新処理
	void Update();
	// 爆発の描画処理
	void Draw();
	// 
	~Explosion();
};

