/*==============================================================================

   頂点管理 [goal.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "goal.h"
#include "collision.h"
#include "player.h"
#include "camera.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX 4	//必要な頂点の数

#define DIVIDE_X 3	//横の分割数
#define DIVIDE_Y 4	//縦の分割数

#define PATTERN_WIDTH (1.0f / DIVIDE_X)
#define PATTERN_HEIGHT (1.0f / DIVIDE_Y)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetVertexGoal(float x, float y, float width, float height,
	float u, float v, float uw, float vh);

void ResetGoal(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBufferGL = NULL;	// 頂点情報
static ID3D11ShaderResourceView	*g_TextureGL = NULL;		// テクスチャ情報

static char *g_TextureNameGL = (char*)"data/TEXTURE/mapchip.png";

static GOAL g_Goal;

//=============================================================================
// 初期化処理
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

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBufferGL);

	//データの初期化
	g_Goal.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	g_Goal.animePtn = 0;
	g_Goal.animeCounter = 0;
	g_Goal.use = 1;

	ResetGoal();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGoal(void)
{
	// テクスチャの解放
	if (g_TextureGL)
	{
		g_TextureGL->Release();
		g_TextureGL = NULL;
	}

	// 頂点バッファの解放
	if (g_VertexBufferGL)
	{
		g_VertexBufferGL->Release();
		g_VertexBufferGL = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGoal(void)
{
	//アニメーションカウンターをカウントアップして、ウエイト値を超えたら
	if (g_Goal.animeCounter > 10)
	{
		//アニメーションパターンを切り替える
		g_Goal.animePtn++;
		//最後のアニメーションパターンを表示したらリセットする
		if (g_Goal.animePtn >= 4)
			g_Goal.animePtn = 0;

		//アニメーションカウンターのリセット
		g_Goal.animeCounter = 0;
	}
	g_Goal.animeCounter++;

	//player.cppにあるg_Playerのアドレスを受け取る
	PLAYER* pPlayer = GetPlayer();

	//プレイヤーとゴールの当たり判定
	if (HitCheckBox(pPlayer->pos, 64.0f, 64.0f,
		g_Goal.pos, 64.0f, 64.0f))
	{
		//新しいゴールを設定する
		ResetGoal();
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawGoal(void)
{
	//ベース座標を受け取る
	D3DXVECTOR2 basePos = GetBase();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBufferGL, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//テクスチャのセット
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureGL);

	//頂点バッファ更新
	SetVertexGoal(basePos.x + g_Goal.pos.x, basePos.y + g_Goal.pos.y, 64.0f, 64.0f,
		0.875f,
		0.75f,
		0.125f,
		0.125f);

	// ポリゴン描画
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
