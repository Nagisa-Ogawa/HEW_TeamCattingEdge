#include "EnemyFactory.h"
#include "Enemy_HitDrop.h"

EnemyFactory::EnemyFactory()
{
	auto enemyIt = m_pEnemyList.begin();
	m_pEnemyList.insert(enemyIt, new Enemy_HitDrop(D3DXVECTOR2(700.0f, 100.0f), 0));
	m_pEnemyList.insert(enemyIt, new Enemy_HitDrop(D3DXVECTOR2(800.0f, 200.0f), 0));
}

void EnemyFactory::Init()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Init();
	}
}

void EnemyFactory::Uninit()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Uninit();
	}
}

void EnemyFactory::Update()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Update();
	}
}

void EnemyFactory::Draw()
{
	for (Enemy* pEnemy : m_pEnemyList)
	{
		pEnemy->Draw();
	}
}

EnemyFactory::~EnemyFactory()
{
}
