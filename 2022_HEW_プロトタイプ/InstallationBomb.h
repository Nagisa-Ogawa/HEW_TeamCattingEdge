#pragma once
#include "Bomb.h"

class ShockWaveFactory;

class InstallationBomb : public Bomb
{
public:
	enum STATE_INSTALLATIONBOMB
	{
		THROW,
		SET,
		EXPLOSION,
	};
private:

	ShockWaveFactory* m_pShockWaveFactory;

	int m_TextureNo = 0;		// ���e���g�p����e�N�X�`���̔ԍ�
	bool m_IsGround = false;
	float m_Gravity = 0.0f;
	D3DXVECTOR2 m_Vel = D3DXVECTOR2(0.0f, 0.0f);

	STATE_INSTALLATIONBOMB m_State = STATE_INSTALLATIONBOMB::THROW;
public:
	InstallationBomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	// ���e�̏���������
	void Init() override;
	// ���e�̏I������
	void Uninit() override;
	// ���e�̍X�V����
	void Update() override;
	// ���e�̕`�揈��
	void Draw() override;
	~InstallationBomb();

	// Get�n�֐�
	STATE_INSTALLATIONBOMB GetState() { return m_State; };

	// �Ռ��g�Ƃ̓����蔻��p�֐�
	void CollisionBombToShockWave();
	// �l�p�̓����蔻��
	bool HitCheckSquare(D3DXVECTOR2 posA, D3DXVECTOR2 sizeA, D3DXVECTOR2 posB, D3DXVECTOR2 sizeB);

};

