#pragma once

#include "Bomb.h"


class ContactBomb:public Bomb
{
public:
	enum STATE_CONTACTBOMB
	{
		THROW,
		EXPLOSION,
	};
private:
	
	int m_TextureNo;		// 爆弾が使用するテクスチャの番号

	STATE_CONTACTBOMB m_State = STATE_CONTACTBOMB::THROW;
public:
	ContactBomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	// 爆弾の初期化処理
	void Init() override;
	// 爆弾の終了処理
	void Uninit() override;
	// 爆弾の更新処理
	void Update() override;
	// 爆弾の描画処理
	void Draw() override;
	~ContactBomb();

};

