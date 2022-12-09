/*==============================================================================

   ���_�Ǘ� [player.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "fade.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "bullet.h"
#include "number.h"
#include "camera.h"
#include "inputx.h"
#include "Block.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX 4	//�K�v�Ȓ��_�̐�

#define DIVIDE_X 3	//���̕�����
#define DIVIDE_Y 4	//�c�̕�����

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)

#define PLAYER_DISP_X (SCREEN_WIDTH/2)	//�v���C���[�̕\�����WX
#define PLAYER_DISP_Y (SCREEN_HEIGHT/2)	//�v���C���[�̕\�����WY

#define PLAYER_SPEED	4.0f //�v���[���[�̃X�s�[�h

#define TEST_CON 0

#define MONOSIZE 64.0f

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void PlayerStatusNormal(void);
void PlayerStatusWarpwait(void);
void PlayerStatusWarp(void);

//�X�e�B�b�N���擾�֐�
D3DXVECTOR2 GetLeftStick(int padNo);
D3DXVECTOR2 GetRightStick(int padNo);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER g_Player;

static int g_TextureNo = 0;//�v���C���[�p�e�N�X�`���̎��ʎq
static int g_TextureAttack;

static float g_AnimeTable[4] =
{
	0.0f,
	0.33333f,
	0.66667f,
	0.33333f,
};

static float g_MukiTable[4] =
{
	0.0f,	//������
	0.25f,	//������
	0.5f,	//�E����
	0.75f,	//�����
};
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/pipo-xmaschara03.png");

	//�f�[�^�̏�����

	//��b���̏�����
	g_Player.pos = D3DXVECTOR2(200.0f, 800.0f);
	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.oldpos = g_Player.pos;
	g_Player.size = 120.0f;
	g_Player.hitsize = g_Player.size - 2.0f;
	g_Player.status = normal;

	//���[�v�֘A���̏�����
	g_Player.warpframe = 0;
	g_Player.warpRecast = 0;
	g_Player.warpStartRecast = 0;
	g_Player.waitafterwarp = 0;
	g_Player.warppower = 420.0f;
	g_Player.gravity = 0.6f;
	g_Player.warpFlag = 3;//int

	//���������֘A�̏�����
	g_Player.dorpspeed = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.LandingFlag = false; //���n�t���O

	//�A�j���[�V�����֘A�̏�����
	g_Player.muki = 0;
	g_Player.animePtn = 0;
	g_Player.animeCounter = 0;
	g_Player.move = false;

	//�퓬�֘A���̏�����
	g_Player.mutekiflag = true;
	g_Player.mutekitime = 0;
	g_Player.attackflag = 0;
	g_Player.hp = 10;

	SetNumber(32768); //�X�R�A�̕`��

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	//�P�t���[���O�̃|�W�V�������X�V
	g_Player.oldpos = g_Player.pos;

	DWORD result = 0;
	//�v���[���[�̏�ԕω��i�X�e�[�^�X�j
	switch (g_Player.status)
	{
		//�k�����
	case normal:
		PlayerStatusNormal();
		result = hitChackNormalPlayer_Block(g_Player.vel);
		g_Player.dorpspeed.y += g_Player.gravity;
		break;
		//���[�v�ҋ@���
	case warpwait:
		PlayerStatusWarpwait();
		result = hitChackNormalPlayer_Block(g_Player.vel);
		g_Player.dorpspeed.y = 1.0f;
		break;
		//���|�v���
	case warp:
		PlayerStatusWarp();
		hitChackWarpPlayer_Block(g_Player.pos);
		break;
	}

	//�����蔻�菈��
	if (result & HIT_LEFT)
	{
		if (g_Player.vel.x > 0.0)
			g_Player.vel.x = 0.0f;
	}
	if (result & HIT_RIGHT)
	{
		if (g_Player.vel.x < 0.0)
			g_Player.vel.x = 0.0f;
	}

	result = hitChackNormalPlayer_Block(g_Player.dorpspeed);

	//���������邩�H����
	if ((result & HIT_UP) == 0 && g_Player.LandingFlag == true)
	{
		g_Player.LandingFlag = false;
	}

	//��������
	if (g_Player.LandingFlag == false)
	{
		if (g_Player.waitafterwarp > 0)
		{
			g_Player.waitafterwarp--;
			g_Player.dorpspeed.y = 0.0f;
		}
		else
		{
			if (result & HIT_UP)
			{
				g_Player.LandingFlag = true;
				g_Player.pos.y = GetBlockHeight() - (g_Player.size / 2);
				g_Player.dorpspeed.y = 0.0f;
				g_Player.waitafterwarp = 0;
			}
		}
	}
	else // �ŏI�I�ɒn�ʂɐG��Ă���
	{
		g_Player.dorpspeed.y = 0.0f;
	}

	g_Player.pos += g_Player.vel;
	g_Player.pos += g_Player.dorpspeed;

	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);

	////�A�j���[�V�����J�E���^�[���J�E���g�A�b�v���āA�E�G�C�g�l�𒴂�����
	//if (g_Player.animeCounter > 10)
	//{
	//	//�A�j���[�V�����p�^�[����؂�ւ���
	//	g_Player.animePtn++;
	//	//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
	//	if (g_Player.animePtn >= 4)
	//		g_Player.animePtn = 0;

	//	//�A�j���[�V�����J�E���^�[�̃��Z�b�g
	//	g_Player.animeCounter = 0;
	//}
	//g_Player.animeCounter++;

	//���G����̏���
	if (g_Player.mutekiflag == true)
	{
		g_Player.mutekitime++;

		if (g_Player.mutekitime >= 120)
		{
			g_Player.mutekitime = 0;
			g_Player.mutekiflag = false;
		}
	}

	if (g_Player.pos.y > 1200.0f)
	{
		SceneTransition(SCENE_OVER_GAME);
	}

	//�J�������W�̍X�V
	CAMERA_2D* pCamera = GetCamera();
	pCamera->pos.x = g_Player.pos.x - PLAYER_DISP_X;
	//pCamera->pos.y = g_Player.pos.y - PLAYER_DISP_Y - 60.0f;
	/*if (pCamera->pos.x < 0)
		pCamera->pos.x = 0;
	if (pCamera->pos.y < 0)
		pCamera->pos.y = 0;*/

}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	//�x�[�X���W���󂯎��
	D3DXVECTOR2 basePos = GetBase();

	//�L�����N�^�[�̕`��
	switch (g_Player.status)
	{
		//�k�����
	case normal:
		if (g_Player.mutekiflag == true)
		{
			float color = g_Player.mutekitime % 30;

			DrawSpriteColor(g_TextureNo,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[g_Player.animePtn],
				g_MukiTable[g_Player.muki],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
		}
		else
		{
			DrawSpriteColor(g_TextureNo,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[g_Player.animePtn],
				g_MukiTable[g_Player.muki],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		break;
		//���[�v�ҋ@���
	case warpwait:
		DrawSpriteColor(g_TextureNo,
			basePos.x + g_Player.pos.x,
			basePos.y + (g_Player.pos.y),
			g_Player.size, g_Player.size,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		DrawSpriteColor(g_TextureNo,
			basePos.x + g_Player.warppos.x,
			basePos.y + (g_Player.warppos.y),
			g_Player.size, g_Player.size,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		break;
		//���|�v���
	case warp:
		DrawSpriteColor(g_TextureNo,
			basePos.x + g_Player.pos.x,
			basePos.y + g_Player.pos.y,
			g_Player.size, g_Player.size,
			g_AnimeTable[g_Player.animePtn],
			g_MukiTable[g_Player.muki],
			PATTERN_WIDTH,
			PATTERN_HEIGHT,
			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		break;
	}

	//�x�[�X���W���󂯎��
	D3DXVECTOR2 BasePos = GetBase();


	if (g_Player.attackflag != 0)
	{
		if (g_Player.attackflag == 2)
		{
			DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x + 32.0f,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				0.6f, 0.0f, -0.2f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f);
		}
		else
		{
			DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x - 32.0f,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				0.4f, 0.0f, 0.2f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				0.0f);
		}

		g_Player.attackflag = 0;
	}
}

void PlayerDamage(int num)
{
	g_Player.hp -= num;

	g_Player.mutekiflag = true;
}

void PlayerStatusNormal(void)
{
	//���E�ړ�
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	//���[�v����(���[�v���ł͂Ȃ���ԂŃX�y�[�X�L�[�������ꂽ�Ƃ�)
	if (0.0 != GetThumbRightX(TEST_CON) || 0.0 != GetThumbRightY(TEST_CON))
	{
		if (g_Player.warpFlag != 0)
		{
			g_Player.status = warpwait;
		}
	}

	//�U�������ڍs�t���O
	if (IsButtonPressedX(TEST_CON, XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
	}

	//���[�v���L���X�g����
	if (g_Player.warpFlag < 3)
	{
		g_Player.warpRecast = timeGetTime();

		if (g_Player.warpRecast - g_Player.warpStartRecast >= 1000)//10.000
		{
			g_Player.warpFlag++;

			if (g_Player.warpFlag == 3)
			{
				g_Player.warpStartRecast = 0;
			}
			else
			{
				g_Player.warpStartRecast = timeGetTime();
			}
		}
	}
}

void PlayerStatusWarpwait(void)
{
	//���E�ړ�
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	if (0.0 == GetThumbRightX(TEST_CON) && 0.0 == GetThumbRightY(TEST_CON))
	{
		g_Player.status = warp;
	}
	else
	{
		if (g_Player.warpframe < 60)
		{
			g_Player.warpframe++;
		}

		D3DXVECTOR2 Direction = GetRightStick(TEST_CON);

		D3DXVec2Normalize(&Direction, &Direction);

		g_Player.warppos.x = g_Player.pos.x + (Direction.x * -1) * (g_Player.warppower * ((float)g_Player.warpframe / 60.0f));
		g_Player.warppos.y = g_Player.pos.y + (Direction.y) * (g_Player.warppower * ((float)g_Player.warpframe / 60.0f));
	}
}

void PlayerStatusWarp(void)
{
	g_Player.pos.x = g_Player.warppos.x;
	g_Player.pos.y = g_Player.warppos.y;

	g_Player.warpframe = 0;
	g_Player.waitafterwarp = 9;
	g_Player.warpFlag--;

	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);

	g_Player.status = normal;

	if (g_Player.warpStartRecast == 0)
	{
		g_Player.warpStartRecast = timeGetTime();
	}
}

void AdjustPlayer(D3DXVECTOR2 pos)
{
	g_Player.pos += pos;
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}

D3DXVECTOR2 GetLeftStick(int padNo)
{
	return D3DXVECTOR2(GetThumbLeftX(padNo), GetThumbLeftY(padNo));
}

D3DXVECTOR2 GetRightStick(int padNo)
{
	return D3DXVECTOR2(GetThumbRightX(padNo), GetThumbRightY(padNo));
}