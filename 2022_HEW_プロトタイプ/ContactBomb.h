#pragma once

#include "Bomb.h"


class ContactBomb:public Bomb
{
public:
	enum STATE_CONTACTBOMB
	{
		THROW,
		EXPLOSION,
	};
private:
	
	int m_TextureNo;		// ���e���g�p����e�N�X�`���̔ԍ�

	STATE_CONTACTBOMB m_State = STATE_CONTACTBOMB::THROW;
public:
	ContactBomb(int ID,D3DXVECTOR2 pos, D3DXVECTOR2 endPos, D3DXVECTOR2 startVec, D3DXVECTOR2 endVec);
	// ���e�̏���������
	void Init() override;
	// ���e�̏I������
	void Uninit() override;
	// ���e�̍X�V����
	void Update() override;
	// ���e�̕`�揈��
	void Draw() override;
	~ContactBomb();

};

