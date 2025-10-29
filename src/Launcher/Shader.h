// Shader.h
#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>


class Shader {
public:
    Shader(const char* vertexSrc, const char* fragmentSrc);
    ~Shader();
    void use() const;
    GLuint getProgramID() const;

    void SetMatrix4(const char* name, const glm::mat4& matrix);
    void SetFloat(const char* name, float value);
    void SetInt(const char* name, int value);
    void SetVec3(const char* name, const glm::vec3& value);
    void SetBool(const char* name, bool value);


private:
    GLuint programID;

    void checkCompileErrors(GLuint shader, const std::string& type);
};
