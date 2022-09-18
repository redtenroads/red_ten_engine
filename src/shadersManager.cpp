#include "shadersManager.h"
#include <windows.h>
#include "glew.h"
#include <stdio.h>
#include <vector>

extern float spritePoints[];
extern float screenPoints[];

extern const char *spriteVertexShader;
extern const char *spriteFramedVertexShader;
extern const char *spriteFragmentShader;

extern const char *screenVertexShader;
extern const char *screenFragmentShader;

extern const char *clearFragmentShader;

ShadersManager::ShadersManager()
{
}

void ShadersManager::compileSysShaders()
{
    if (bSysShadersCompiled)
        return;
    bSysShadersCompiled = true;

    printf("compiling sprite shader ...\n");
    compileSpriteShader();
    printf("compiling framed sprite shader ...\n");
    compileSpriteFramedShader();
    printf("compiling screen shader ...\n");
    compileScreenShader();
    printf("compiling effect shader ...\n");
    compileEffectShader();
    printf("compiling clear shader ...\n");
    compileClearShader();
    printf("done\n");

    currentShader = nullptr;
}

void ShadersManager::switchShader(ShaderDescriptor *shader)
{
    if (currentShader != shader)
    {
        currentShader = shader;
        if (currentShader)
        {
            glUseProgram(currentShader->programm);
        }
    }
}

void ShadersManager::log(unsigned int shader)
{
    GLchar log[1024];

    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);

    printf("Shader Compilation Error:\n%s\n", log);
    return;
}

ShaderDescriptor *ShadersManager::getSpriteShader()
{
    return &spriteShader;
}
ShaderDescriptor *ShadersManager::getSpriteFramedShader()
{
    return &spriteFramedShader;
}
ShaderDescriptor *ShadersManager::getScreenShader()
{
    return &screenShader;
}

ShaderDescriptor *ShadersManager::getEffectShader()
{
    return &effectShader;
}

ShaderDescriptor *ShadersManager::getClearShader()
{
    return &clearShader;
}

unsigned int ShadersManager::makeVBO(float *points, int amount)
{
    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(float), points, GL_STATIC_DRAW);
    return vbo;
}

unsigned int ShadersManager::makeVAO(unsigned int vbo)
{
    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    return vao;
}

unsigned int ShadersManager::makeProgramm(const char *vertexShader, const char *fragmentShader)
{
    int isCompiled;
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        log(vs);
        return 0;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        log(fs);
        return 0;
    }

    unsigned int programm = glCreateProgram();
    glAttachShader(programm, fs);
    glAttachShader(programm, vs);
    glLinkProgram(programm);

    return programm;
}

void ShadersManager::compileSpriteShader()
{
    unsigned int vbo = makeVBO(spritePoints, 20);
    unsigned int vao = makeVAO(vbo);
    unsigned int programm = makeProgramm(spriteVertexShader, spriteFragmentShader);

    unsigned int mTransformLoc = glGetUniformLocation(programm, "mTransform");
    unsigned int mViewProjectionLoc = glGetUniformLocation(programm, "mViewProjection");
    unsigned int fOpacityLoc = glGetUniformLocation(programm, "fOpacity");

    memset(&spriteShader, 0, sizeof(ShaderDescriptor));
    spriteShader.vbo = vbo;
    spriteShader.vao = vao;
    spriteShader.programm = programm;
    spriteShader.mTransformLoc = mTransformLoc;
    spriteShader.mViewProjectionLoc = mViewProjectionLoc;
    spriteShader.fOpacityLoc = fOpacityLoc;
}

void ShadersManager::compileSpriteFramedShader()
{
    unsigned int vbo = makeVBO(spritePoints, 20);
    unsigned int vao = makeVAO(vbo);
    unsigned int programm = makeProgramm(spriteFramedVertexShader, spriteFragmentShader);

    unsigned int mTransformLoc = glGetUniformLocation(programm, "mTransform");
    unsigned int mViewProjectionLoc = glGetUniformLocation(programm, "mViewProjection");
    unsigned int aTexCoordShift = glGetUniformLocation(programm, "aTexCoordShift");
    unsigned int aTexCoordMul = glGetUniformLocation(programm, "aTexCoordMul");
    unsigned int fOpacityLoc = glGetUniformLocation(programm, "fOpacity");

    memset(&spriteFramedShader, 0, sizeof(ShaderDescriptor));
    spriteFramedShader.vbo = vbo;
    spriteFramedShader.vao = vao;
    spriteFramedShader.programm = programm;
    spriteFramedShader.mTransformLoc = mTransformLoc;
    spriteFramedShader.mViewProjectionLoc = mViewProjectionLoc;
    spriteFramedShader.v2TexCoordShift = aTexCoordShift;
    spriteFramedShader.v2TexCoordMul = aTexCoordMul;
    spriteFramedShader.fOpacityLoc = fOpacityLoc;
}

void ShadersManager::compileScreenShader()
{
    unsigned int vbo = makeVBO(screenPoints, 20);
    unsigned int vao = makeVAO(vbo);
    unsigned int programm = makeProgramm(screenVertexShader, screenFragmentShader);

    memset(&screenShader, 0, sizeof(ShaderDescriptor));
    screenShader.vbo = vbo;
    screenShader.vao = vao;
    screenShader.programm = programm;
}

void ShadersManager::compileEffectShader()
{
    unsigned int vbo = makeVBO(screenPoints, 20);
    unsigned int vao = makeVAO(vbo);
    unsigned int programm = makeProgramm(screenVertexShader, spriteFragmentShader);
    unsigned int fOpacityLoc = glGetUniformLocation(programm, "fOpacity");

    memset(&effectShader, 0, sizeof(ShaderDescriptor));
    effectShader.vbo = vbo;
    effectShader.vao = vao;
    effectShader.programm = programm;
    effectShader.fOpacityLoc = fOpacityLoc;
}

void ShadersManager::compileClearShader()
{
    unsigned int vbo = makeVBO(screenPoints, 20);
    unsigned int vao = makeVAO(vbo);
    unsigned int programm = makeProgramm(screenVertexShader, clearFragmentShader);
    unsigned int colorLoc = glGetUniformLocation(programm, "clearColor");

    memset(&clearShader, 0, sizeof(ShaderDescriptor));
    clearShader.vbo = vbo;
    clearShader.vao = vao;
    clearShader.programm = programm;
    clearShader.v3ColorLoc = colorLoc;
}

float spritePoints[] = {
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

float screenPoints[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

const char *spriteVertexShader =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "uniform mat4 mTransform;\n"
    "uniform mat4 mViewProjection;\n"
    "void main() {\n"
    "   gl_Position = mViewProjection * mTransform * vec4(aPos, 1.0);\n"
    "   texCoord = aTexCoord;\n"
    "}\n";

const char *spriteFramedVertexShader =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "uniform mat4 mTransform;\n"
    "uniform mat4 mViewProjection;\n"
    "uniform vec2 aTexCoordShift;\n"
    "uniform vec2 aTexCoordMul;\n"
    "void main() {\n"
    "   gl_Position = mViewProjection * mTransform * vec4(aPos, 1.0);\n"
    "   texCoord = vec2(aTexCoord.x * aTexCoordMul.x + aTexCoordShift.x, aTexCoord.y * aTexCoordMul.y + aTexCoordShift.y);\n"
    "}\n";

const char *spriteFragmentShader =
    "#version 400\n"
    "out vec4 fragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D t;\n"
    "uniform float fOpacity;\n"
    "void main() {\n"
    "   vec4 color = texture(t, texCoord);\n"
    "   fragColor = color * fOpacity;\n"
    "}\n";

const char *screenVertexShader =
    "#version 400\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   texCoord = aTexCoord;\n"
    "}\n";

const char *screenFragmentShader =
    "#version 400\n"
    "out vec4 fragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D t;\n"
    "void main() {\n"
    "   fragColor = texture(t, texCoord);\n"
    "}\n";

const char *clearFragmentShader =
    "#version 400\n"
    "out vec4 fragColor;\n"
    "uniform vec3 clearColor;\n"
    "void main() {\n"
    "   fragColor = vec4(clearColor, 1.0);\n"
    "}\n";
