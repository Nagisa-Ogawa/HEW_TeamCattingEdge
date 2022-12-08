#include "BombFactory.h"

BombFactory::BombFactory()
{
}

void BombFactory::Create(D3DXVECTOR2 createPos,D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec)
{
	m_pBombList.push_back(new Bomb(createPos,endPos,startVec,endVec));
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
