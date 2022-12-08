
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
#include "enemyemitter.h"
#include "inputx.h"
#include "EnemyFactory.h"

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
BombFactory bombFactory;

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

	// �G�̏�����
	//InitEnemy();
	enemyFactory.Init();

	bombFactory.Init();

	//���l�\���̏�����
	InitNumber();

	//�J�����̏�����
	InitCamera();

	//�G�̔�������
	//InitEnemyEmitter();

	//�Q�[���pBGM�̓ǂݍ���
	g_BGMNo = LoadSound((char*)"data/BGM/sample001.wav");

	//BGM�Đ�(��ڂ̈����̓��[�v�� ���̒l���w�肷��Ɩ������[�v)
	//PlaySound(g_BGMNo, -1);
}

void UninitGame(void)
{
	//UninitEnemyEmitter();

	//�J�����̏I������
	UninitCamera();

	//���l�\���̏I������
	UninitNumber();

	bombFactory.Uninit();
	//�G�̏I������
	//UninitEnemy();
	enemyFactory.Uninit();

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
	//�G�̔�������
	//UpdateEnemyEmitter();

	// �v���C���[�̍X�V����
	UpdatePlayer();



	// �G�̍X�V����
	enemyFactory.Update();

	bombFactory.Update();

	// �X�e�[�W�̍X�V����
	UpdateBlock();

	// �w�i�̍X�V
	UpdateBG();

	//�J�����̍X�V����
	UpdateCamera();

	//���l�\���̍X�V����
	UpdateNumber();

	//�S�ẴA�b�v�f�[�g���I������瓖���蔻����s��
	// Collision_Bullet_Enemy();
}

void DrawGame(void)
{
	// �w�i�̕`��
	DrawBG();

	// �X�e�[�W�̕`�揈��
	DrawBlock();

	// �G�̕`�揈��
	enemyFactory.Draw();

	bombFactory.Draw();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �S�[���̕`�揈��
	DrawGoal();

	// �J�����̕`�揈��
	DrawCamera();

	// �e�̕`�揈��
	DrawBullet();

	//DrawEnemyEmitter();

	//��������UI�̕`��
	//�Q�[���֌W�̕`�悪�S�ďI����Ă���UI�̕`�������

	//���l�\���̕`�揈��
	//DrawNumber();
}

BombFactory* GetBombFactory()
{
	return &bombFactory;
}
