/*==============================================================================

   頂点管理 [polygon.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "polygon.h"
#include "collision.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX 4	//必要な頂点の数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetVertex(float x, float y, float width, float height,
	float u, float v, float uw, float vh);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView	*g_Texture = NULL;		// テクスチャ情報
static ID3D11ShaderResourceView	*g_TextureEnemy = NULL;	// テクスチャ情報
static ID3D11ShaderResourceView	*g_TextureBG = NULL;	// テクスチャ情報
static ID3D11ShaderResourceView	*g_TextureRM = NULL;	// テクスチャ情報

static char *g_TextureName = (char*)"data/TEXTURE/bullet00.png";
static char *g_TextureNameEnemy = (char*)"data/TEXTURE/enemy00.png";
static char *g_TextureNameBG = (char*)"data/TEXTURE/IMG_0189.JPG";
static char *g_TextureNameRM = (char*)"data/TEXTURE/runningman002.png";



struct VERTEX_T{
	D3DXVECTOR2		pos;	//位置ベクトル
	D3DXVECTOR2		vel;	//速度ベクトル
	D3DXCOLOR		color;	//頂点カラー
	int				use;
};

static VERTEX_T g_Polygon;
static VERTEX_T g_Enemy;
static VERTEX_T g_RunningMan;

static int g_AnimePtn;
static int g_AnimeCounter;

//=============================================================================
// 初期化処理
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

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//初期化
	g_Polygon.pos.x = frand() * SCREEN_WIDTH;
	g_Polygon.pos.y = frand() * SCREEN_HEIGHT;

	g_Polygon.vel.x = frand() * 3 - 1.5f;
	g_Polygon.vel.y = frand() * 3 - 1.5f;

	g_Polygon.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


	//初期化
	g_Enemy.pos.x = frand() * SCREEN_WIDTH;//0.0～960.0の値のランダム値を作る
	g_Enemy.pos.y = frand() * SCREEN_HEIGHT;//0.0～540.0の値のランダム値を作る

	g_Enemy.vel.x = frand() * 3 - 1.5f;//-1.5～1.5の値のランダム値を作る
	g_Enemy.vel.y = frand() * 3 - 1.5f;//-1.5～1.5の値のランダム値を作る

	g_Enemy.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_Enemy.use = 1;


	//初期化
	g_RunningMan.pos.x = SCREEN_WIDTH / 2;//0.0～960.0の値のランダム値を作る
	g_RunningMan.pos.y = SCREEN_HEIGHT / 2;//0.0～540.0の値のランダム値を作る

	g_RunningMan.vel.x = frand() * 3 - 1.5f;//-1.5～1.5の値のランダム値を作る
	g_RunningMan.vel.y = frand() * 3 - 1.5f;//-1.5～1.5の値のランダム値を作る

	g_RunningMan.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_RunningMan.use = 1;


	g_AnimePtn = 0;

	// 頂点バッファ更新
//	SetVertex();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPolygon(void)
{
	// テクスチャの解放
	if (g_Texture)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}

	// テクスチャの解放
	if (g_TextureEnemy)
	{
		g_TextureEnemy->Release();
		g_TextureEnemy = NULL;
	}

	// テクスチャの解放
	if (g_TextureBG)
	{
		g_TextureBG->Release();
		g_TextureBG = NULL;
	}

	// テクスチャの解放
	if (g_TextureRM)
	{
		g_TextureRM->Release();
		g_TextureRM = NULL;
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
void UpdatePolygon(void)
{
	//位置ベクトルに速度ベクトルを足す
	g_Polygon.pos = g_Polygon.pos + g_Polygon.vel;

	//画面右端で跳ね返る
	if (SCREEN_WIDTH <= g_Polygon.pos.x)
		g_Polygon.vel.x *= -1;

	//画面下端で跳ね返る
	if (SCREEN_HEIGHT <= g_Polygon.pos.y)
		g_Polygon.vel.y *= -1;

	//画面上端で跳ね返る
	if (0.0f >= g_Polygon.pos.y)
		g_Polygon.vel.y *= -1;

	//画面左端で跳ね返る
	if (0.0f >= g_Polygon.pos.x)
		g_Polygon.vel.x *= -1;



	//全てのアップデートが終わったあとに当たり判定を行う
	if (HitCheckBox(g_Polygon.pos, 100.0f, 100.0f, g_Enemy.pos, 100.0f, 100.0f))
	{
		g_Enemy.use = 0;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPolygon(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//背景の描画

	//テクスチャのセット
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureBG);

	//頂点バッファ更新
//	SetVertex(480.0f, 270.0f, 960.0f, 540.0f);

	// ポリゴン描画
//	GetDeviceContext()->Draw(NUM_VERTEX, 0);


	//プレイヤーの描画

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	SetVertex(g_Polygon.pos.x, g_Polygon.pos.y, 100.0f, 100.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// ポリゴン描画
//	GetDeviceContext()->Draw(NUM_VERTEX, 0);


	//敵の描画

	if (g_Enemy.use == 1)
	{
		//テクスチャのセット
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureEnemy);

		//頂点バッファ更新
		SetVertex(g_Enemy.pos.x, g_Enemy.pos.y, 100.0f, 100.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
//		GetDeviceContext()->Draw(NUM_VERTEX, 0);
	}



	//ランニングマンの描画

	//テクスチャのセット
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureRM);

	//頂点バッファ更新
	SetVertex(g_RunningMan.pos.x, g_RunningMan.pos.y, 140.0f, 200.0f, 0.2f*g_AnimePtn, 0.0f, 0.2f, 0.5f);

	//アニメーションカウンターをカウントアップして、ウエイト値を超えたら
	if (g_AnimeCounter > 10)
	{
		//アニメーションパターンを切り替える
		g_AnimePtn++;
		//最後のアニメーションパターンを表示したらリセットする
		if (g_AnimePtn >= 5)
			g_AnimePtn = 0;

		//アニメーションカウンターのリセット
		g_AnimeCounter = 0;
	}
	g_AnimeCounter++;

	// ポリゴン描画
//	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


//=============================================================================
// 頂点データ設定
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
