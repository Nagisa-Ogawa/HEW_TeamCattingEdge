#pragma once

#include "main.h"
#include "game.h"

class EnemyFactory;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

enum STATUS
{
	normal,
	warpwait,
	warp,
	death,
	
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
	int			muki;	// 0:��~ 1:�E���� 2:�E���� 3:������ 4:������
	int			animeWalk;
	int			animeRun;
	int			animeAttack;
	int			animeDeath;
	int			animeCounterWalk;
	int			animeCounterRun;
	int			animeCounterAttack;
	int			animeCounterDeath;
	bool		move;

	//�퓬�֘A
	bool		mutekiflag;		//���G�t���O
	int			mutekitime;		//���G����
	int			attackflag;		//�A�^�b�N�t���O
	int			attackRecast;	//�A�^�b�N���L���X�g
	int			hp;				//HP

	//���̑�
	GAMESCENE	gamescene;
	EnemyFactory* enemyfactory = nullptr;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(GAMESCENE gamescene);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void AdjustPlayer(D3DXVECTOR2 pos);

//�_���[�W�֐�
void PlayerDamage(int num);

PLAYER* GetPlayer(void);
D3DXVECTOR2 GetPlayerPos(void);
