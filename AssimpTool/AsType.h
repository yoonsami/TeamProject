#pragma once


using VertexType = VTXTEXNORTANBlendDATA;

struct asBone
{
	string name;
	_int index = -1;
	_int parent = -1;
	_float4x4 transform;
	_float4x4 offsetTransform;
};

struct asMesh
{
	string name;
	aiMesh* mesh;
	vector<VertexType> vertices;
	vector<_uint> indices;

	_int boneIndex;
	string materialName;
};

struct asMaterial
{
	string name;
	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;
	string diffuseFile;
	string specularFile;
	string normalFile;
	string filterFile;
	string filterNormalFile;
	string maskFile;
};

struct asBlendWeight
{
	_float4 indices = _float4(0.f);
	_float4 weights = _float4(0.f);

	void Set(_uint index, _uint boneIndex, _float weight)
	{
		_float i = (_float)boneIndex;
		_float w = weight;

		switch (index)
		{
		case 0:	indices.x = i; weights.x = w; break;
		case 1:	indices.y = i; weights.y = w; break;
		case 2:	indices.z = i; weights.z = w; break;
		case 3:	indices.w = i; weights.w = w; break;
		}
	}
};

struct asBoneWeight
{
	using Pair = pair<_int, _float>;
	vector<Pair> boneWeights;

	void Add_Weights(_uint boneIndex, _float weight)
	{
		if (weight <= 0.f)
			return;

		auto findit = find_if(boneWeights.begin(), boneWeights.end(),
			[weight](const Pair& p) {return weight > p.second; });

		boneWeights.insert(findit, Pair(boneIndex, weight));
	}

	void Normalize()
	{
		if (boneWeights.size() >= 4)
			boneWeights.resize(4);

		_float totalWeight = 0.f;
		for (const auto& item : boneWeights)
			totalWeight += item.second;

		for (auto& item : boneWeights)
			item.second /= totalWeight;
	}

	asBlendWeight Get_BlendWeight()
	{
		asBlendWeight blendWeights;

		for (_uint i = 0; i < _uint(boneWeights.size()); ++i)
		{
			if(i>=4)
				break;
			blendWeights.Set(i, boneWeights[i].first, boneWeights[i].second);

		}

		return blendWeights;
	}
};

struct asKeyFrameData
{
	_float time;
	_float3 scale;
	Quaternion rotation;
	_float3 translation;
};

struct asKeyFrame
{
	string boneName;
	vector<asKeyFrameData> transforms;
};

struct asAnimation 
{
	string name;
	_uint frameCount;
	_float frameRate;
	_float duration;
	vector<shared_ptr<asKeyFrame>> keyFrames;
};

struct asAnimationNode
{
	aiString name;
	vector<asKeyFrameData> keyFrame;
};