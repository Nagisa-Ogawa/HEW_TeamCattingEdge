#pragma once

#include "Bomb.h"


class ContactBomb_Boss :public Bomb
{
public:
	enum STATE_CONTACTBOMB_BOSS
	{
		THROW,
		EXPLOSION,
	};
private:

	int m_TextureNo;		// ���e���g�p����e�N�X�`���̔ԍ�

	STATE_CONTACTBOMB_BOSS m_State = STATE_CONTACTBOMB_BOSS::THROW;
public:
	ContactBomb_Boss(int ID, D3DXVECTOR2 pos, D3DXVECTOR2 endPos);
	// ���e�̏���������
	void Init() override;
	// ���e�̏I������
	void Uninit() override;
	// ���e�̍X�V����
	void Update() override;
	// ���e�̕`�揈��
	void Draw() override;
	~ContactBomb_Boss();

};

