#include "WindBladeFactory.h"
#include "player.h"
#include "camera.h"
#include "WindBlade.h"
#include "texture.h"

WindBladeFactory::WindBladeFactory()
{
}

void WindBladeFactory::Init()
{
	m_pPlayer = GetPlayer();
	m_pCamera = GetCamera();
	m_WindBladeNo = LoadTexture((char*)"data/TEXTURE/ShockWave.png");
}

void WindBladeFactory::Uninit()
{
	for (WindBlade* pWindBlade : m_pWindBladeList)
	{
		pWindBlade->Uninit();
	}
}

void WindBladeFactory::Update()
{
	for (WindBlade* pWindBlade : m_pWindBladeList)
	{
		pWindBlade->Update();
	}
}

void WindBladeFactory::Draw()
{
	for (WindBlade* pWindBlade : m_pWindBladeList)
	{
		pWindBlade->Draw();
	}
}

void WindBladeFactory::Create(D3DXVECTOR2 pos, int muki)
{
	m_pWindBladeList.push_back(new WindBlade(pos, muki,m_WindBladeNo));
}

void WindBladeFactory::CollisionWindBladeToPlayer()
{
}

WindBladeFactory::~WindBladeFactory()
{
}
