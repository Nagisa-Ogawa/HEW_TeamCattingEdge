/*==============================================================================

   ���_�Ǘ� [bg.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "bg.h"
#include "camera.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX 4	//�K�v�Ȓ��_�̐�

#define DIVIDE_X 8	//���̕�����
#define DIVIDE_Y 8	//�c�̕�����

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetVertexBG(float x, float y, float width, float height,
	float u, float v, float uw, float vh);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView	*g_Texture = NULL;		// �e�N�X�`�����

static char *g_TextureName = (char*)"data/TEXTURE/�V��BG.png";

float g_StartU = 0.0f;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBG(void)
{
	ID3D11Device *pDevice = GetDevice();

	D3DX11CreateShaderResourceViewFromFile(pDevice,
		g_TextureName,
		NULL,
		NULL,
		&g_Texture,
		NULL);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_StartU = 0.0f;

	//�f�[�^�̏�����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBG(void)
{
	// �e�N�X�`���̉��
	if (g_Texture)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_StartU = 0.0f;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBG(void)
{
	//g_StartU = GetPlayerPos().x / (float)SCREEN_WIDTH;
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//�e�N�X�`���̃Z�b�g
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	//�x�[�X���W���󂯎��
	D3DXVECTOR2 basePos = GetBase();

	//�x�[�X���C���[�̕`��
	SetVertexBG(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		g_StartU,0.0f,1.0f,1.0f);

	// �|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


void SetVertexBG(float x, float y, float width, float height,
	float u, float v, float uw, float vh)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(x, y, 0.0f);
	vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(u, v);

	vertex[1].Position = D3DXVECTOR3(x + width, y, 0.0f);
	vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(u + uw, v);

	vertex[2].Position = D3DXVECTOR3(x, y + height, 0.0f);
	vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(u, v + vh);

	vertex[3].Position = D3DXVECTOR3(x + width, y + height, 0.0f);
	vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(u + uw, v + vh);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}
