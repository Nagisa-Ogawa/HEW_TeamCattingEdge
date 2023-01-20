#pragma once

#include <list>
#include "Flash.h"

class FlashFactory
{
private:
	std::list<Flash*> m_pFlashList;
	int m_TextureNo = -1;
public:
	FlashFactory();
	// �G���쐬
	void Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	void Init();
	// ���e�H��̏I������
	void Uninit();
	// ���e�H��̍X�V����
	void Update();
	// ���e�H��̕`�揈��
	void Draw();
	~FlashFactory();

};

