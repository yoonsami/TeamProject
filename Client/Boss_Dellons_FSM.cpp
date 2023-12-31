#include "pch.h"
#include "MainCameraScript.h"
#include "Boss_Dellons_FSM.h"
#include "ModelAnimator.h"
#include "MotionTrailRenderer.h"
#include "Debug_CreateMotionTrail.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"
#include "Camera.h"
#include "Boss_DellonsWraith_FSM.h"
#include "MathUtils.h"
#include "UiDamageCreate.h"
#include "UIBossHpBar.h"
#include "ObjectDissolve.h"
#include "CharacterController.h"
#include "UiBossDialog.h"
#include "MainUiController.h"


Boss_Dellons_FSM::Boss_Dellons_FSM()
{
}

Boss_Dellons_FSM::~Boss_Dellons_FSM()
{
    SWITCHMGR.Add_Score(SCORE_TYPE::KRIS_SCENE);
}

HRESULT Boss_Dellons_FSM::Init()
{
    if (!m_bInitialize)
    {
        auto animator = Get_Owner()->Get_Animator();
        if (animator)
        {
            animator->Set_CurrentAnim(L"b_idle", true, 1.f);
            m_eCurState = STATE::n_idle;
        }
        shared_ptr<GameObject> attackCollider = make_shared<GameObject>();
        attackCollider->GetOrAddTransform();
        attackCollider->Add_Component(make_shared<SphereCollider>(1.f));
        attackCollider->Get_Collider()->Set_CollisionGroup(Monster_Attack);

        m_pAttackCollider = attackCollider;

        EVENTMGR.Create_Object(m_pAttackCollider.lock());
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);

        m_pAttackCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
        m_pAttackCollider.lock()->Set_Name(L"Boss_Dellons_AttackCollider");
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(Get_Owner());

        m_pWeapon = CUR_SCENE->Get_GameObject(L"Weapon_Boss_Dellons");

        m_pCamera = CUR_SCENE->Get_MainCamera();

        m_iCenterBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_CP");
        m_iCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Cam");
        m_iSkillCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_SkillCam");

        m_fDetectRange = 20.f;
        m_fRunSpeed = 6.5f;
        m_fSprintSpeed = 8.5f;

        if (!m_pTarget.expired())
            Get_Transform()->LookAt(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS));

        if (!m_pOwner.expired())
            m_pOwner.lock()->Add_Component(make_shared<UiBossDialog>());

        // HP Init
        if (!m_pOwner.expired())
        {
            m_pOwner.lock()->Set_MaxHp(DATAMGR.Get_MonsterData(MONSTER::DELLONS).MaxHp);
        }

        m_bInitialize = true;
    }
	m_fNormalAttack_AnimationSpeed = 1.2f;
	m_fSkillAttack_AnimationSpeed = 1.0f;
	m_fEvade_AnimationSpeed = 1.5f;

    m_fVoiceVolume = 0.5f;
    m_fSwingVolume = 0.4f;
    m_fFootStepVolume = 0.4f;
    m_fEffectVolume = 0.4f;

    m_fMySoundDistance = 15.f;

    return S_OK;
}

void Boss_Dellons_FSM::Tick()
{
    DeadCheck();

    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up);
    }
}

void Boss_Dellons_FSM::State_Tick()
{
    State_Init();

    m_iCurFrame = Get_CurFrame();
    Recovery_Color();
    switch (m_eCurState)
    {
    case STATE::n_idle:
        n_idle();
        break;
    case STATE::talk_01:
        talk_01();
        break;
    case STATE::b_idle:
        b_idle();
        break;
    case STATE::b_run_start:
        b_run_start();
        break;
    case STATE::b_run:
        b_run();
        break;
    case STATE::b_run_end_r:
        b_run_end_r();
        break;
    case STATE::b_run_end_l:
        b_run_end_l();
        break;
    case STATE::b_sprint:
        b_sprint();
        break;
    case STATE::b_walk:
        b_walk();
        break;
    case STATE::die:
        die();
        break;
    case STATE::airborne_start:
        airborne_start();
        break;
    case STATE::airborne_end:
        airborne_end();
        break;
    case STATE::airborne_up:
        airborne_up();
        break;
    case STATE::hit:
        hit();
        break;
    case STATE::knock_start:
        knock_start();
        break;
    case STATE::knock_end:
        knock_end();
        break;
    case STATE::knock_end_loop:
        knock_end_loop();
        break;
    case STATE::knock_end_hit:
        knock_end_hit();
        break;
    case STATE::knock_up:
        knock_up();
        break;
    case STATE::knockdown_start:
        knockdown_start();
        break;
    case STATE::knockdown_end:
        knockdown_end();
        break;
    case STATE::skill_1100:
        skill_1100();
        break;
    case STATE::skill_1200:
        skill_1200();
        break;
    case STATE::skill_1300:
        skill_1300();
        break;
    case STATE::skill_1400:
        skill_1400();
        break;
    case STATE::skill_91100:
        skill_91100();
        break;
    case STATE::skill_93100:
        skill_93100();
        break;
    case STATE::skill_100100:
        skill_100100();
        break;
    case STATE::skill_100200:
        skill_100200();
        break;
    case STATE::skill_200100:
        skill_200100();
        break;
    case STATE::skill_200200:
        skill_200200();
        break;
    case STATE::skill_300100:
        skill_300100();
        break;
    case STATE::skill_400100:
        skill_400100();
        break;
    case STATE::skill_501100:
        skill_501100();
        break;
    }

    if (m_iPreFrame != m_iCurFrame)
        m_iPreFrame = m_iCurFrame;
}

void Boss_Dellons_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::n_idle:
            n_idle_Init();
            break;
        case STATE::talk_01:
            talk_01_Init();
            break;
        case STATE::b_idle:
            b_idle_Init();
            break;
        case STATE::b_run_start:
            b_run_start_Init();
            break;
        case STATE::b_run:
            b_run_Init();
            break;
        case STATE::b_run_end_r:
            b_run_end_r_Init();
            break;
        case STATE::b_run_end_l:
            b_run_end_l_Init();
            break;
        case STATE::b_sprint:
            b_sprint_Init();
            break;
        case STATE::b_walk:
            b_walk_Init();
            break;
        case STATE::die:
            die_Init();
            break;
        case STATE::airborne_start:
            airborne_start_Init();
            break;
        case STATE::airborne_end:
            airborne_end_Init();
            break;
        case STATE::airborne_up:
            airborne_up_Init();
            break;
        case STATE::hit:
            hit_Init();
            break;
        case STATE::knock_start:
            knock_start_Init();
            break;
        case STATE::knock_end:
            knock_end_Init();
            break;
        case STATE::knock_end_loop:
            knock_end_loop_Init();
            break;
        case STATE::knock_end_hit:
            knock_end_hit_Init();
            break;
        case STATE::knock_up:
            knock_up_Init();
            break;
        case STATE::knockdown_start:
            knockdown_start_Init();
            break;
        case STATE::knockdown_end:
            knockdown_end_Init();
            break;
        case STATE::skill_1100:
            skill_1100_Init();
            break;
        case STATE::skill_1200:
            skill_1200_Init();
            break;
        case STATE::skill_1300:
            skill_1300_Init();
            break;
        case STATE::skill_1400:
            skill_1400_Init();
            break;
        case STATE::skill_91100:
            skill_91100_Init();
            break;
        case STATE::skill_93100:
            skill_93100_Init();
            break;
        case STATE::skill_100100:
            skill_100100_Init();
            break;
        case STATE::skill_100200:
            skill_100200_Init();
            break;
        case STATE::skill_200100:
            skill_200100_Init();
            break;
        case STATE::skill_200200:
            skill_200200_Init();
            break;
        case STATE::skill_300100:
            skill_300100_Init();
            break;
        case STATE::skill_400100:
            skill_400100_Init();
            break;
        case STATE::skill_501100:
            skill_501100_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void Boss_Dellons_FSM::Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget, _uint iElementType)
{
    // Random 20 Percent
    _float fHitDamage = Utils::Random_In_Range(fDamage * 0.8f, fDamage * 1.2f);
    if (iElementType == ElementType::LIGHT)
        fHitDamage *= 1.2f; // 속성추뎀

    if (!m_bSuperArmor)
    {
        if (rand() % 10 == 0)
            m_bEvade = true;
    }
    else
        m_bEvade = false;

    if (!m_bEvade)
    {
        //Calculate Damage 
        m_pOwner.lock()->Get_Hurt(fHitDamage);
		CUR_SCENE->Get_UI(L"UI_Damage_Controller")->Get_Script<UiDamageCreate>()->Create_Damage_Font(Get_Owner(), fHitDamage, ElementType(iElementType));
    }


    _float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
    _float3 vOppositePos = pLookTarget->Get_Transform()->Get_State(Transform_State::POS).xyz();

    m_vHitDir = vOppositePos - vMyPos;
    m_vHitDir.y = 0.f;
    m_vHitDir.Normalize();
	Set_HitColor();

    if (skillname == NORMAL_ATTACK || skillname == NORMAL_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (!m_bEvade)
            {
                if (m_eCurState == STATE::hit)
                    Reset_Frame();
                else if (m_eCurState == STATE::knock_end_hit)
                    Reset_Frame();
                else if (m_eCurState == STATE::knock_end_loop)
                    m_eCurState = STATE::knock_end_hit;
                else
                    m_eCurState = STATE::hit;

                wstring strSoundTag = L"Dellons_hit_0";
                strSoundTag = strSoundTag + to_wstring(rand() % 5 + 1);
                SOUND.Play_Sound(strSoundTag, CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

            }
            else
            {
                m_eCurState = STATE::skill_91100;
            }
        }
    }
    else if (skillname == KNOCKBACK_ATTACK || skillname == KNOCKBACK_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (!m_bEvade)
            {
                if (m_eCurState == STATE::knock_end_hit)
                    Reset_Frame();
                else if (m_eCurState == STATE::knock_end_loop)
                    m_eCurState = STATE::knock_end_hit;
                else
                    m_eCurState = STATE::knock_start;

                wstring strSoundTag = L"Dellons_hit_0";
                strSoundTag = strSoundTag + to_wstring(rand() % 5 + 1);
                SOUND.Play_Sound(strSoundTag, CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

            }
            else
            {
                m_eCurState = STATE::skill_91100;
            }
        }
    }
    else if (skillname == KNOCKDOWN_ATTACK || skillname == KNOCKDOWN_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (!m_bEvade)
            {
                if (m_eCurState == STATE::knock_end_hit)
                    Reset_Frame();
                else if (m_eCurState == STATE::knock_end_loop)
                    m_eCurState = STATE::knock_end_hit;
                else
                    m_eCurState = STATE::knockdown_start;

                wstring strSoundTag = L"Dellons_hit_0";
                strSoundTag = strSoundTag + to_wstring(rand() % 5 + 1);
                SOUND.Play_Sound(strSoundTag, CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

            }
            else
            {
                m_eCurState = STATE::skill_91100;
            }
        }
    }
    else if (skillname == AIRBORNE_ATTACK || skillname == AIRBORNE_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (!m_bEvade)
            {
                if (m_eCurState == STATE::knock_end_hit)
                    Reset_Frame();
                else if (m_eCurState == STATE::knock_end_loop)
                    m_eCurState = STATE::knock_end_hit;
                else
                    m_eCurState = STATE::airborne_start;

                wstring strSoundTag = L"Dellons_hit_0";
                strSoundTag = strSoundTag + to_wstring(rand() % 5 + 1);
                SOUND.Play_Sound(strSoundTag, CHANNELID::SOUND_EFFECT, m_fVoiceVolume* g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

            }
            else
            {
                m_eCurState = STATE::skill_91100;
            }
        }
    }
}

void Boss_Dellons_FSM::Set_State(_uint iIndex)
{
}

void Boss_Dellons_FSM::n_idle()
{   
    //Change
    if (Target_In_DetectRange())
        m_bDetected = true;

    if (m_bDetected)
        m_eCurState = STATE::talk_01;
}

void Boss_Dellons_FSM::n_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);
    
    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
    m_bSetPattern = false;
}

void Boss_Dellons_FSM::talk_01()
{
    if (!m_pCamera.expired())
    {
        _float4 m_vCamPos = m_vCenterBonePos + _float4{ 0.f,1.f,0.f,0.f };
        _float4 vDir = m_pCamera.lock()->Get_Transform()->Get_State(Transform_State::POS) - m_vCamPos;

        vDir.Normalize();

        m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
        m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCamPos.xyz());
        m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.2f, vDir.xyz(), 4.f);
    }

    Calculate_CamBoneMatrix();

    //Dialogue END
    if (m_pOwner.lock()->Get_Script<UiBossDialog>())
    {
        if (m_pOwner.lock()->Get_Script<UiBossDialog>()->Is_Finish() == true)
        {
            CUR_SCENE->Set_AttackCall(true);
            m_eCurState = STATE::b_idle;
            g_bCutScene = false;

            auto pController = CUR_SCENE->Get_UI(L"Main_UI_Controller");

            if (pController)
                pController->Get_Script<MainUiController>()->Set_MainUI_Render(true);


            SOUND.Play_Sound(L"Skill_10_A_Dellons_01", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), 30.f);
        }
    }
}

void Boss_Dellons_FSM::talk_01_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"talk_01", 0.2f ,true, 0.5f);

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;

    g_bCutScene = true;

    auto pController = CUR_SCENE->Get_UI(L"Main_UI_Controller");

    if (pController)
        pController->Get_Script<MainUiController>()->Set_MainUI_Render(false);

    if (m_pOwner.lock()->Get_Script<UiBossDialog>())
        m_pOwner.lock()->Get_Script<UiBossDialog>()->Create_Dialog(BOSS::DELLONS);

    Calculate_CamBoneMatrix();
}

void Boss_Dellons_FSM::b_idle()
{
    Battle_Start();

    if (m_bBattleStart)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);

        m_tAttackCoolTime.fAccTime += fDT;

        if (!m_bSetPattern)
        {
            if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
                Set_AttackSkill_Phase1();
        }
        else
        {
            if (Target_In_AttackRange())
                m_eCurState = m_ePatternState;
            else
                m_eCurState = STATE::b_run_start;
        }

        Dead_Setting();
    }
}

void Boss_Dellons_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    if (m_ePreState != STATE::skill_400100)
        animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);
    else
        animator->Set_NextTweenAnim(L"b_idle", 0.3f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;

    if (!m_bCreateUI)
    {
        //Add_BossHp UI
        if (!m_pOwner.expired())
        {
            auto pScript = make_shared<UIBossHpBar>(BOSS::DELLONS);
            m_pOwner.lock()->Add_Component(pScript);
            pScript->Init();
        }

        m_bCreateUI = true;
    }
}

void Boss_Dellons_FSM::b_run_start()
{
    if (Init_CurFrame(8))
        SOUND.Play_Sound(L"footstep_Right", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
    else if (Init_CurFrame(18))
        SOUND.Play_Sound(L"footstep_Left", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);

    Get_Transform()->Go_Straight();
    
    if (Is_AnimFinished())
        m_eCurState = STATE::b_run;

    if (Target_In_AttackRange())
        m_eCurState = m_ePatternState;
    else
    {
        if (Is_AnimFinished())
            m_eCurState = STATE::b_run;
    }
}

void Boss_Dellons_FSM::b_run_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_start", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_run()
{
    if (Init_CurFrame(9))
        SOUND.Play_Sound(L"footstep_Right", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
    else if (Init_CurFrame(18))
        SOUND.Play_Sound(L"footstep_Left", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);

    Get_Transform()->Go_Straight();

    m_tSprintCoolTime.fAccTime += fDT;
    
    if (Target_In_AttackRange())
    {
        m_eCurState = m_ePatternState;
    }
    else
    {
        if (m_tSprintCoolTime.fAccTime >= m_tSprintCoolTime.fCoolTime)
            m_bSprint = true;

        if (m_bSprint)
        {
            if (m_iCurFrame == 1)
            {
                m_bSprint = false;
                m_eCurState = STATE::b_sprint;
            }
        }
    }
}

void Boss_Dellons_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
    m_bSprint = false;
}

void Boss_Dellons_FSM::b_run_end_r()
{
}

void Boss_Dellons_FSM::b_run_end_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_r", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_run_end_l()
{
}

void Boss_Dellons_FSM::b_run_end_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_l", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_sprint()
{
    if (Init_CurFrame(6))
        SOUND.Play_Sound(L"footstep_Right", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
    else if (Init_CurFrame(13))
        SOUND.Play_Sound(L"footstep_Left", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);

    Get_Transform()->Go_Straight();

    if (Target_In_AttackRange())
        m_eCurState = m_ePatternState;
}

void Boss_Dellons_FSM::b_sprint_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_sprint", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fSprintSpeed);

    AttackCollider_Off();

    m_tSprintCoolTime.fAccTime = 0.f;

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_walk()
{
}

void Boss_Dellons_FSM::b_walk_Init()
{
}

void Boss_Dellons_FSM::die()
{
    if (Is_AnimFinished())
    {
        auto script = make_shared<ObjectDissolve>(1.f);
        Get_Owner()->Add_Component(script);
        script->Init();

        if (!m_pAttackCollider.expired())
            EVENTMGR.Delete_Object(m_pAttackCollider.lock());

        if (!m_pWeapon.expired())
        {
            auto script = make_shared<ObjectDissolve>(1.f);
            m_pWeapon.lock()->Add_Component(script);
            script->Init();
        }

        if (!m_pDellonsWraith.expired())
            EVENTMGR.Delete_Object(m_pDellonsWraith.lock());
    }
}

void Boss_Dellons_FSM::die_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"die", 0.2f, false, 1.f);


    m_bInvincible = true;
    m_bSuperArmor = true;

    SOUND.Play_Sound(L"Dellons_die_06", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);


    if (!m_pDellonsWraith.expired())
    {
        if (m_pDellonsWraith.lock()->Get_FSM())
            m_pDellonsWraith.lock()->Get_FSM()->Remove_Object();
    }

    if (!m_pOwner.expired())
    {
        if (m_pOwner.lock()->Get_Script<UIBossHpBar>())
            m_pOwner.lock()->Get_Script<UIBossHpBar>()->Remove_HpBar();
    }
}

void Boss_Dellons_FSM::airborne_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, m_fTurnSpeed);

    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_end;
}

void Boss_Dellons_FSM::airborne_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_start", 0.2f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_CharacterController()->Add_Velocity(6.f);
}

void Boss_Dellons_FSM::airborne_end()
{
    if (Is_AnimFinished())
    {
        if (!m_bIsDead)
            m_eCurState = STATE::airborne_up;
        else
        {
         
            m_bInvincible = true;

            Get_Owner()->Get_Animator()->Set_AnimState(true);

            auto script = make_shared<ObjectDissolve>(1.f);
            Get_Owner()->Add_Component(script);
            script->Init();

            if (!m_pAttackCollider.expired())
            {
                SOUND.Play_Sound(L"Dellons_die_06", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

                EVENTMGR.Delete_Object(m_pAttackCollider.lock());
            }

            if (!m_pWeapon.expired())
            {
                auto script = make_shared<ObjectDissolve>(1.f);
                m_pWeapon.lock()->Add_Component(script);
                script->Init();
            }

            if (!m_pDellonsWraith.expired())
            {
                if (m_pDellonsWraith.lock()->Get_FSM())
                    m_pDellonsWraith.lock()->Get_FSM()->Remove_Object();
            }

            if (!m_pOwner.expired())
            {
                if (m_pOwner.lock()->Get_Script<UIBossHpBar>())
                    m_pOwner.lock()->Get_Script<UIBossHpBar>()->Remove_HpBar();
            }
        }
    }
}

void Boss_Dellons_FSM::airborne_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::airborne_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::airborne_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.1f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::hit()
{
    Soft_Turn_ToInputDir(m_vHitDir, m_fTurnSpeed);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    Dead_Setting();
}

void Boss_Dellons_FSM::hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"hit", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::knock_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, m_fTurnSpeed);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end;
}

void Boss_Dellons_FSM::knock_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_start", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.8f);
}

void Boss_Dellons_FSM::knock_end()
{
    if (m_iCurFrame < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void Boss_Dellons_FSM::knock_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.5f);
}

void Boss_Dellons_FSM::knock_end_loop()
{
    m_tKnockDownEndCoolTime.fAccTime += fDT;

    if (m_iCurFrame > Get_FinalFrame() / 2)
        m_eCurState = STATE::knock_up;

    if (m_bIsDead)
    {

        m_bInvincible = true;

        Get_Owner()->Get_Animator()->Set_AnimState(true);

        auto script = make_shared<ObjectDissolve>(1.f);
        Get_Owner()->Add_Component(script);
        script->Init();

        if (!m_pAttackCollider.expired())
        {
            SOUND.Play_Sound(L"Dellons_die_06", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

            EVENTMGR.Delete_Object(m_pAttackCollider.lock());
        }

        if (!m_pWeapon.expired())
        {
            auto script = make_shared<ObjectDissolve>(1.f);
            m_pWeapon.lock()->Add_Component(script);
            script->Init();
        }

        if (!m_pDellonsWraith.expired())
        {
            if (m_pDellonsWraith.lock()->Get_FSM())
                m_pDellonsWraith.lock()->Get_FSM()->Remove_Object();
        }

        if (!m_pOwner.expired())
        {
            if (m_pOwner.lock()->Get_Script<UIBossHpBar>())
                m_pOwner.lock()->Get_Script<UIBossHpBar>()->Remove_HpBar();
        }
    }
}

void Boss_Dellons_FSM::knock_end_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_loop", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::knock_end_hit()
{
    m_tKnockDownEndCoolTime.fAccTime += fDT;

    if (Is_AnimFinished())
    {
        if (m_tKnockDownEndCoolTime.fAccTime >= m_tKnockDownEndCoolTime.fCoolTime)
            m_eCurState = STATE::knock_up;
        else
            m_eCurState = STATE::knock_end_loop;
    }
}

void Boss_Dellons_FSM::knock_end_hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_hit", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::knock_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::knock_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    m_tKnockDownEndCoolTime.fAccTime = 0.f;

    Get_Transform()->Set_Speed(m_fRunSpeed);
}

void Boss_Dellons_FSM::knockdown_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, m_fTurnSpeed);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knockdown_end;
}

void Boss_Dellons_FSM::knockdown_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knockdown_start", 0.1f, false, 2.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed);
}

void Boss_Dellons_FSM::knockdown_end()
{
    if (m_iCurFrame < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
    {
        if (!m_bIsDead)
            m_eCurState = STATE::knock_up;
        else
        {
       
            m_bInvincible = true;

            Get_Owner()->Get_Animator()->Set_AnimState(true);

            auto script = make_shared<ObjectDissolve>(1.f);
            Get_Owner()->Add_Component(script);
            script->Init();

            if (!m_pAttackCollider.expired())
            {
                SOUND.Play_Sound(L"Dellons_die_06", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

                EVENTMGR.Delete_Object(m_pAttackCollider.lock());
            }

            if (!m_pWeapon.expired())
            {
                auto script = make_shared<ObjectDissolve>(1.f);
                m_pWeapon.lock()->Add_Component(script);
                script->Init();
            }

            if (!m_pDellonsWraith.expired())
            {
                if (m_pDellonsWraith.lock()->Get_FSM())
                    m_pDellonsWraith.lock()->Get_FSM()->Remove_Object();
            }

            if (!m_pOwner.expired())
            {
                if (m_pOwner.lock()->Get_Script<UIBossHpBar>())
                    m_pOwner.lock()->Get_Script<UIBossHpBar>()->Remove_HpBar();
            }
        }
    }
}

void Boss_Dellons_FSM::knockdown_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_CurrentAnim(L"knockdown_end", false, 1.5f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed * 0.5f);
}

void Boss_Dellons_FSM::stun()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::stun_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"stun", 0.1f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_1100()
{
    if (Init_CurFrame(9))
        Add_And_Set_Effect(L"Dellons_1100");

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(9))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        AttackCollider_On(NORMAL_ATTACK, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamageMul * 0.2f);
    }
    else if (Init_CurFrame(19))
    {
        AttackCollider_Off();
    }


    if (m_iCurFrame == 25)
        m_eCurState = STATE::skill_1200;
}

void Boss_Dellons_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    SOUND.Play_Sound(L"Dellons_att_s_03", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_1200()
{
    if (Init_CurFrame(8))
        Add_And_Set_Effect(L"Dellons_1200");

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(8))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        AttackCollider_On(NORMAL_ATTACK, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamageMul * 0.2f);
    }
    else if (Init_CurFrame(18))
    {
        AttackCollider_Off();
    }
  
    if (m_iCurFrame == 21)
        m_eCurState = STATE::skill_1300;
}

void Boss_Dellons_FSM::skill_1200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1200", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    SOUND.Play_Sound(L"Dellons_att_s_04", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_1300()
{
    if (Init_CurFrame(9))
        Add_And_Set_Effect(L"Dellons_1300");

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(8))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        AttackCollider_On(NORMAL_ATTACK, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamageMul * 0.2f);
    }
    else if (Init_CurFrame(33))
    {
        AttackCollider_Off();
    }

    if (m_iCurFrame == 19)
        m_eCurState = STATE::skill_1400;
}

void Boss_Dellons_FSM::skill_1300_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1300", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    SOUND.Play_Sound(L"Dellons_att_s_05", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_1400()
{
    if (Init_CurFrame(8))
        Add_And_Set_Effect(L"Dellons_1400");
    if (Init_CurFrame(14))
        Add_And_Set_Effect(L"Dellons_1400_1");

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());


    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(8))
    {
        SOUND.Play_Sound(L"swing_hammer_02", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        AttackCollider_On(NORMAL_ATTACK, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamageMul * 0.2f);
    }
    else if (Init_CurFrame(14))
        AttackCollider_Off();
    else if (Init_CurFrame(16))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        AttackCollider_On(KNOCKBACK_ATTACK, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamageMul * 0.2f);
    }
    else if (Init_CurFrame(24))
        AttackCollider_Off();

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::skill_1400_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1400", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    SOUND.Play_Sound(L"Dellons_att_l_03", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_91100()
{
    if (Init_CurFrame(0))
        Add_Effect(L"Dellons_91100");
    else if (Init_CurFrame(17))
        Add_And_Set_Effect(L"Dellons_91100_End");

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());


    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::skill_91100_Init()
{
    m_vTurnVector = MathUtils::Get_RandomVector(_float3{ -1.f,0.f,-1.f }, _float3{ 1.f,0.f,1.f });
    m_vTurnVector.Normalize();

    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_91100", 0.15f, false, m_fEvade_AnimationSpeed);

    SOUND.Play_Sound(L"Dellons_att_s_06", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    AttackCollider_Off();
    
    m_bInvincible = true;
    m_bSuperArmor = false;
    m_bEvade = false;
}

void Boss_Dellons_FSM::skill_93100()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::skill_93100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_93100", 0.15f, false, m_fEvade_AnimationSpeed);

    SOUND.Play_Sound(L"Dellons_att_s_01", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_100100()
{
    if (Init_CurFrame(10))
    {
        SOUND.Play_Sound(L"Skill_05_A_Dellons_02", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_100100");
    }

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(12))
    {
        FORWARDMOVINGSKILLDESC desc;
        desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
        desc.fMoveSpeed = 0.f;
        desc.fLifeTime = 0.5f;
        desc.fLimitDistance = 0.f;

        _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
            Get_Transform()->Get_State(Transform_State::LOOK) * 2.f +
            _float3::Up;

        Create_ForwardMovingSkillCollider(Monster_Skill, L"Monster_SkillCollider", vSkillPos, 1.5f, desc, NORMAL_ATTACK, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).Skill1DamageMul * 0.5f
            , L"Hit_Slash_Dark");
    }

    if (m_iCurFrame == 27)
        m_eCurState = STATE::skill_100200;

    Dead_Setting();
}

void Boss_Dellons_FSM::skill_100100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;


    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_100200()
{
    if (Init_CurFrame(0))
        Add_And_Set_Effect(L"Dellons_100200_Start");
    if (Init_CurFrame(18))
        Add_And_Set_Effect(L"Dellons_100200");

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(15))
    {
        SOUND.Play_Sound(L"hit_explosive_bomb_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio , Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        FORWARDMOVINGSKILLDESC desc;
        desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
        desc.fMoveSpeed = 0.f;
        desc.fLifeTime = 0.5f;
        desc.fLimitDistance = 0.f;

        _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
            Get_Transform()->Get_State(Transform_State::LOOK) * 2.f +
            _float3::Up;
        Create_ForwardMovingSkillCollider(Monster_Skill, L"Monster_SkillCollider", vSkillPos, 1.5f, desc, AIRBORNE_ATTACK, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).Skill1DamageMul * 0.5f
            , L"Hit_Slash_Dark");
    }
 
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    Dead_Setting();
}

void Boss_Dellons_FSM::skill_100200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100200", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_200100()
{
    if (Init_CurFrame(5))
        Add_And_Set_Effect(L"Dellons_200100");

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(7))
    {
        SOUND.Play_Sound(L"Skill_08_A_Dellons_02", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        AttackCollider_On(KNOCKBACK_ATTACK, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).Skill2DamageMul * 0.5f);
    }
    else if (Init_CurFrame(12))
        AttackCollider_Off();


    if (m_iCurFrame == 21)
        m_eCurState = STATE::skill_200200;

    Dead_Setting();
}

void Boss_Dellons_FSM::skill_200100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;


    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_200200()
{
    if (Init_CurFrame(5))
        Add_And_Set_Effect(L"Dellons_200200");

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(7))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        FORWARDMOVINGSKILLDESC desc;
        desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
        desc.fMoveSpeed = 0.f;
        desc.fLifeTime = 1.f;
        desc.fLimitDistance = 0.f;

        _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 3.f + _float3::Up;
        Create_ForwardMovingSkillCollider(Monster_Attack, L"Monster_SkillCollider", vSkillPos, 2.f, desc, KNOCKBACK_SKILL, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).Skill2DamageMul * 0.5f
            , L"Hit_Slash_Dark");
    }

    if (Is_AnimFinished())
    {
        m_eCurState = STATE::b_idle;
    }


    Dead_Setting();
}

void Boss_Dellons_FSM::skill_200200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200200", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_300100()
{
    if (Init_CurFrame(9))
        Add_Effect(L"Dellons_300100");
    if (Init_CurFrame(24))
        Add_Effect(L"Dellons_300100_Wraith");
    if (Init_CurFrame(57))
        Add_Effect(L"Dellons_300100_Wraith2");

    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (m_iCurFrame == 9)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            Summon_Wraith();

            Set_WraithState((_uint)Boss_DellonsWraith_FSM::STATE::FX_DellonsWraith_skill_30010);
        }
    }

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
    
    Dead_Setting();
}

void Boss_Dellons_FSM::skill_300100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_300100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    SOUND.Play_Sound(L"Skill_07_A_Dellons", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Calculate_SkillCamRight();
}

void Boss_Dellons_FSM::skill_400100()
{
    if (Init_CurFrame(17))
    {
        SOUND.Play_Sound(L"dark_magic_swing_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100_charging");
    }
    else if (Init_CurFrame(31))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100");
    }
    else if (Init_CurFrame(39))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100_2");
    }
    else if (Init_CurFrame(50))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100_3");
    }
    else if (Init_CurFrame(58))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100_9");
    }
    else if (Init_CurFrame(63))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100_4");
    }
    else if (Init_CurFrame(73))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100_5");
    }
    else if (Init_CurFrame(80))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100_6");
    }
    else if (Init_CurFrame(98))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100_7");
    }
    else if (Init_CurFrame(99))
    {
        SOUND.Play_Sound(L"swing_hammer_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_And_Set_Effect(L"Dellons_400100_8");
    }

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (m_iCurFrame == 20)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            Summon_Wraith();

            Set_WraithState((_uint)Boss_DellonsWraith_FSM::STATE::FX_Mn_Dellons_skill_5100);
        }
    }
    else if (m_iCurFrame == 33 ||
             m_iCurFrame == 40 ||
             m_iCurFrame == 47 ||
             m_iCurFrame == 60 ||
             m_iCurFrame == 67 ||
             m_iCurFrame == 72)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 3.5f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
            Create_ForwardMovingSkillCollider(Monster_Skill, L"Boss_Dellons_SkillCollider", vSkillPos, 1.f, desc, NORMAL_ATTACK, 
                DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage * DATAMGR.Get_MonsterData(MONSTER::DELLONS).Skill4DamageMul * 0.1f);
        }
    }
    else if (Init_CurFrame(99))
    {
        SOUND.Play_Sound(L"swing_axe_02_gigan", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
        SOUND.Play_Sound(L"Dellons_att_s_03", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        FORWARDMOVINGSKILLDESC desc;
        desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
        desc.fMoveSpeed = 20.f;
        desc.fLifeTime = 1.f;
        desc.fLimitDistance = 5.f;

        _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * -0.5f + _float3::Up;
        Create_ForwardMovingSkillCollider(Monster_Skill, L"Boss_Dellons_SkillCollider", vSkillPos, 2.f, desc, KNOCKDOWN_SKILL, 
            DATAMGR.Get_MonsterData(MONSTER::DELLONS).AttackDamage* DATAMGR.Get_MonsterData(MONSTER::DELLONS).Skill4DamageMul * 0.2f);
    }

    if (m_iCurFrame >= 110)
        m_eCurState = STATE::b_idle;
    
    Dead_Setting();
}

void Boss_Dellons_FSM::skill_400100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_400100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    SOUND.Play_Sound(L"Skill_06_A_Dellons_02", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Calculate_SkillCamRight();
}

void Boss_Dellons_FSM::skill_501100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);


    if (Init_CurFrame(4))
    {
        //Summon Wraith
        SOUND.Play_Sound(L"dark_magic_swing_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

        Add_Effect(L"Dellons_501100");

        Summon_Wraith();

        Set_WraithState((_uint)Boss_DellonsWraith_FSM::STATE::FX_Mn_Dellons_skill_500200);
    }

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
    
    Dead_Setting();
}

void Boss_Dellons_FSM::skill_501100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_501100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    SOUND.Play_Sound(L"Skill_04_A_Dellons_02", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fMonsterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::Battle_Start()
{
    if (!m_bBattleStart)
    {
        m_tBattleStartTime.fAccTime += fDT;

        if (m_tBattleStartTime.fAccTime >= m_tBattleStartTime.fCoolTime)
            m_bBattleStart = true;
    }
}


void Boss_Dellons_FSM::Summon_Wraith()
{
    if (!m_pDellonsWraith.expired())
        EVENTMGR.Delete_Object(m_pDellonsWraith.lock());

    //Add. Player's Wraith
    shared_ptr<GameObject> ObjWraith = make_shared<GameObject>();

    ObjWraith->Add_Component(make_shared<Transform>());
    {
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> renderer = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Dellons_Wraith");
            renderer->Set_Model(model);
        }

        ObjWraith->Add_Component(renderer);

    }
    ObjWraith->Add_Component(make_shared<Boss_DellonsWraith_FSM>());
    ObjWraith->Get_FSM()->Set_Target(m_pOwner.lock());
    ObjWraith->Get_FSM()->Init();
    ObjWraith->Set_Name(L"Boss_Dellons_Wraith");

    EVENTMGR.Create_Object(ObjWraith);

    m_pDellonsWraith = ObjWraith;
}

void Boss_Dellons_FSM::Set_WraithState(_uint iAnimindex)
{
    if (!m_pDellonsWraith.expired())
        m_pDellonsWraith.lock()->Get_FSM()->Set_State(iAnimindex);
}

void Boss_Dellons_FSM::Calculate_SkillCamRight()
{
    _float4 vDir = _float4(0.f);
    vDir = m_pCamera.lock()->Get_Transform()->Get_State(Transform_State::POS) - Get_Transform()->Get_State(Transform_State::POS);
    vDir.y = 0.f;
    vDir.Normalize();

    _float4 vCross = _float4(0.f);
    vCross = XMVector3Cross(Get_Transform()->Get_State(Transform_State::LOOK), vDir);

    if (XMVectorGetY(vCross) < 0.f) // LEFT 
        m_vSkillCamRight = (Get_Transform()->Get_State(Transform_State::RIGHT) * -3.f);
    else //RIGHT	
        m_vSkillCamRight = (Get_Transform()->Get_State(Transform_State::RIGHT) * 3.f);
}

void Boss_Dellons_FSM::Set_AttackSkill_Phase1()
{
   _uint iRan = rand() % 6;

    while (true)
    {
        if (iRan == m_iPreAttack)
            iRan = rand() % 6;
        else
            break;
    }

    if (iRan == 0)
    {
        m_fAttackRange = 2.f;
        m_ePatternState = STATE::skill_1100;
        m_iPreAttack = 0;
    }
    else if (iRan == 1)
    {
        m_fAttackRange = 3.f;
        m_ePatternState = STATE::skill_100100;
        m_iPreAttack = 1;
    }
    else if (iRan == 2)
    {
        m_fAttackRange = 5.f;
        m_ePatternState = STATE::skill_200100;
        m_iPreAttack = 2;
    }
    else if (iRan == 3)
    {
        m_fAttackRange = 5.f;
        m_ePatternState = STATE::skill_300100;
        m_iPreAttack = 3;
    }
    else if (iRan == 4)
    {
        m_fAttackRange = 2.5f;
        m_ePatternState = STATE::skill_400100;
        m_iPreAttack = 4;
    }
    else if (iRan == 5)
    {
        m_fAttackRange = 10.f;
        m_ePatternState = STATE::skill_501100;
        m_iPreAttack = 5;
    }

    m_bSetPattern = true;
}

void Boss_Dellons_FSM::Dead_Setting()
{
    if (m_bIsDead)
    {
        m_bInvincible = true;

        m_eCurState = STATE::die;
    }
}


_float3 Boss_Dellons_FSM::Calculate_TargetTurnVector()
{
    if (m_pTarget.expired())
        return _float3(0.f);
    else
        return m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz() - m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
}
