/*==============================================================================

   ���_�Ǘ� [goal.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "goal.h"
#include "collision.h"
#include "player.h"
#include "camera.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX 4	//�K�v�Ȓ��_�̐�

#define DIVIDE_X 3	//���̕�����
#define DIVIDE_Y 4	//�c�̕�����

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetVertexGoal(float x, float y, float width, float height,
	float u, float v, float uw, float vh);

void ResetGoal(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBufferGL = NULL;	// ���_���
static ID3D11ShaderResourceView	*g_TextureGL = NULL;		// �e�N�X�`�����

static char *g_TextureNameGL = (char*)"data/TEXTURE/mapchip.png";

static GOAL g_Goal;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGoal(void)
{
	ID3D11Device *pDevice = GetDevice();

	D3DX11CreateShaderResourceViewFromFile(pDevice,
		g_TextureNameGL,
		NULL,
		NULL,
		&g_TextureGL,
		NULL);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBufferGL);

	//�f�[�^�̏�����
	g_Goal.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	g_Goal.animePtn = 0;
	g_Goal.animeCounter = 0;
	g_Goal.use = 1;

	ResetGoal();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGoal(void)
{
	// �e�N�X�`���̉��
	if (g_TextureGL)
	{
		g_TextureGL->Release();
		g_TextureGL = NULL;
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBufferGL)
	{
		g_VertexBufferGL->Release();
		g_VertexBufferGL = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGoal(void)
{
	//�A�j���[�V�����J�E���^�[���J�E���g�A�b�v���āA�E�G�C�g�l�𒴂�����
	if (g_Goal.animeCounter > 10)
	{
		//�A�j���[�V�����p�^�[����؂�ւ���
		g_Goal.animePtn++;
		//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
		if (g_Goal.animePtn >= 4)
			g_Goal.animePtn = 0;

		//�A�j���[�V�����J�E���^�[�̃��Z�b�g
		g_Goal.animeCounter = 0;
	}
	g_Goal.animeCounter++;

	//player.cpp�ɂ���g_Player�̃A�h���X���󂯎��
	PLAYER* pPlayer = GetPlayer();

	//�v���C���[�ƃS�[���̓����蔻��
	if (HitCheckBox(pPlayer->pos, 64.0f, 64.0f,
		g_Goal.pos, 64.0f, 64.0f))
	{
		//�V�����S�[����ݒ肷��
		ResetGoal();
	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawGoal(void)
{
	//�x�[�X���W���󂯎��
	D3DXVECTOR2 basePos = GetBase();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBufferGL, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//�e�N�X�`���̃Z�b�g
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureGL);

	//���_�o�b�t�@�X�V
	SetVertexGoal(basePos.x + g_Goal.pos.x, basePos.y + g_Goal.pos.y, 64.0f, 64.0f,
		0.875f,
		0.75f,
		0.125f,
		0.125f);

	// �|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


void SetVertexGoal(float x, float y, float width, float height,
	float u, float v, float uw, float vh)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBufferGL, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(x - (width / 2), y - (height / 2), 0.0f);
	vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(u, v);

	vertex[1].Position = D3DXVECTOR3(x + (width / 2), y - (height / 2), 0.0f);
	vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(u + uw, v);

	vertex[2].Position = D3DXVECTOR3(x - (width / 2), y + (height / 2), 0.0f);
	vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(u, v + vh);

	vertex[3].Position = D3DXVECTOR3(x + (width / 2), y + (height / 2), 0.0f);
	vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(u + uw, v + vh);

	GetDeviceContext()->Unmap(g_VertexBufferGL, 0);
}

void ResetGoal(void)
{
	g_Goal.pos.x = frand() * SCREEN_WIDTH;
	g_Goal.pos.y = frand() * SCREEN_HEIGHT;
}
