/*==============================================================================

   敵の処理 [enemy.cpp]
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
// マクロ定義
//*****************************************************************************
#define ENEMY_SPEED (6.0f)	//敵の速度

////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//void EnemyState_Chase(int index);
//void EnemyState_Wait(int index);
//void EnemyState_Return(int index);
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//static int	g_TextureNo;		// テクスチャ識別子
//static ENEMY g_Enemy[MAX_ENEMY];//敵バッファ
//
//
////=============================================================================
//// 初期化処理
////=============================================================================
//void InitEnemy(void)
//{
//	//タイトル画面用テクスチャの読み込み
//	g_TextureNo = LoadTexture((char*)"data/TEXTURE/enemy00.png");
//
//	//敵バッファのフラグの初期化
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
//		//可視フラグがオンであれば更新する
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
//			//敵の時間を進める
//			g_Enemy[i].frame++;
//		}
//	}
//}
//
//void DrawEnemy(void)
//{
//
//	SetBlendState(BLEND_MODE_ALPHABLEND);//通常のアルファ合成
////	SetBlendState(BLEND_MODE_ADD);		 //加算合成
////	SetBlendState(BLEND_MODE_SUBTRACT);	 //減算合成
//	
//	//フィルターモードとアドレッシングモードの設定
//	//アドレッシングモード:WRAP　ラップ → 繰り返し
//	//					  :MIRROR  ミラー → 鏡面繰り返し
//	//                    :CLAMP　クランプ → 最終ピクセルの色で塗りつぶす
//	//                    :BORDER　ボーダー → 設定色で塗りつぶす　
////	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_WRAP);
////	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_MIRROR);
////	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_CLAMP);
//	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_BORDER);
//	SetSamplerBorderColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//
//	//フィルターモード :POINT　ポイント → ポリゴンのピクセルに一番近いテクセルの色を取得する 
//	//                 :LINEAR　リニア → 対象テクセルの平均値を取得する
//	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_BORDER);
//	SetSamplerState(FILTER_MODE_LINEAR, ADDRESS_MODE_BORDER);
//	
//
//	//ベース座標を受け取る
//	D3DXVECTOR2 basePos = GetBase();
//
//	for (int i = 0; i < MAX_ENEMY; i++)
//	{
//		//可視フラグがオンであれば描画する
//		if (g_Enemy[i].use == true)
//		{
//			float rot;
//
//			//移動方向から回転角度を計算する(絵に合わせて-90°回転する)
//			rot = atan2f(g_Enemy[i].dir.y, g_Enemy[i].dir.x) - (D3DX_PI / 2);
//
//			DrawSpriteColorRotate(g_TextureNo, basePos.x + g_Enemy[i].pos.x, basePos.y + g_Enemy[i].pos.y, 64.0f, 64.0f,
//				0.0f, 0.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), rot);
//		}
//	}
//
//	SetBlendState(BLEND_MODE_ALPHABLEND);//通常のアルファ合成に戻す
//}
//
//void SetEnemy(void)
//{
//	for (int i = 0; i < MAX_ENEMY; i++)
//	{
//		//弾バッファの中から表示されていないものを探す
//		if (g_Enemy[i].use == false)
//		{
//			PLAYER* pPlayer = GetPlayer();
//
//			//画面の外周に接する円を作る
//
//			//角度を作る
//			float rot = frand() * 2 * D3DX_PI;
//
//			//画面の外周に接する円の半径を計算する
//			D3DXVECTOR2 screen;
//			screen.x = SCREEN_WIDTH / 2;
//			screen.y = SCREEN_HEIGHT / 2;
//			float length;
//			length = D3DXVec2Length(&screen);
//
//			//円の座標とプレイヤーの座標から敵の発生座標を作成する
//			g_Enemy[i].pos.x = cos(rot) * length + pPlayer->pos.x;
//			g_Enemy[i].pos.y = sin(rot) * length + pPlayer->pos.y;
//
//			g_Enemy[i].start_pos = g_Enemy[i].pos;
//
//			g_Enemy[i].muki = 0;
//
//			g_Enemy[i].speed = 1.0f;
//
//			g_Enemy[i].height = 0.0f; //後で対応する
//
//			//有効フレームをセット
//			g_Enemy[i].frame = 0;
//
//			//状態をリセットする
//			g_Enemy[i].state = chase;
//
//			//可視フラグオン
//			g_Enemy[i].use = true;
//
//			//敵を一つセットしたらループ終了
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
//	//敵とプレイヤーの距離を所得する
//	D3DXVECTOR2 vDist;
//	vDist = g_Enemy[index].pos - pPlayer->pos;
//
//	D3DXVec2Normalize(&vDist, &vDist);
//
//	float x = D3DXVec2Dot(&vDist, &g_Enemy[index].dir);
//
//	if (x < 0.0f)
//	{
//		//敵がプレイヤーの方に向かってくるベクトルを作る
//		//目的のベクトル = プレイヤー座標 - 敵座標
//		g_Enemy[index].dir = pPlayer->pos - g_Enemy[index].pos;
//		//ベクトルを正規化する関数
//		D3DXVec2Normalize(&g_Enemy[index].dir, &g_Enemy[index].dir);
//
//		//敵の座標更新（敵の移動速度 = 敵の移動方向 * 敵の移動スピード）
//		g_Enemy[index].pos += (g_Enemy[index].dir * g_Enemy[index].speed);
//	}
//
//	if (g_Enemy[index].frame > 200)
//	{
//		//待機状態へ移行
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
//	//敵とプレイヤーの距離を所得する
//	D3DXVECTOR2 vDist;
//	vDist = pPlayer->pos - g_Enemy[index].pos;
//	float len = D3DXVec2Length(&vDist);
//
//	//プレイヤーとの距離を調べて200.0fより小さくなったら動く
//	if(len < 200.0f)
//	{
//		D3DXVec2Normalize(&vDist, &vDist);
//
//		float x = D3DXVec2Dot(&vDist,&g_Enemy[index].dir);
//
//			if (x > 0.5f)
//			{
//				//待機状態へ移行
//				g_Enemy[index].state = chase;
//				g_Enemy[index].frame = 0;
//			}
//	}
//}
//
//void EnemyState_Return(int index)
//{
//	//敵がスタート地点の方に向かってくるベクトルを作る
//	//目的のベクトル = スタート地点 - 敵座標
//	g_Enemy[index].dir = g_Enemy[index].start_pos - g_Enemy[index].pos;
//	//ベクトルを正規化する関数
//	D3DXVec2Normalize(&g_Enemy[index].dir, &g_Enemy[index].dir);
//	//敵の座標更新（敵の移動速度 = 敵の移動方向 * 敵の移動スピード）
//	g_Enemy[index].pos += (g_Enemy[index].dir * g_Enemy[index].speed);
//
//	D3DXVECTOR2 vDist;
//	vDist = g_Enemy[index].pos - g_Enemy[index].start_pos;
//	float len = D3DXVec2Length(&vDist);
//
//	//スタート位置に戻ったか？
//	if (len < g_Enemy[index].speed)
//	{
//		g_Enemy[index].pos = g_Enemy[index].start_pos;
//		g_Enemy[index].frame = 0;
//		g_Enemy[index].state = wait;
//	}
//}