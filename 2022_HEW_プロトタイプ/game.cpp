
/*==============================================================================

   �Q�[����� [game.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "game.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "polygon.h"
#include "player.h"
#include "goal.h"
#include "bg.h"
#include "Block.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "number.h"
#include "camera.h"
#include "inputx.h"
#include "EnemyFactory.h"
#include "BombFactory.h"
#include "ExplosionFactory.h"
#include "ShockWaveFactory.h"
#include "FireBallFactory.h"
#include "UI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int g_BGMNo;
static int g_TextureCloud = 0;

static EnemyFactory enemyFactory;
static BombFactory bombFactory;
static ExplosionFactory explosionFactory;
static ShockWaveFactory shockWaveFactory;
static FireBallFactory fireBallFactory;
static UI ui(GetPlayer());

//=============================================================================
// ����������
//=============================================================================
void InitGame(void)
{
	// �v���C���[�̏�����
	InitPlayer();

	// �S�[���̏�����
	InitGoal();

	// �w�i�̏�����
	InitBG();

	// �X�e�[�W�̏�����
	InitBlock();

	// �e�̏�����
	InitBullet();

	// �G�l�~�[�H��̏�����
	enemyFactory.Init();

	// ���e�H��̏�����
	bombFactory.Init();

	// �����H��̏�����
	explosionFactory.Init();

	// �Ռ��g�H��̏�����
	shockWaveFactory.Init();

	// �΂̋ʍH��̏�����
	fireBallFactory.Init();

	//���l�\���̏�����
	InitNumber();

	//�J�����̏�����
	InitCamera();

	//�Q�[���pBGM�̓ǂݍ���
	g_BGMNo = LoadSound((char*)"data/BGM/sample001.wav");

	//BGM�Đ�(��ڂ̈����̓��[�v�� ���̒l���w�肷��Ɩ������[�v)
	//PlaySound(g_BGMNo, -1);
}

void UninitGame(void)
{
	//�J�����̏I������
	UninitCamera();

	//���l�\���̏I������
	UninitNumber();

	// �Ռ��g�H��̏I������
	shockWaveFactory.Uninit();

	// �����H��̏I������
	explosionFactory.Uninit();

	// �����H��̏I������
	bombFactory.Uninit();

	//�G�l�~�[�H��̏I������
	enemyFactory.Uninit();

	// �΂̋ʍH��̏I������
	fireBallFactory.Uninit();

	// �e�̏I������
	UninitBullet();

	// �X�e�[�W�̏I������
	UninitBlock();

	// �w�i�̏I������
	UninitBG();

	// �S�[���̏I������
	UninitGoal();

	// �v���C���[�̏I������
	UninitPlayer();
}

void UpdateGame(void)
{

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�̍X�V����
	enemyFactory.Update();

	// ���e�H��̍X�V����
	bombFactory.Update();

	// ���e�H��̍X�V����
	explosionFactory.Update();

	// �Ռ��g�H��̍X�V����
	shockWaveFactory.Update();

	// �΂̋ʍH��̍X�V����
	fireBallFactory.Update();

	// �X�e�[�W�̍X�V����
	UpdateBlock();

	// �w�i�̍X�V
	UpdateBG();

	//�J�����̍X�V����
	UpdateCamera();

	//���l�\���̍X�V����
	UpdateNumber();

	ui.UpdateUI();

}

void DrawGame(void)
{
	// �w�i�̕`��
	DrawBG();

	// �X�e�[�W�̕`�揈��
	DrawBlock();

	// �Ռ��g�H��̕`�揈��
	shockWaveFactory.Draw();

	// ���e�H��̕`�揈��
	bombFactory.Draw();

	// �����H��̕`�揈��
	explosionFactory.Draw();

	// �G�̕`�揈��
	enemyFactory.Draw();

	// �΂̋ʍH��̕`�揈��
	fireBallFactory.Draw();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �S�[���̕`�揈��
	DrawGoal();

	// �J�����̕`�揈��
	DrawCamera();

	// �e�̕`�揈��
	DrawBullet();

	//��������UI�̕`��
	//�Q�[���֌W�̕`�悪�S�ďI����Ă���UI�̕`�������

	//���l�\���̕`�揈��
	//DrawNumber();
}

EnemyFactory* GetEnemyFactory()
{
	return &enemyFactory;
}

BombFactory* GetBombFactory()
{
	return &bombFactory;
}

ExplosionFactory* GetExplosionFactory()
{
	return &explosionFactory;
}

ShockWaveFactory * GetShockWaveFactory()
{
	return  &shockWaveFactory;
}

FireBallFactory * GetFireBallFactory()
{
	return &fireBallFactory;
}

