
/*==============================================================================

   �Q�[����� [game.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "game.h"
#include "player.h"
#include "bg.h"
#include "Block.h"
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
#include "DamageFloorFactory.h"
#include "UI.h"
#include "gameover.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static GAMESCENE g_Scene;
static GAMESCENE g_SceneNext;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGameStage(void);
void UninitGameStage(void);
void UpdateGameStage(void);
void DrawGameStage(void);
void ChangeGameScene(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static EnemyFactory enemyFactory;
static BombFactory bombFactory;
static ExplosionFactory explosionFactory;
static ShockWaveFactory shockWaveFactory;
static FireBallFactory fireBallFactory;
static DamageFloorFactory damageFloorFactory;
static UI ui(GetPlayer());

//=============================================================================
// ����������
//=============================================================================
void InitGame(void)
{
	g_Scene = g_SceneNext = GAMESCENE_STAGE_KASYA;

	InitGameStage();
}

void UninitGame(void)
{
	switch (g_Scene)
	{
	case GAMESCENE_NONE:
		break;
	case GAMESCENE_STAGE_TENGU:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		UninitGameStage();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UninitGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}
}

void UpdateGame(void)
{
	switch (g_Scene)
	{
	case GAMESCENE_NONE:
		break;
	case GAMESCENE_STAGE_TENGU:
		UpdateGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		UpdateGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		UpdateGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		UpdateGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		UpdateGameStage();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UpdateGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}

	//�J�ڐ�V�[�����ݒ肳��Ă�����
	if (g_Scene != g_SceneNext)
	{
		//�V�[���̐؂�ւ����s��
		ChangeGameScene();
	}
}

void DrawGame(void)
{
	switch (g_Scene)
	{
	case GAMESCENE_NONE:
		break;
	case GAMESCENE_STAGE_TENGU:
		DrawGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		DrawGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		DrawGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		DrawGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		DrawGameStage();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		DrawGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}
}

//=============================================================================
// �Q�[���{��
//=============================================================================
void InitGameStage(void)
{
	// �v���C���[�̏�����
	InitPlayer(g_Scene);

	// �w�i�̏�����
	InitBG();

	// �X�e�[�W�̏�����
	InitBlock(g_Scene);

	// �G�l�~�[�H��̏�����
	enemyFactory.Init();

	damageFloorFactory.Init();

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
}

void UninitGameStage(void)
{
	//�J�����̏I������
	UninitCamera();

	//���l�\���̏I������
	UninitNumber();

	// �΂̋ʍH��̏I������
	fireBallFactory.Uninit();

	// �Ռ��g�H��̏I������
	shockWaveFactory.Uninit();

	// �����H��̏I������
	explosionFactory.Uninit();

	// �����H��̏I������
	bombFactory.Uninit();

	damageFloorFactory.Uninit();

	//�G�l�~�[�H��̏I������
	enemyFactory.Uninit();


	// �X�e�[�W�̏I������
	UninitBlock();

	// �w�i�̏I������
	UninitBG();

	// �v���C���[�̏I������
	UninitPlayer();
}

void UpdateGameStage(void)
{
	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �X�e�[�W�̍X�V����
	UpdateBlock();


	// �w�i�̍X�V
	UpdateBG();

	//�J�����̍X�V����
	UpdateCamera();

	//���l�\���̍X�V����
	UpdateNumber();

	// �G�̍X�V����
	enemyFactory.Update();

	damageFloorFactory.Update();

	// ���e�H��̍X�V����
	bombFactory.Update();

	// ���e�H��̍X�V����
	explosionFactory.Update();

	// �Ռ��g�H��̍X�V����
	shockWaveFactory.Update();

	// �΂̋ʍH��̍X�V����
	fireBallFactory.Update();

	//UI�̃A�b�v�f�[�g
	ui.UpdateUI();
}

void DrawGameStage(void)
{
	// �w�i�̕`��
	DrawBG();

	// �X�e�[�W�̕`�揈��
	DrawBlock();

	damageFloorFactory.Draw();

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

	// �J�����̕`�揈��
	DrawCamera();

	//��������UI�̕`��
	//�Q�[���֌W�̕`�悪�S�ďI����Ă���UI�̕`�������
	ui.DrawUI();

	//���l�\���̕`�揈��
	//DrawNumber();
}

void ChangeGameScene(void)
{
	//���ݓ��삵�Ă���V�[�����I��������
	switch (g_Scene)
	{
	case GAMESCENE_STAGE_TENGU:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UninitGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}

	//���݂̃V�[�����X�V����
	g_Scene = g_SceneNext;

	//�X�V���ꂽ�V�[��������������
	switch (g_SceneNext)
	{
	case GAMESCENE_STAGE_TENGU:
		InitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		InitGameStage();
		break;
	case GAMESCENE_STAGE_NUM:
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		InitGameOver();
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}
}

void SetGameScene(GAMESCENE scene)
{
	g_SceneNext = scene;
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
