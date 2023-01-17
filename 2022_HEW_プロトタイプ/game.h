#pragma once

#include "main.h"

class EnemyFactory;
class BombFactory;
class ExplosionFactory;
class ShockWaveFactory;
class FireBallFactory;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum GAMESCENE {
	GAMESCENE_NONE,				//
	GAMESCENE_STAGE_TENGU,		//�V��X�e�[�W
	GAMESCENE_BASS_TENGU,		//�V��{�X�X�e�[�W
	GAMESCENE_STAGE_KASYA,		//�ΎԃX�e�[�W
	GAMESCENE_BASS_KASYA,		//�Ύԃ{�X�X�e�[�W
	GAMESCENE_STAGE_NUM,		//�ΎԃX�e�[�W

	GAMESCENE_PICTURE_OVERGAME, //�Q�[���I�[�o�[
	GAMESCENE_PICTURE_RESULT,	//���U���g

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

void SetGameScene(GAMESCENE scene);