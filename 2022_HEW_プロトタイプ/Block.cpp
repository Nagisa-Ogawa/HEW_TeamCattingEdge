/*==============================================================================

   �X�e�[�W�u���b�N�Ǘ� [bg.cpp]
														 Author :�@���쏍��
														 Date   :  2022/11/19
--------------------------------------------------------------------------------

==============================================================================*/
#include "Block.h"
#include "camera.h"
#include "player.h"
#include "Enemy_SelfDestruct.h"


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
void SetVertexBlock(float x, float y, float width, float height,
	float u, float v, float uw, float vh);
bool HitCheckBox_Block(D3DXVECTOR2 box1pos, float box1width, float box1height,
	D3DXVECTOR2 box2pos, float box2width, float box2height);
// ���Ɛ��̓����蔻��
bool HitCheckLine(D3DXVECTOR2 A, D3DXVECTOR2 B, D3DXVECTOR2 C, D3DXVECTOR2 D);
// ���Ɛ��̌�_�����߂�֐�
D3DXVECTOR2 GetCrossPoint(D3DXVECTOR2 A, D3DXVECTOR2 B, D3DXVECTOR2 C, D3DXVECTOR2 D);
// �O��
float Cross(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView	*g_Texture = NULL;		// �e�N�X�`�����

static float g_Block_Height = 0.0f;

static char *g_TextureName = (char*)"data/TEXTURE/wall_03.png";

// 0 : �u���b�N�Ȃ�
// 1 : �u���b�N����
// 100 : �q�b�g�h���b�v����G
// 101 : ��������G
// 102: ���e�𓊂���G
// 200 : Boss
static std::vector<std::vector<int>> g_Blocks = {
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
};


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBlock(void)
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

	//�f�[�^�̏�����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBlock(void)
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
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBlock(void)
{
	//�v���C���[���W���󂯎��
	PLAYER* pPlayer = GetPlayer();

}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawBlock(void)
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

	//�X�e�[�W�̕`��
	for (int y = 0; y < g_Blocks.size(); y++)
	{
		for (int x = 0; x < g_Blocks[0].size(); x++)
		{
			// �u���b�N���Ȃ��Ȃ�X�L�b�v
			if (g_Blocks[y][x] != 1){
				continue;
			}

			SetVertexBlock(basePos.x + BLOCK_SIZE * x, basePos.y + BLOCK_SIZE * y, BLOCK_SIZE, BLOCK_SIZE,
				0.0f,
				0.0f,
				1.0f,
				1.0f);

			// �|���S���`��
			GetDeviceContext()->Draw(NUM_VERTEX, 0);
		}
	}
}

DWORD hitChackNormalPlayer_Block( D3DXVECTOR2 move)
{
	PLAYER* pPlayer = GetPlayer();

	DWORD result = 0;
	//�v���C���[�̈ړ�����W���쐬����
	D3DXVECTOR2 vPlayerPos = pPlayer->pos + move;

	float block_length = 99999;//��ԋ߂��u���b�N�̋���
	int block_index[2]{ -99999,-99999 };//1��ԋ߂��u���b�N�̓Y����

	//�G�o�b�t�@�̂��ׂĂ��`�F�b�N����
	for (int x = 0; x < g_Blocks[0].size(); x++)
	{
		for (int y = 0; y < g_Blocks.size(); y++)
		{
			//�G�̉��t���O���I�t�̏ꍇ�̓X�L�b�v����
			if (g_Blocks[y][x] != 1) {
				continue;
			}

			//�q�b�g���Ă��邩�𔻒肷��
			D3DXVECTOR2 BlockPos = D3DXVECTOR2(x * BLOCK_SIZE, y * BLOCK_SIZE);

			if (HitCheckBox_Block(BlockPos, BLOCK_SIZE, BLOCK_SIZE, vPlayerPos, pPlayer->size, pPlayer->size))
			{
				//�����ɓ������Ă��钆�ň�ԋ߂��u���b�N��T��
				D3DXVECTOR2 vLength = BlockPos - vPlayerPos;
				float length = D3DXVec2Length(&(vLength));
				if (block_length > length)
				{
					//���߂��u���b�N�̏��ɍX�V����
					block_length = length;
					block_index[0] = x;
					block_index[1] = y;
				}
			}
		}
	}

	//�u���b�N�̃q�b�g���������𒲂ׂ�
	if (block_length != 99999)
	{
		D3DXVECTOR2 vY(0.0f, -1.0f);

		//�u���b�N�̍��W(���S)
		D3DXVECTOR2 BlockPos = D3DXVECTOR2((block_index[0] * BLOCK_SIZE) + 30.0f, (block_index[1] * BLOCK_SIZE) + 30.0f);

		D3DXVECTOR2 vDist = vPlayerPos - BlockPos;
		D3DXVec2Normalize(&vDist, &vDist);

		//�q�b�g�����̔���
		float hit_vartical = D3DXVec2Dot(&vY, &vDist);

		if (hit_vartical < 0) {
			result |= HIT_DOWN;
		}
		else if (hit_vartical > 0) {
			result |= HIT_UP;
		}

		D3DXVECTOR2 vX(-1.0f, 0.0f);
		float hit_horizontal = D3DXVec2Dot(&vX, &vDist);
		if (hit_horizontal < 0) {
			result |= HIT_RIGHT;
		}
		else if (hit_horizontal > 0) {
			result |= HIT_LEFT;
		}

		//�u���b�N�̏���W���X�V����
		g_Block_Height = BlockPos.y - 30.0f;
	}

	return result;
}

DWORD HitChackEnemy_Block(D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 moveVec)
{
	DWORD result = 0;
	//�v���C���[�̈ړ�����W���쐬����
	D3DXVECTOR2 vEnemyPos = pos + moveVec;

	float block_length = 99999;//��ԋ߂��u���b�N�̋���
	int block_index[2]{ -99999,-99999 };//1��ԋ߂��u���b�N�̓Y����

	//�G�o�b�t�@�̂��ׂĂ��`�F�b�N����
	for (int x = 0; x < g_Blocks[0].size(); x++)
	{
		for (int y = 0; y < g_Blocks.size(); y++)
		{
			//�G�̉��t���O���I�t�̏ꍇ�̓X�L�b�v����
			if (g_Blocks[y][x] != 1)
			{
				continue;
			}

			//�q�b�g���Ă��邩�𔻒肷��
			D3DXVECTOR2 BlockPos = D3DXVECTOR2(x * BLOCK_SIZE, y * BLOCK_SIZE);

			if (HitCheckBox_Block(BlockPos, BLOCK_SIZE, BLOCK_SIZE, vEnemyPos, size.x,size.y))
			{
				//�����ɓ������Ă��钆�ň�ԋ߂��u���b�N��T��
				D3DXVECTOR2 vLength = BlockPos - vEnemyPos;
				float length = D3DXVec2Length(&(vLength));
				if (block_length > length)
				{
					//���߂��u���b�N�̏��ɍX�V����
					block_length = length;
					block_index[0] = x;
					block_index[1] = y;
				}
			}
		}
	}

	//�u���b�N�̃q�b�g���������𒲂ׂ�
	if (block_length != 99999)
	{
		D3DXVECTOR2 vY(0.0f, -1.0f);

		//�u���b�N�̍��W(���S)
		D3DXVECTOR2 BlockPos = D3DXVECTOR2((block_index[0] * BLOCK_SIZE) + 30.0f, (block_index[1] * BLOCK_SIZE) + 30.0f);

		D3DXVECTOR2 vDist = vEnemyPos - BlockPos;
		D3DXVec2Normalize(&vDist, &vDist);

		//�q�b�g�����̔���
		float hit_vartical = D3DXVec2Dot(&vY, &vDist);

		if (hit_vartical < 0)
		{
			result |= HIT_DOWN;
		}
		else if (hit_vartical > 0)
		{
			result |= HIT_UP;
		}

		D3DXVECTOR2 vX(-1.0f, 0.0f);
		float hit_horizontal = D3DXVec2Dot(&vX, &vDist);
		if (hit_horizontal <= 0)
		{
			result |= HIT_RIGHT;
		}
		else if (hit_horizontal > 0)
		{
			result |= HIT_LEFT;
		}

		//�u���b�N�̏���W���X�V����
		g_Block_Height = BlockPos.y - 30.0f;
	}

	return result;
}


float GetBlockHeight(void)
{
	return g_Block_Height;
}

void hitChackWarpPlayer_Block(D3DXVECTOR2 move)
{
	PLAYER* pPlayer = GetPlayer();

	bool isHit = false; //�q�b�g�m�F�t���O

	int NumHit = 0;	//�q�b�g��

	// �����蔻�胋�[�v
	do
	{
		isHit = false; //�q�b�g�m�F�t���O�̏�����

		// �v���C���[�̈ړ��x�N�g��
		D3DXVECTOR2 pVec = pPlayer->pos - pPlayer->oldpos;

		// �v���C���[�̊e���_�̍��W�̔z��
		D3DXVECTOR2 playerVertices[4] =
		{
			D3DXVECTOR2(pPlayer->pos.x - pPlayer->size / 2, pPlayer->pos.y - pPlayer->size / 2), // ����
			D3DXVECTOR2(pPlayer->pos.x + pPlayer->size / 2, pPlayer->pos.y - pPlayer->size / 2), // �E��
			D3DXVECTOR2(pPlayer->pos.x - pPlayer->size / 2, pPlayer->pos.y + pPlayer->size / 2), // ����
			D3DXVECTOR2(pPlayer->pos.x + pPlayer->size / 2, pPlayer->pos.y + pPlayer->size / 2), // �E��

		};
		// 1�t���[���O�̃v���C���[�̊e���_�̍��W�̔z��
		D3DXVECTOR2 oldPlayerVertices[4] =
		{
			D3DXVECTOR2(pPlayer->oldpos.x - pPlayer->size / 2, pPlayer->oldpos.y - pPlayer->size / 2), // ����
			D3DXVECTOR2(pPlayer->oldpos.x + pPlayer->size / 2, pPlayer->oldpos.y - pPlayer->size / 2), // �E��
			D3DXVECTOR2(pPlayer->oldpos.x - pPlayer->size / 2, pPlayer->oldpos.y + pPlayer->size / 2), // ����
			D3DXVECTOR2(pPlayer->oldpos.x + pPlayer->size / 2, pPlayer->oldpos.y + pPlayer->size / 2), // �E��

		};

		// �v���C���[�̈ړ��x�N�g���̕���
		// 0 : �����ĂȂ�
		// 1 : �^��
		// 2 : �^��
		// 3 : �^��
		// 4 : �^�E
		// 5 : ����
		// 6 : �E��
		// 7 : ����
		// 8 : �E��
		int playerVecDirection = -1;

		D3DXVECTOR2 LookUpPos[3][2];
		//LookUpPos�̏�����
		LookUpPos[0][0] = D3DXVECTOR2(0.0f, 0.0f);
		LookUpPos[0][1] = D3DXVECTOR2(0.0f, 0.0f);
		LookUpPos[1][0] = D3DXVECTOR2(0.0f, 0.0f);
		LookUpPos[1][1] = D3DXVECTOR2(0.0f, 0.0f);
		LookUpPos[2][0] = D3DXVECTOR2(0.0f, 0.0f);
		LookUpPos[2][1] = D3DXVECTOR2(0.0f, 0.0f);

		// �v���C���[�̈ړ��x�N�g���̕��������߂�
		// ��������Q�Ƃ���x�N�g�������߂�
		if (pVec.x == 0.0f&&pVec.y == 0.0f)
		{
			playerVecDirection = 0;		// �����ĂȂ�
		}
		else if (pVec.x == 0.0f&&pVec.y < 0.0f)
		{
			playerVecDirection = 1;		// �^��

			LookUpPos[0][0] = playerVertices[0];
			LookUpPos[0][1] = oldPlayerVertices[0];
			LookUpPos[1][0] = playerVertices[1];
			LookUpPos[1][1] = oldPlayerVertices[1];
		}
		else if (pVec.x == 0.0f&&pVec.y > 0.0f)
		{
			playerVecDirection = 2;		// �^��

			LookUpPos[0][0] = playerVertices[2];
			LookUpPos[0][1] = oldPlayerVertices[2];
			LookUpPos[1][0] = playerVertices[3];
			LookUpPos[1][1] = oldPlayerVertices[3];
		}
		else if (pVec.x < 0.0f&&pVec.y == 0.0f)
		{
			playerVecDirection = 3;		// �^��

			LookUpPos[0][0] = playerVertices[0];
			LookUpPos[0][1] = oldPlayerVertices[0];
			LookUpPos[1][0] = playerVertices[2];
			LookUpPos[1][1] = oldPlayerVertices[2];
		}
		else if (pVec.x > 0.0f&&pVec.y == 0.0f)
		{
			playerVecDirection = 4;		// �^�E

			LookUpPos[0][0] = playerVertices[1];
			LookUpPos[0][1] = oldPlayerVertices[1];
			LookUpPos[1][0] = playerVertices[3];
			LookUpPos[1][1] = oldPlayerVertices[3];
		}
		else if (pVec.x < 0.0f&&pVec.y < 0.0f)
		{
			playerVecDirection = 5;		// ����

			LookUpPos[0][0] = playerVertices[0];
			LookUpPos[0][1] = oldPlayerVertices[0];
		}
		else if (pVec.x > 0.0f&&pVec.y < 0.0f)
		{
			playerVecDirection = 6;		// �E��

			LookUpPos[0][0] = playerVertices[1];
			LookUpPos[0][1] = oldPlayerVertices[1];
		}
		else if (pVec.x < 0.0f&&pVec.y > 0.0f)
		{
			playerVecDirection = 7;		// �E��

			LookUpPos[0][0] = playerVertices[2];
			LookUpPos[0][1] = oldPlayerVertices[2];
		}
		else if (pVec.x > 0.0f&&pVec.y > 0.0f)
		{
			playerVecDirection = 8;		// ����

			LookUpPos[0][0] = playerVertices[3];
			LookUpPos[0][1] = oldPlayerVertices[3];
		}


		// �v���C���[�̈ړ��x�N�g���ƃu���b�N�̂S�ӂ̌�_�̔z��
		std::vector<D3DXVECTOR2> crossPoints;
		std::vector<int> hitblockX;
		std::vector<int> hitblockY;
		std::vector<int> dore;

		// �v���C���[�Ƃ̓����蔻��
		for (int y = 0; y < g_Blocks.size(); y++)
		{
			for (int x = 0; x < g_Blocks[0].size(); x++)
			{
				// �u���b�N���Ȃ��Ȃ�X�L�b�v
				if (g_Blocks[y][x] == 0)
				{
					continue;
				}

				if (HitCheckBox_Block(
					D3DXVECTOR2(BLOCK_SIZE * x, BLOCK_SIZE * y),
					BLOCK_SIZE, BLOCK_SIZE,
					pPlayer->pos, 120.0f, 120.0f))
				{
					isHit = true;

					hitblockX.push_back(x);
					hitblockY.push_back(y);
					// �S�ӂ̂Q�_���i�[
					D3DXVECTOR2 blockPoints[4][2] =
					{
						// �J�n�_                                                               �I���_
						{D3DXVECTOR2(BLOCK_SIZE * x,              BLOCK_SIZE * y),D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE, BLOCK_SIZE * y)}, // ��
						{D3DXVECTOR2(BLOCK_SIZE * x,              BLOCK_SIZE * y + BLOCK_SIZE),D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE, BLOCK_SIZE * y + BLOCK_SIZE)}, // ��
						{D3DXVECTOR2(BLOCK_SIZE * x,              BLOCK_SIZE * y),D3DXVECTOR2(BLOCK_SIZE * x             , BLOCK_SIZE * y + BLOCK_SIZE)}, // ��
						{D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE, BLOCK_SIZE * y),D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE, BLOCK_SIZE * y + BLOCK_SIZE)}, // �E
					};

					for (int i = 0; i < 2; i++)// �Q�ƃx�N�g���������[�v
					{
						for (int j = 0; j < 4; j++)// �������Ă���{�b�N�X�̕ӕ����[�v
						{
							if (HitCheckLine(LookUpPos[i][0], LookUpPos[i][1], blockPoints[j][0], blockPoints[j][1]))
							{
								// ��_��z��Ɋi�[
								crossPoints.push_back(GetCrossPoint(LookUpPos[i][0], LookUpPos[i][1], blockPoints[j][0], blockPoints[j][1]));
							}
						}
					}
				}
			}
		}

		// ��ԉ����x�N�g���̒���
		float farLength = -99999.0f;
		D3DXVECTOR2 farVec = D3DXVECTOR2(0.0f, 0.0f);
		bool isDore = false;

		if (isHit)
		{
			NumHit++;

			if (NumHit > 10)
			{
				pPlayer->pos = pPlayer->oldpos;

				isHit = false;
			}
			else if (crossPoints.size() != 0)
			{
				// �Q�ƃx�N�g�������ԃv���C���[�ɉ�����_��T��
				for (size_t i = 0; i < crossPoints.size(); i++)
				{
					// ���s�Ɍ�����Ă���ꍇ�͏��O
					if (crossPoints[i].y == -99999.0f)
					{
						continue;
					}

					D3DXVECTOR2 vec = crossPoints[i] - LookUpPos[0][0];

					float len = D3DXVec2Length(&vec);
					if (len > farLength)
					{
						farLength = len;
						farVec = vec;
					}
				}
			}
			//�Q�ƃx�N�g���Ɍ�_�����������ꍇ
			else
			{
				std::vector<D3DXVECTOR2> kouten;

				for (int x = 0; x < hitblockX.size(); x++)
				{
					for (int i = 0; i < 4; i++)// 
					{
						for (int j = 0; j < 4; j++)// �������Ă���{�b�N�X�̕ӕ����[�v
						{
							D3DXVECTOR2 blockPoints[4][2] =
							{
								// �J�n�_                                                                                       �I���_
								{D3DXVECTOR2(BLOCK_SIZE * hitblockX[x],              BLOCK_SIZE * hitblockY[x]),				D3DXVECTOR2(BLOCK_SIZE * hitblockX[x] + BLOCK_SIZE, BLOCK_SIZE * hitblockY[x])}, // ��
								{D3DXVECTOR2(BLOCK_SIZE * hitblockX[x],              BLOCK_SIZE * hitblockY[x] + BLOCK_SIZE),	D3DXVECTOR2(BLOCK_SIZE * hitblockX[x] + BLOCK_SIZE, BLOCK_SIZE * hitblockY[x] + BLOCK_SIZE)}, // ��
								{D3DXVECTOR2(BLOCK_SIZE * hitblockX[x],              BLOCK_SIZE * hitblockY[x]),				D3DXVECTOR2(BLOCK_SIZE * hitblockX[x]             , BLOCK_SIZE * hitblockY[x] + BLOCK_SIZE)}, // ��
								{D3DXVECTOR2(BLOCK_SIZE * hitblockX[x] + BLOCK_SIZE, BLOCK_SIZE * hitblockY[x]),				D3DXVECTOR2(BLOCK_SIZE * hitblockX[x] + BLOCK_SIZE, BLOCK_SIZE * hitblockY[x] + BLOCK_SIZE)}, // �E
							};

							if (HitCheckLine(pPlayer->pos, playerVertices[i], blockPoints[j][0], blockPoints[j][1]))
							{
								// ��_��z��Ɋi�[
								kouten.push_back(GetCrossPoint(pPlayer->pos, playerVertices[i], blockPoints[j][0], blockPoints[j][1]));
								dore.push_back(i);
							}
						}
					}
				}

				for (int x = 0; x < kouten.size(); x++)
				{
					D3DXVECTOR2 vec = kouten[x] - playerVertices[dore[x]];

					float len = D3DXVec2Length(&vec);
					if (len > farLength)
					{
						farLength = len;
						farVec = vec;
					}
				}
			}
		}

		switch (playerVecDirection)
		{
		case 0:
			farVec.x = 0.0f;
			farVec.y = 0.0f;
			break;
		case 1:
			farVec.x = 0.0f;
			break;
		case 2:
			farVec.x = 0.0f;
			break;
		case 3:
			farVec.y = 0.0f;
			break;
		case 4:
			farVec.y = 0.0f;
			break;
		default:
			break;
		}

		AdjustPlayer(farVec);

	} while (isHit);

}


void SetVertexBlock(float x, float y, float width, float height,
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

bool HitCheckBox_Block(D3DXVECTOR2 box1pos, float box1width, float box1height,
	D3DXVECTOR2 box2pos, float box2width, float box2height)
{
	float box1Xmin = box1pos.x;
	float box1Xmax = box1pos.x + box1width;
	float box1Ymin = box1pos.y;
	float box1Ymax = box1pos.y + box1height;

	float box2Xmin = box2pos.x - (box2width / 2);
	float box2Xmax = box2pos.x + (box2width / 2);
	float box2Ymin = box2pos.y - (box2height / 2);
	float box2Ymax = box2pos.y + (box2height / 2);

	if (box1Xmin < box2Xmax)
	{
		if (box1Xmax > box2Xmin)
		{
			if (box1Ymin < box2Ymax)
			{
				if (box1Ymax > box2Ymin)
				{
					return true;
				}
			}
		}
	}

	return false;
}

// ���Ɛ��̓����蔻��
bool HitCheckLine(D3DXVECTOR2 A, D3DXVECTOR2 B, D3DXVECTOR2 C, D3DXVECTOR2 D)
{
	// �Q�̐����̃x�N�g����AB��CD�Ƃ���
	// �R�̃x�N�g������Ȃ��̃O���[�v���쐬

	// �O���[�v�P
	D3DXVECTOR2 AB = B - A;
	D3DXVECTOR2 AC = C - A;
	D3DXVECTOR2 AD = D - A;

	// �O���[�v�Q
	D3DXVECTOR2 CD = D - C;
	D3DXVECTOR2 CA = A - C;
	D3DXVECTOR2 CB = B - C;

	// �����A�����̑g�ݍ��킹�Ȃ瓖�����Ă���
	float cross_AB_AC = Cross(AB, AC);
	float cross_AB_AD = Cross(AB, AD);
	if (cross_AB_AC*cross_AB_AD > 0.0f)
	{
		return false;
	}

	// �����A�����̑g�ݍ��킹�Ȃ瓖�����Ă���
	float cross_CD_CA = Cross(CD, CA);
	float cross_CD_CB = Cross(CD, CB);
	if (cross_CD_CA*cross_CD_CB > 0.0f)
	{
		return false;

	}
	return true;
}

// ��_�����߂�֐�
D3DXVECTOR2 GetCrossPoint(D3DXVECTOR2 A, D3DXVECTOR2 B, D3DXVECTOR2 C, D3DXVECTOR2 D)
{
	// ���W����x�N�g���𐶐�
	D3DXVECTOR2 v =  C - A;
	D3DXVECTOR2 v1 = B - A;
	D3DXVECTOR2 v2 = D - C;

	//  Cross(v,v1)
	// -------------
	//  Cross(v1,v2)
	float cross_v_v1 = Cross(v, v1);
	float cross_v1_v2 = Cross(v1, v2);
	if (cross_v_v1 == 0.0f&&cross_v1_v2 == 0.0f){
		return D3DXVECTOR2(-99999.0f, -99999.0f);
	}
	float t2 = Cross(v, v1) / Cross(v1, v2);


	// ��_
	D3DXVECTOR2 crossPos = C;
	crossPos.x += v2.x*t2;
	crossPos.y += v2.y*t2;

	return crossPos;
}

// �O��
float Cross(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2)
{
	return (vec1.x*vec2.y) - (vec2.x*vec1.y);
}

std::vector<std::vector<int>> GetBlocks()
{
	return g_Blocks;
}


