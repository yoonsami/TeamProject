#include "pch.h"
#include "ModelAnimator.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Material.h" 
#include "ModelAnimation.h"
#include "InputMgr.h"
#include "Camera.h"
#include "Light.h"

ModelAnimator::ModelAnimator(shared_ptr<Shader> shader)
	:Component(COMPONENT_TYPE::Animator)
	, m_pShader(shader)
{

}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::Tick()
{
	m_TweenDesc.curr.sumTime += fDT;
	//현재 애니메이션
	{
		shared_ptr<ModelAnimation> currentAnim = m_pModel->Get_AnimationByIndex(m_TweenDesc.curr.animIndex);
		if (currentAnim)
		{
			_float timePerFrame = 1 / (currentAnim->frameRate * currentAnim->speed);
			if (m_TweenDesc.curr.sumTime >= timePerFrame)
			{
				m_TweenDesc.curr.sumTime = 0;
				
				if(!m_bFinished)
				{
					m_TweenDesc.curr.currentFrame = (m_TweenDesc.curr.currentFrame + 1) % currentAnim->frameCount;
					m_TweenDesc.curr.nextFrame = (m_TweenDesc.curr.currentFrame + 1);
				}
				else if(m_bFinished && m_TweenDesc.curr.nextFrame == currentAnim->frameCount -1)
				{
					if (m_iNextAnimation > 0)
					{
						m_TweenDesc.curr.animIndex = m_iNextAnimation;
						m_iNextAnimation = -1;
						m_TweenDesc.curr.currentFrame = 0;
						m_TweenDesc.curr.nextFrame = 0;
						m_bFinished = false;
					}
				}
				// 다음 프레임이 끝프레임과 같을 때 ( 끝났을 때 )


				if (m_TweenDesc.curr.nextFrame == currentAnim->frameCount)
				{
					// 다음 애니메이션이 없을 때
					if(m_iNextAnimation<0 && m_TweenDesc.next.animIndex <0)
					{
						// 루프 애니메이션이면
						if (currentAnim->loop)
						{
							// 그대로 진행
							m_TweenDesc.curr.nextFrame %= currentAnim->frameCount;
						}
						// 루프 애니메이션 아니면
						else
						{
							// 마지막프레임 -1 에 고정 후 m_bFinished true
							m_TweenDesc.curr.currentFrame = currentAnim->frameCount - 1;
							m_TweenDesc.curr.nextFrame = currentAnim->frameCount - 1;
							m_bFinished = true;
						}
					}
					// 다음 애니메이션 있을 때,
					else
					{
						if(m_iNextAnimation <0)
						{
							m_TweenDesc.curr.animIndex = m_iNextAnimation;
							m_iNextAnimation = -1;
							m_TweenDesc.curr.currentFrame = 0;
							m_TweenDesc.curr.nextFrame = 0;
							m_bFinished = false;
						}
						else
						{
							m_TweenDesc.curr = m_TweenDesc.next;
							m_TweenDesc.ClearNextAnim();
							m_bFinished = false;
						}
						
					}
				}
			}

			m_TweenDesc.curr.ratio = (m_TweenDesc.curr.sumTime / timePerFrame);
		}
	}


	//다음 애니메이션
	if (m_TweenDesc.next.animIndex >= 0)
	{
		m_TweenDesc.tweenSumTime += fDT;
		m_TweenDesc.tweenRatio = m_TweenDesc.tweenSumTime / m_TweenDesc.tweenDuration;

		if (m_TweenDesc.tweenRatio >= 1.f || m_bFinished)
		{
			m_TweenDesc.curr = m_TweenDesc.next;
			m_TweenDesc.ClearNextAnim();
			m_bFinished = false;

		}
		else
		{
			shared_ptr<ModelAnimation> nextAnim = m_pModel->Get_AnimationByIndex(m_TweenDesc.next.animIndex);
			m_TweenDesc.next.sumTime += fDT;

			_float timePerFrame = 1.f / (nextAnim->frameRate * nextAnim->speed);

			if (m_TweenDesc.next.ratio >= 1.f)
			{
				m_TweenDesc.next.sumTime = 0;
				m_TweenDesc.next.currentFrame = (m_TweenDesc.next.currentFrame + 1) % nextAnim->frameCount;
				m_TweenDesc.next.nextFrame = (m_TweenDesc.next.currentFrame + 1) % nextAnim->frameCount;
			}

			m_TweenDesc.next.ratio = m_TweenDesc.next.sumTime / timePerFrame;
		}
	}

	if (m_pModel->Get_AnimationCount() > 0)
		m_TweenDesc.curr.animIndex %= m_pModel->Get_AnimationCount();

	_float3 vDistToMove;

	auto& preAnimRootPositions = m_pModel->Get_RootBonePosition()[m_preTweenDesc.curr.animIndex];
	auto& curAnimRootPositions = m_pModel->Get_RootBonePosition()[m_TweenDesc.curr.animIndex];
	{
		if(m_TweenDesc.curr.currentFrame <= m_TweenDesc.curr.nextFrame)
		{
			_float3 vPrePosition = _float3::Lerp(preAnimRootPositions[m_preTweenDesc.curr.currentFrame], preAnimRootPositions[m_preTweenDesc.curr.nextFrame], m_preTweenDesc.curr.ratio);
			_float3 vCurPosition = _float3::Lerp(curAnimRootPositions[m_TweenDesc.curr.currentFrame], curAnimRootPositions[m_TweenDesc.curr.nextFrame], m_TweenDesc.curr.ratio);
			vDistToMove = vCurPosition - vPrePosition;
		}
		else
			vDistToMove = _float3(0.f);
		

	}
	

	if (m_TweenDesc.next.animIndex >= 0)
	{
		auto& nextAnimRootPositions = m_pModel->Get_RootBonePosition()[m_TweenDesc.next.animIndex];

		if(m_preTweenDesc.next.animIndex >=0)
		{
			auto& preNextAnimRootPositions = m_pModel->Get_RootBonePosition()[m_preTweenDesc.next.animIndex];

			_float3 vPrePosition = _float3::Lerp(preAnimRootPositions[m_preTweenDesc.next.currentFrame], preAnimRootPositions[m_preTweenDesc.next.nextFrame], m_preTweenDesc.next.ratio);
			_float3 vCurPosition = _float3::Lerp(curAnimRootPositions[m_TweenDesc.next.currentFrame], curAnimRootPositions[m_TweenDesc.next.nextFrame], m_TweenDesc.next.ratio);
		
			vDistToMove = _float3::Lerp(vDistToMove, vCurPosition - vPrePosition, m_TweenDesc.tweenRatio);
					
		}
		else
		{
			auto& preNextAnimRootPositions = m_pModel->Get_RootBonePosition()[m_TweenDesc.next.animIndex];

			_float3 vPrePosition = _float3::Lerp(preAnimRootPositions[m_preTweenDesc.next.currentFrame], preAnimRootPositions[m_preTweenDesc.next.nextFrame], m_preTweenDesc.next.ratio);
			_float3 vCurPosition = _float3::Lerp(curAnimRootPositions[m_TweenDesc.next.currentFrame], curAnimRootPositions[m_TweenDesc.next.nextFrame], m_TweenDesc.next.ratio);

			vDistToMove = _float3::Lerp(vDistToMove, vCurPosition - vPrePosition, m_TweenDesc.tweenRatio);


		}
	}


	Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + (vDistToMove));

	m_preTweenDesc = m_TweenDesc;
	//Cal_AnimTransform();
}

void ModelAnimator::Set_NextAnim(_int index)
{
	if (m_bFinished)
	{
		m_TweenDesc.curr.animIndex = index;
		m_iNextAnimation = -1;
		m_TweenDesc.curr.currentFrame = 0;
		m_TweenDesc.curr.nextFrame = 0;
		m_bFinished = false;
	}
	else
	{
		m_iNextAnimation = index; 
		m_bFinished = false;
	}
	
}

void ModelAnimator::Set_Model(shared_ptr<Model> model)
{
	m_pModel = model;

	const auto& materials = m_pModel->Get_Materials();

	for (auto& material : materials)
	{
		material->Set_Shader(m_pShader);
	}
}

void ModelAnimator::Render()
{
	if (!m_pModel)
		return;

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());

	{
		m_pShader->Push_TweenData(m_TweenDesc);

		m_pShader->GetSRV("TransformMap")->SetResource(m_pModel->Get_TransformSRV().Get());

		auto& desc = m_pModel->Get_AnimAddonDesc();

		m_pShader->Push_AnimAddonData(desc);
	}

	m_pShader->Push_RenderParamData(m_RenderParams);


	auto world = Get_Transform()->Get_WorldMatrix();
	m_pShader->Push_TransformData(TransformDesc{ world });

	const auto& meshes = m_pModel->Get_Meshes();

	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();
		_float4 lineColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pShader->GetVector("g_LineColor")->SetFloatVector((_float*)(&lineColor));
		m_pShader->GetScalar("g_LineThickness")->SetFloat(Model::m_fOutlineThickness);
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pShader->DrawIndexed(1, PS_ANIM, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
		m_pShader->DrawIndexed(0, PS_ANIM, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
	}
}

void ModelAnimator::Render_Instancing(shared_ptr<class InstancingBuffer>& buffer, shared_ptr<InstanceTweenDesc> desc, shared_ptr<InstanceRenderParamDesc> renderParamDesc)
{
	if (!m_pModel)
		return;

	{
		m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());
	}

	m_pShader->Push_InstanceTweenData(*desc);
	m_pShader->Push_InstanceRenderParamData(*renderParamDesc);
	m_pShader->GetSRV("TransformMap")->SetResource(m_pModel->Get_TransformSRV().Get());

	auto& animAddonDesc = m_pModel->Get_AnimAddonDesc();

	m_pShader->Push_AnimAddonData(animAddonDesc);

	const auto& meshes = m_pModel->Get_Meshes();

	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();

		buffer->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_float4 lineColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pShader->GetVector("g_LineColor")->SetFloatVector((_float*)(&lineColor));
		m_pShader->GetScalar("g_LineThickness")->SetFloat(Model::m_fOutlineThickness);
		
		m_pShader->DrawIndexedInstanced(1, PS_ANIMINSTANCING, mesh->indexBuffer->Get_IndicesNum(), buffer->Get_Count());

		m_pShader->DrawIndexedInstanced(0, PS_ANIMINSTANCING, mesh->indexBuffer->Get_IndicesNum(), buffer->Get_Count());
	}
}

void ModelAnimator::Render_Shadow()
{
	if (!m_pModel)
		return;

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());

	{
		m_pShader->Push_TweenData(m_TweenDesc);

		m_pShader->GetSRV("TransformMap")->SetResource(m_pModel->Get_TransformSRV().Get());

		auto& desc = m_pModel->Get_AnimAddonDesc();

		m_pShader->Push_AnimAddonData(desc);
	}

	auto world = Get_Transform()->Get_WorldMatrix();
	m_pShader->Push_TransformData(TransformDesc{ world });

	const auto& meshes = m_pModel->Get_Meshes();

	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pShader->DrawIndexed(0, PS_ANIM_SHADOW, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
	}
}

void ModelAnimator::Render_Shadow_Instancing(shared_ptr<InstancingBuffer>& buffer, shared_ptr<InstanceTweenDesc> desc, shared_ptr<InstanceRenderParamDesc> renderParamDesc)
{
	if (!m_pModel)
		return;

	{
		m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());
	}

	m_pShader->Push_InstanceTweenData(*desc);
	m_pShader->Push_InstanceRenderParamData(*renderParamDesc);

	m_pShader->GetSRV("TransformMap")->SetResource(m_pModel->Get_TransformSRV().Get());

	auto& animAddonDesc = m_pModel->Get_AnimAddonDesc();

	m_pShader->Push_AnimAddonData(animAddonDesc);


	const auto& meshes = m_pModel->Get_Meshes();

	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();

		buffer->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pShader->DrawIndexedInstanced(0, PS_ANIM_SHADOW_INSTANCING, mesh->indexBuffer->Get_IndicesNum(), buffer->Get_Count());
	}
}

InstanceID ModelAnimator::Get_InstanceID()
{
	return make_pair(_ulonglong(m_pModel.get()),_ulonglong(m_pShader.get()));
}

void ModelAnimator::Cal_AnimTransform()
{
	//vector<_float4x4> tmpAnimBoneTransforms(MAX_MODEL_TRANSFORMS, _float4x4::Identity);

	//shared_ptr<ModelAnimation> currAnim = m_pModel->Get_AnimationByIndex(m_TweenDesc.curr.animIndex);

	////for (_uint frame = 0; frame < animation->frameCount; ++frame)
	//{
	//	for (_uint boneIndex = 0; boneIndex < m_pModel->Get_BoneCount(); ++boneIndex)
	//	{
	//		shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(boneIndex);

	//		_float4x4 matAnimation;

	//		shared_ptr<ModelKeyFrame> keyFrame = currAnim->Get_KeyFrame(bone->name);
	//		if (keyFrame)
	//		{
	//			ModelKeyFrameData& currData = keyFrame->transforms[m_TweenDesc.curr.currentFrame];
	//			ModelKeyFrameData& nextData = keyFrame->transforms[m_TweenDesc.curr.nextFrame];

	//			_float3 vScale = _float3::Lerp(currData.scale,nextData.scale,m_TweenDesc.curr.ratio);
	//			_float3 vTrans = _float3::Lerp(currData.translation,nextData.translation,m_TweenDesc.curr.ratio);
	//			Quaternion vRot = Quaternion::Slerp(currData.rotation,nextData.rotation,m_TweenDesc.curr.ratio);

	//			DirectX::XMStoreFloat4x4(&matAnimation, XMMatrixAffineTransformation(XMLoadFloat3(&(vScale)), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRot), XMLoadFloat3(&vTrans)));
	//		}
	//		else
	//		{
	//			matAnimation = _float4x4::Identity;
	//		}

	//		if (m_TweenDesc.next.animIndex >= 0)
	//		{
	//			shared_ptr<ModelAnimation> nextAnim = m_pModel->Get_AnimationByIndex(m_TweenDesc.next.animIndex);

	//			shared_ptr<ModelKeyFrame> keyFrame = nextAnim->Get_KeyFrame(bone->name);
	//			if (keyFrame)
	//			{
	//				ModelKeyFrameData& currData = keyFrame->transforms[m_TweenDesc.next.currentFrame];
	//				ModelKeyFrameData& nextData = keyFrame->transforms[m_TweenDesc.next.nextFrame];

	//				_float3 vScale = _float3::Lerp(currData.scale, nextData.scale, m_TweenDesc.next.ratio);
	//				_float3 vTrans = _float3::Lerp(currData.translation, nextData.translation, m_TweenDesc.next.ratio);
	//				Quaternion vRot = Quaternion::Slerp(currData.rotation, nextData.rotation, m_TweenDesc.next.ratio);


	//				_float3 curScale, curTrans; Quaternion curRot;
	//				matAnimation.Decompose(curScale, curRot, curTrans);

	//				vScale = _float3::Lerp(curScale, vScale, m_TweenDesc.tweenRatio);
	//				vTrans = _float3::Lerp(curTrans, vTrans, m_TweenDesc.tweenRatio);
	//				vRot = Quaternion::Slerp(curRot, vRot, m_TweenDesc.tweenRatio);

	//				DirectX::XMStoreFloat4x4(&matAnimation, XMMatrixAffineTransformation(XMLoadFloat3(&(vScale)), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRot), XMLoadFloat3(&vTrans)));
	//			}
	//			else 
	//				matAnimation = _float4x4::Identity;
	//		}

	//		//_float4x4 toRootMat = bone->transform;
	//		_float4x4 invGlobal = bone->offsetTransform;

	//		_int parentIndex = bone->parentIndex;

	//		_float4x4 matParent = _float4x4::Identity;
	//		if (parentIndex >= 0)
	//			matParent = tmpAnimBoneTransforms[parentIndex];

	//		tmpAnimBoneTransforms[boneIndex] = matAnimation * matParent;

	//		m_BoneTransformData[boneIndex] = invGlobal * tmpAnimBoneTransforms[boneIndex] * _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI);
	//	}
	//}

	//shared_ptr<ModelAnimation> currAnim = m_pModel->Get_AnimationByIndex(m_TweenDesc.curr.animIndex);

	//{
	//	for (_uint boneIndex = 0; boneIndex < m_pModel->Get_BoneCount(); ++boneIndex)
	//	{
	//		
	//		shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(boneIndex);

	//		_float4x4 curMat = m_pModel->Get_AnimationTransform(m_TweenDesc.curr.animIndex,m_TweenDesc.curr.currentFrame,boneIndex);
	//		_float4x4 nextMat = m_pModel->Get_AnimationTransform(m_TweenDesc.curr.animIndex,m_TweenDesc.curr.nextFrame,boneIndex);

	//		_float4x4 matAnimation = Transform::SLerpMatrix(curMat, nextMat, m_TweenDesc.curr.ratio);

	//		if (m_TweenDesc.next.animIndex >= 0)
	//		{
	//			curMat = m_pModel->Get_AnimationTransform(m_TweenDesc.next.animIndex, m_TweenDesc.next.currentFrame, boneIndex);
	//			nextMat = m_pModel->Get_AnimationTransform(m_TweenDesc.next.animIndex, m_TweenDesc.next.nextFrame, boneIndex);

	//			_float4x4 matNext = Transform::SLerpMatrix(curMat, nextMat, m_TweenDesc.next.ratio);

	//			matAnimation = Transform::SLerpMatrix(matAnimation, matNext, m_TweenDesc.tweenRatio);
	//		}

	//		//_float4x4 toRootMat = bone->transform;
	//		_float4x4 invGlobal = bone->offsetTransform;

	//		m_BoneTransformData[boneIndex] = invGlobal * matAnimation * _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI);
	//	}
	//}
}

_float4x4 ModelAnimator::Get_CurAnimTransform(_int boneIndex)
{
	shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(boneIndex);

	_float4x4 curMat = m_pModel->Get_AnimationTransform(m_TweenDesc.curr.animIndex, m_TweenDesc.curr.currentFrame, boneIndex);
	_float4x4 nextMat = m_pModel->Get_AnimationTransform(m_TweenDesc.curr.animIndex, m_TweenDesc.curr.nextFrame, boneIndex);

	_float4x4 matAnimation = Transform::SLerpMatrix(curMat, nextMat, m_TweenDesc.curr.ratio);

	if (m_TweenDesc.next.animIndex >= 0)
	{
		curMat = m_pModel->Get_AnimationTransform(m_TweenDesc.next.animIndex, m_TweenDesc.next.currentFrame, boneIndex);
		nextMat = m_pModel->Get_AnimationTransform(m_TweenDesc.next.animIndex, m_TweenDesc.next.nextFrame, boneIndex);

		_float4x4 matNext = Transform::SLerpMatrix(curMat, nextMat, m_TweenDesc.next.ratio);

		matAnimation = Transform::SLerpMatrix(matAnimation, matNext, m_TweenDesc.tweenRatio);
	}

	_float4x4 invGlobal = bone->offsetTransform;

	return matAnimation;

}

void ModelAnimator::Set_NextTweenAnim(const wstring& animName, _float tweenDuration, _bool loop, _float animSpeed)
{
	_int animIndex = -1;
	auto anim = Get_Model()->Get_AnimationByName(animName, &animIndex);
	assert(animIndex >= 0);

	m_TweenDesc.ClearNextAnim();
	m_TweenDesc.next.animIndex = animIndex;
	m_TweenDesc.tweenDuration = tweenDuration;

	anim->loop = loop;
	anim->speed = animSpeed;
}

void ModelAnimator::Set_CurrentAnim(const wstring& animName, _bool loop, _float animSpeed)
{
	_int animIndex = -1;
	auto anim = Get_Model()->Get_AnimationByName(animName, &animIndex);
	assert(animIndex >= 0);

	m_TweenDesc.ClearNextAnim();
	Set_CurrentAnim(animIndex);

	anim->loop = loop;
	anim->speed = animSpeed;
}

_uint ModelAnimator::Get_FinalFrame(_int animIndex)
{
	return m_pModel->Get_AnimationByIndex(animIndex)->frameCount;
}
