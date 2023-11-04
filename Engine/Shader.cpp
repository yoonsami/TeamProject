#include "pch.h"
#include "Shader.h"
#include "Utils.h"

Shader::Shader()
	: ResourceBase(ResourceType::Shader)
{

	
}

Shader::~Shader()
{
	
}

void Shader::Load(const wstring& path)
{
	_file = (L"..\\Shaders\\" + path); 
	m_strShaderTag = (path);

		_initialStateBlock = make_shared<StateBlock>();
	{
		CONTEXT->RSGetState(_initialStateBlock->RSRasterizerState.GetAddressOf());
		CONTEXT->OMGetBlendState(_initialStateBlock->OMBlendState.GetAddressOf(), _initialStateBlock->OMBlendFactor, &_initialStateBlock->OMSampleMask);
		CONTEXT->OMGetDepthStencilState(_initialStateBlock->OMDepthStencilState.GetAddressOf(), &_initialStateBlock->OMStencilRef);
	}

	CreateEffect();
}

void Shader::CreateEffect()
{
	_shaderDesc = ShaderManager::GetEffect(_file);

	_shaderDesc.effect->GetDesc(&_effectDesc);
	for (UINT t = 0; t < _effectDesc.Techniques; t++)
	{
		Technique technique;
		technique.technique = _shaderDesc.effect->GetTechniqueByIndex(t);
		technique.technique->GetDesc(&technique.desc);
		technique.name = Utils::ToWString(technique.desc.Name);

		for (UINT p = 0; p < technique.desc.Passes; p++)
		{
			Pass pass;
			pass.pass = technique.technique->GetPassByIndex(p);
			pass.pass->GetDesc(&pass.desc);
			pass.name = Utils::ToWString(pass.desc.Name);
			pass.pass->GetVertexShaderDesc(&pass.passVsDesc);
			pass.passVsDesc.pShaderVariable->GetShaderDesc(pass.passVsDesc.ShaderIndex, &pass.effectVsDesc);

			for (UINT s = 0; s < pass.effectVsDesc.NumInputSignatureEntries; s++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC desc;

				HRESULT hr = pass.passVsDesc.pShaderVariable->GetInputSignatureElementDesc(pass.passVsDesc.ShaderIndex, s, &desc);


				pass.signatureDescs.push_back(desc);
			}

			pass.inputLayout = CreateInputLayout(_shaderDesc.blob, &pass.effectVsDesc, pass.signatureDescs);
			pass.stateBlock = _initialStateBlock;

			technique.passes.push_back(pass);
		}

		_techniques.push_back(technique);
	}

	for (UINT i = 0; i < _effectDesc.ConstantBuffers; i++)
	{
		ID3DX11EffectConstantBuffer* iBuffer;
		iBuffer = _shaderDesc.effect->GetConstantBufferByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		iBuffer->GetDesc(&vDesc);
	}

	for (UINT i = 0; i < _effectDesc.GlobalVariables; i++)
	{
		ID3DX11EffectVariable* effectVariable;
		effectVariable = _shaderDesc.effect->GetVariableByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		effectVariable->GetDesc(&vDesc);
	}
}

ComPtr<ID3D11InputLayout> Shader::CreateInputLayout(ComPtr<ID3DBlob> fxBlob, D3DX11_EFFECT_SHADER_DESC* effectVsDesc, vector<D3D11_SIGNATURE_PARAMETER_DESC>& params)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

	for (D3D11_SIGNATURE_PARAMETER_DESC& paramDesc : params)
	{
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string name = paramDesc.SemanticName;
		transform(name.begin(), name.end(), name.begin(), toupper);

		if (name == "POSITION")
		{
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}

		if (Utils::StartsWith(name, "INST") == true)
		{
			elementDesc.InputSlot = 1;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}

		if (Utils::StartsWith(name, "SV_") == false)
			inputLayoutDesc.push_back(elementDesc);
	}

	const void* code = effectVsDesc->pBytecode;
	UINT codeSize = effectVsDesc->BytecodeLength;

	if (inputLayoutDesc.size() > 0)
	{
		ComPtr<ID3D11InputLayout> inputLayout;

		HRESULT hr = DEVICE->CreateInputLayout
		(
			&inputLayoutDesc[0]
			, _uint(inputLayoutDesc.size())
			, code
			, codeSize
			, inputLayout.GetAddressOf()
		);



		return inputLayout;
	}

	return nullptr;
}

void Shader::Draw(UINT technique, UINT pass, UINT vertexCount, UINT startVertexLocation)
{
	_techniques[technique].passes[pass].Draw(vertexCount, startVertexLocation);
}

void Shader::DrawIndexed(UINT technique, UINT pass, UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	_techniques[technique].passes[pass].DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Shader::DrawInstanced(UINT technique, UINT pass, UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	_techniques[technique].passes[pass].DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
}

void Shader::DrawIndexedInstanced(UINT technique, UINT pass, UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	_techniques[technique].passes[pass].DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void Shader::Dispatch(UINT technique, UINT pass, UINT x, UINT y, UINT z)
{
	_techniques[technique].passes[pass].Dispatch(x, y, z);
}

ComPtr<ID3DX11EffectVariable> Shader::GetVariable(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str());
}

ComPtr<ID3DX11EffectScalarVariable> Shader::GetScalar(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsScalar();
}

ComPtr<ID3DX11EffectVectorVariable> Shader::GetVector(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsVector();
}

ComPtr<ID3DX11EffectMatrixVariable> Shader::GetMatrix(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsMatrix();
}

ComPtr<ID3DX11EffectStringVariable> Shader::GetString(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsString();
}

ComPtr<ID3DX11EffectShaderResourceVariable> Shader::GetSRV(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsShaderResource();
}

ComPtr<ID3DX11EffectRenderTargetViewVariable> Shader::GetRTV(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsRenderTargetView();
}

ComPtr<ID3DX11EffectDepthStencilViewVariable> Shader::GetDSV(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsDepthStencilView();
}

ComPtr<ID3DX11EffectConstantBuffer> Shader::GetConstantBuffer(string name)
{
	return _shaderDesc.effect->GetConstantBufferByName(name.c_str());
}

ComPtr<ID3DX11EffectShaderVariable> Shader::GetShader(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsShader();
}

ComPtr<ID3DX11EffectBlendVariable> Shader::GetBlend(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsBlend();
}

ComPtr<ID3DX11EffectDepthStencilVariable> Shader::GetDepthStencil(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsDepthStencil();
}

ComPtr<ID3DX11EffectRasterizerVariable> Shader::GetRasterizer(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsRasterizer();
}

ComPtr<ID3DX11EffectSamplerVariable> Shader::GetSampler(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsSampler();
}

ComPtr<ID3DX11EffectUnorderedAccessViewVariable> Shader::GetUAV(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsUnorderedAccessView();
}

unordered_map<wstring, ShaderDesc> ShaderManager::shaders;

ShaderDesc ShaderManager::GetEffect(wstring fileName)
{
	if (shaders.count(fileName) == 0)
	{
		ComPtr<ID3DBlob> blob;
		ComPtr<ID3DBlob> error;
		INT flag = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;

		HRESULT hr = ::D3DCompileFromFile(fileName.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "fx_5_0", flag, NULL, blob.GetAddressOf(), error.GetAddressOf());
		if (FAILED(hr))
		{
			if (error != NULL)
			{
				string str = (const char*)error->GetBufferPointer();
				MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
			}
			assert(false);
		}

		ComPtr<ID3DX11Effect> effect;
		hr = ::D3DX11CreateEffectFromMemory(blob->GetBufferPointer(), blob->GetBufferSize(), 0, DEVICE.Get(), effect.GetAddressOf());

		
		shaders[fileName] = ShaderDesc{blob, effect};
	}
	
	ShaderDesc desc = shaders.at(fileName);
	ComPtr<ID3DX11Effect> effect;
	desc.effect->CloneEffect(D3DX11_EFFECT_CLONE_FORCE_NONSINGLE, effect.GetAddressOf());

	return ShaderDesc{desc.blob, effect};
}

void Shader::Push_GlobalData(const _float4x4& matView, const _float4x4& matProj)
{
	if (!m_pGlobalEffectBuffer)
	{
		m_pGlobalBuffer = make_shared<ConstantBuffer>();
		m_pGlobalBuffer->Create<GlobalDesc>();
		m_pGlobalEffectBuffer = GetConstantBuffer("GlobalBuffer");
	}

	m_GlobalDesc.V = matView;
	m_GlobalDesc.P = matProj;
	m_GlobalDesc.VP = matView * matProj;
	m_GlobalDesc.VInv = matView.Invert();

	m_pGlobalBuffer->CopyData(m_GlobalDesc);

	m_pGlobalEffectBuffer->SetConstantBuffer(m_pGlobalBuffer->Get_ComPtr().Get());
}

void Shader::Push_TransformData(const TransformDesc& desc)
{
	if (!m_pTransformEffectBuffer)
	{
		m_pTransformBuffer = make_shared<ConstantBuffer>();
		m_pTransformBuffer->Create<TransformDesc>();
		m_pTransformEffectBuffer = GetConstantBuffer("TransformBuffer");
	}

	m_TransformDesc = desc;

	m_pTransformBuffer->CopyData(m_TransformDesc);

	m_pTransformEffectBuffer->SetConstantBuffer(m_pTransformBuffer->Get_ComPtr().Get());

}

void Shader::Push_LightData(const LightParams& desc)
{
	if (!m_pLightEffectBuffer)
	{
		m_pLightBuffer = make_shared<ConstantBuffer>();
		m_pLightBuffer->Create<LightParams>();
		m_pLightEffectBuffer = GetConstantBuffer("LightBuffer");
	}

	m_LightDesc = desc;

	m_pLightBuffer->CopyData(m_LightDesc);

	m_pLightEffectBuffer->SetConstantBuffer(m_pLightBuffer->Get_ComPtr().Get());
}

void Shader::Push_MaterialData(const MaterialDesc& desc)
{
	if (!m_pMaterialEffectBuffer)
	{
		m_pMaterialBuffer = make_shared<ConstantBuffer>();
		m_pMaterialBuffer->Create<MaterialDesc>();
		m_pMaterialEffectBuffer = GetConstantBuffer("MaterialBuffer");
	}
	m_MaterialDesc = desc;

	m_pMaterialBuffer->CopyData(m_MaterialDesc);

	m_pMaterialEffectBuffer->SetConstantBuffer(m_pMaterialBuffer->Get_ComPtr().Get());

}

void Shader::Push_BoneData(const BoneDesc& desc)
{
	if (!m_pBoneEffectBuffer)
	{
		m_pBoneBuffer = make_shared<ConstantBuffer>();
		m_pBoneBuffer->Create<BoneDesc>();
		m_pBoneEffectBuffer = GetConstantBuffer("BoneBuffer");
	}
	m_BoneDesc = desc;

	m_pBoneBuffer->CopyData(m_BoneDesc);

	m_pBoneEffectBuffer->SetConstantBuffer(m_pBoneBuffer->Get_ComPtr().Get());
}

void Shader::Push_KeyframeData(const KeyFrameDesc& desc)
{
	if (!m_pKeyFrameEffectBuffer)
	{
		m_pKeyFrameBuffer = make_shared<ConstantBuffer>();
		m_pKeyFrameBuffer->Create<KeyFrameDesc>();
		m_pKeyFrameEffectBuffer = GetConstantBuffer("KeyFrameBuffer");
	}
	m_KeyFrameDesc = desc;

	m_pKeyFrameBuffer->CopyData(m_KeyFrameDesc);

	m_pKeyFrameEffectBuffer->SetConstantBuffer(m_pKeyFrameBuffer->Get_ComPtr().Get());

}

void Shader::Push_TweenData(const TweenDesc& desc)
{
	if (!m_pTweenEffectBuffer)
	{
		m_pTweenBuffer = make_shared<ConstantBuffer>();
		m_pTweenBuffer->Create<TweenDesc>();
		m_pTweenEffectBuffer = GetConstantBuffer("TweenBuffer");
	}
	m_TweenDesc = desc;

	m_pTweenBuffer->CopyData(m_TweenDesc);

	m_pTweenEffectBuffer->SetConstantBuffer(m_pTweenBuffer->Get_ComPtr().Get());
}

void Shader::Push_InstanceTweenData(const InstanceTweenDesc& desc)
{
	if (!m_pInstanceTweenEffectBuffer)
	{
		m_pInstanceTweenBuffer = make_shared<ConstantBuffer>();
		m_pInstanceTweenBuffer->Create<InstanceTweenDesc>();
		m_pInstanceTweenEffectBuffer = GetConstantBuffer("InstanceTweenBuffer");
	}
	m_InstanceTweenDesc = desc;

	m_pInstanceTweenBuffer->CopyData(m_InstanceTweenDesc);

	m_pInstanceTweenEffectBuffer->SetConstantBuffer(m_pInstanceTweenBuffer->Get_ComPtr().Get());
}

void Shader::Push_InstanceRenderParamData(const InstanceRenderParamDesc& desc)
{
	if (!m_pInstanceRenderParamEffectBuffer)
	{
		m_pInstanceRenderParamBuffer = make_shared<ConstantBuffer>();
		m_pInstanceRenderParamBuffer->Create<InstanceRenderParamDesc>();
		m_pInstanceRenderParamEffectBuffer = GetConstantBuffer("InstanceRenderParamBuffer");
	}
	m_InstanceRenderParamDesc = desc;

	m_pInstanceRenderParamBuffer->CopyData(m_InstanceRenderParamDesc);

	m_pInstanceRenderParamEffectBuffer->SetConstantBuffer(m_pInstanceRenderParamBuffer->Get_ComPtr().Get());
}

void Shader::Push_AnimAddonData(const AnimAddonDesc& desc)
{
	if (!m_pBoneIndexEffectBuffer)
	{
		m_pBoneIndexBuffer = make_shared<ConstantBuffer>();
		m_pBoneIndexBuffer->Create<AnimAddonDesc>();
		m_pBoneIndexEffectBuffer = GetConstantBuffer("AnimAddonBuffer");
	}
	m_BoneIndexDesc = desc;

	m_pBoneIndexBuffer->CopyData(m_BoneIndexDesc);

	m_pBoneIndexEffectBuffer->SetConstantBuffer(m_pBoneIndexBuffer->Get_ComPtr().Get());

}

void Shader::Push_EffectData(const EffectDesc& desc)
{
	if (!m_pEffectEffectBuffer)
	{
		m_pEffectBuffer = make_shared<ConstantBuffer>();
		m_pEffectBuffer->Create<EffectDesc>();
		m_pEffectEffectBuffer = GetConstantBuffer("EffectBuffer");
	}
	m_EffectDesc = desc;

	m_pEffectBuffer->CopyData(m_EffectDesc);

	m_pEffectEffectBuffer->SetConstantBuffer(m_pEffectBuffer->Get_ComPtr().Get());

}

void Shader::Push_ParticleData(const ParticleDesc& desc)
{
	if (!m_pParticleEffectBuffer)
	{
		m_pParticleBuffer = make_shared<ConstantBuffer>();
		m_pParticleBuffer->Create<ParticleDesc>();
		m_pParticleEffectBuffer = GetConstantBuffer("ParticleBuffer");
	}

	m_ParticleDesc = desc;
	m_pParticleBuffer->CopyData(m_ParticleDesc);
	m_pParticleEffectBuffer->SetConstantBuffer(m_pParticleBuffer->Get_ComPtr().Get());
}

void Shader::Push_CreateParticleData(const CreateParticleDesc& desc)
{
	if (!m_pCreateParticleEffectBuffer)
	{
		m_pCreateParticleBuffer = make_shared<ConstantBuffer>();
		m_pCreateParticleBuffer->Create<CreateParticleDesc>();
		m_pCreateParticleEffectBuffer = GetConstantBuffer("CreateParticleBuffer");
	}

	m_CreateParticleDesc = desc;
	m_pCreateParticleBuffer->CopyData(m_CreateParticleDesc);
	m_pCreateParticleEffectBuffer->SetConstantBuffer(m_pCreateParticleBuffer->Get_ComPtr().Get());
}

void Shader::Push_TextureMapData(const TextureMapDesc& boolArray)
{
	if (!m_pTextureMapEffectBuffer)
	{
		m_pTextureMapBuffer = make_shared<ConstantBuffer>();
		m_pTextureMapBuffer->Create<TextureMapDesc>();
		m_pTextureMapEffectBuffer = GetConstantBuffer("TextureMapBuffer");
	}

	m_pTextureMapBuffer->CopyData(boolArray);
	m_pTextureMapEffectBuffer->SetConstantBuffer(m_pTextureMapBuffer->Get_ComPtr().Get());
}

void Shader::Push_SubmapData(const SubMapDesc& desc)
{
	if (!m_pSubmapEffectBuffer)
	{
		m_pSubmapBuffer = make_shared<ConstantBuffer>();
		m_pSubmapBuffer->Create<SubMapDesc>();
		m_pSubmapEffectBuffer = GetConstantBuffer("SubMapBuffer");
	}

	m_pSubmapBuffer->CopyData(desc);
	m_pSubmapEffectBuffer->SetConstantBuffer(m_pSubmapBuffer->Get_ComPtr().Get());


}

void Shader::Push_RenderParamData(const RenderParams& params)
{
	if (!m_pRenderParamEffectBuffer)
	{
		m_pRenderParamBuffer = make_shared<ConstantBuffer>();
		m_pRenderParamBuffer->Create<RenderParams>();
		m_pRenderParamEffectBuffer = GetConstantBuffer("RenderParamBuffer");
	}

	m_pRenderParamBuffer->CopyData(params);
	m_pRenderParamEffectBuffer->SetConstantBuffer(m_pRenderParamBuffer->Get_ComPtr().Get());
}


//for (auto& scene : m_Scenes)
//{
//	wstring finalPath = m_strSceneDataPath + scene->Get_Name() + L".Scene";
//	auto path = fs::path(finalPath);
//	fs::create_directory(path.parent_path());
//
//	shared_ptr<FileUtils> file = make_shared<FileUtils>();
//	file->Open(finalPath, FileMode::Write);
//
//	// Scene ÀÌ¸§
//	file->Write<wstring>(scene->Get_Name());
//
//	// Object °¹¼ö
//	file->Write<_uint>(_uint(scene->Get_Objects().size()));
//	for (auto& object : scene->Get_Objects())
//	{
//
//		file->Write<_bool>(object->Get_Collider() != nullptr);
//		if (object->Get_Collider())
//		{
//			ColliderType colliderType = object->Get_Collider()->Get_ColliderType();
//			file->Write<ColliderType>(colliderType);
//
//			switch (colliderType)
//			{
//			case ColliderType::Sphere:
//			{
//				BoundingSphere& tmp = dynamic_pointer_cast<SphereCollider>(object->Get_Collider())->Get_BoundingSphere();
//				file->Write<_float>(tmp.Radius);
//			}
//
//			break;
//			case ColliderType::AABB:
//			{
//				BoundingBox& tmp = dynamic_pointer_cast<AABBBoxCollider>(object->Get_Collider())->Get_BoundingBox();
//				file->Write<_float3>(tmp.Extents);
//			}
//			break;
//			case ColliderType::OBB:
//			{
//				BoundingOrientedBox& tmp = dynamic_pointer_cast<OBBBoxCollider>(object->Get_Collider())->Get_BoundingBox();
//				file->Write<_float3>(tmp.Extents);
//			}
//			break;
//			case ColliderType::Mesh:
//			{
//				wstring colliderModelTag = dynamic_pointer_cast<MeshCollider>(object->Get_Collider())->Get_ModelTag();
//				file->Write<wstring>(colliderModelTag);
//			}
//			break;
//			default:
//				break;
//			}
//
//
//
//		}
//
//	}
//
//
//}