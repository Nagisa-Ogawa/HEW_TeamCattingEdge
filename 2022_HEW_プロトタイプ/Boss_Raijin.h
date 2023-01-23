#pragma once
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

class ExplosionFactory;
class RayFactory;
class EnemyFactory;
class FlashFactory;
class ThunderBladeFactory;
struct PLAYER;

class Boss_Raijin :public Enemy
{
public:
	enum STATE_ENEMY_RAIJIN
	{
		IDLE,			// 待機
		ATTACK,			// 近接攻撃
		SWITCH_BULLET,	// 特殊弾と位置を入れ替え
		BULLET_T,		// ＋弾
		THUNDERBLADE,	// 雷の刃
		AVATOR,			// 分身
		MOVE,
		WAIT,			// 待機
		DEAD,			// 死亡
	};
private:
	const float m_AnimeTable[6] =
	{
		0.000000f,
		0.166667f,
		0.333334f,
		0.500000f,
		0.666667f,
		0.833334f
	};
	const float M_MukiTable[10] =
	{
		0.0f,	// 待機（左）
		0.1f,	// 待機（右）
		0.2f,	// やられ（左）
		0.3f,	// やられ（右）
		0.4f,	// ブレス（左）
		0.5f,	// ブレス（右）
		0.6f,	// 移動（左）
		0.7f,	// 移動（右）
		0.8f,	// 停止（左）
		0.9f,	// 停止（右）
	};

	STATE_ENEMY_RAIJIN m_State = IDLE;
	STATE_ENEMY_RAIJIN m_BeforeState = IDLE;

	// 待機フレーム
	int m_WaitFrame = 0;
	PLAYER* m_pPlayer = nullptr;
	RayFactory* m_pRayFactory = nullptr;
	EnemyFactory* m_pEnemyFactory = nullptr;
	FlashFactory* m_pFlashFactory = nullptr;
	ThunderBladeFactory* m_pThunderFactory = nullptr;
	// 近接攻撃用変数宣言
	int m_AttackTextureNo = -1;
	D3DXVECTOR2 m_AttackCollisionSize = D3DXVECTOR2(0.0f, 0.0f);
	// 雷撃用変数宣言
	D3DXVECTOR2 m_ThunderBoltPos = D3DXVECTOR2(0.0f, 0.0f);
	// 移動用変数宣言
	D3DXVECTOR2 m_MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_StartPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 m_EndPos = D3DXVECTOR2(0.0f, 0.0f);
	float m_MoveDistance = 0.0f;
	int m_BeforeMuki = -1;
	int m_MoveCount = 0;


public:
	Boss_Raijin(D3DXVECTOR2 pos, int ID, int textureNo);
	// エネミーの初期化処理
	void Init() override;
	// エネミーの終了処理
	void Uninit() override;
	// エネミーの更新処理
	void Update() override;
	// エネミーの描画処理
	void Draw() override;

	void ThunderBlade();
	void Avator();

	/*void SetMove(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, STATE_ENEMY_RAIJIN state, int muki);
	void Move();
*/
	void AfterHitCheckBlockX(DWORD result) override;
	void AfterHitCheckBlockY(DWORD result) override;

	~Boss_Raijin();
};