#pragma once

enum
{ 
	MAX_COLLIDER_GROUP = 32 
};

class BaseCollider;
union COLLIDER_ID
{
	struct
	{
		_uint iLeft_id;
		_uint iRight_id;
	};
	_ulonglong ID;
};



class CollisionMgr
{
	DECLARE_SINGLETON(CollisionMgr)

public:
	HRESULT Init();
	void Tick();

	void Check_Group(_int eLeft, _int eRight);
	void Reset_Collision_Group() { m_arrCollisionCheck.fill(0); }
	void Add_ColliderGroup(_int eGroup, shared_ptr<BaseCollider> pCollider);

private:
	void Collision_Group_Update(_int eLeft, _int eRight);
	_bool IsCollison(shared_ptr<BaseCollider> pLeftObj, shared_ptr<BaseCollider> pRightObj, OUT _float& pGap);


private:
	map<_ulonglong, bool> m_mapColInfo;
	array<_uint, MAX_COLLIDER_GROUP> m_arrCollisionCheck{};
	array<vector<shared_ptr<BaseCollider>>, MAX_COLLIDER_GROUP> m_CollisionGroup;

};

