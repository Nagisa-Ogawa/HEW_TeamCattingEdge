#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY 100		//�G�̕\���ő吔

enum ENEMYSTATE
{
	chase = 0,
	wait,
	Return,
	NUN_STATE
};

struct ENEMY
{
	D3DXVECTOR2 pos;
//	D3DXVECTOR2	vel;	//���x�x�N�g��
	D3DXVECTOR2 dir;	//�ړ�����
	float		speed;	//�ړ��X�s�[�h

	D3DXVECTOR2 start_pos;

	float		height;	//����
	int			muki;	//����

	ENEMYSTATE		state;  //��ԊǗ�

	int			frame;	//�L���t���[��
	int			use;	//���t���O
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(void);

ENEMY* GetEnemy(void);
