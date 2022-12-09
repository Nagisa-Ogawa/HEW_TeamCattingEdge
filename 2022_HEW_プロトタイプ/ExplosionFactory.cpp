#include "ExplosionFactory.h"

ExplosionFactory::ExplosionFactory()
{
}

void ExplosionFactory::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	m_pExplosion.push_back(new Explosion(pos, size));
}

void ExplosionFactory::Init()
{
	// Create(D3DXVECTOR2(300.0f, 300.0f), D3DXVECTOR2(120.0f, 120.0f));
	for (Explosion* pExplosion : m_pExplosion)
	{
		pExplosion->Init();
	}
}

void ExplosionFactory::Uninit()
{
	for (Explosion* pExplosion : m_pExplosion)
	{
		pExplosion->Uninit();
	}
}

void ExplosionFactory::Update()
{
	for (Explosion* pExplosion : m_pExplosion)
	{
		pExplosion->Update();
	}
}

void ExplosionFactory::Draw()
{
	for (Explosion* pExplosion : m_pExplosion)
	{
		pExplosion->Draw();
	}
}

ExplosionFactory::~ExplosionFactory()
{
	for (Explosion* pExplosion : m_pExplosion)
	{
		delete pExplosion;
	}
	m_pExplosion.clear();
}
