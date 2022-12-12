#pragma once
#include "Bomb.h"

class ShockWaveFactory;

class InstallationBomb : public Bomb
{
public:
	enum STATE_INSTALLATIONBOMB
	{
		THROW,
		SET,
		EXPLOSION,
	};
private:

	ShockWaveFactory* m_pShockWaveFactory;

	int m_TextureNo = 0;		// 爆弾が使用するテクスチャの番号
	bool m_IsGround = false;
	float m_Gravity = 0.0f;
	D3DXVECTOR2 m_Vel = D3DXVECTOR2(0.0f, 0.0f);

	STATE_INSTALLATIONBOMB m_State = STATE_INSTALLATIONBOMB::THROW;
public:
	InstallationBomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	// 爆弾の初期化処理
	void Init() override;
	// 爆弾の終了処理
	void Uninit() override;
	// 爆弾の更新処理
	void Update() override;
	// 爆弾の描画処理
	void Draw() override;
	~InstallationBomb();

	// Get系関数
	STATE_INSTALLATIONBOMB GetState() { return m_State; };

	// 衝撃波との当たり判定用関数
	void CollisionBombToShockWave();
	// 四角の当たり判定
	bool HitCheckSquare(D3DXVECTOR2 posA, D3DXVECTOR2 sizeA, D3DXVECTOR2 posB, D3DXVECTOR2 sizeB);

};

