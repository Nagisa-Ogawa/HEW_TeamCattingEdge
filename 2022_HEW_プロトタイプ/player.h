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
	float		size;		//�T�C�Y
	float       hitsize;    //�q�b�g�T�C�Y
	D3DXVECTOR2	vel;		//���x�x�N�g��
	STATUS		status;		//�v���C���[�̏��

	//���[�v�֘A
	int			warpframe;  //���[�v���̓��͎���
	int         waitafterwarp;//���[�v��̒�~����
	float		warppower;  //���[�v��
	float		gravity;    //�d��
	D3DXVECTOR2		dorpspeed;  //�������x
	int			warpFlag;	//���[�v�񐔃t���O

	bool        LandingFlag; //���n�t���O

	int			muki;
	int			animePtn;
	int			animeCounter;

	bool		move;

	int			attackflag;
	int			hp;			//HP
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void AdjustPlayer(D3DXVECTOR2 pos);

PLAYER* GetPlayer(void);
