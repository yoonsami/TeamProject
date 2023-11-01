#include "pch.h"
#include "PhysxMgr.h"

PhysxMgr::~PhysxMgr()
{
    PX_RELEASE(m_pManager);
    PX_RELEASE(m_pScene);
    PX_RELEASE(m_pDistpatcher);
    PX_RELEASE(m_pPhysics);
    PX_RELEASE(m_pPvd);
    PX_RELEASE(m_pTransport);
    PX_RELEASE(m_pFoundation);
}

HRESULT PhysxMgr::Init()
{

	PX_RELEASE(m_pManager);
	PX_RELEASE(m_pScene);
	PX_RELEASE(m_pDistpatcher);
	PX_RELEASE(m_pPhysics);
	PX_RELEASE(m_pPvd);
    PX_RELEASE(m_pTransport);
	PX_RELEASE(m_pFoundation);


    m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefalutAllocatorCallback, m_DefaultErrorCallback);
    assert(m_pFoundation);
    
    m_pPvd = PxCreatePvd(*m_pFoundation);
    m_pTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    m_pPvd->connect(*m_pTransport, PxPvdInstrumentationFlag::eALL);
    m_ToleranceScale.length = 100;
    m_ToleranceScale.speed = 981;


    m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, m_ToleranceScale, true, m_pPvd);

    PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.f, -9.81f, 0.f);
    m_pDistpatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_pDistpatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    m_pScene = m_pPhysics->createScene(sceneDesc);

    m_pManager = PxCreateControllerManager(*m_pScene);

    PxPvdSceneClient* pvdCilent = m_pScene->getScenePvdClient();
    if (pvdCilent)
    {
        pvdCilent->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdCilent->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdCilent->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);

    }

    return S_OK;
}

void PhysxMgr::Tick()
{
    m_pScene->simulate(fDT);
    m_pScene->fetchResults(true);
}
