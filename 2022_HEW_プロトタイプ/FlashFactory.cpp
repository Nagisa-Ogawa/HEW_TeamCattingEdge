#include "FlashFactory.h"
#include "texture.h"

FlashFactory::FlashFactory()
{
}

void FlashFactory::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	m_pFlashList.push_back(new Flash(pos, size,m_TextureNo));
}

void FlashFactory::Init()
{
	m_TextureNo = LoadTexture((char*)"data/TEXTURE/explosion.png");
}

void FlashFactory::Uninit()
{
	for (Flash* pFlash : m_pFlashList)
	{
		pFlash->Uninit();
	}
	for (Flash* pFlash : m_pFlashList)
	{
		delete pFlash;
	}

	m_pFlashList.clear();
}

void FlashFactory::Update()
{
	for (Flash* pFlash : m_pFlashList)
	{
		pFlash->Update();
	}
}

void FlashFactory::Draw()
{
	for (Flash* pFlash : m_pFlashList)
	{
		pFlash->Draw();
	}
}

FlashFactory::~FlashFactory()
{
}
