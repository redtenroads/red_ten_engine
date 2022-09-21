#include "resource/shader.h"
#include "opengl/glew.h"
#include <string.h>

Shader *Shader::currentShader = nullptr;

Shader::Shader()
{
}

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;
}

Shader::Shader(const char *vertex, const char *fragment)
{
    this->vertexCode = _strdup(vertex);
    this->fragmentCode = _strdup(fragment);
}

void Shader::reload()
{
    if (vertexCode && fragmentCode)
    {
        if (compile(vertexCode, fragmentCode))
            bIsLoaded = true;
    }
    else if (vertexPath.length() > 0 && fragmentPath.length() > 0)
    {
        FILE *fileVertex, *fileFragment;
        fopen_s(&fileVertex, vertexPath.c_str(), "rb");
        fopen_s(&fileFragment, fragmentPath.c_str(), "rb");

        if (!fileVertex || !fileFragment)
            return;

        const int size = 32 * 1024;
        char fileStringVertex[size];
        char fileStringFragment[size];
        int readVertex = fread(fileStringVertex, size, 1, fileVertex);
        int readFragment = fread(fileStringFragment, size, 1, fileFragment);
        if ((readVertex == 0 || readVertex - 1 >= size) && (readFragment == 0 || readFragment - 1 >= size))
            if (compile(fileStringVertex, fileStringFragment))
                bIsLoaded = true;
    }
}

bool Shader::isLoaded()
{
    return bIsLoaded;
}

bool Shader::isPath(std::string vertexPath, std::string fragmentPath)
{
    return this->vertexPath == vertexPath && this->fragmentPath == fragmentPath;
}

void Shader::use()
{
    if (currentShader != this)
    {
        if (!bIsLoaded)
            reload();

        if (bIsLoaded)
        {
            currentShader = this;
            glUseProgram(programm);
        }
        else
        {
            printf("Failed to load shader\n");
        }
    }
}

int Shader::getUniformLocation(const char *name)
{
    if (programm)
        return glGetUniformLocation(programm, name);
    return 0;
}

void Shader::provideFloatValue(int uniform, int amount, float *value)
{
    use();
    if (programm)
        glUniform1fv(uniform, amount, value);
}

void Shader::provideFloat2Value(int uniform, int amount, float *value)
{
    use();
    if (programm)
        glUniform2fv(uniform, amount, value);
}

void Shader::provideFloat3Value(int uniform, int amount, float *value)
{
    use();
    if (programm)
        glUniform3fv(uniform, amount, value);
}

void Shader::provideIntValue(int uniform, int amount, int *value)
{
    use();
    if (programm)
        glUniform1iv(uniform, amount, value);
}

void Shader::provideInt2Value(int uniform, int amount, int *value)
{
    use();
    if (programm)
        glUniform2iv(uniform, amount, value);
}

void Shader::provideInt3Value(int uniform, int amount, int *value)
{
    use();
    if (programm)
        glUniform3iv(uniform, amount, value);
}

bool Shader::compile(const char *vertexShader, const char *fragmentShader)
{
    int isCompiled;
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        showCompilationError(vs);
        return false;
    }

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        showCompilationError(fs);
        return false;
    }

    programm = glCreateProgram();
    if (programm)
    {
        glAttachShader(programm, fs);
        glAttachShader(programm, vs);
        glLinkProgram(programm);

        mTransformLoc = glGetUniformLocation(programm, "mTransform");
        mViewProjectionLoc = glGetUniformLocation(programm, "mViewProjection");
        v2TexCoordShiftLoc = glGetUniformLocation(programm, "aTexCoordShift");
        v2TexCoordMulLoc = glGetUniformLocation(programm, "aTexCoordMul");
        fOpacityLoc = glGetUniformLocation(programm, "fOpacity");
        v3ColorLoc = glGetUniformLocation(programm, "clearColor");

        return true;
    }
    return false;
}

void Shader::showCompilationError(unsigned int shader)
{
    char log[2048];
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    printf("Shader Compilation Error:\n%s\n", log);
}