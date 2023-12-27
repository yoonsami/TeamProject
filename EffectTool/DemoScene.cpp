#include "pch.h"
#include "DemoScene.h"

// For. Components
#include "Light.h"
#include "Camera.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"

// For. Script
#include "FreeCameraScript.h"

// For. Etc
#include "Model.h"
#include "ImGuiToolMgr.h"
#include <MeshCollider.h>
#include <Terrain.h>
#include "RigidBody.h"
#include "TerrainRenderer.h"
#include "Geometry.h"

GranseedScene::GranseedScene()
{
}

void GranseedScene::Init()
{
	__super::Init();

	COLLISION.Check_Group(_int(CollisionGroup::Player), _int(CollisionGroup::Monster));

	// For. Initialize ImGui Tool mgr
	IMGUITOOL_MGR.Init(DEVICE.Get(), CONTEXT.Get());
	IMGUITOOL_MGR.Tick();
}

void GranseedScene::Tick()
{
	__super::Tick();
	IMGUITOOL_MGR.Tick();

	if (KEYHOLD(KEY_TYPE::LALT) && KEYHOLD(KEY_TYPE::LBUTTON))
		INPUT.Set_Mouse_Move(false);
	else
		INPUT.Set_Mouse_Move(true);
}

void GranseedScene::Late_Tick()
{
	__super::Late_Tick();
}

void GranseedScene::Final_Tick()
{
	__super::Final_Tick();
	PHYSX.Tick();
}

void GranseedScene::Render()
{
	__super::Render();

	IMGUITOOL_MGR.Render();
}

HRESULT GranseedScene::Load_Scene()
{
	PHYSX.Init();
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Parts\\Player\\", true);
	RESOURCES.LoadParts(); 
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Hero\\");
	RESOURCES.CreateModel(L"..\\Resources\\Models\\VfxMesh\\");
	//RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\SkyBox\\");
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\");
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Weapon\\");
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\Mir\\");
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\");

	RESOURCES.LoadParts();
	Load_DemoModel();
	Load_Camera();

	Load_MapFile(L"MirMap", nullptr);
	//Load_MapFile(L"SpikeMap", nullptr);
	//Load_Terrain();

	return S_OK;
}

void GranseedScene::Load_DemoModel()
{
	// For. ���� ���� �� GameObject���� 
	shared_ptr<GameObject> testObj = make_shared<GameObject>();

	// For. GameObject�� Transform Component �߰�
		// ���� �ϳ� ���. AddComponent �Ǵ� GetOrAddTransform(������ ��ȯ ������ ������ ��ȯ)
	testObj->Add_Component(make_shared<Transform>());
		//testObj->GetOrAddTransform();

	// For. GameObject�� Transform �������ֱ� 
	testObj->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
	testObj->Get_Transform()->Scaled(_float3(3.f,1.f,3.f));
	
	// For. Animator�����Ҷ� �ʿ��� 
	shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

	// For. GameObject�� ���� animator component �����ϱ� 
	shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
	
	// For. Animator component�� ���� Model �����ϱ� 
	shared_ptr<Model> model = RESOURCES.Get<Model>(L"GridPlane");

	// For. Animator�� Model ���̱�
	animator->Set_Model(model);

	// For. ���� GameObject�߰� 
	Add_GameObject(testObj);
}

void GranseedScene::Load_Light()
{
	// For. ���� �߰��� �� ������ �� GameObject���� 
	shared_ptr<GameObject> lightObj = make_shared<GameObject>();

	//// For. GameObject�� Transform ���� 
	lightObj->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, 25.f, 0.f, 1.f));
	lightObj->GetOrAddTransform()->Set_LookDir(_float3(-1.f, -1.f, -1.f));

	//// For. GameObject�� ���� LightComponent ����
	shared_ptr<Light> lightCom = make_shared<Light>();
	lightCom->Set_Diffuse(Color(1.f));
	lightCom->Set_Ambient(Color(0.8f));
	lightCom->Set_Specular(Color(0.f));
	lightCom->Set_Emissive(Color(0.f));
	lightCom->Set_LightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);

	//// For. GameObject�� Light Component ���̱� 
	lightObj->Add_Component(lightCom);

	//// For. ���� �� ������ �ϴ� GameObject �߰� 
	Add_GameObject(lightObj);
}

void GranseedScene::Load_Terrain()
{
	// 타일텍스쳐로드
	wstring partsPath = L"..\\Resources\\Textures\\MapObject\\TerrainTile\\";
	for (auto& entry : fs::recursive_directory_iterator(partsPath))
	{

		if (entry.is_directory())
			continue;

		string fileName = entry.path().filename().string();
		Utils::DetachExt(fileName);

		// 타일의 텍스쳐이름을 리소스에 로드
		wstring TileTexture = L"..\\Resources\\Textures\\MapObject\\TerrainTile\\";
		TileTexture += Utils::ToWString(fileName) + L".dds";
		auto texture = RESOURCES.GetOrAddTexture(Utils::ToWString(fileName), TileTexture);
		if (!texture)
		{
			Utils::ChangeExt(TileTexture, L".tga");
			texture = RESOURCES.GetOrAddTexture(Utils::ToWString(fileName), TileTexture);
		}
		if (!texture)
		{
			Utils::ChangeExt(TileTexture, L".png");
			texture = RESOURCES.GetOrAddTexture(Utils::ToWString(fileName), TileTexture);
		}
		if (!texture)
		{
			Utils::ChangeExt(TileTexture, L".jpg");
			texture = RESOURCES.GetOrAddTexture(Utils::ToWString(fileName), TileTexture);
		}
	}

	shared_ptr<Terrain> loadedTerrain = make_shared<Terrain>();

	string strFilePath = "..\\Resources\\MapData\\TerrainData.TRdat";
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(Utils::ToWString(strFilePath), FileMode::Read);

	vector<VTXTEXNORTANDATA> loadedVertices;
	// 버텍스의 개수와 포지션,노말 로드
	loadedVertices.resize(file->Read<_int>());
	for (size_t i = 0; i < loadedVertices.size(); ++i)
	{
		file->Read<_float3>(loadedVertices[i].vPosition);
		file->Read<_float2>(loadedVertices[i].vTexCoord);
		file->Read<_float3>(loadedVertices[i].vNormal);
		file->Read<_float3>(loadedVertices[i].vTangent);

	}

	// 인덱스의 개수와 정보 로드
	vector<_uint> loadedIndices;
	// 버텍스의 개수와 포지션,노말 로드
	loadedIndices.resize(file->Read<_int>());
	for (size_t i = 0; i < loadedIndices.size(); i++)
	{
		file->Read<_uint>(loadedIndices[i]);
	}

	_int loadedSizeX = 0;
	_int loadedSizeY = 0;
	file->Read<_int>(loadedSizeX);
	file->Read<_int>(loadedSizeY);

	// 터레인버퍼생성
	loadedTerrain->Get_Geometry()->Set_Vertices(loadedVertices);
	loadedTerrain->Get_Geometry()->Set_Indices(loadedIndices);
	loadedTerrain->Create_Buffer();

	/// 지형오브젝트 생성
	shared_ptr<GameObject> TerrainObject = make_shared<GameObject>();
	TerrainObject->Set_Name(L"Terrain");
	TerrainObject->GetOrAddTransform();
	// 터레인생성
		// 메시렌더러
		//shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Terrain.fx"));
		//renderer->Set_Mesh(loadedTerrain);
		//renderer->Set_Pass(18); // 터레인패스
	shared_ptr<TerrainRenderer> renderer = make_shared<TerrainRenderer>(RESOURCES.Get<Shader>(L"Shader_Terrain.fx"));
	renderer->CreateGrid(loadedSizeX, loadedSizeY);

	// 디퓨즈텍스쳐
	shared_ptr<Material> material = make_shared<Material>();
	auto Grasstexture = RESOURCES.Get<Texture>(L"TileGrass");
	if (Grasstexture == nullptr)
	{
		MSG_BOX("NoDiffuseTexture");
		return;
	}
	material->Set_TextureMap(Grasstexture, TextureMapType::DIFFUSE);

	// 노말텍스쳐
	auto Normaltexture = RESOURCES.Get<Texture>(L"TileNormal");
	if (Normaltexture == nullptr)
	{
		MSG_BOX("NoNormalTexture");
		return;
	}
	material->Set_TextureMap(Normaltexture, TextureMapType::NORMAL);

	// Mask텍스쳐
	auto Masktexture = RESOURCES.Get<Texture>(L"TileMask");
	if (Masktexture == nullptr)
	{
		MSG_BOX("NoMaskTexture");
		return;
	}
	material->Set_TextureMap(Masktexture, TextureMapType::TEXTURE7);

	// Road텍스쳐
	auto Roadtexture = RESOURCES.Get<Texture>(L"TileRoad");
	if (Roadtexture == nullptr)
	{
		MSG_BOX("NoSubTexture");
		return;
	}
	material->Set_TextureMap(Roadtexture, TextureMapType::TEXTURE8);

	{
		auto HeightMap = RESOURCES.ReloadOrAddTexture(L"HeightMap", L"..\\Resources\\Textures\\MapObject\\TerrainTile\\HeightMap.png");
		if (HeightMap == nullptr)
		{
			MSG_BOX("NoSubTexture");
			return;
		}
		material->Set_TextureMap(HeightMap, TextureMapType::TEXTURE9);
	}

	renderer->Set_Material(material);

	// 메시를 통해 메시콜라이더 생성
	shared_ptr<MeshCollider> pCollider = make_shared<MeshCollider>(*loadedTerrain.get());
	TerrainObject->Add_Component(pCollider);
	pCollider->Set_Activate(true);
	// 리지드바디 생성
	auto rigidBody = make_shared<RigidBody>();
	rigidBody->Create_RigidBody(pCollider, TerrainObject->GetOrAddTransform()->Get_WorldMatrix());
	TerrainObject->Add_Component(rigidBody);
	TerrainObject->Add_Component(renderer);

	Add_GameObject(TerrainObject);
}

void GranseedScene::Load_Camera()
{
	/* UI�� �ƴ� �Ϲ� �𵨵��� �׸��� ī�޶� �����ؼ� ���� �߰��ϱ� */

	// For. ���� �߰��� Camera�� �� GameObject ���� 
	shared_ptr<GameObject> camera = make_shared<GameObject>();

	// For. ī�޶� Transform ���� 
	camera->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
	camera->GetOrAddTransform()->Set_Speed(5.f);

	// For. GameObject�� ���� ī�޶� ������Ʈ ���� 
	CameraDesc desc;
	desc.fFOV = XM_PI / 3.f;
	desc.strName = L"Default";
	desc.fSizeX = _float(g_iWinSizeX);
	desc.fSizeY = _float(g_iWinSizeY);
	desc.fNear = 0.1f;
	desc.fFar = 1000.f;
	shared_ptr<Camera> cameraComponent = make_shared<Camera>(desc);

	// For. ������ ī�޶� ������Ʈ�� GameObject�� ���̱� 
	camera->Add_Component(cameraComponent);

	// For. GameObject�� ī�޶� ������Ʈ�� � ������ ���� �����ϱ� 
	camera->Get_Camera()->Set_ProjType(ProjectionType::Perspective);

	// For. GameObject�� ī�޶� ������Ʈ�� UI�� �׸��� �ʵ��� ���� 
	camera->Get_Camera()->Set_CullingMaskLayerOnOff(Layer_UI, true);

	// For. GameObject�� key input�� ���� �̵��� ���� ��ũ��Ʈ ���̱� 
	camera->Add_Component(make_shared<FreeCameraScript>());

	// For. ���� ������ ī�޶� �߰� 
	Add_GameObject(camera);
}