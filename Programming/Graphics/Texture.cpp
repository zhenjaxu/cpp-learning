#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL2/SDL.h>

Texture::Texture()
{}

Texture::~Texture()
{}

bool Texture::Load(const std::string& fileName)
{
    int channels = 0;
    unsigned char* image = stbi_load(       // 使用 stb_image 加载纹理
        fileName.c_str(),
        &mWidth,
        &mHeight,
        &channels,
        0
    );

    if(!image)
    {
        SDL_Log("stb_image failed to load image %s: %s",
            fileName.c_str(), stbi_failure_reason());
        return false;
    }

    int format = channels == 4 ? GL_RGBA : GL_RGB;

    // 创建 OpenGL 纹理对象
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    // 复制图像数据
    glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);     // 释放分配的内存

    // 双线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::Unload()
{
    glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}