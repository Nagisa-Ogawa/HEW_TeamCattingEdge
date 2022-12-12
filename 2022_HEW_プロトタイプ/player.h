#pragma once

#include "main.h"
#include "renderer.h"

class EnemyFactory;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

enum STATUS
{
	normal,
	warpwait,
	warp,
	
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
	//��b���
	D3DXVECTOR2 pos;		//���ݍ��W
	D3DXVECTOR2 oldpos;		//�P�t���[���O�̍��W
	D3DXVECTOR2 warppos;	//���[�v�n�_�̍��W
	float		size;		//�T�C�Y
	float       hitsize;    //�q�b�g�T�C�Y
	D3DXVECTOR2	vel;		//���x�x�N�g��
	STATUS		status;		//�v���C���[�̏��

	//���[�v�֘A
	int			warpframe;		//���[�v���̓��͎���
	DWORD		warpRecast;		//���[�v�̃��L���X�g����
	DWORD		warpStartRecast;//���[�v�̃��L���X�g�̏��߂̎���
	int         waitafterwarp;	//���[�v��̒�~����
	float		warppower;		//���[�v��
	float		gravity;		//�d��
	int			warpFlag;		//���[�v�񐔃t���O

	//���������֘A
	D3DXVECTOR2	dorpspeed;		//�������x
	bool        LandingFlag;	//���n�t���O

	//�A�j���[�V�����֘A
	int			muki;
	int			animePtn;
	int			animeCounter;
	bool		move;

	//�퓬�֘A
	bool		mutekiflag;		//���G�t���O
	int			mutekitime;		//���G����
	int			attackflag;		//�A�^�b�N�t���O
	int			hp;				//HP

	//���̑�
	EnemyFactory* enemyfactory = nullptr;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void AdjustPlayer(D3DXVECTOR2 pos);

//�_���[�W�֐�
void PlayerDamage(int num);

PLAYER* GetPlayer(void);
