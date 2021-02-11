#include "Texture.h";

Texture::Texture(std::string filePath, bool transparent)
{
    loadFromFile(filePath, transparent);
}

Texture::~Texture()
{
    glDeleteTextures(1, &this->id);
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::loadFromFile(std::string filePath, bool transparent)
{
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    cv::Mat image = cv::imread(filePath, cv::IMREAD_UNCHANGED);
    if (image.empty())
    {
        std::cerr << "ERROR::TEXTURE::FAILED_TO_READ_TEXTURE_IMAGE:" << filePath << std::endl;
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, transparent ? GL_RGBA : GL_RGB, image.cols, image.rows, 0, transparent ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, image.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

}
