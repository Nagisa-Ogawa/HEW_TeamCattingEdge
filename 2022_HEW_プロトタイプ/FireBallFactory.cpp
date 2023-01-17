#include <algorithm>
#include "FireBallFactory.h"
#include "FireBall.h"
#include "texture.h"
#include "Block.h"


FireBallFactory::FireBallFactory()
{
	
}

void FireBallFactory::Create(D3DXVECTOR2 pos, int muki, D3DXVECTOR2 power,int mode)
{
	m_nowID++;
	m_pFireBallList.push_back(new FireBall(m_nowID, pos, muki, power, (FireBall::MODE)mode, m_FireBallTextureNo));
}

void FireBallFactory::Init()
{
	m_FireBallTextureNo = LoadTexture((char*)"data/TEXTURE/FireBall.png");
	m_pCamera = GetCamera();
	m_pPlayer = GetPlayer();
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Init();
	}
}

void FireBallFactory::Uninit()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Uninit();
	}
}

void FireBallFactory::Update()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Update();
	}
	CollisionPlayerToFireBall();
	CollisionBlockToFireBall();
	// 使い終わった火の玉がないかチェック
	DeleteFireBall();
}

void FireBallFactory::Draw()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		fireBall->Draw();
	}
}

FireBallFactory::~FireBallFactory()
{
	for (FireBall* fireBall : m_pFireBallList)
	{
		delete fireBall;
	}

	m_pFireBallList.clear();
}

void FireBallFactory::DeleteFireBall()
{
	while (true)
	{
		auto deleteIt = std::find_if(m_pFireBallList.begin(), m_pFireBallList.end(), ([&](FireBall* fireBall) {return (fireBall->GetIsActive() == false); }));
		// 表示していない爆弾が一個もないなら抜ける
		if (deleteIt == m_pFireBallList.end())
		{
			break;
		}
		// 爆発済みの爆弾は削除
		delete (*deleteIt);
		m_pFireBallList.erase(deleteIt);
	}
}

void FireBallFactory::HitCheckWallToFireBall(std::vector<std::vector<int>>* g_Stage)
{
	for (FireBall* pFireBall : m_pFireBallList)
	{
		pFireBall->SetBlockLength(99999);
		pFireBall->SetBlockIndexX(-99999);
		pFireBall->SetBlockIndexY(-99999);
	}
	//敵バッファのすべてをチェックする
	for (int x = 0; x < (*g_Stage)[0].size(); x++)
	{
		for (int y = 0; y < g_Stage->size(); y++)
		{
			//敵の可視フラグがオフの場合はスキップする
			if ((*g_Stage)[y][x] != 1)
			{
				continue;
			}

			//ヒットしているかを判定する
			D3DXVECTOR2 BlockPos = D3DXVECTOR2(x * BLOCK_SIZE, y * BLOCK_SIZE);

			if (m_pCamera->pos.x <= BlockPos.x)
			{
				if (m_pCamera->pos.x + SCREEN_WIDTH >= BlockPos.x)
				{
					// すべての敵をチェック
					for (FireBall* pFireBall : m_pFireBallList)
					{
						if (!pFireBall->GetIsActive()) {
							continue;
						}
						if (pFireBall->GetState() == FireBall::FIRE::BURNSOUT) {
							continue;
						}
						if (pFireBall->GetPos().x - m_pCamera->pos.x >= -120.0f && pFireBall->GetPos().x - m_pCamera->pos.x <= SCREEN_WIDTH + 120.0f) {
							D3DXVECTOR2 enemyPos = pFireBall->GetPos();// + pFireBall->GetVel();

							if (HitCheckBox_Block(BlockPos, BLOCK_SIZE, BLOCK_SIZE,
								enemyPos, pFireBall->GetSize().x, pFireBall->GetSize().y))
							{
								//自分に当たっている中で一番近いブロックを探す
								D3DXVECTOR2 vLength = BlockPos - enemyPos;
								float length = D3DXVec2Length(&(vLength));
								if (pFireBall->GetBlockLength() > length)
								{
									//より近いブロックの情報に更新する
									pFireBall->SetBlockLength(length);
									pFireBall->SetBlockIndexX(x);
									pFireBall->SetBlockIndexY(y);
								}
							}
						}
					}
				}
			}
		}
	}
}

void FireBallFactory::CollisionBlockToFireBall()
{
	std::vector<std::vector<int>>* g_Stage = GetBlocks();

	HitCheckWallToFireBall(g_Stage);

	// すべての敵をチェック
	for (FireBall* pFireBall : m_pFireBallList)
	{
		if (pFireBall->GetPos().x - m_pCamera->pos.x >= -120.0f && pFireBall->GetPos().x - m_pCamera->pos.x <= SCREEN_WIDTH + 120.0f) {

			m_Result = 0;
			bool isHit = false;

			if (!pFireBall->GetIsActive()) {
				continue;
			}
			//ブロックのヒットした向きを調べる
			if (pFireBall->GetBlockLength() != 99999)
			{
				D3DXVECTOR2 vY(0.0f, -1.0f);

				//ブロックの座標(中心)
				D3DXVECTOR2 BlockPos = D3DXVECTOR2((pFireBall->GetBlockIndexX() * BLOCK_SIZE) + 30.0f, (pFireBall->GetBlockIndexY() * BLOCK_SIZE) + 30.0f);

				D3DXVECTOR2 vDist = -BlockPos;
				D3DXVec2Normalize(&vDist, &vDist);

				//ヒット方向の判定
				float hit_vartical = D3DXVec2Dot(&vY, &vDist);

				if (hit_vartical < 0)
				{
					m_Result |= HIT_DOWN;
				}
				else if (hit_vartical > 0)
				{
					m_Result |= HIT_UP;
				}

				D3DXVECTOR2 vX(-1.0f, 0.0f);
				float hit_horizontal = D3DXVec2Dot(&vX, &vDist);
				if (hit_horizontal <= 0)
				{
					m_Result |= HIT_RIGHT;
				}
				else if (hit_horizontal > 0)
				{
					m_Result |= HIT_LEFT;
				}

				//ブロックの上座標を更新する
				pFireBall->SetBlockHeight(BlockPos.y - 30.0f);
			}

			// 当たり判定後のエネミーの処理
			if (m_Result != 0) {
				D3DXVECTOR2 vec = pFireBall->GetPower();
				if (pFireBall->GetHittingFlag() == false) {
					pFireBall->SetHittingFlag(true);
				}
				switch (pFireBall->GetMode())
				{
				case FireBall::MODE::GHOSTFIRE:
					pFireBall->Hit();
					break;
				case FireBall::MODE::KASYA_ONESHOT:
					if (pFireBall->GetIsReflectFlag() == true) {
						continue;
					}
					// 反射する壁なら
					if (pFireBall->GetPassThroughFlag() == false) {
						// 反射処理
						//if (result == HIT_LEFT)
						//{
						//	vec.x *= -1.0f;
						//	fireBall->SetThrowPower(vec);
						//}
						//if (result == HIT_RIGHT)
						//{
						//	vec.x *= -1.0f;
						//	fireBall->SetThrowPower(vec);
						//}
						if (m_Result & HIT_DOWN)
						{
							vec.y *= -1.0f;
							pFireBall->SetThrowPower(vec);
						}
						if (m_Result & HIT_UP)
						{
							vec.y *= -1.0f;
							pFireBall->SetThrowPower(vec);
						}
						pFireBall->RotToVecotr(pFireBall->GetPower());
						// 壁で反射したかどうかのフラグを更新
						pFireBall->SetReflect(true);
					}
					break;
				case FireBall::MODE::KASYA_THREESHOT:
					if (pFireBall->GetIsReflectFlag() == true) {
						continue;
					}
					// 規定回数反射していたら燃え尽きる
					if (pFireBall->GetNowReflectAllCount() >= pFireBall->GetBurnsOutCount()) {
						pFireBall->Hit();
					}
					// 反射処理
					//if (result == HIT_LEFT)
					//{
					//	vec.x *= -1.0f;
					//	fireBall->SetThrowPower(vec);
					//}
					//if (result == HIT_RIGHT)
					//{
					//	vec.x *= -1.0f;
					//	fireBall->SetThrowPower(vec);
					//}
					if (m_Result & HIT_DOWN)
					{
						vec.y *= -1.0f;
						pFireBall->SetThrowPower(vec);
					}
					if (m_Result & HIT_UP)
					{
						vec.y *= -1.0f;
						pFireBall->SetThrowPower(vec);
					}
					pFireBall->RotToVecotr(pFireBall->GetPower());
					// 壁で反射したかどうかのフラグを更新
					pFireBall->SetReflect(true);
					break;
				default:
					break;
				}
				isHit = true;
			}
			if (isHit == false && pFireBall->GetMode() == FireBall::MODE::KASYA_ONESHOT) {
				// 壁から抜けた時
				if (pFireBall->GetHittingFlag() == true) {
					pFireBall->SetHittingFlag(false);
					// その壁をすり抜けていたなら
					if (pFireBall->GetIsReflectFlag() == false) {
						pFireBall->SetPassThroughFlag(false);
					}
					else {
						// 反射回数を更新
						pFireBall->SetNowReflectCount(pFireBall->GetNowReflectCount() + 1);
						pFireBall->SetNowReflectAllCount(pFireBall->GetNowReflectAllCount() + 1);
						// 規定回数反射していたら燃え尽きる
						if (pFireBall->GetNowReflectAllCount() >= pFireBall->GetBurnsOutCount()) {
							pFireBall->Hit();
						}
						// 規定回数反射したかチェック
						if (pFireBall->GetNowReflectCount() >= pFireBall->GetReflectWallCounts()) {
							// 規定回数反射したら次の壁を反射せずに通り過ぎる
							pFireBall->SetNowReflectCount(0);
							pFireBall->SetPassThroughFlag(true);
							pFireBall->SetNowReflectWallCount(pFireBall->GetNowReflectWallCount() + 1);
						}
						pFireBall->SetReflect(false);
					}
				}
			}
			if (isHit == false && pFireBall->GetMode() == FireBall::MODE::KASYA_THREESHOT) {
				// 壁から抜けた時
				if (pFireBall->GetHittingFlag() == true) {
					pFireBall->SetHittingFlag(false);
					// その壁をすり抜けていたなら
					if (pFireBall->GetIsReflectFlag() == true) {
						// 反射回数を更新
						pFireBall->SetNowReflectAllCount(pFireBall->GetNowReflectAllCount() + 1);
						pFireBall->SetReflect(false);
					}
				}
			}

		}
	}

	//auto blocks = GetBlocks();
	//for (FireBall* fireBall : m_pFireBallList)
	//{
	//	// 死亡していたならスキップ
	//	if (!fireBall->GetIsActive())
	//	{
	//		continue;
	//	}
	//	if (fireBall->GetState() == FireBall::FIRE::BURNSOUT) {
	//		continue;
	//	}
	//	DWORD result = 0;
	//	result = HitCheckFireBall_Block(fireBall->GetPos(), fireBall->GetSize());

	//}
	return;
}

void FireBallFactory::CollisionPlayerToFireBall()
{
	// プレイヤーが無敵ならスキップ
	if (m_pPlayer->mutekiflag)
	{
		return;
	}
	for (FireBall* fireBall : m_pFireBallList)
	{
		// 死亡していたならスキップ
		if (!fireBall->GetIsActive())
		{
			continue;
		}
		// プレイヤーと当たったなら
		if (HitCheckBox(fireBall->GetPos(), fireBall->GetSize(),
			m_pPlayer->pos, D3DXVECTOR2(m_pPlayer->size, m_pPlayer->size)))
		{
			// プレイヤーのHPを減らす
			PlayerDamage(1);
			fireBall->Hit();
		}
	}
	return;
}


bool FireBallFactory::HitCheckBox(D3DXVECTOR2 enemyPos, D3DXVECTOR2 enemySize,
	D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize)
{
	float box1Xmin = enemyPos.x - (enemySize.x / 2);
	float box1Xmax = enemyPos.x + (enemySize.x / 2);
	float box1Ymin = enemyPos.y - (enemySize.y / 2);
	float box1Ymax = enemyPos.y + (enemySize.y / 2);

	float box2Xmin = playerPos.x - (playerSize.x / 2);
	float box2Xmax = playerPos.x + (playerSize.x / 2);
	float box2Ymin = playerPos.y - (playerSize.y / 2);
	float box2Ymax = playerPos.y + (playerSize.y / 2);

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

bool FireBallFactory::HitCheckBox_Block(D3DXVECTOR2 box1pos, float box1width, float box1height,
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

bool FireBallFactory::HitCheckBox_Block(D3DXVECTOR2 blockPos, D3DXVECTOR2 blockSize, D3DXVECTOR2 playerPos, D3DXVECTOR2 playerSize)
{

	float box1Xmin = blockPos.x;
	float box1Xmax = blockPos.x + blockSize.x;
	float box1Ymin = blockPos.y ;
	float box1Ymax = blockPos.y + blockSize.y;

	float box2Xmin = playerPos.x - (playerSize.x / 2);
	float box2Xmax = playerPos.x + (playerSize.x / 2);
	float box2Ymin = playerPos.y - (playerSize.y / 2);
	float box2Ymax = playerPos.y + (playerSize.y / 2);

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

