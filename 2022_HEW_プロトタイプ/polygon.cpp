/*==============================================================================

   ���_�Ǘ� [polygon.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "polygon.h"
#include "collision.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX 4	//�K�v�Ȓ��_�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetVertex(float x, float y, float width, float height,
	float u, float v, float uw, float vh);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView	*g_Texture = NULL;		// �e�N�X�`�����
static ID3D11ShaderResourceView	*g_TextureEnemy = NULL;	// �e�N�X�`�����
static ID3D11ShaderResourceView	*g_TextureBG = NULL;	// �e�N�X�`�����
static ID3D11ShaderResourceView	*g_TextureRM = NULL;	// �e�N�X�`�����

static char *g_TextureName = (char*)"data/TEXTURE/bullet00.png";
static char *g_TextureNameEnemy = (char*)"data/TEXTURE/enemy00.png";
static char *g_TextureNameBG = (char*)"data/TEXTURE/IMG_0189.JPG";
static char *g_TextureNameRM = (char*)"data/TEXTURE/runningman002.png";



struct VERTEX_T{
	D3DXVECTOR2		pos;	//�ʒu�x�N�g��
	D3DXVECTOR2		vel;	//���x�x�N�g��
	D3DXCOLOR		color;	//���_�J���[
	int				use;
};

static VERTEX_T g_Polygon;
static VERTEX_T g_Enemy;
static VERTEX_T g_RunningMan;

static int g_AnimePtn;
static int g_AnimeCounter;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPolygon(void)
{
	ID3D11Device *pDevice = GetDevice();

	D3DX11CreateShaderResourceViewFromFile(pDevice,
		g_TextureName,
		NULL,
		NULL,
		&g_Texture,
		NULL);

	D3DX11CreateShaderResourceViewFromFile(pDevice,
		g_TextureNameEnemy,
		NULL,
		NULL,
		&g_TextureEnemy,
		NULL);

	D3DX11CreateShaderResourceViewFromFile(pDevice,
		g_TextureNameBG,
		NULL,
		NULL,
		&g_TextureBG,
		NULL);

	D3DX11CreateShaderResourceViewFromFile(pDevice,
		g_TextureNameRM,
		NULL,
		NULL,
		&g_TextureRM,
		NULL);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//������
	g_Polygon.pos.x = frand() * SCREEN_WIDTH;
	g_Polygon.pos.y = frand() * SCREEN_HEIGHT;

	g_Polygon.vel.x = frand() * 3 - 1.5f;
	g_Polygon.vel.y = frand() * 3 - 1.5f;

	g_Polygon.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


	//������
	g_Enemy.pos.x = frand() * SCREEN_WIDTH;//0.0�`960.0�̒l�̃����_���l�����
	g_Enemy.pos.y = frand() * SCREEN_HEIGHT;//0.0�`540.0�̒l�̃����_���l�����

	g_Enemy.vel.x = frand() * 3 - 1.5f;//-1.5�`1.5�̒l�̃����_���l�����
	g_Enemy.vel.y = frand() * 3 - 1.5f;//-1.5�`1.5�̒l�̃����_���l�����

	g_Enemy.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_Enemy.use = 1;


	//������
	g_RunningMan.pos.x = SCREEN_WIDTH / 2;//0.0�`960.0�̒l�̃����_���l�����
	g_RunningMan.pos.y = SCREEN_HEIGHT / 2;//0.0�`540.0�̒l�̃����_���l�����

	g_RunningMan.vel.x = frand() * 3 - 1.5f;//-1.5�`1.5�̒l�̃����_���l�����
	g_RunningMan.vel.y = frand() * 3 - 1.5f;//-1.5�`1.5�̒l�̃����_���l�����

	g_RunningMan.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_RunningMan.use = 1;


	g_AnimePtn = 0;

	// ���_�o�b�t�@�X�V
//	SetVertex();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPolygon(void)
{
	// �e�N�X�`���̉��
	if (g_Texture)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}

	// �e�N�X�`���̉��
	if (g_TextureEnemy)
	{
		g_TextureEnemy->Release();
		g_TextureEnemy = NULL;
	}

	// �e�N�X�`���̉��
	if (g_TextureBG)
	{
		g_TextureBG->Release();
		g_TextureBG = NULL;
	}

	// �e�N�X�`���̉��
	if (g_TextureRM)
	{
		g_TextureRM->Release();
		g_TextureRM = NULL;
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePolygon(void)
{
	//�ʒu�x�N�g���ɑ��x�x�N�g���𑫂�
	g_Polygon.pos = g_Polygon.pos + g_Polygon.vel;

	//��ʉE�[�Œ��˕Ԃ�
	if (SCREEN_WIDTH <= g_Polygon.pos.x)
		g_Polygon.vel.x *= -1;

	//��ʉ��[�Œ��˕Ԃ�
	if (SCREEN_HEIGHT <= g_Polygon.pos.y)
		g_Polygon.vel.y *= -1;

	//��ʏ�[�Œ��˕Ԃ�
	if (0.0f >= g_Polygon.pos.y)
		g_Polygon.vel.y *= -1;

	//��ʍ��[�Œ��˕Ԃ�
	if (0.0f >= g_Polygon.pos.x)
		g_Polygon.vel.x *= -1;



	//�S�ẴA�b�v�f�[�g���I��������Ƃɓ����蔻����s��
	if (HitCheckBox(g_Polygon.pos, 100.0f, 100.0f, g_Enemy.pos, 100.0f, 100.0f))
	{
		g_Enemy.use = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPolygon(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//�w�i�̕`��

	//�e�N�X�`���̃Z�b�g
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureBG);

	//���_�o�b�t�@�X�V
//	SetVertex(480.0f, 270.0f, 960.0f, 540.0f);

	// �|���S���`��
//	GetDeviceContext()->Draw(NUM_VERTEX, 0);


	//�v���C���[�̕`��

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	SetVertex(g_Polygon.pos.x, g_Polygon.pos.y, 100.0f, 100.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// �|���S���`��
//	GetDeviceContext()->Draw(NUM_VERTEX, 0);


	//�G�̕`��

	if (g_Enemy.use == 1)
	{
		//�e�N�X�`���̃Z�b�g
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureEnemy);

		//���_�o�b�t�@�X�V
		SetVertex(g_Enemy.pos.x, g_Enemy.pos.y, 100.0f, 100.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
//		GetDeviceContext()->Draw(NUM_VERTEX, 0);
	}



	//�����j���O�}���̕`��

	//�e�N�X�`���̃Z�b�g
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureRM);

	//���_�o�b�t�@�X�V
	SetVertex(g_RunningMan.pos.x, g_RunningMan.pos.y, 140.0f, 200.0f, 0.2f*g_AnimePtn, 0.0f, 0.2f, 0.5f);

	//�A�j���[�V�����J�E���^�[���J�E���g�A�b�v���āA�E�G�C�g�l�𒴂�����
	if (g_AnimeCounter > 10)
	{
		//�A�j���[�V�����p�^�[����؂�ւ���
		g_AnimePtn++;
		//�Ō�̃A�j���[�V�����p�^�[����\�������烊�Z�b�g����
		if (g_AnimePtn >= 5)
			g_AnimePtn = 0;

		//�A�j���[�V�����J�E���^�[�̃��Z�b�g
		g_AnimeCounter = 0;
	}
	g_AnimeCounter++;

	// �|���S���`��
//	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


//=============================================================================
// ���_�f�[�^�ݒ�
//=============================================================================
void SetVertex(float x, float y, float width, float height,
	           float u, float v, float uw, float vh )
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

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

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}
