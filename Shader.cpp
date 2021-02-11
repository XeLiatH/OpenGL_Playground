#include "Shader.h";

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    unsigned int vertexShader = this->loadShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int fragmentShader = this->loadShader(GL_FRAGMENT_SHADER, fragmentPath);

    this->linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(this->id);
}

void Shader::use()
{
    glUseProgram(this->id);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

std::string Shader::loadShaderSource(std::string shaderFilePath)
{
    std::string shaderSource;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(shaderFilePath);
        std::stringstream shaderFileStream;

        shaderFileStream << shaderFile.rdbuf();

        shaderFile.close();
        shaderSource = shaderFileStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::COULD_NOT_READ_FILE:" << shaderFilePath << std::endl;
    }

    return shaderSource;
}

unsigned int Shader::loadShader(GLenum type, std::string shaderFilePath)
{
    char infoLog[512];
    int success;

    unsigned int shader = glCreateShader(type);

    std::string source = this->loadShaderSource(shaderFilePath);
    const char* sourceToCompile = source.c_str();

    glShaderSource(shader, 1, &sourceToCompile, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        std::cerr
            << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER:" << shaderFilePath << std::endl
            << infoLog << std::endl;
    }

    return shader;
}

void Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    char infoLog[512];
    int success;

    this->id = glCreateProgram();
    glUseProgram(this->id);

    glAttachShader(this->id, vertexShader);
    glAttachShader(this->id, fragmentShader);

    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);

        std::cerr
            << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << std::endl
            << infoLog << std::endl;
    }

    glUseProgram(0);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
