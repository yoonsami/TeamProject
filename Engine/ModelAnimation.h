#pragma once

struct ModelKeyFrameData
{
	_float time;
	_float3 scale;
	Quaternion rotation;
	_float3 translation;
};

struct ModelKeyFrame
{
	wstring boneName;
	vector<ModelKeyFrameData> transforms;
};


struct ModelAnimation
{
	_uint index;
	wstring name;
	_uint frameCount = 0;
	_float frameRate = 0.f;
	_float duration = 0.f;
	_float speed = 1.f;
	_bool loop = false;
	vector<shared_ptr<ModelKeyFrame>> keyFrames;

	shared_ptr<ModelKeyFrame> Get_KeyFrame(const wstring& name);
}; 

