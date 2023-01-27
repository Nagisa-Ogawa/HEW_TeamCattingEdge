/*==============================================================================

   �^�C�g����� [title.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "result.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
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
static int	g_TextureNo;	// �e�N�X�`�����ʎq
static int  g_StaffRollTextureNo;

enum RESULT_SCENE
{
	RESULT,
	STAFFROLL,
};

RESULT_SCENE g_ResultScene;

//=============================================================================
// ����������
//=============================================================================
void InitResult(void)
{
	g_ResultScene = RESULT_SCENE::RESULT;
	//�^�C�g����ʗp�e�N�X�`���̓ǂݍ���
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/ED.png");
	g_StaffRollTextureNo= LoadTexture((char*)"data/TEXTURE/endroll.png");

}

void UninitResult(void)
{
}

void UpdateResult(void)
{
	switch (g_ResultScene)
	{
	case RESULT:
		//�X�y�[�X�L�[�������ꂽ��Q�[���V�[���ֈڍs����
		if (IsButtonTriggeredX(0, XINPUT_GAMEPAD_A))
		{
			g_ResultScene = STAFFROLL;
		}
		break;
	case STAFFROLL:
		if (IsButtonTriggeredX(0, XINPUT_GAMEPAD_A))
		{
			SetFadeColor(0.0f, 0.0f, 0.0f, 0.0f);//�t�F�[�h�J���[�����ɕς���
			SceneTransition(SCENE_TITLE);
		}
		break;
	default:
		break;
	}
}

void DrawResult(void)
{
	switch (g_ResultScene)
	{
	case RESULT:
		DrawSpriteLeftTop(g_TextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case STAFFROLL:
		DrawSpriteLeftTop(g_StaffRollTextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f);
		break;
	default:
		break;
	}
}