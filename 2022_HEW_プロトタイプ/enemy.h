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
private:
	int m_ID = -99999;	// 敵を識別するためのID
protected:
	D3DXVECTOR2 m_Pos = D3DXVECTOR2(0.0f, 0.0f);	// 位置座標
	D3DXVECTOR2 m_OldPos = D3DXVECTOR2(0.0f, 0.0);	// 1フレーム前の位置座標
	D3DXVECTOR2 m_Vel = D3DXVECTOR2(0.0f, 0.0f);	// 速度ベクトル
	D3DXVECTOR2 m_Dir = D3DXVECTOR2(0.0f, 0.0f);	// 移動方向
	bool m_Muki = true;			// 敵が向いている方向(右ならtrue,左ならfalse)
	float m_Speed = 0.0f;		// 移動速度
	//bool m_IsUse = false;		// 使用しているかのフラグ
	int m_EnemyTextureNo;		// 敵が使用するテクスチャの番号
	D3DXVECTOR2 m_Size = D3DXVECTOR2(0.0f, 0.0f);	// 敵の表示サイズ
	int m_AnimationPtn = 0;		// 敵のアニメーションのパターン番号
	int m_AnimationCounter = 0;	// 敵のアニメーションのカウンター
	float m_Gravity = 0.98f;	// 重力
	bool m_IsGround = false;	// 敵の設置判定フラグ
	int m_HP;
	D3DXVECTOR2 m_divid;
	D3DXVECTOR2 m_pttern;
	bool m_IsActive = false;
public:
	Enemy(D3DXVECTOR2 pos, int ID, D3DXVECTOR2 size, D3DXVECTOR2 divid) :
			m_Pos(pos), m_ID(ID), m_Size(size), m_divid(divid) 
	{ 
		m_pttern.x = 1.0f / m_divid.x;
		m_pttern.y = 1.0f / m_divid.y;
		m_IsActive = true;
		m_IsGround = false;
	};
	// エネミーの初期化処理
	virtual void Init() = 0;
	// エネミーの終了処理
	virtual void Uninit() = 0;
	// エネミーの更新処理
	virtual void Update() = 0;
	// エネミーの描画処理
	virtual void Draw() = 0;

	// エネミーの戦闘系関数
	// プレイヤーの攻撃との当たり判定関数
	void HitCheckPlayerAttack(D3DXVECTOR2 AttackPos);
	// ダメージ関数
	void Damege(int damage);
	// Get系関数
	D3DXVECTOR2 GetPos() { return m_Pos; };
	D3DXVECTOR2 GetSize() { return m_Size; };
	bool GetIsActive() { return m_IsActive; };
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
