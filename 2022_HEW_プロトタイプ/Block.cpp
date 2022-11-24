/*==============================================================================

   ステージブロック管理 [bg.cpp]
														 Author :　小川渚紗
														 Date   :  2022/11/19
--------------------------------------------------------------------------------

==============================================================================*/
#include <vector>
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
// 線と線の当たり判定
bool HitCheckLine(D3DXVECTOR2 A, D3DXVECTOR2 B, D3DXVECTOR2 C, D3DXVECTOR2 D);
// 線と線の交点を求める関数
D3DXVECTOR2 GetCrossPoint(D3DXVECTOR2 A, D3DXVECTOR2 B, D3DXVECTOR2 C, D3DXVECTOR2 D);
// 外積
float Cross(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2);


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
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
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
	//プレイヤー座標を受け取る
	PLAYER* pPlayer = GetPlayer();
	bool isHit = false;
	do
	{
		// プレイヤーの移動ベクトルとブロックの４辺の交点の配列
		std::vector<D3DXVECTOR2> crossPoints;
		std::vector<int> dore;
		// プレイヤーの移動ベクトル
		D3DXVECTOR2 pVec = pPlayer->pos - pPlayer->oldpos;
		// プレイヤーの各頂点の座標の配列
		D3DXVECTOR2 playerVertices[4] =
		{
			D3DXVECTOR2(pPlayer->pos.x - pPlayer->size / 2, pPlayer->pos.y - pPlayer->size / 2), // 左上
			D3DXVECTOR2(pPlayer->pos.x + pPlayer->size / 2, pPlayer->pos.y - pPlayer->size / 2), // 右上
			D3DXVECTOR2(pPlayer->pos.x - pPlayer->size / 2, pPlayer->pos.y + pPlayer->size / 2), // 左下
			D3DXVECTOR2(pPlayer->pos.x + pPlayer->size / 2, pPlayer->pos.y + pPlayer->size / 2), // 右下

		};
		// 1フレーム前のプレイヤーの各頂点の座標の配列
		D3DXVECTOR2 oldPlayerVertices[4] =
		{
			D3DXVECTOR2(pPlayer->oldpos.x - pPlayer->size / 2, pPlayer->oldpos.y - pPlayer->size / 2), // 左上
			D3DXVECTOR2(pPlayer->oldpos.x + pPlayer->size / 2, pPlayer->oldpos.y - pPlayer->size / 2), // 右上
			D3DXVECTOR2(pPlayer->oldpos.x - pPlayer->size / 2, pPlayer->oldpos.y + pPlayer->size / 2), // 左下
			D3DXVECTOR2(pPlayer->oldpos.x + pPlayer->size / 2, pPlayer->oldpos.y + pPlayer->size / 2), // 右下

		};
		isHit = false;
		// プレイヤーとの当たり判定
		for (int y = 0; y < STAGE_HEIGHT; y++)
		{
			for (int x = 0; x < STAGE_WIDTH; x++)
			{
				// ブロックがないならスキップ
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
					// ４辺の２点を格納
					D3DXVECTOR2 blockPoints[4][2] =
					{
						// 開始点                                                               終了点
						{D3DXVECTOR2(BLOCK_SIZE * x,              BLOCK_SIZE * y),D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE, BLOCK_SIZE * y)}, // 上
						{D3DXVECTOR2(BLOCK_SIZE * x,              BLOCK_SIZE * y + BLOCK_SIZE),D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE, BLOCK_SIZE * y + BLOCK_SIZE)}, // 下
						{D3DXVECTOR2(BLOCK_SIZE * x,              BLOCK_SIZE * y),D3DXVECTOR2(BLOCK_SIZE * x             , BLOCK_SIZE * y + BLOCK_SIZE)}, // 左
						{D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE, BLOCK_SIZE * y),D3DXVECTOR2(BLOCK_SIZE * x + BLOCK_SIZE, BLOCK_SIZE * y + BLOCK_SIZE)}, // 右
					};
					// プレイヤーの頂点数分ループ
					for (int i = 0; i < 4; i++)
					{
						// 当たった辺との交点を求める
						for (int j = 0; j < 4; j++)
						{
							if (HitCheckLine(oldPlayerVertices[i], playerVertices[i], blockPoints[j][0], blockPoints[j][1]))
							{
								// 交点を配列に格納
								crossPoints.push_back(GetCrossPoint(oldPlayerVertices[i], playerVertices[i], blockPoints[j][0], blockPoints[j][1]));
								dore.push_back(i);
							}
						}
					}
				}
			}
		}
		D3DXVECTOR2 moveVec = D3DXVECTOR2(0.0f, 0.0f);
		// 0 : 動いてない
		// 1 : 真上
		// 2 : 真下
		// 3 : 真左
		// 4 : 真右
		// 5 : 左上
		// 6 : 右上
		// 7 : 左下
		// 8 : 右下
		int playerVecDirection = -1;

		// プレイヤーの移動ベクトルの方向を決める
		if (pVec.x == 0.0f&&pVec.y == 0.0f)
		{
			playerVecDirection = 0;		// 動いてない
		}
		else if (pVec.x == 0.0f&&pVec.y < 0.0f)
		{
			playerVecDirection = 1;		// 真上
		}
		else if (pVec.x == 0.0f&&pVec.y > 0.0f)
		{
			playerVecDirection = 2;		// 真下
		}
		else if (pVec.x < 0.0f&&pVec.y == 0.0f)
		{
			playerVecDirection = 3;		// 真左
		}
		else if (pVec.x > 0.0f&&pVec.y == 0.0f)
		{
			playerVecDirection = 4;		// 真右
		}
		else if (pVec.x < 0.0f&&pVec.y < 0.0f)
		{
			playerVecDirection = 5;		// 左上
		}
		else if (pVec.x > 0.0f&&pVec.y < 0.0f)
		{
			playerVecDirection = 6;		// 右上
		}
		else if (pVec.x < 0.0f&&pVec.y > 0.0f)
		{
			playerVecDirection = 7;		// 左下
		}
		else if (pVec.x > 0.0f&&pVec.y > 0.0f)
		{
			playerVecDirection = 8;		// 右下
		}

		// 一番遠いベクトルの長さ
		float farLength = -99999.0f;
		D3DXVECTOR2 farVec = D3DXVECTOR2(0.0f, 0.0f);
		bool isDore = false;
		switch (playerVecDirection)
		{
		case 0:	// 動いてない
			break;
		case 1:	// 真上
			// 全ての交点の中から一番プレイヤーに遠い交点を探す
			for (size_t i = 0; i < crossPoints.size(); i++)
			{
				// 平行に交わっている場合は除外
				if (crossPoints[i].y == -99999.0f)
				{
					continue;
				}
				D3DXVECTOR2 vec = crossPoints[i] - playerVertices[0];
				vec.x = 0.0f;
				float len = D3DXVec2Length(&vec);
				if (len > farLength)
				{
					farLength = len;
					farVec = vec;
				}
			}
			farVec.y += 1.0f;
			break;
		case 2:	// 真下
			// 全ての交点の中から一番プレイヤーに遠い交点を探す
			for (size_t i = 0; i < crossPoints.size(); i++)
			{
				// 平行に交わっている場合は除外
				if (crossPoints[i].y == -99999.0f)
				{
					continue;
				}
				D3DXVECTOR2 vec = crossPoints[i] - playerVertices[2];
				vec.x = 0.0f;
				float len = D3DXVec2Length(&vec);
				if (len > farLength)
				{
					farLength = len;
					farVec = vec;
				}
			}
			farVec.y -= 1.0f;
			break;
		case 3:	// 真左
			// 全ての交点の中から一番プレイヤーに遠い交点を探す
			for (size_t i = 0; i < crossPoints.size(); i++)
			{
				// 平行に交わっている場合は除外
				if (crossPoints[i].y == -99999.0f)
				{
					continue;
				}
				D3DXVECTOR2 vec = crossPoints[i] - playerVertices[0];
				vec.y = 0.0f;
				float len = D3DXVec2Length(&vec);
				if (len > farLength)
				{
					farLength = len;
					farVec = vec;
				}
			}
			farVec.x += 1.0f;
			break;
		case 4:	// 真右
			// 全ての交点の中から一番プレイヤーに遠い交点を探す
			for (size_t i = 0; i < crossPoints.size(); i++)
			{
				// 平行に交わっている場合は除外
				if (crossPoints[i].y == -99999.0f)
				{
					continue;
				}
				D3DXVECTOR2 vec = crossPoints[i] - playerVertices[1];
				vec.y = 0.0f;
				float len = D3DXVec2Length(&vec);
				if (len > farLength)
				{
					farLength = len;
					farVec = vec;
				}
			}
			farVec.x -= 1.0f;
			break;
		case 5:	// 左上
			// 基準のプレイヤーのベクトルとの交点があるかチェック
			for (size_t i = 0; i < crossPoints.size(); i++)
			{
				if (dore[i] == 0)
				{
					isDore = true;
				}
			}
			if (isDore)
			{
				// 全ての交点の中から一番プレイヤーに遠い交点を探す
				for (size_t i = 0; i < crossPoints.size(); i++)
				{
					// 平行に交わっている場合は除外
					if (crossPoints[i].y == -99999.0f)
					{
						continue;
					}
					if (dore[i] == 0)
					{
						D3DXVECTOR2 vec = crossPoints[i] - playerVertices[0];
						float len = D3DXVec2Length(&vec);
						if (len > farLength)
						{
							farLength = len;
							farVec = vec;
						}
						farVec.x += 1.0f;
						farVec.y += 1.0f;
					}
				}
			}
			else
			{
				// 全ての交点の中から一番プレイヤーに遠い交点を探す
				for (size_t i = 0; i < crossPoints.size(); i++)
				{
					// 平行に交わっている場合は除外
					if (crossPoints[i].y == -99999.0f)
					{
						continue;
					}
					D3DXVECTOR2 vec = crossPoints[i] - playerVertices[0];
					float len = D3DXVec2Length(&vec);
					if (len > farLength)
					{
						farLength = len;
						farVec = vec;
					}
					farVec.x += 1.0f;
					farVec.y += 1.0f;
				}
			}

			break;
		case 6:	// 右上
			// 基準のプレイヤーのベクトルとの交点があるかチェック
			for (size_t i = 0; i < crossPoints.size(); i++)
			{
				if (dore[i] == 1)
				{
					isDore = true;
				}
			}
			if (isDore)
			{
				// 全ての交点の中から一番プレイヤーに遠い交点を探す
				for (size_t i = 0; i < crossPoints.size(); i++)
				{
					// 平行に交わっている場合は除外
					if (crossPoints[i].y == -99999.0f)
					{
						continue;
					}
					if (dore[i] == 1)
					{
						D3DXVECTOR2 vec = crossPoints[i] - playerVertices[1];
						float len = D3DXVec2Length(&vec);
						if (len > farLength)
						{
							farLength = len;
							farVec = vec;
						}
						farVec.x -= 1.0f;
						farVec.y += 1.0f;
					}
				}
			}
			else
			{
				// 全ての交点の中から一番プレイヤーに遠い交点を探す
				for (size_t i = 0; i < crossPoints.size(); i++)
				{
					// 平行に交わっている場合は除外
					if (crossPoints[i].y == -99999.0f)
					{
						continue;
					}
					D3DXVECTOR2 vec = crossPoints[i] - playerVertices[1];
					float len = D3DXVec2Length(&vec);
					if (len > farLength)
					{
						farLength = len;
						farVec = vec;
					}
					farVec.x -= 1.0f;
					farVec.y += 1.0f;
				}
			}
			break;
		case 7: // 左下
			// 基準のプレイヤーのベクトルとの交点があるかチェック
			for (size_t i = 0; i < crossPoints.size(); i++)
			{
				if (dore[i] == 2)
				{
					isDore = true;
				}
			}
			if (isDore)
			{
				// 全ての交点の中から一番プレイヤーに遠い交点を探す
				for (size_t i = 0; i < crossPoints.size(); i++)
				{
					// 平行に交わっている場合は除外
					if (crossPoints[i].y == -99999.0f)
					{
						continue;
					}
					if (dore[i] == 2)
					{
						D3DXVECTOR2 vec = crossPoints[i] - playerVertices[2];
						float len = D3DXVec2Length(&vec);
						if (len > farLength)
						{
							farLength = len;
							farVec = vec;
						}
						farVec.x += 1.0f;
						farVec.y -= 1.0f;
					}
				}
			}
			else
			{
				// 全ての交点の中から一番プレイヤーに遠い交点を探す
				for (size_t i = 0; i < crossPoints.size(); i++)
				{
					// 平行に交わっている場合は除外
					if (crossPoints[i].y == -99999.0f)
					{
						continue;
					}
					D3DXVECTOR2 vec = crossPoints[i] - playerVertices[2];
					float len = D3DXVec2Length(&vec);
					if (len > farLength)
					{
						farLength = len;
						farVec = vec;
					}
					farVec.x += 1.0f;
					farVec.y -= 1.0f;
				}
			}
			break;
		case 8: // 右下
			// 基準のプレイヤーのベクトルとの交点があるかチェック
			for (size_t i = 0; i < crossPoints.size(); i++)
			{
				if (dore[i] == 3)
				{
					isDore = true;
				}
			}
			if (isDore)
			{
				// 全ての交点の中から一番プレイヤーに遠い交点を探す
				for (size_t i = 0; i < crossPoints.size(); i++)
				{
					// 平行に交わっている場合は除外
					if (crossPoints[i].y == -99999.0f)
					{
						continue;
					}
					if (dore[i] == 3)
					{
						D3DXVECTOR2 vec = crossPoints[i] - playerVertices[3];
						float len = D3DXVec2Length(&vec);
						if (len > farLength)
						{
							farLength = len;
							farVec = vec;
						}
						farVec.x -= 1.0f;
						farVec.y -= 1.0f;
					}
				}
			}
			else
			{
				// 全ての交点の中から一番プレイヤーに遠い交点を探す
				for (size_t i = 0; i < crossPoints.size(); i++)
				{
					// 平行に交わっている場合は除外
					if (crossPoints[i].y == -99999.0f)
					{
						continue;
					}
					D3DXVECTOR2 vec = crossPoints[i] - playerVertices[3];
					float len = D3DXVec2Length(&vec);
					if (len > farLength)
					{
						farLength = len;
						farVec = vec;
					}
					farVec.x -= 1.0f;
					farVec.y -= 1.0f;
				}
			}
			break;
		default:
			break;
		}

		AdjustPlayer(farVec);
	}while (isHit == true);
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

// 線と線の当たり判定
bool HitCheckLine(D3DXVECTOR2 A, D3DXVECTOR2 B, D3DXVECTOR2 C, D3DXVECTOR2 D)
{
	// ２つの線分のベクトルをABとCDとする
	// ３つのベクトルからなる二つのグループを作成

	// グループ１
	D3DXVECTOR2 AB = B - A;
	D3DXVECTOR2 AC = C - A;
	D3DXVECTOR2 AD = D - A;

	// グループ２
	D3DXVECTOR2 CD = D - C;
	D3DXVECTOR2 CA = A - C;
	D3DXVECTOR2 CB = B - C;

	// 正負、負正の組み合わせなら当たっている
	float cross_AB_AC = Cross(AB, AC);
	float cross_AB_AD = Cross(AB, AD);
	if (cross_AB_AC*cross_AB_AD > 0.0f)
	{
		return false;
	}

	// 正負、負正の組み合わせなら当たっている
	float cross_CD_CA = Cross(CD, CA);
	float cross_CD_CB = Cross(CD, CB);
	if (cross_CD_CA*cross_CD_CB > 0.0f)
	{
		return false;

	}
	return true;
}

// 交点を求める関数
D3DXVECTOR2 GetCrossPoint(D3DXVECTOR2 A, D3DXVECTOR2 B, D3DXVECTOR2 C, D3DXVECTOR2 D)
{
	// 座標からベクトルを生成
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


	// 交点
	D3DXVECTOR2 crossPos = C;
	crossPos.x += v2.x*t2;
	crossPos.y += v2.y*t2;

	return crossPos;
}

// 外積
float Cross(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2)
{
	return (vec1.x*vec2.y) - (vec2.x*vec1.y);
}



