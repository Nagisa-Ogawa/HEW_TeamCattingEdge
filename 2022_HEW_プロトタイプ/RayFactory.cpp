#include "RayFactory.h"
#include "XRay.h"
#include "TRay.h"
#include <algorithm>

void RayFactory::Init(void)
{
}

void RayFactory::Update(void)
{
	for (RayInterface* p : m_pRayList)
	{
		p->Update();
	}

	while (1)
	{
		auto deleteit = std::find_if(m_pRayList.begin(), m_pRayList.end(), [=](RayInterface* p) {
			if (p->GetUse() == false) {
				return true;
			}
			return false;
		});

		if (deleteit != m_pRayList.end())
		{
			delete (*deleteit);
			m_pRayList.erase(deleteit);
		}
		else
		{
			break;
		}
	}
	
}

void RayFactory::Draw(void)
{
	for (RayInterface* p : m_pRayList)
	{
		p->Draw();
	}
}

void RayFactory::Uninit(void)
{
}

void RayFactory::CreateXRay(D3DXVECTOR2 pos, D3DXVECTOR2 playerpos)
{
	auto It = m_pRayList.begin();
	m_pRayList.insert(It, new XRay(pos, playerpos));
}

void RayFactory::CreateTRay(D3DXVECTOR2 pos, D3DXVECTOR2 playerpos)
{
	auto It = m_pRayList.begin();
	m_pRayList.insert(It, new TRay(pos, playerpos));
}
