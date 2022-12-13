#include <algorithm>
#include "BombFactory.h"
#include "Bomb.h"
#include "ContactBomb.h"
#include "InstallationBomb.h"
#include "game.h"
#include "ShockWaveFactory.h"

BombFactory::BombFactory()
{
}

void BombFactory::CreateContactBomb(D3DXVECTOR2 createPos,D3DXVECTOR2 endPos)
{
	m_nowID++;
	m_pBombList.push_back(new ContactBomb(m_nowID, createPos,endPos));
}

void BombFactory::CreateInstallationBomb(D3DXVECTOR2 createPos, D3DXVECTOR2 endPos)
{
	m_nowID++;
	m_pBombList.push_back(new InstallationBomb(m_nowID, createPos, endPos));
}

void BombFactory::Init()
{
	// Create(D3DXVECTOR2(300.0f,300.0f), D3DXVECTOR2(0.0f, 0.0f));
	for (Bomb* bomb : m_pBombList)
	{
		bomb->Init();
	}
}

void BombFactory::Uninit()
{
	for (Bomb* bomb : m_pBombList)
	{
		bomb->Uninit();
	}
}

void BombFactory::Update()
{
	for (Bomb* bomb : m_pBombList)
	{
		bomb->Update();
	}
	// 爆発済みの爆弾がないかチェック
	DeleteBomb();
}

void BombFactory::Draw()
{
	for (Bomb* bomb : m_pBombList)
	{
		bomb->Draw();
	}
}

BombFactory::~BombFactory()
{
	for (Bomb* bomb : m_pBombList)
	{
		delete bomb;
	}
	m_pBombList.clear();
}

void BombFactory::DeleteBomb()
{
	while (true)
	{
		auto deleteIt = std::find_if(m_pBombList.begin(), m_pBombList.end(), ([&](Bomb* bomb) {return (bomb->GetIsActive() == false); }));
		// 表示していない爆弾が一個もないなら抜ける
		if (deleteIt == m_pBombList.end())
		{
			break;
		}
		// 爆発済みの爆弾は削除
		delete (*deleteIt);
		m_pBombList.erase(deleteIt);
	}
}
