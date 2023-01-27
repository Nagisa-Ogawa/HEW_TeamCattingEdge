/*==============================================================================

   �v���C���[�Ǘ� [player.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "texture.h"
#include "sprite.h"
#include "number.h"
#include "camera.h"
#include "inputx.h"
#include "Block.h"
#include "game.h"
#include "EnemyFactory.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX 4	//�K�v�Ȓ��_�̐�

#define DIVIDE_X 8	//���̕�����
#define DIVIDE_Y 7	//�c�̕�����

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)

#define PLAYER_SPEED	4.0f //�v���[���[�̃X�s�[�h

#define TEST_CON 0

#define MONOSIZE 64.0f

#define WARPRECAST 3000//3000

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void PlayerStatusNormal(void);
void PlayerStatusWarpwait(void);
void PlayerStatusWarp(void);
void PlayerStatusAttack(void);
void PlayerStatusDeath(void);
void DrawWarpKaminari(D3DXVECTOR2 beforepos, D3DXVECTOR2 afterpos);

//�X�e�B�b�N���擾�֐�
D3DXVECTOR2 GetLeftStick(int padNo);
D3DXVECTOR2 GetRightStick(int padNo);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER g_Player;
static D3DXVECTOR2 g_Direction;

static int	g_SE_wapu;		// SE�̎��ʎq
static int	g_SE_attack;		// SE�̎��ʎq
static int	g_SE_walk;		// SE�̎��ʎq
static int	g_SE_dash;		// SE�̎��ʎq
static int	g_SE_damage;		// SE�̎��ʎq
static bool g_Frag_warpTextrue = false;
static bool g_Frag_dashSE = false;
static bool g_Frag_damageSE = false;

static int g_TextureLeft = 0;	//�v���C���[�p�e�N�X�`���̎��ʎq
static int g_TextureRight = 0;	//�v���C���[�p�e�N�X�`���̎��ʎq
static int g_TextureWarp = 0;	//�v���C���[�p�e�N�X�`���̎��ʎq
static int g_TextureAttack = 0;	//�v���C���[�p�e�N�X�`���̎��ʎq

static float g_AnimeTable[8] =
{
	0.0f,
	0.125f,
	0.25f,
	0.375f,
	0.5f,
	0.625f,
	0.75f,
	0.875f,
};

static float g_AnimeTableTate[7]=
{
	PATTERN_HEIGHT * 0.0f,	//�ҋ@
	PATTERN_HEIGHT * 1.0f,	//����
	PATTERN_HEIGHT * 2.0f,	//����
	PATTERN_HEIGHT * 3.0f,	//�U��
	PATTERN_HEIGHT * 4.0f,	//���[�v
	PATTERN_HEIGHT * 5.0f,	//���
	PATTERN_HEIGHT * 6.0f,	//���S
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(GAMESCENE gamescene)
{
	//�e�N�X�`���̓ǂݍ���
	g_TextureLeft  = LoadTexture((char*)"data/TEXTURE/hituzi_L.png");
	g_TextureRight = LoadTexture((char*)"data/TEXTURE/hituzi_R.png");
	g_TextureWarp = LoadTexture((char*)"data/TEXTURE/���[�v.png");
	g_TextureAttack = LoadTexture((char*)"data/TEXTURE/���[�v.png");

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
	g_Player.warppower = 420.0f; //�ő勗��
	g_Player.warpFlag = 3;//int
	g_Player.gravity = 0.6f;
	g_Player.beforewarppos = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.afterwarppos = D3DXVECTOR2(0.0f, 0.0f);

	//���������֘A�̏�����
	g_Player.dorpspeed = D3DXVECTOR2(0.0f, 0.0f);
	g_Player.LandingFlag = false; //���n�t���O

	//�A�j���[�V�����֘A�̏�����
	g_Player.muki = 0;
	g_Player.animeWalk  = 0;
	g_Player.animeAttack = 0;
	g_Player.animeDeath = 0;
	g_Player.animeCounterRun = 0;
	g_Player.animeCounterWalk = 0;

	g_Player.move = false;

	//�퓬�֘A���̏�����
	g_Player.mutekiflag = false;
	g_Player.mutekitime = 0;
	g_Player.attackflag = 0;
	g_Player.attackRecast = 0;
	g_Player.hp = 10;

	//���֘A�̏�����
	g_SE_wapu = LoadSound((char*)"data/SE/wa-pu.wav");
	SetVolume(g_SE_wapu, 0.5f);
	g_SE_attack = LoadSound((char*)"data/SE/player_attack.wav");
	SetVolume(g_SE_attack, 0.5f);
	g_SE_walk = LoadSound((char*)"data/SE/player_walk.wav");
	SetVolume(g_SE_walk, 0.5f);
	g_SE_dash = LoadSound((char*)"data/SE/kasya-hasiri.wav");
	SetVolume(g_SE_dash, 1.0f);
	g_SE_damage = LoadSound((char*)"data/SE/player_damage.wav");
	SetVolume(g_SE_damage, 0.8f);

	//���̑��̏�����
	g_Player.enemyfactory = GetEnemyFactory();
	g_Player.gamescene = gamescene;

	switch (gamescene)
	{
	case GAMESCENE_STAGE_TENGU:
		break;
	case GAMESCENE_BASS_TENGU:
		break;
	case GAMESCENE_STAGE_KASYA:
		g_Player.pos = D3DXVECTOR2(400.0f, 540.0f);
		break;
	case GAMESCENE_BASS_KASYA:
		break;
	case GAMESCENE_BASS_FUJIN:
		g_Player.pos = D3DXVECTOR2(960.0f, 540.0f);
		break;
	case GAMESCENE_BASS_FINAL:
		g_Player.pos = D3DXVECTOR2(960.0f, 540.0f);
		break;
	case GAMESCENE_PICTURE_OVERGAME:
		break;
	case GAMESCENE_PICTURE_RESULT:
		break;
	case GAMESCENE_NUM:
		break;
	default:
		break;
	}

	SetNumber(32768); //�X�R�A�̕`��

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	StopSound(g_SE_dash);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	if (g_Frag_damageSE)
	{
		PlaySound(g_SE_damage, 0);

		g_Frag_damageSE = false;
	}

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
	case attack:
		PlayerStatusAttack();
		result = hitChackNormalPlayer_Block(g_Player.vel);

		g_Player.dorpspeed.y += g_Player.gravity;
		break;
	case death:
		PlayerStatusDeath();
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

	//�A�j���[�V�����J�E���^�[���J�E���g�A�b�v���āA�E�G�C�g�l�𒴂�����
	if (g_Player.animeCounterWalk > 10)
	{
		//�A�j���[�V�����p�^�[����؂�ւ���
		g_Player.animeWalk++;
		//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
		if (g_Player.animeWalk >= 8)
			g_Player.animeWalk = 0;

		//�A�j���[�V�����J�E���^�[�̃��Z�b�g
		g_Player.animeCounterWalk = 0;
	}
	g_Player.animeCounterWalk++;

	//����
	if (g_Player.animeCounterRun > 5)
	{
		//�A�j���[�V�����p�^�[����؂�ւ���
		g_Player.animeRun++;
		//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
		if (g_Player.animeRun >= 8)
			g_Player.animeRun = 0;

		//�A�j���[�V�����J�E���^�[�̃��Z�b�g
		g_Player.animeCounterRun = 0;
	}
	g_Player.animeCounterRun++;

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
		SetGameScene(GAMESCENE_PICTURE_OVERGAME);
	}

	//�U�����L���X�g����
	if (g_Player.attackflag > 0)
	{
		g_Player.attackRecast++;

		if (g_Player.attackRecast == 30)
		{
			g_Player.attackflag = 3;
			g_Player.animeCounterAttackKaminari = 0;
			g_Player.animeAttackKaminari = 0;
		}

		if (g_Player.attackRecast >= 60)
		{
			g_Player.attackflag = 0;
			g_Player.attackRecast = 0;
		}
	}

	//���S�m�F
	if (g_Player.hp <= 0)
	{
		g_Player.status = death;
	}

	//�J�������W�̍X�V
	CAMERA_2D* pCamera = GetCamera();

	switch (g_Player.gamescene)
	{
	case GAMESCENE_STAGE_TENGU:
		pCamera->pos.x = g_Player.pos.x - SCREEN_WIDTH / 2;
		if (pCamera->pos.x <= 0.0f)
		{
			pCamera->pos.x = 0.0f;
		}
		if (pCamera->pos.x >= 5460.0f)
		{
			pCamera->pos.x = 5460.0f;
		}
		break;
	case GAMESCENE_STAGE_KASYA:
		if (pCamera->pos.x <= 13380.0f)
			pCamera->pos.x += 3.0f;//���X�N���[���̑��x 2.0f->3.0f

		//�����ŉΎԃ{�X�X�e�[�W�փV�[���ڍs
		if (g_Player.pos.x >= 15360.0f)
		{
			SetGameScene(GAMESCENE_PICTURE_STAGE02_BOSS	);
		}
		break;
	default:
		//pCamera->pos.x = SCREEN_WIDTH / 2;
		break;
	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	//�x�[�X���W���󂯎��
	D3DXVECTOR2 basePos = GetBase();

	//���[�v���̗��̕`��
	if (g_Frag_warpTextrue == true)
	{
		DrawWarpKaminari(g_Player.beforewarppos, g_Player.pos);
	}

	if (g_Player.mutekiflag == true)
	{
		float color = (float)(g_Player.mutekitime % 30);

		switch (g_Player.status)
		{
			//�k�����
		case normal:
			if (g_Player.waitafterwarp > 0)
			{
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[1],
					g_AnimeTableTate[4],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				switch (g_Player.muki)
				{
				case 0:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[0],
						g_AnimeTableTate[0],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
					break;
				case 1:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeWalk],
						g_AnimeTableTate[1],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
					break;
				case 2:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeRun],
						g_AnimeTableTate[2],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
					break;
				case 3:
					DrawSpriteColor(g_TextureLeft,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeWalk],
						g_AnimeTableTate[1],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
					break;
				case 4:
					DrawSpriteColor(g_TextureLeft,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeRun],
						g_AnimeTableTate[2],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));					
					break;
				default:
					break;
				}
			}
			break;
			//���[�v�ҋ@���
		case warpwait:
			//�{�̂̕`��
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[0],
				g_AnimeTableTate[4],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));

			//���[�v��̕`��
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.warppos.x,
				basePos.y + (g_Player.warppos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[1],
				g_AnimeTableTate[4],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			break;
			//���|�v���
		case warp:
			break;
		case attack:
			switch (g_Player.muki)
			{
			case 2:
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
				break;
			case 4:
				DrawSpriteColor(g_TextureLeft,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
				break;
			default:
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, color / 30));
				break;
			}
			break;
		case death:
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[g_Player.animeDeath],
				g_AnimeTableTate[6],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		}
	}
	else
	{
		switch (g_Player.status)
		{
		//�k�����
		case normal:
			if (g_Player.waitafterwarp > 0)
			{
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[1],
					g_AnimeTableTate[4],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				switch (g_Player.muki)
				{
				case 0:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[0],
						g_AnimeTableTate[0],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 1:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeWalk],
						g_AnimeTableTate[1],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 2:
					DrawSpriteColor(g_TextureRight,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeRun],
						g_AnimeTableTate[2],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 3:
					DrawSpriteColor(g_TextureLeft,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeWalk],
						g_AnimeTableTate[1],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 4:
					DrawSpriteColor(g_TextureLeft,
						basePos.x + g_Player.pos.x,
						basePos.y + (g_Player.pos.y),
						g_Player.size, g_Player.size,
						g_AnimeTable[g_Player.animeRun],
						g_AnimeTableTate[2],
						PATTERN_WIDTH,
						PATTERN_HEIGHT,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				default:
					break;
				}
			}
			break;
		//���[�v�ҋ@���
		case warpwait:
			//�{�̂̕`��
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[0],
				g_AnimeTableTate[4],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			//���[�v��̕`��
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.warppos.x,
				basePos.y + (g_Player.warppos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[1],
				g_AnimeTableTate[4],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			break;
			//���|�v���
		case warp:
			break;
		case attack:
			switch (g_Player.muki)
			{
			case 2:
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			case 4:
				DrawSpriteColor(g_TextureLeft,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			default:
				DrawSpriteColor(g_TextureRight,
					basePos.x + g_Player.pos.x,
					basePos.y + (g_Player.pos.y),
					g_Player.size, g_Player.size,
					g_AnimeTable[g_Player.animeAttack],
					g_AnimeTableTate[3],
					PATTERN_WIDTH,
					PATTERN_HEIGHT,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			}
			break;
		case death:
			DrawSpriteColor(g_TextureRight,
				basePos.x + g_Player.pos.x,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				g_AnimeTable[g_Player.animeDeath],
				g_AnimeTableTate[6],
				PATTERN_WIDTH,
				PATTERN_HEIGHT,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		}
	}

	//�x�[�X���W���󂯎��
	D3DXVECTOR2 BasePos = GetBase();

	if (g_Player.attackflag != 0)
	{
		float Width = 1.0f / 13.0f;

		if (g_Player.animeCounterAttackKaminari > 4)
		{
			//�A�j���[�V�����p�^�[����؂�ւ���
			g_Player.animeAttackKaminari++;
			//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
			if (g_Player.animeAttackKaminari >= 7)
			{
				g_Player.animeAttackKaminari = 0;
			}
			//�A�j���[�V�����J�E���^�[�̃��Z�b�g
			g_Player.animeCounterAttackKaminari = 0;

		}
		g_Player.animeCounterAttackKaminari++;

		switch (g_Player.attackflag)
		{
		case 1://��
		/*	DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x - 120.0f,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				0.4f, 0.0f, 0.2f, 1.0f,
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
				0.0f);*/

			DrawSpriteLeftTop(g_TextureAttack,
				basePos.x + g_Player.pos.x - 360.0f,
				basePos.y + g_Player.pos.y - 1020.0f,
				480.0f, 1080.0f,
				Width * (6 + g_Player.animeAttackKaminari), 0.0f, Width, 1.0f);
			break;
		case 2://�E
		/*	DrawSpriteColorRotate(g_TextureAttack,
				basePos.x + g_Player.pos.x + 120.0f,
				basePos.y + (g_Player.pos.y),
				g_Player.size, g_Player.size,
				0.6f, 0.0f, -0.2f, 1.0f,
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
				0.0f);*/

			DrawSpriteLeftTop(g_TextureAttack,
				basePos.x + g_Player.pos.x - 120.0f,
				basePos.y + g_Player.pos.y - 1020.0f,
				480.0f, 1080.0f,
				Width * (6 + g_Player.animeAttackKaminari), 0.0f, Width, 1.0f);
			break;
		default:
			break;
		}
	}

}

void PlayerDamage(int num)
{
	g_Player.hp -= num;

	g_Player.mutekiflag = true;

	g_Frag_damageSE = true;
}

void PlayerStatusNormal(void)
{
	//���E�ړ�
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	if (g_Player.vel.x != 0.0f && g_Frag_dashSE == false)
	{
		PlaySound(g_SE_dash, -1);

		g_Frag_dashSE = true;
	}
	else if (g_Player.vel.x == 0.0f)
	{
		StopSound(g_SE_dash);

		g_Frag_dashSE = false;
	}

	if (GetThumbLeftX(TEST_CON) > 0.8f)
	{
		g_Player.muki = 2;
	}
	else if (GetThumbLeftX(TEST_CON) > 0.1f)
	{
		g_Player.muki = 1;
	}
	else if (GetThumbLeftX(TEST_CON) < -0.8f)
	{
		g_Player.muki = 4;
	}
	else if (GetThumbLeftX(TEST_CON) < -0.1f)
	{
		g_Player.muki = 3;
	}
	else
	{
		g_Player.muki = 0;
	}

	//���[�v����
	if (0.0 != GetThumbRightX(TEST_CON) || 0.0 != GetThumbRightY(TEST_CON))
	{
		if (g_Player.warpFlag != 0)
		{
			g_Player.status = warpwait;
		}
	}

	//���[�v���L���X�g����
	if (g_Player.warpFlag < 3)
	{
		g_Player.warpRecast = timeGetTime();

		if (g_Player.warpRecast - g_Player.warpStartRecast >= WARPRECAST)//3.000
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

	//�U�������ڍs�t���O
	if (IsButtonPressedX(TEST_CON, XINPUT_GAMEPAD_RIGHT_SHOULDER) && g_Player.attackflag == 0)
	{
		g_Player.enemyfactory->CollisoinAttacktoEnemy(D3DXVECTOR2(g_Player.pos.x + 120.0f, g_Player.pos.y));

		g_Player.attackflag = 2;
		g_Player.muki = 2;
		g_Player.status = attack;

		g_Player.animeCounterAttack = 0;
		g_Player.animeAttack = 0;

		PlaySound(g_SE_attack, 0);
	}

	if (IsButtonPressedX(TEST_CON, XINPUT_GAMEPAD_LEFT_SHOULDER) && g_Player.attackflag == 0)
	{
		g_Player.enemyfactory->CollisoinAttacktoEnemy(D3DXVECTOR2(g_Player.pos.x - 120.0f, g_Player.pos.y));

		g_Player.attackflag = 1;
		g_Player.muki = 4;
		g_Player.status = attack;
		g_Player.animeCounterAttack = 0;
		g_Player.animeAttack = 0;

		PlaySound(g_SE_attack, 0);
	}
}

void PlayerStatusWarpwait(void)
{
	//���E�ړ�
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	StopSound(g_SE_dash);

	g_Frag_dashSE = false;

	if (0.0 == GetThumbRightX(TEST_CON) && 0.0 == GetThumbRightY(TEST_CON))
	{
		g_Player.status = warp;
	}
	else
	{
		if (g_Player.warpframe < 30)//692,693�̐��l���ꏏ�ɕύX���K�v
		{
			g_Player.warpframe++;
		}

		D3DXVECTOR2 nowvel = GetRightStick(TEST_CON);
		D3DXVECTOR2 oldvel = g_Direction;

		if (oldvel.x < 0.0f)
		{
			oldvel.x += 0.1f;
		}
		else if(oldvel.x > 0.0f)
		{
			oldvel.x -= 0.1f;
		}

		if (oldvel.y < 0.0f)
		{
			oldvel.y += 0.1f;
		}
		else if (oldvel.y > 0.0f)
		{
			oldvel.y -= 0.1f;
		}

		if (D3DXVec2Length(&oldvel) <= D3DXVec2Length(&nowvel))
		{
			g_Direction = nowvel;
		}
		
		D3DXVECTOR2 warpvel = g_Direction;

		D3DXVec2Normalize(&warpvel, &warpvel);

		g_Player.warppos.x = g_Player.pos.x + (warpvel.x * -1) * (g_Player.warppower * ((float)g_Player.warpframe / 30.0f));
		g_Player.warppos.y = g_Player.pos.y + (warpvel.y) * (g_Player.warppower * ((float)g_Player.warpframe / 30.0f));
	}
}

void PlayerStatusWarp(void)
{
	//���[�v����
	g_Direction = D3DXVECTOR2(0.0f, 0.0f);

	g_Player.beforewarppos = g_Player.pos;

	g_Player.pos.x = g_Player.warppos.x;
	g_Player.pos.y = g_Player.warppos.y;

	g_Player.afterwarppos = g_Player.pos;

	g_Player.warpframe = 0;
	g_Player.waitafterwarp = 15;
	g_Player.warpFlag--;

	g_Player.vel = D3DXVECTOR2(0.0f, 0.0f);

	g_Player.status = normal;

	PlaySound(g_SE_wapu, 0);

	g_Frag_warpTextrue = true;

	if (g_Player.warpStartRecast == 0)
	{
		g_Player.warpStartRecast = timeGetTime();
	}
}

void PlayerStatusAttack(void)
{
	//���E�ړ�
	g_Player.vel.x += GetThumbLeftX(TEST_CON) * PLAYER_SPEED;

	if (g_Player.vel.x != 0.0f && g_Frag_dashSE == false)
	{
		PlaySound(g_SE_dash, -1);

		g_Frag_dashSE = true;
	}
	else if (g_Player.vel.x == 0.0f)
	{
		StopSound(g_SE_dash);

		g_Frag_dashSE = false;
	}

	//���[�v����
	if (0.0 != GetThumbRightX(TEST_CON) || 0.0 != GetThumbRightY(TEST_CON))
	{
		if (g_Player.warpFlag != 0)
		{
			g_Player.status = warpwait;
		}
	}

	//���[�v���L���X�g����
	if (g_Player.warpFlag < 3)
	{
		g_Player.warpRecast = timeGetTime();

		if (g_Player.warpRecast - g_Player.warpStartRecast >= WARPRECAST)//3.000
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

	if (g_Player.attackflag == 3)
	{
		g_Player.status = normal;
	}

	//�U���A�^�b�N�p�^�[���̍X�V
	if (g_Player.animeCounterAttack > 6)
	{
		//�A�j���[�V�����p�^�[����؂�ւ���
		g_Player.animeAttack++;
		//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
		if (g_Player.animeAttack >= 4)
			g_Player.animeAttack = 0;

		//�A�j���[�V�����J�E���^�[�̃��Z�b�g
		g_Player.animeCounterAttack = 0;
	}
	g_Player.animeCounterAttack++;
}

void PlayerStatusDeath(void)
{
	StopSound(g_SE_dash);

	g_Frag_dashSE = false;

	//���S�p�^�[���̍X�V
	if (g_Player.animeCounterDeath > 10)
	{
		//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
		if (g_Player.animeDeath >= 5 - 1)
		{
			if (g_Player.animeCounterDeath > 60)
			{
				SetGameScene(GAMESCENE_PICTURE_OVERGAME);
			}
		}
		else
		{
			//�A�j���[�V�����p�^�[����؂�ւ���
			g_Player.animeDeath++;

			//�A�j���[�V�����J�E���^�[�̃��Z�b�g
			g_Player.animeCounterDeath = 0;
		}
	}
	g_Player.animeCounterDeath++;
}

void DrawWarpKaminari(D3DXVECTOR2 beforepos, D3DXVECTOR2 afterpos)
{
	if (g_Player.animeCounterWarpKaminari > 2)
	{
		//�A�j���[�V�����p�^�[����؂�ւ���
		g_Player.animeWarpKaminari++;
		//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
		if (g_Player.animeWarpKaminari >= 4)
		{
			g_Player.animeWarpKaminari = 0;
			g_Frag_warpTextrue = false;
		}
		//�A�j���[�V�����J�E���^�[�̃��Z�b�g
		g_Player.animeCounterWarpKaminari = 0;

	}
	g_Player.animeCounterWarpKaminari++;

	D3DXVECTOR2 BeforeKaminari(beforepos.x - 120.0f, beforepos.y - 1020.0f);
	D3DXVECTOR2 AfterKaminari(afterpos.x - 120.0f, afterpos.y - 1020.0f);

	float Width = 1.0f / 13.0f;

	//�x�[�X���W���󂯎��
	D3DXVECTOR2 basePos = GetBase();

	//���i�r�t�H�[�j�̕`��
	DrawSpriteLeftTop(g_TextureWarp,
		basePos.x + BeforeKaminari.x,
		basePos.y + BeforeKaminari.y,
		240.0f, 1080.0f,
		Width * (4 + g_Player.animeWarpKaminari), 0.0f, Width, 1.0f);

	//���i�r�t�H�[�j�̕`��
	DrawSpriteLeftTop(g_TextureWarp,
		basePos.x + AfterKaminari.x,
		basePos.y + AfterKaminari.y,
		240.0f, 1080.0f,
		Width * (8 - g_Player.animeWarpKaminari), 0.0f, Width, 1.0f);
}

void AdjustPlayer(D3DXVECTOR2 pos)
{
	g_Player.pos += pos;
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}

D3DXVECTOR2 GetPlayerPos(void)
{
	return g_Player.pos;
}

D3DXVECTOR2 GetLeftStick(int padNo)
{
	return D3DXVECTOR2(GetThumbLeftX(padNo), GetThumbLeftY(padNo));
}

D3DXVECTOR2 GetRightStick(int padNo)
{
	return D3DXVECTOR2(GetThumbRightX(padNo), GetThumbRightY(padNo));
}