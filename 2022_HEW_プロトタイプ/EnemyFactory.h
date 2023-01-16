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

	// 敵のテクスチャ番号
	int m_EnemyHitDropNo = -1;
	int m_EnemySelfDestructNo = -1;
	int m_EnemyThrowBombNo = -1;
	int m_EnemyTenguNo = -1;

	int m_EnemyGhostFireNo = -1;
	int m_EnemyRushNo = -1;
	int m_EnemyExplosionGasNo = -1;
	int m_EnemyKasyaNo = -1;
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

	// Create関数
	void Create_HitDrop(D3DXVECTOR2 pos);
	void Create_SelfDestruction(D3DXVECTOR2 pos);
	void Create_ThrowBomb(D3DXVECTOR2 pos);
	void Create_GhostFire(D3DXVECTOR2 pos, int muki);
	void Create_Rush(D3DXVECTOR2 pos);
	void Create_ExplosionGas(D3DXVECTOR2 pos);

	// BOSS系Create関数
	void Create_Boss_Tengu(D3DXVECTOR2 pos);
	void Create_Boss_Kasya(D3DXVECTOR2 pos);

	// マップ情報からエネミーを作成
	void SetEnemy();

	// 当たり判定系関数
	// プレイヤーとブロックの当たり判定
	void CollisionWallToEnemy();
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

