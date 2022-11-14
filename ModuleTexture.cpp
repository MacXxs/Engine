#include "ModuleTexture.h"
#include "Libraries/DirectXTex/DirectXTex.h"

bool ModuleTexture::Init()
{
	const wchar_t *srcFile = (const wchar_t*)"Assets/textures/baboon.png";

	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	HRESULT result = DirectX::LoadFromDDSFile(srcFile, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &md, img);

	if (FAILED(result))
	{
	}

	return true;
}