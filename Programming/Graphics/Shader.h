#include<string>
#include<GL/glew.h>

class Shader{
    Shader();
    ~Shader();

    bool Load(const std::string& vertName,
              const std::string& fragName);     // 加载顶点着色器和片元着色器
    void SetActive();

private:
    bool CompileShader(const std::string& fileName,
                       GLenum shaderType, GLuint& outShader);       // 编译着色器
    
    bool IsCompiled(GLuint shader);     // 是否编译成功
    bool IsValidProgram();              // 是否链接成功

    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};