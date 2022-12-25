
#include "collision.h"
#include "enemy.h"


bool HitCheckBox(D3DXVECTOR2 box1pos, float box1width, float box1height,
	D3DXVECTOR2 box2pos, float box2width, float box2height)
{
	float box1Xmin = box1pos.x - (box1width / 2);
	float box1Xmax = box1pos.x + (box1width / 2);
	float box1Ymin = box1pos.y - (box1height / 2);
	float box1Ymax = box1pos.y + (box1height / 2);

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





//void Collision_Bullet_Enemy(void)
//{
//	ENEMY* pEnemy = GetEnemy();
//	BULLET* pBullet = GetBullet();
//
//	//�G�o�b�t�@�̂��ׂĂ��`�F�b�N����
//	for (int e = 0; e < MAX_ENEMY; e++)
//	{
//		//�G�̉��t���O���I�t�̏ꍇ�̓X�L�b�v����
//		if (pEnemy[e].use == false)
//			continue;
//
//		//�e�o�b�t�@�̂��ׂĂ��`�F�b�N����
//		for (int b = 0; b < MAX_BULLET; b++)
//		{
//			//�e�̉��t���O���I�t�̏ꍇ�̓X�L�b�v����
//			if (pBullet[b].use == false)
//				continue;
//
//			if (HitCheckBox(pEnemy[e].pos, 64.0f, 64.0f, pBullet[b].pos, 64.0f, 64.0f))
//			{
//				pEnemy[e].use = false; //�G������
//				pBullet[b].use = false;//�e������
//			}
//		}
//	}
//}