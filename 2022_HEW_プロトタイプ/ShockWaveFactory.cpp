#include <algorithm>
#include "ShockWaveFactory.h"

ShockWaveFactory::ShockWaveFactory()
{
}

void ShockWaveFactory::Create(D3DXVECTOR2 pos, D3DXVECTOR2 moveVec,int muki)
{
	m_pShockWaveList.push_back(new ShockWave(pos, moveVec,muki));
}

void ShockWaveFactory::Init()
{
	for (ShockWave* shockWave : m_pShockWaveList)
	{
		shockWave->Init();
	}
	// Create(D3DXVECTOR2(1200.0f, 900.0f), D3DXVECTOR2(-6.0f, 0.0f));
}

void ShockWaveFactory::Uninit()
{
	for (ShockWave* shockWave : m_pShockWaveList)
	{
		shockWave->Uninit();
	}
	for (ShockWave* shockWave : m_pShockWaveList)
	{
		delete shockWave;
	}
	m_pShockWaveList.clear();
}

void ShockWaveFactory::Update()
{
	for (ShockWave* shockWave : m_pShockWaveList)
	{
		shockWave->Update();
	}
	DeleteShockWave();
}

void ShockWaveFactory::Draw()
{
	for (ShockWave* shockWave : m_pShockWaveList)
	{
		shockWave->Draw();
	}
}

ShockWaveFactory::~ShockWaveFactory()
{
	for (ShockWave* shockWave : m_pShockWaveList)
	{
		delete shockWave;
	}
	m_pShockWaveList.clear();
}

void ShockWaveFactory::DeleteShockWave()
{
	while (true)
	{
		auto deleteIt = std::find_if(m_pShockWaveList.begin(), m_pShockWaveList.end(), ([&](ShockWave* shockWave) {return (shockWave->GetIsActive() == false); }));
		// •\¦‚µ‚Ä‚¢‚È‚¢”š’e‚ªˆêŒÂ‚à‚È‚¢‚È‚ç”²‚¯‚é
		if (deleteIt == m_pShockWaveList.end())
		{
			break;
		}
		// ”š”­Ï‚İ‚Ì”š’e‚Ííœ
		delete (*deleteIt);
		m_pShockWaveList.erase(deleteIt);
	}
}
