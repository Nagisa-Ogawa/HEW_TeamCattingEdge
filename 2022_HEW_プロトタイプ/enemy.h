#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// #define MAX_ENEMY 100		//敵の表示最大数


//*****************************************************************************
// 敵の基底クラス
//*****************************************************************************
class Enemy 
{
public:
	enum ENEMY_TYPE
	{
		HITDROP,
		SELFDESTRUCT,
		THROWBOMB,
		GHOSTFIRE,
		RUSH,
		EXPLOSIONGAS,
		FUFINAVATOR,
		BOSS_TENGU,
		BOSS_KASYA,
		BOSS_FUJIN,
		BOSS_RAIJIN,
	};
private:
	int m_ID = -99999;	// 敵を識別するためのID
protected:
	ENEMY_TYPE m_EnemyType;
	D3DXVECTOR2 m_Pos = D3DXVECTOR2(0.0f, 0.0f);	// 位置座標
	D3DXVECTOR2 m_OldPos = D3DXVECTOR2(0.0f, 0.0);	// 1フレーム前の位置座標
	D3DXVECTOR2 m_Vel = D3DXVECTOR2(0.0f, 0.0f);	// 速度ベクトル
	D3DXVECTOR2 m_Dir = D3DXVECTOR2(0.0f, 0.0f);	// 移動方向
	int m_Muki = -1;			// 敵が向いている方向(右ならtrue,左ならfalse)
	float m_Speed = 0.0f;		// 移動速度
	//bool m_IsUse = false;		// 使用しているかのフラグ
	int m_EnemyTextureNo;		// 敵が使用するテクスチャの番号
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 敵の表示サイズ
	D3DXVECTOR2 m_CollisionSize = D3DXVECTOR2(0.0f, 0.0f); // 敵の当たり判定サイズ
	int m_AnimationPtn = 0;		// 敵のアニメーションのパターン番号
	int m_AnimationCounter = 0;	// 敵のアニメーションのカウンター
	float m_Gravity = 0.98f;	// 重力
	bool m_IsGround = false;	// 敵の設置判定フラグ
	int m_HP;
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;	// 表示フラグ
	bool m_IsDie = false;		// 死亡した後1フレームの間だけtrueになるフラグ
	bool m_IsDead = false;		// 死亡フラグ
	int m_DeadAnimeNum = 0;		// 死亡時アニメーションの枚数
	int m_DeadAnimeFrame = 0;	// 死亡時アニメーションの切り替えフレーム数

	float m_BlockLength = 99999;//一番近いブロックの距離
	int m_BlockIndex[2]{ -99999,-99999 };//1一番近いブロックの添え字
	float m_BlockHeight = 0.0f;
	bool m_IsEndSetUp = false;
	bool m_IsStealth = true;

public:
	Enemy(D3DXVECTOR2 pos, int ID, D3DXVECTOR2 size, D3DXVECTOR2 divid,int textureNo,ENEMY_TYPE enemyType) :
			m_Pos(pos), m_ID(ID), m_Size(size), m_divid(divid) ,m_EnemyTextureNo(textureNo),m_EnemyType(enemyType)
	{ 
		m_pttern.x = 1.0f / m_divid.x;
		m_pttern.y = 1.0f / m_divid.y;
		m_IsActive = true;
		m_IsGround = false;
		m_IsDead = false;
		m_CollisionSize = size;
	};
	// エネミーの初期化処理
	virtual void Init() = 0;
	// エネミーの終了処理
	virtual void Uninit() = 0;
	// エネミーの更新処理
	virtual void Update() = 0;
	// エネミーの描画処理
	virtual void Draw() = 0;

	// エネミーの当たり判定をした後の処理
	virtual void AfterHitCheckBlockX(DWORD result) = 0;
	virtual void AfterHitCheckBlockY(DWORD result) = 0;

	// エネミーの戦闘系関数
	// プレイヤーの攻撃との当たり判定関数
	void HitCheckPlayerAttack(D3DXVECTOR2 AttackPos);
	// ダメージ関数
	void Damege(int damage);
	// Get系関数
	ENEMY_TYPE GetEnemyType() { return m_EnemyType; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	D3DXVECTOR2 GetCollisionSize() { return m_CollisionSize; };
	D3DXVECTOR2 GetVel() { return m_Vel; };
	bool GetIsActive() { return m_IsActive; };
	bool GetIsDead() { return m_IsDead; };
	int GetBlockIndexX() { return m_BlockIndex[0]; };
	int GetBlockIndexY() { return m_BlockIndex[1]; };
	float GetBlockLength() { return m_BlockLength; };
	float GetBlockHeight() { return m_BlockHeight; };
	bool GetIsEndSetUp() { return m_IsEndSetUp; };
	bool GetIsStealth() { return m_IsStealth; };

	// Set系関数
	void SetBlockIndexX(int num) { m_BlockIndex[0] = num; };
	void SetBlockIndexY(int num) { m_BlockIndex[1] = num; };
	void SetBlockLength(float num) { m_BlockLength = num; };
	void SetBlockHeight(float num) { m_BlockHeight = num; };
	~Enemy() {};
};

//enum ENEMYSTATE
//{
//	chase = 0,
//	wait,
//	Return,
//	NUN_STATE
//};

//struct ENEMY
//{
//	D3DXVECTOR2 pos;
////	D3DXVECTOR2	vel;	//速度ベクトル
//	D3DXVECTOR2 dir;	//移動方向
//	float		speed;	//移動スピード
//
//	D3DXVECTOR2 start_pos;
//
//	float		height;	//高さ
//	int			muki;	//向き
//
//	ENEMYSTATE		state;  //状態管理
//
//	int			frame;	//有効フレーム
//	int			use;	//可視フラグ
//};
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//void InitEnemy(void);
//void UninitEnemy(void);
//void UpdateEnemy(void);
//void DrawEnemy(void);
//
//void SetEnemy(void);
//
//ENEMY* GetEnemy(void);
