#pragma once

enum class ResourceType : _char
{
	None = -1,
	//
	Mesh,
	Shader,
	Texture,
	Material,
	Model,
	Animation,
	Font,
	Sound,
	End,
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<_char>(ResourceType::End)
};
class ResourceBase : public enable_shared_from_this<ResourceBase>
{
public:
	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

public:
	virtual shared_ptr<ResourceBase> Clone() { return nullptr; }
	ResourceType Get_Type() { return m_eType; }
	void Set_Name(const wstring& name) { m_strName = name; }
	const wstring& Get_Name() { return m_strName; }

	_uint Get_ID() { return m_iID; }

protected:
	virtual void Load(const wstring& path){}
	virtual void Save(const wstring& path){}

protected:
	ResourceType m_eType = ResourceType::None;
	wstring m_strName = L"";
	wstring m_strPath = L"";
	_uint m_iID = 0;
};
