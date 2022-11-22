/*==============================================================================

   ステージブロック管理 [bg.cpp]
														 Author :　小川渚紗
														 Date   :  2022/11/19
--------------------------------------------------------------------------------

==============================================================================*/
#include "Block.h"
#include "camera.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX 4	//必要な頂点の数

#define DIVIDE_X 8	//横の分割数
#define DIVIDE_Y 8	//縦の分割数

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetVertexBlock(float x, float y, float width, float height,
	float u, float v, float uw, float vh);
bool HitCheckBox_Block(D3DXVECTOR2 box1pos, float box1width, float box1height,
	D3DXVECTOR2 box2pos, float box2width, float box2height);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView	*g_Texture = NULL;		// テクスチャ情報

static char *g_TextureName = (char*)"data/TEXTURE/wall_03.png";

// 0 : ブロックなし
// 1 : ブロックあり
static int g_Blocks[STAGE_HEIGHT][STAGE_WIDTH] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
};

//=============================================================================
// 初期化処理
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

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//データの初期化

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlock(void)
{
	// テクスチャの解放
	if (g_Texture)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}

	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock(void)
{
	//ベース座標を受け取る
	D3DXVECTOR2 basePos = GetBase();
	//プレイヤー座標を受け取る
	PLAYER* pPlayer = GetPlayer();
	// プレイヤーとの当たり判定
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			// ブロックがないならスキップ
			if (g_Blocks[y][x] == 0){
				continue;
			}
			if (HitCheckBox_Block(
				D3DXVECTOR2(BLOCK_SIZE * x, BLOCK_SIZE * y),
				BLOCK_SIZE, BLOCK_SIZE,
				pPlayer->pos, 120.0f, 120.0f))
			{
				// 当たった
				pPlayer->pos = pPlayer->oldpos;

				//どれにあたったか？
			}
		}
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawBlock(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//テクスチャのセット
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	//ベース座標を受け取る
	D3DXVECTOR2 basePos = GetBase();

	//ステージの描画
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			// ブロックがないならスキップ
			if (g_Blocks[y][x] == 0){
				continue;
			}
			SetVertexBlock(basePos.x + BLOCK_SIZE*x, basePos.y + BLOCK_SIZE*y, BLOCK_SIZE, BLOCK_SIZE,
				0.0f,
				0.0f,
				1.0f,
				1.0f);

			// ポリゴン描画
			GetDeviceContext()->Draw(NUM_VERTEX, 0);
		}
	}
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

