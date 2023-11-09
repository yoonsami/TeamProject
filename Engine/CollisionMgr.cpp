#include "pch.h"
#include "CollisionMgr.h"

#include "BaseCollider.h"

HRESULT CollisionMgr::Init()
{
	Reset_Collision_Group();
	return S_OK;
}

void CollisionMgr::Tick()
{
	for (_uint iRow = 0; iRow < MAX_COLLIDER_GROUP; ++iRow)
	{
		for (_uint iCol = iRow; iCol < MAX_COLLIDER_GROUP; ++iCol)
		{
			if (m_arrCollisionCheck[iRow] & (1 << iCol))
			{
				Collision_Group_Update(iRow, iCol);
			}
		}
	}

	for (_uint i = 0; i < MAX_COLLIDER_GROUP; ++i)
		m_CollisionGroup[i].clear();
}

void CollisionMgr::Check_Group(_int eLeft, _int eRight)
{
	_uint iRow = min((_uint)eLeft, (_uint)eRight);
	_uint iCol = max((_uint)eLeft, (_uint)eRight);

	if (m_arrCollisionCheck[iRow] & (1 << iCol))
	{
		// 이미 충돌 그룹으로 지정해놓은 경우, 해제.
		m_arrCollisionCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCollisionCheck[iRow] |= (1 << iCol);
	}
}

void CollisionMgr::Add_ColliderGroup(_int eGroup, shared_ptr<BaseCollider> pCollider)
{
	m_CollisionGroup[eGroup].push_back(pCollider);
}

void CollisionMgr::Collision_Group_Update(_int eLeft, _int eRight)
{
	_uint lower = eLeft < eRight ? eLeft : eRight;
	_uint bigger = eLeft < eRight ? eRight : eLeft;

	if (m_CollisionGroup[lower].empty() || m_CollisionGroup[bigger].empty())
		return;

	for (_uint i = 0; i < m_CollisionGroup[lower].size(); ++i)
	{
		shared_ptr<BaseCollider> pLeftCol = m_CollisionGroup[lower][i];

		if (!pLeftCol)
			continue;

		for (_uint j = 0; j < m_CollisionGroup[bigger].size(); ++j)
		{
			shared_ptr<BaseCollider> pRightCol = m_CollisionGroup[bigger][j];

			if (!pLeftCol)
				continue;

			if (!pRightCol)
				continue;

			if (pLeftCol == pRightCol)
				continue;

			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->Get_ID();
			ID.iRight_id = pRightCol->Get_ID();

			auto iter = m_mapColInfo.find(ID.ID);

			if (iter == m_mapColInfo.end())
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}
			_float fGap = 0.f;

			// 현재 충돌
			if (IsCollison(pLeftCol, pRightCol, fGap))
			{
				// 이전에도 충돌
				if (iter->second)
				{
					if(pLeftCol->Is_Activated() && pRightCol->Is_Activated())
					{
						pLeftCol->OnCollision(pRightCol, fGap);
						pRightCol->OnCollision(pLeftCol, fGap);
					}
					else
					{
						pLeftCol->OnCollisionExit(pRightCol, fGap);
						pRightCol->OnCollisionExit(pLeftCol, fGap);
						iter->second = false;
					}
				}
				// 이전에는 안충돌
				else
				{
					if (pLeftCol->Is_Activated() && pRightCol->Is_Activated())
					{
						pLeftCol->OnCollisionEnter(pRightCol, fGap);
						pRightCol->OnCollisionEnter(pLeftCol, fGap);
						iter->second = true;
					}

				}
			}
			// 현재 안충돌
			else
			{
				// 이전에는 충돌
				if (iter->second)
				{
					pLeftCol->OnCollisionExit(pRightCol, fGap);
					pRightCol->OnCollisionExit(pLeftCol, fGap);

					iter->second = false;
				}
			}
		}
	}
}

_bool CollisionMgr::IsCollison(shared_ptr<BaseCollider> pLeftObj, shared_ptr<BaseCollider> pRightObj, OUT _float& pGap)
{
	_bool collided = pLeftObj->Intersects(pRightObj);

	return collided;
}
