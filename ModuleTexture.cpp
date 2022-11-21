#include "ModuleTexture.h"
#include "Globals.h"

#include "Libraries/DirectXTex/DirectXTex.h"
#include "GL/glew.h"

bool ModuleTexture::Init()
{
	const wchar_t *srcFile = (const wchar_t*)"Assets/textures/baboon.png";

	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	HRESULT result = DirectX::LoadFromDDSFile(srcFile, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &md, img);

	if (FAILED(result))
	{
		result = DirectX::LoadFromTGAFile(srcFile, &md, img);

		if (FAILED(result))
		{
			result = DirectX::LoadFromWICFile(srcFile, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &md, img);
		}
	}

	GLint internalFormat;
	GLenum format, type;
	
	switch (md.format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}

	md.width = 512;
	md.height = 512;

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, md.width, md.height, 0, format, type, &texture);

	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);




	return true;
}