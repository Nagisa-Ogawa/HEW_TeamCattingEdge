#pragma once

#include "main.h"

class EnemyFactory;
class BombFactory;
class ExplosionFactory;
class ShockWaveFactory;
class FireBallFactory;
class WindBladeFactory;
class FlashFactory;
class RayFactory;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum GAMESCENE {

	GAMESCENE_NONE,
	GAMESCENE_STAGE_TENGU,		//�V��X�e�[�W
	GAMESCENE_BASS_TENGU,		//�V��{�X�X�e�[�W
	GAMESCENE_STAGE_KASYA,		//�ΎԃX�e�[�W
	GAMESCENE_BASS_KASYA,		//�Ύԃ{�X�X�e�[�W
	GAMESCENE_BASS_FUJINRAIJIN,
	GAMESCENE_BASS_FINAL,

	GAMESCENE_STAGE_NUM,
	
	GAMESCENE_PICTURE_STAGE01,		//�X�e�[�W�P�i�V��j�̑}�G
	GAMESCENE_PICTURE_STAGE01_BOSS, //�X�e�[�W�P�i�V��j�̑}�G
	GAMESCENE_PICTURE_STAGE02,		//�X�e�[�W�Q�i�Ύԁj�̑}�G
	GAMESCENE_PICTURE_STAGE02_BOSS, //�X�e�[�W�Q�i�Ύԁj�̑}�G
	GAMESCENE_PICTURE_STAGE03,		//�X�e�[�W�R�i���X�{�X�j�̑}�G
	GAMESCENE_PICTURE_STAGE03_BOSS, //�X�e�[�W�R�i���X�{�X�j�̑}�G
	GAMESCENE_PICTURE_OVERGAME,		//�Q�[���I�[�o�[
	GAMESCENE_PICTURE_RESULT,		//���U���g

	GAMESCENE_NUM,	//�I�[�R�[�h
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

EnemyFactory* GetEnemyFactory();
BombFactory* GetBombFactory();
ExplosionFactory* GetExplosionFactory();
ShockWaveFactory* GetShockWaveFactory();
FireBallFactory* GetFireBallFactory();
WindBladeFactory* GetWindBladeFactory();
FlashFactory* GetFlashFactory();
RayFactory*	GetRayFactory();

void SetGameScene(GAMESCENE scene);