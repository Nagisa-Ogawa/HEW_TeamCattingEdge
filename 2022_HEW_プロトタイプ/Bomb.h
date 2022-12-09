#pragma once

#include "game.h"
// #include "main.h"
#include "player.h"

class Bomb
{
public:
	// アニメーションのUV値の指定に使う配列
	const float m_AnimeTable[2] =
	{
		0.0f,
		0.5f,
	};
	const float M_MukiTable[1] =
	{
		0.0f,	//右向き
	};
	enum STATE_BOMB
	{
		THROW,
		EXPLOSION,
	};
private:
	
	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_StartPos;
	D3DXVECTOR2 m_EndPos;
	D3DXVECTOR2 m_StartVec;
	D3DXVECTOR2 m_EndVec;
	D3DXVECTOR2 m_NextPos;

	PLAYER* m_pPlayer = nullptr;	// プレイヤーのポインタ
	ExplosionFactory* m_pExplosionFactory = nullptr;

	float m_CollisionRad = 0.0f;	// 爆弾の当たり判定用の半径

	int	m_ThrowFrame;
	int m_NowFrame;
	int m_TextureNo;		// 爆弾が使用するテクスチャの番号
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 爆弾の表示サイズ
	int m_AnimationPtn = 0;		// 爆弾のアニメーションのパターン番号
	int m_AnimationCounter = 0;	// 爆弾のアニメーションのカウンター
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
	STATE_BOMB m_State = STATE_BOMB::THROW;
public:
	Bomb(D3DXVECTOR2 pos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	// 爆弾の初期化処理
	void Init();
	// 爆弾の終了処理
	void Uninit();
	// 爆弾の更新処理
	void Update();
	// 爆弾の描画処理
	void Draw();
	~Bomb();

	void Throw();
	bool HitCheckCircle();
	bool HitCheckBlock(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize);
	void CollisionBombToBlock();
};

