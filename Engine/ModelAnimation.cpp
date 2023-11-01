#include "pch.h"
#include "ModelAnimation.h"

shared_ptr<ModelKeyFrame> ModelAnimation::Get_KeyFrame(const wstring& name)
{
	for (auto& keyFrame : keyFrames)
	{
		if (keyFrame->boneName == name)
			return keyFrame;
	}
	return nullptr;
}
