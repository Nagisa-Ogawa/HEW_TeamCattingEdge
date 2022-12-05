/*==============================================================================

   �G�̏��� [enemy.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "enemy.h"
#include "texture.h"
#include "sprite.h"
#include "sound.h"
#include "player.h"
#include "camera.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_SPEED (6.0f)	//�G�̑��x

////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//void EnemyState_Chase(int index);
//void EnemyState_Wait(int index);
//void EnemyState_Return(int index);
//
////*****************************************************************************
//// �O���[�o���ϐ�
////*****************************************************************************
//static int	g_TextureNo;		// �e�N�X�`�����ʎq
//static ENEMY g_Enemy[MAX_ENEMY];//�G�o�b�t�@
//
//
////=============================================================================
//// ����������
////=============================================================================
//void InitEnemy(void)
//{
//	//�^�C�g����ʗp�e�N�X�`���̓ǂݍ���
//	g_TextureNo = LoadTexture((char*)"data/TEXTURE/enemy00.png");
//
//	//�G�o�b�t�@�̃t���O�̏�����
//	for (int i = 0; i < MAX_ENEMY; i++)
//	{
//		g_Enemy[i].use = false;
//	}
//}
//
//void UninitEnemy(void)
//{
//
//}
//
//void UpdateEnemy(void)
//{
//	PLAYER* pPlayer = GetPlayer();
//
//	for (int i = 0; i < MAX_ENEMY; i++)
//	{
//		//���t���O���I���ł���΍X�V����
//		if (g_Enemy[i].use == true)
//		{
//			switch (g_Enemy[i].state)
//			{
//			case chase:
//				EnemyState_Chase(i);
//				break;
//			case wait:
//				EnemyState_Wait(i);
//				break;
//			case Return:
//				EnemyState_Return(i);
//				break;
//			default:
//				break;
//			}
//
//			//�G�̎��Ԃ�i�߂�
//			g_Enemy[i].frame++;
//		}
//	}
//}
//
//void DrawEnemy(void)
//{
//
//	SetBlendState(BLEND_MODE_ALPHABLEND);//�ʏ�̃A���t�@����
////	SetBlendState(BLEND_MODE_ADD);		 //���Z����
////	SetBlendState(BLEND_MODE_SUBTRACT);	 //���Z����
//	
//	//�t�B���^�[���[�h�ƃA�h���b�V���O���[�h�̐ݒ�
//	//�A�h���b�V���O���[�h:WRAP�@���b�v �� �J��Ԃ�
//	//					  :MIRROR  �~���[ �� ���ʌJ��Ԃ�
//	//                    :CLAMP�@�N�����v �� �ŏI�s�N�Z���̐F�œh��Ԃ�
//	//                    :BORDER�@�{�[�_�[ �� �ݒ�F�œh��Ԃ��@
////	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_WRAP);
////	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_MIRROR);
////	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_CLAMP);
//	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_BORDER);
//	SetSamplerBorderColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//
//	//�t�B���^�[���[�h :POINT�@�|�C���g �� �|���S���̃s�N�Z���Ɉ�ԋ߂��e�N�Z���̐F���擾���� 
//	//                 :LINEAR�@���j�A �� �Ώۃe�N�Z���̕��ϒl���擾����
//	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_BORDER);
//	SetSamplerState(FILTER_MODE_LINEAR, ADDRESS_MODE_BORDER);
//	
//
//	//�x�[�X���W���󂯎��
//	D3DXVECTOR2 basePos = GetBase();
//
//	for (int i = 0; i < MAX_ENEMY; i++)
//	{
//		//���t���O���I���ł���Ε`�悷��
//		if (g_Enemy[i].use == true)
//		{
//			float rot;
//
//			//�ړ����������]�p�x���v�Z����(�G�ɍ��킹��-90����]����)
//			rot = atan2f(g_Enemy[i].dir.y, g_Enemy[i].dir.x) - (D3DX_PI / 2);
//
//			DrawSpriteColorRotate(g_TextureNo, basePos.x + g_Enemy[i].pos.x, basePos.y + g_Enemy[i].pos.y, 64.0f, 64.0f,
//				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), rot);
//		}
//	}
//
//	SetBlendState(BLEND_MODE_ALPHABLEND);//�ʏ�̃A���t�@�����ɖ߂�
//}
//
//void SetEnemy(void)
//{
//	for (int i = 0; i < MAX_ENEMY; i++)
//	{
//		//�e�o�b�t�@�̒�����\������Ă��Ȃ����̂�T��
//		if (g_Enemy[i].use == false)
//		{
//			PLAYER* pPlayer = GetPlayer();
//
//			//��ʂ̊O���ɐڂ���~�����
//
//			//�p�x�����
//			float rot = frand() * 2 * D3DX_PI;
//
//			//��ʂ̊O���ɐڂ���~�̔��a���v�Z����
//			D3DXVECTOR2 screen;
//			screen.x = SCREEN_WIDTH / 2;
//			screen.y = SCREEN_HEIGHT / 2;
//			float length;
//			length = D3DXVec2Length(&screen);
//
//			//�~�̍��W�ƃv���C���[�̍��W����G�̔������W���쐬����
//			g_Enemy[i].pos.x = cos(rot) * length + pPlayer->pos.x;
//			g_Enemy[i].pos.y = sin(rot) * length + pPlayer->pos.y;
//
//			g_Enemy[i].start_pos = g_Enemy[i].pos;
//
//			g_Enemy[i].muki = 0;
//
//			g_Enemy[i].speed = 1.0f;
//
//			g_Enemy[i].height = 0.0f; //��őΉ�����
//
//			//�L���t���[�����Z�b�g
//			g_Enemy[i].frame = 0;
//
//			//��Ԃ����Z�b�g����
//			g_Enemy[i].state = chase;
//
//			//���t���O�I��
//			g_Enemy[i].use = true;
//
//			//�G����Z�b�g�����烋�[�v�I��
//			break;
//		}
//	}
//}
//
//ENEMY* GetEnemy(void)
//{
////	return g_Enemy;
//	return (&g_Enemy[0]);
//}
//
//void EnemyState_Chase(int index)
//{
//	PLAYER* pPlayer = GetPlayer();
//
//	//�G�ƃv���C���[�̋�������������
//	D3DXVECTOR2 vDist;
//	vDist = g_Enemy[index].pos - pPlayer->pos;
//
//	D3DXVec2Normalize(&vDist, &vDist);
//
//	float x = D3DXVec2Dot(&vDist, &g_Enemy[index].dir);
//
//	if (x < 0.0f)
//	{
//		//�G���v���C���[�̕��Ɍ������Ă���x�N�g�������
//		//�ړI�̃x�N�g�� = �v���C���[���W - �G���W
//		g_Enemy[index].dir = pPlayer->pos - g_Enemy[index].pos;
//		//�x�N�g���𐳋K������֐�
//		D3DXVec2Normalize(&g_Enemy[index].dir, &g_Enemy[index].dir);
//
//		//�G�̍��W�X�V�i�G�̈ړ����x = �G�̈ړ����� * �G�̈ړ��X�s�[�h�j
//		g_Enemy[index].pos += (g_Enemy[index].dir * g_Enemy[index].speed);
//	}
//
//	if (g_Enemy[index].frame > 200)
//	{
//		//�ҋ@��Ԃֈڍs
//		g_Enemy[index].state = Return;
//		g_Enemy[index].frame = 0;
//	}
//}
//
//void EnemyState_Wait(int index)
//{
//	PLAYER *pPlayer = GetPlayer();
//	//if (g_Enemy[index].frame > 100)
//	
//	//�G�ƃv���C���[�̋�������������
//	D3DXVECTOR2 vDist;
//	vDist = pPlayer->pos - g_Enemy[index].pos;
//	float len = D3DXVec2Length(&vDist);
//
//	//�v���C���[�Ƃ̋����𒲂ׂ�200.0f��菬�����Ȃ����瓮��
//	if(len < 200.0f)
//	{
//		D3DXVec2Normalize(&vDist, &vDist);
//
//		float x = D3DXVec2Dot(&vDist,&g_Enemy[index].dir);
//
//			if (x > 0.5f)
//			{
//				//�ҋ@��Ԃֈڍs
//				g_Enemy[index].state = chase;
//				g_Enemy[index].frame = 0;
//			}
//	}
//}
//
//void EnemyState_Return(int index)
//{
//	//�G���X�^�[�g�n�_�̕��Ɍ������Ă���x�N�g�������
//	//�ړI�̃x�N�g�� = �X�^�[�g�n�_ - �G���W
//	g_Enemy[index].dir = g_Enemy[index].start_pos - g_Enemy[index].pos;
//	//�x�N�g���𐳋K������֐�
//	D3DXVec2Normalize(&g_Enemy[index].dir, &g_Enemy[index].dir);
//	//�G�̍��W�X�V�i�G�̈ړ����x = �G�̈ړ����� * �G�̈ړ��X�s�[�h�j
//	g_Enemy[index].pos += (g_Enemy[index].dir * g_Enemy[index].speed);
//
//	D3DXVECTOR2 vDist;
//	vDist = g_Enemy[index].pos - g_Enemy[index].start_pos;
//	float len = D3DXVec2Length(&vDist);
//
//	//�X�^�[�g�ʒu�ɖ߂������H
//	if (len < g_Enemy[index].speed)
//	{
//		g_Enemy[index].pos = g_Enemy[index].start_pos;
//		g_Enemy[index].frame = 0;
//		g_Enemy[index].state = wait;
//	}
//}