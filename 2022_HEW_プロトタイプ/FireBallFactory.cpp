#include <algorithm>
#include "FireBallFactory.h"
#include "FireBall.h"
#include "Block.h"
#include "texture.h"

FireBallFactory::FireBallFactory()
{
	
}

void FireBallFactory::Create(D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power,int mode)
{
	m_nowID++;
	m_pFireBallList.push_back(new FireBall(m_nowID, pos, muki, power, (FireBall::MODE)mode, m_FireBallTextureNo));
}

void FireBallFactory::Init()
{
	m_FireBallTextureNo = LoadTexture((char*)"data/TEXTURE/FireBall.png");

	m_pPlayer = GetPlayer();
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Init();
	}
}

void FireBallFactory::Uninit()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Uninit();
	}
}

void FireBallFactory::Update()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Update();
	}
	CollisionPlayerToFireBall();
	CollisionBlockToFireBall();
	// �g���I������΂̋ʂ��Ȃ����`�F�b�N
	DeleteFireBall();
}

void FireBallFactory::Draw()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Draw();
	}
}

FireBallFactory::~FireBallFactory()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		delete fireBall;
	}

	m_pFireBallList.clear();
}

void FireBallFactory::DeleteFireBall()
{
	while (true)
	{
		auto deleteIt = std::find_if(m_pFireBallList.begin(), m_pFireBallList.end(), ([&](FireBall* fireBall) {return (fireBall->GetIsActive() == false); }));
		// �\�����Ă��Ȃ����e������Ȃ��Ȃ甲����
		if (deleteIt == m_pFireBallList.end())
		{
			break;
		}
		// �����ς݂̔��e�͍폜
		delete (*deleteIt);
		m_pFireBallList.erase(deleteIt);
	}
}

void FireBallFactory::CollisionPlayerToFireBall()
{
	// �v���C���[�����G�Ȃ�X�L�b�v
	if (m_pPlayer->mutekiflag)
	{
		return;
	}
	for (FireBall* fireBall : m_pFireBallList)
	{
		// ���S���Ă����Ȃ�X�L�b�v
		if (!fireBall->GetIsActive())
		{
			continue;
		}
		// �v���C���[�Ɠ��������Ȃ�
		if (HitCheckBox(fireBall->GetPos(), fireBall->GetSize(),
			m_pPlayer->pos, D3DXVECTOR2(m_pPlayer->size, m_pPlayer->size)))
		{
			// �v���C���[��HP�����炷
			PlayerDamage(1);
			fireBall->Hit();
		}
	}
	return;
}

void FireBallFactory::CollisionBlockToFireBall()
{
	auto blocks = GetBlocks();
	for (FireBall* fireBall : m_pFireBallList)
	{
		bool isHit = false;
		// ���S���Ă����Ȃ�X�L�b�v
		if (!fireBall->GetIsActive())
		{
			continue;
		}
		if (fireBall->GetState() == FireBall::FIRE::BURNSOUT) {
			continue;
		}
		DWORD result = 0;
		result = HitCheckFireBall_Block(fireBall->GetPos(), fireBall->GetSize());
		if (result != 0) {
			D3DXVECTOR2 vec = fireBall->GetPower();
			if (fireBall->GetHittingFlag() == false) {
				fireBall->SetHittingFlag(true);
			}
			switch (fireBall->GetMode())
			{
			case FireBall::MODE::GHOSTFIRE:
				fireBall->Hit();
				break;
			case FireBall::MODE::KASYA_ONESHOT:
				if (fireBall->GetIsReflectFlag() == true) {
					continue;
				}
				// ���˂���ǂȂ�
				if (fireBall->GetPassThroughFlag() == false) {
					// ���ˏ���
					//if (result == HIT_LEFT)
					//{
					//	vec.x *= -1.0f;
					//	fireBall->SetThrowPower(vec);
					//}
					//if (result == HIT_RIGHT)
					//{
					//	vec.x *= -1.0f;
					//	fireBall->SetThrowPower(vec);
					//}
					if (result & HIT_DOWN)
					{
						vec.y *= -1.0f;
						fireBall->SetThrowPower(vec);
					}
					if (result & HIT_UP)
					{
						vec.y *= -1.0f;
						fireBall->SetThrowPower(vec);
					}
					fireBall->RotToVecotr(fireBall->GetPower());
					// �ǂŔ��˂������ǂ����̃t���O���X�V
					fireBall->SetReflect(true);
				}
				break;
			case FireBall::MODE::KASYA_THREESHOT:
				if (fireBall->GetIsReflectFlag() == true) {
					continue;
				}
				// �K��񐔔��˂��Ă�����R���s����
				if (fireBall->GetNowReflectAllCount() >= fireBall->GetBurnsOutCount()) {
					fireBall->Hit();
				}
				// ���ˏ���
				//if (result == HIT_LEFT)
				//{
				//	vec.x *= -1.0f;
				//	fireBall->SetThrowPower(vec);
				//}
				//if (result == HIT_RIGHT)
				//{
				//	vec.x *= -1.0f;
				//	fireBall->SetThrowPower(vec);
				//}
				if (result & HIT_DOWN)
				{
					vec.y *= -1.0f;
					fireBall->SetThrowPower(vec);
				}
				if (result & HIT_UP)
				{
					vec.y *= -1.0f;
					fireBall->SetThrowPower(vec);
				}
				fireBall->RotToVecotr(fireBall->GetPower());
				// �ǂŔ��˂������ǂ����̃t���O���X�V
				fireBall->SetReflect(true);
				break;
			default:
				break;
			}
			isHit = true;
		}
		if (isHit == false && fireBall->GetMode() == FireBall::MODE::KASYA_ONESHOT) {
			// �ǂ��甲������
			if (fireBall->GetHittingFlag() == true) {
				fireBall->SetHittingFlag(false);
				// ���̕ǂ����蔲���Ă����Ȃ�
				if (fireBall->GetIsReflectFlag() == false) {
					fireBall->SetPassThroughFlag(false);
				}
				else {
					// ���ˉ񐔂��X�V
					fireBall->SetNowReflectCount(fireBall->GetNowReflectCount() + 1);
					fireBall->SetNowReflectAllCount(fireBall->GetNowReflectAllCount() + 1);
					// �K��񐔔��˂��Ă�����R���s����
					if (fireBall->GetNowReflectAllCount() >= fireBall->GetBurnsOutCount()) {
						fireBall->Hit();
					}
					// �K��񐔔��˂������`�F�b�N
					if (fireBall->GetNowReflectCount() >= fireBall->GetReflectWallCounts()) {
						// �K��񐔔��˂����玟�̕ǂ𔽎˂����ɒʂ�߂���
						fireBall->SetNowReflectCount(0);
						fireBall->SetPassThroughFlag(true);
						fireBall->SetNowReflectWallCount(fireBall->GetNowReflectWallCount() + 1);
					}
					fireBall->SetReflect(false);
				}
			}
		}
		if (isHit == false && fireBall->GetMode() == FireBall::MODE::KASYA_THREESHOT) {
			// �ǂ��甲������
			if (fireBall->GetHittingFlag() == true) {
				fireBall->SetHittingFlag(false);
				// ���̕ǂ����蔲���Ă����Ȃ�
				if (fireBall->GetIsReflectFlag() == true) {
					// ���ˉ񐔂��X�V
					fireBall->SetNowReflectAllCount(fireBall->GetNowReflectAllCount() + 1);
					fireBall->SetReflect(false);
				}
			}
		}
	}
	return;
}

bool FireBallFactory::HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
	D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize)
{
	float box1Xmin = enemyPos.x - (enemySize.x / 2);
	float box1Xmax = enemyPos.x + (enemySize.x / 2);
	float box1Ymin = enemyPos.y - (enemySize.y / 2);
	float box1Ymax = enemyPos.y + (enemySize.y / 2);

	float box2Xmin = playerPos.x - (playerSize.x / 2);
	float box2Xmax = playerPos.x + (playerSize.x / 2);
	float box2Ymin = playerPos.y - (playerSize.y / 2);
	float box2Ymax = playerPos.y + (playerSize.y / 2);

	if (box1Xmin < box2Xmax)
	{
		if (box1Xmax > box2Xmin)
		{
			if (box1Ymin < box2Ymax)
			{
				if (box1Ymax > box2Ymin)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool FireBallFactory::HitCheckBox_Block(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize, D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize)
{

	float box1Xmin = blockPos.x;
	float box1Xmax = blockPos.x + blockSize.x;
	float box1Ymin = blockPos.y ;
	float box1Ymax = blockPos.y + blockSize.y;

	float box2Xmin = playerPos.x - (playerSize.x / 2);
	float box2Xmax = playerPos.x + (playerSize.x / 2);
	float box2Ymin = playerPos.y - (playerSize.y / 2);
	float box2Ymax = playerPos.y + (playerSize.y / 2);

	if (box1Xmin < box2Xmax)
	{
		if (box1Xmax > box2Xmin)
		{
			if (box1Ymin < box2Ymax)
			{
				if (box1Ymax > box2Ymin)
				{
					return true;
				}
			}
		}
	}

	return false;
}

