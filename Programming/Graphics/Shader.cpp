#include"Shader.h"

Shader::Shader(){}

Shader::~Shader(){}

// 编译着色器
bool Shader::CompileShader(const std::string& fileName,
    GLenum shaderType,
    GLuint& outShader)
{
    std::ifstream shaderFile(fileName);
    if(shaderFile.is_open()){
        // 获取文件内容
        std::stringstream sstream;
        sstream<<shaderFile.rdbuf();
        std::string contents=sstream.str();
        const char* contentsChar=contents.c_str();

        // 生成着色器
        outShader=glCreatShader(shaderType);

        // 配置资源和编译
        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);

        if(!IsCompiled(outShader)){
            SDL_Log("Failed to compile shader %s", fileName.c_str());
            return false;
        }
    }
    else{
        SDL_Log("Shader file not found: %s", fileName.c_str());
        return false;
    }

    return true;
}

bool Shader::IsCompiled(GLuint shader){
    GLint status;

    // 查询编译状态
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status!=GL_TRUE){
        char buffer[512];
        memset(buffer, 0, 512);
        glGetShaderInfoLog(shader, 511, nullptr, buffer);   // 获取错误信息
        SDL_Log("GLSL Compile Failed:\n%s", buffer);
        return false;
    }
    return true;
}

// 编译并链接着色器
bool Shader::Load(const std::string& vertName,
                  const std::string& fragName)
{
    if(!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader)||
       !CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
    {
        return false;
    }

    // 生成程序，链接着色器
    mShaderProgram=glCreateProgram();
    glAttachShader(mShaderProgram, mVertexShader);
    glAttachShader(mShaderProgram, mFragShader);
    glLinkProgram(mShaderProgram);

    if(!IsValidProgram()){
        return false;
    }

    return true;
}

bool Shader::IsValidProgram(){
    GLint status;

    // 查询链接状态
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
    if(status!=GL_TRUE){
        char buffer[512];
        memset(buffer, 0, 512);
        glGetProgramInfoLog(shader, 511, nullptr, buffer);   // 获取错误信息
        SDL_Log("GLSL Compile Failed:\n%s", buffer);
        return false;
    }
    return true;
}

// 设置为活跃状态
void Shader::SetActive(){
    glUseProgram(mShaderProgram);
}

// 删除着色器程序、顶点着色器和片元着色器
void Shader::Unload(){
    glDeleteProgram(mShaderProgram);
    glDeleteShader(mVertexShader);
    glDeleteShader(mFragShader);
}

// 包装器
void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix){
    GLuint loc=glGetUniformLocation(mShaderProgram, name);      // 获取着色器程序中对应的 uniform 矩阵变量
    
    // 包装矩阵
    glUniformMatrix4fv(
        loc,
        1,          // 矩阵数量
        GL_TRUE,    // 行主序为 true，列主序为 false
        matrix.GetAsFloatPtr()  // 矩阵数据指针
    );
}