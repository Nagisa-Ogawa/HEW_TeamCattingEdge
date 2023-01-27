
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
#include "WindBladeFactory.h"
#include "FlashFactory.h"
#include "ThunderBladeFactory.h"
#include "RayFactory.h"
#include "SwitchBulletFactory.h"
#include "UI.h"
#include "fade.h"
#include "Game_illustration.h"

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
static WindBladeFactory windBladeFactory;
static FlashFactory flashFactory;
static RayFactory rayFactory;
static ThunderBladeFactory thunderBladeFactory;
static SwitchBulletFactory switchBulletFactory;
static UI ui(GetPlayer());

//=============================================================================
// ����������
//=============================================================================
void InitGame(void)
{
	g_Scene = g_SceneNext = GAMESCENE_STAGE_TENGU;

	switch (g_Scene)
	{
	case GAMESCENE_STAGE_TENGU:
		InitGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		InitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		InitGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		InitGameStage();
		break;
	case GAMESCENE_BASS_FUJIN:
		InitGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		InitGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		InitGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE02:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_RESULT:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	default:
		break;
	}
}

void UninitGame(void)
{
	switch (g_Scene)
	{
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
	case GAMESCENE_BASS_FUJIN:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		UninitGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_RESULT:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		UninitGameIllust();
		break;
	default:
		break;
	}
}

void UpdateGame(void)
{
	switch (g_Scene)
	{
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
	case GAMESCENE_BASS_FUJIN:
		UpdateGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		UpdateGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		UpdateGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_RESULT:
		UpdateGameIllust();
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		UpdateGameIllust();
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
	case GAMESCENE_BASS_FUJIN:
		DrawGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		DrawGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		DrawGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_RESULT:
		DrawGameIllust();
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		DrawGameIllust();
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

	windBladeFactory.Init();

	flashFactory.Init();

	thunderBladeFactory.Init();

	switchBulletFactory.Init();

	rayFactory.Init();

	//UI
	ui.InitUI();

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

	rayFactory.Uninit();

	switchBulletFactory.Init();

	thunderBladeFactory.Uninit();

	flashFactory.Uninit();

	windBladeFactory.Uninit();

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

	windBladeFactory.Update();

	flashFactory.Update();

	thunderBladeFactory.Update();

	switchBulletFactory.Update();

	rayFactory.Update();

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

	windBladeFactory.Draw();

	flashFactory.Draw();

	thunderBladeFactory.Draw();

	switchBulletFactory.Draw();

	rayFactory.Draw();

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
	case GAMESCENE_BASS_TENGU:
		UninitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_FUJIN:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		UninitGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		UninitGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_RESULT:
		UninitGameIllust();
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		UninitGameIllust();
		break;
	default:
		break;
	}

	switch (g_SceneNext)
	{
	case GAMESCENE_STAGE_TENGU:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_TENGU:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_STAGE_KASYA:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_KASYA:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_FUJIN:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_RAIJIN:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_BASS_FINAL:
		g_Scene = g_SceneNext;
		InitGameStage();
		break;
	case GAMESCENE_PICTURE_STAGE01:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE01_BOSS:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE02:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE02_BOSS:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_FUJIN:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_RAIJIN:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAGE03_FINAL:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_RESULT:
		InitGameIllust(g_SceneNext, g_Scene);
		break;
	case GAMESCENE_PICTURE_STAFFROLL:
		InitGameIllust(g_SceneNext, g_Scene);
	default:
		break;
	}

	//���݂̃V�[�����X�V����
	g_Scene = g_SceneNext;
}

void SetGameScene(GAMESCENE scene)
{
	g_SceneNext = scene;
}

GAMESCENE * GetNowScene()
{
	return &g_Scene;
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

WindBladeFactory * GetWindBladeFactory()
{
	return &windBladeFactory;
}

FlashFactory * GetFlashFactory()
{
	return &flashFactory;
}

RayFactory * GetRayFactory()
{
	return &rayFactory;
}

ThunderBladeFactory * GetThunderBladeFactory()
{
	return &thunderBladeFactory;
}

SwitchBulletFactory * GetSwitchBulletFactory()
{
	return &switchBulletFactory;
}
