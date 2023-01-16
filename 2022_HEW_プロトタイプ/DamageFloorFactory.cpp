#include "DamageFloorFactory.h"
#include "DamageFloor.h"
#include "texture.h"


DamageFloorFactory::DamageFloorFactory()
{
}

void DamageFloorFactory::Init()
{
	m_DamageFloorTextureNo = LoadTexture((char*)"data/TEXTURE/DamageFloor.png");
	// Create(D3DXVECTOR2(960.0f, 300.0f));
}

void DamageFloorFactory::Uninit()
{
	for (DamageFloor* pDamageFloor : m_pDamageFloorList)
	{
		pDamageFloor->Uninit();
	}
}

void DamageFloorFactory::Update()
{
	for (DamageFloor* pDamageFloor : m_pDamageFloorList)
	{
		pDamageFloor->Update();
	}
}

void DamageFloorFactory::Draw()
{
	for (DamageFloor* pDamageFloor : m_pDamageFloorList)
	{
		pDamageFloor->Draw();
	}
}

DamageFloorFactory::~DamageFloorFactory()
{
	for (DamageFloor* pDamageFloor : m_pDamageFloorList)
	{
		delete pDamageFloor;
	}
	m_pDamageFloorList.clear();
}

void DamageFloorFactory::Create(D3DXVECTOR2 pos)
{
	m_pDamageFloorList.push_back(new DamageFloor(pos, m_DamageFloorTextureNo));
}
