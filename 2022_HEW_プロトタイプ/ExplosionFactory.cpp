#include "ExplosionFactory.h"
#include "player.h"

ExplosionFactory::ExplosionFactory()
{
}

void ExplosionFactory::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	m_pExplosion.push_back(new Explosion(pos, size, (size.x / 2.0f), (size.x / 2.0f) + 50.0f));
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
	CollisionBlastToPlayer();
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

void ExplosionFactory::CollisionBlastToPlayer()
{
	// �v���C���[���擾
	PLAYER* pPlayer = GetPlayer();
	// �v���C���[�����G�Ȃ瓖����Ȃ�
	if (pPlayer->mutekiflag)
	{
		return;
	}
	// �S�Ă̔������`�F�b�N
	for (Explosion* pExplosion : m_pExplosion)
	{
		// ��������ʊO�Ȃ瓖����Ȃ�
		if (!pExplosion->GetIsActive())
		{
			continue;
		}
		// �v���C���[�Ɣ��e�̃x�N�g�����v�Z
		D3DXVECTOR2 vec = pExplosion->GetPos() - pPlayer->pos;
		// �x�N�g���̒������v�Z
		float len = D3DXVec2Length(&vec);
		// �v���C���[�Ɣ��e�̓����蔻��̔��a�𑫂������l����
		// �x�N�g���̕����Z���Ȃ瓖�����Ă���
		if (len <= pExplosion->GetCollsionRad() + pPlayer->size / 2.0f)
		{
			PlayerDamage(10);
		}
		return;
	}
}
