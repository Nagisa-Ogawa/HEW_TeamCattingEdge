#pragma once

#include <list>
#include "enemy.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY_HITDORP 10		//敵の表示最大数

//*****************************************************************************
// ヒットドロップする敵のクラス
//*****************************************************************************
class EnemyFactory
{
private:
	std::list<Enemy*> m_pEnemyList;
	PLAYER* m_pPlayer = nullptr;
	int m_nowID = -1;
public:
	EnemyFactory();
	// エネミー工場の初期化処理
	void Init();
	// エネミー工場の終了処理
	void Uninit();
	// エネミー工場の更新処理
	void Update();
	// エネミー工場の描画処理
	void Draw();

	void Create_HitDrop(D3DXVECTOR2 pos);
	void Create_SelfDestruction(D3DXVECTOR2 pos);
	void Create_ThrowBomb(D3DXVECTOR2 pos);
	void Create_Boss(D3DXVECTOR2 pos);

	// マップ情報からエネミーを作成
	void SetEnemy();

	// 当たり判定系関数
	// プレイヤーとエネミーの当たり判定(プレイヤーへのダメージ)
	void CollisionPlayerToEnemy();
	// プレイヤーとボスの当たり判定(プレイヤーへのダメージ)
	// void CollisionPlayerToBoss();
	// プレイヤー攻撃とエネミーの当たり判定(エネミーへのダメージ）
	void CollisoinAttacktoEnemy(D3DXVECTOR2 AttackPos);

	// エネミーを削除する
	void DeleteEnemy();

	~EnemyFactory();
};

