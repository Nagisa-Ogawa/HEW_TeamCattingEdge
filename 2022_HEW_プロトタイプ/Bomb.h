#pragma once
#include "main.h"
#include "player.h"

class ExplosionFactory;

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
	enum BOMB_TYPE
	{
		CONTACT,
		INSTALLATION,
		BOSS_CONTACT,
	};
protected:
	int m_ID = -1;

	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_StartPos;
	D3DXVECTOR2 m_EndPos;
	D3DXVECTOR2 m_StartVec;
	D3DXVECTOR2 m_EndVec;
	D3DXVECTOR2 m_NextPos;

	D3DXVECTOR2 m_ThrowPower = D3DXVECTOR2(0.0f, 0.0f);

	BOMB_TYPE m_BombType;	// 爆弾の種類

	int	m_ThrowFrame;	// 爆弾の移動フレーム

	PLAYER* m_pPlayer = nullptr;	// プレイヤーのポインタ
	ExplosionFactory* m_pExplosionFactory = nullptr;

	float m_CollisionRad = 0.0f;	// 爆弾の当たり判定用の半径

	int m_NowFrame;
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 爆弾の表示サイズ
	int m_AnimationPtn = 0;		// 爆弾のアニメーションのパターン番号
	int m_AnimationCounter = 0;	// 爆弾のアニメーションのカウンター
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
public:
	Bomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos,BOMB_TYPE type);
	// 爆弾の初期化処理
	virtual void Init() = 0;
	// 爆弾の終了処理
	virtual void Uninit() = 0;
	// 爆弾の更新処理
	virtual void Update() = 0;
	// 爆弾の描画処理
	virtual void Draw() = 0;
	~Bomb();

	// Get系関数
	BOMB_TYPE GetBombType() { return m_BombType; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	int GetID() { return m_ID; };
	bool GetIsActive() { return m_IsActive; };

	// 放物線に爆弾を投げる関数
	void Throw();
	// 円と円の当たり判定
	bool HitCheckCircle();
	// 四角と四角の当たり判定
	bool HitCheckBlock(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize);
	// ブロックとの当たり判定
	bool CollisionBombToBlock();

};

