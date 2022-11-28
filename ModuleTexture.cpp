#include "ModuleTexture.h"
#include "Globals.h"

#include <string>

#include "GL/glew.h"
#include "Libraries/DirectXTex/DirectXTex.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/stb_image.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	return true;
}

bool ModuleTexture::Start()
{
	/*std::string narrow_string("Assets/textures/baboon.ppm");
	std::wstring wide_string = std::wstring(narrow_string.begin(), narrow_string.end());
	const wchar_t* path = wide_string.c_str();

	DirectX::TexMetadata md;
	DirectX::ScratchImage* img = new DirectX::ScratchImage;

	HRESULT result = DirectX::LoadFromDDSFile(path, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &md, *img);

	if (FAILED(result))
	{
		result = DirectX::LoadFromTGAFile(path, &md, *img);

		if (FAILED(result))
		{
			result = DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &md, *img);
		}
	}

	DirectX::ScratchImage *flippedImg = new DirectX::ScratchImage;

	result = DirectX::FlipRotate(img->GetImages(), img->GetImageCount(), img->GetMetadata(), 
		DirectX::TEX_FR_FLAGS::TEX_FR_FLIP_VERTICAL, *flippedImg);

	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint internalFormat;
	GLenum format, type;

	switch (flippedImg->GetMetadata().format)
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

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, md.width, md.height, 0, format, type, flippedImg->GetPixels());
	glGenerateMipmap(GL_TEXTURE_2D);*/

	return true;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

GLuint ModuleTexture::Load(const char* fileName)
{
	GLuint texture;

	std::string narrow_string(fileName);
	std::wstring wide_string = std::wstring(narrow_string.begin(), narrow_string.end());
	const wchar_t* path = wide_string.c_str();

	DirectX::TexMetadata md;
	DirectX::ScratchImage* img = new DirectX::ScratchImage;

	HRESULT result = DirectX::LoadFromDDSFile(path, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &md, *img);

	if (FAILED(result))
	{
		result = DirectX::LoadFromTGAFile(path, &md, *img);

		if (FAILED(result))
		{
			result = DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &md, *img);
		}
	}

	DirectX::ScratchImage* flippedImg = new DirectX::ScratchImage;

	result = DirectX::FlipRotate(img->GetImages(), img->GetImageCount(), img->GetMetadata(),
		DirectX::TEX_FR_FLAGS::TEX_FR_FLIP_VERTICAL, *flippedImg);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, this->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint internalFormat;
	GLenum format, type;

	switch (flippedImg->GetMetadata().format)
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

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, md.width, md.height, 0, format, type, flippedImg->GetPixels());
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}
