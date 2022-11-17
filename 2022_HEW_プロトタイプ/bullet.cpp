/*==============================================================================

   �v���C���[�e�̏��� [bullet.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "bullet.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "camera.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_SPEED (4.0f)//�e�̑��x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_TextureNo;	// �e�N�X�`�����ʎq
static BULLET g_Bullet[MAX_BULLET];//�e�o�b�t�@

static float g_RotateTable[4] =
{
	D3DX_PI,		//������(180����]������)
	(-D3DX_PI / 2),	//������(-90����]������)
	(+D3DX_PI / 2),	//�E����(+90����]������)
	0.0f,			//�����
};

//=============================================================================
// ����������
//=============================================================================
void InitBullet(void)
{
	//�^�C�g����ʗp�e�N�X�`���̓ǂݍ���
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/btleffect004.png");

	for (int i = 0; i < MAX_BULLET; i++)
	{
		g_Bullet[i].use = false;
	}
}

void UninitBullet(void)
{
	
}

void UpdateBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//���t���O���I���ł���΍X�V����
		if (g_Bullet[i].use == true)
		{
			//�e�̍��W�X�V
			g_Bullet[i].pos += g_Bullet[i].vel;

			//�L���t���[�������炷
			g_Bullet[i].frame--;

			//�L���t���[����0�ȉ��ɂȂ�������t���O���I�t�ɂ���
			if (g_Bullet[i].frame <= 0)
			{
				g_Bullet[i].use = false;
			}
		}
	}
}

void DrawBullet(void)
{
	//�x�[�X���W���󂯎��
	D3DXVECTOR2 basePos = GetBase();

	for (int i = 0; i < MAX_BULLET; i++)
	{
		//���t���O���I���ł���Ε`�悷��
		if (g_Bullet[i].use == true)
		{
//			DrawSprite(g_TextureNo, g_Bullet[i].pos.x, g_Bullet[i].pos.y, 64.0f, 64.0f,
//				0.0f, 0.0f, 1.0f, 1.0f);
			DrawSpriteColorRotate(g_TextureNo, basePos.x + g_Bullet[i].pos.x, basePos.y + g_Bullet[i].pos.y, 32.0f, 32.0f,
				0.4f, 0.0f, 0.2f, 1.0f, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), g_RotateTable[g_Bullet[i].muki]);

		}
	}
}

void SetBullet(D3DXVECTOR2 pos, int muki)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//�e�o�b�t�@�̒�����\������Ă��Ȃ����̂�T��
		if (g_Bullet[i].use == false)
		{
			g_Bullet[i].pos = pos;
			g_Bullet[i].muki = muki;

			//�����ɂ���Ĉړ����������߂�
			switch (g_Bullet[i].muki)
			{
			case 0://������
				g_Bullet[i].vel = D3DXVECTOR2(0.0f, BULLET_SPEED);
				break;
			case 1://������
				g_Bullet[i].vel = D3DXVECTOR2(-BULLET_SPEED, 0.0f);
				break;
			case 2://�E����
				g_Bullet[i].vel = D3DXVECTOR2(BULLET_SPEED, 0.0f);
				break;
			case 3://�����
				g_Bullet[i].vel = D3DXVECTOR2(0.0f, -BULLET_SPEED);
				break;
			}

			g_Bullet[i].height = 0.0f; //��őΉ�����
	
			//�L���t���[�����Z�b�g
			g_Bullet[i].frame = 200;

			//���t���O�I��
			g_Bullet[i].use = true;

			//�e����Z�b�g�����烋�[�v�I��
			break;
		}
	}
}

BULLET* GetBullet(void)
{
	return (&g_Bullet[0]);
}
