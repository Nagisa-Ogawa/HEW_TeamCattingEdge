
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

//=============================================================================
// ����������
//=============================================================================
void InitGame(void)
{
	// ���_�Ǘ��̏���������
//	InitPolygon();

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

	//���l�\���̏�����
	InitNumber();

	//�J�����̏�����
	InitCamera();

	//�G�̔�������
	//InitEnemyEmitter();

	////�G�̏����z�u
	//for (int i = 0; i < 10; i++)
	//{
	//	D3DXVECTOR2 pos;
	//	pos.x = rand() % SCREEN_WIDTH;
	//	pos.y = rand() % SCREEN_HEIGHT;
	//	SetEnemy(pos, 0);
	//}

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

	//�G�̏I������
	//UninitEnemy();

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

	// ���_�Ǘ��̏I������
//	UninitPolygon();

	//�e�N�X�`���̉��
//	UninitTexture();
}

void UpdateGame(void)
{
	// ���_�Ǘ��̍X�V����
//	UpdatePolygon();

	//�G�̔�������
	//UpdateEnemyEmitter();


	// �G�̍X�V����
	//UpdateEnemy();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �e�̍X�V����
	UpdateBullet();

	// �S�[���̍X�V����
	UpdateGoal();

	// �X�e�[�W�̍X�V����
	UpdateBlock();

	// �w�i�̍X�V
	UpdateBG();

	//�J�����̍X�V����
	UpdateCamera();

	UpdateEndPlayer();

	//���l�\���̍X�V����
	UpdateNumber();

	//�S�ẴA�b�v�f�[�g���I������瓖���蔻����s��
	Collision_Bullet_Enemy();
}

void DrawGame(void)
{
	// �w�i�̕`��
	DrawBG();

	// ���_�Ǘ��̕`�揈��
//	DrawPolygon();

	//// �e�̕`�揈��
	//DrawBullet();

	// �G�̕`�揈��
	//DrawEnemy();

	// �X�e�[�W�̕`�揈��
	DrawBlock();

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
