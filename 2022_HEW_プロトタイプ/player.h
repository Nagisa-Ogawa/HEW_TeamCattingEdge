#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

enum STATUS
{
	normal,
	warpwait,
	warp,
	fall,
	STATUS_NUM,
};

enum MAPS
{
	tutorial,
	bass,
	NUM_MAPS,
};

struct PLAYER
{
	D3DXVECTOR2 pos;		//���ݍ��W
	D3DXVECTOR2 oldpos;		//�P�t���[���O�̍��W
	D3DXVECTOR2 warppos;	//���[�v�n�_�̍��W
	D3DXVECTOR2	vel;		//���x�x�N�g��
	STATUS		status;		//�v���C���[�̏��

	int			warpframe;  //���[�v���̓��͎���
	int         waitafterwarp;
	float		warppower;  //���[�v��
	float		gravity;    //�d��
	int			warpFlag;	//���[�v�񐔃t���O

	int			muki;
	int			animePtn;
	int			animeCounter;

	bool		move;

	int attackflag;

	int			hp;			//HP
	MAPS		map;		//����MAP
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);
