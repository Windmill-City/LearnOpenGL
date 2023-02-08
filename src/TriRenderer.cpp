#include "TriRenderer.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <string>

float vertices[] = {
    // clang-format off
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
    -0.0f, 0.0f, 0.0f   // 中间
    // clang-format on
};

unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    // clang-format off
    0, 1, 4, // 第一个三角形
    2, 3, 4  // 第二个三角形
    // clang-format on
};

unsigned int shaderProgram;
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

/**
 * @brief Read shader file and return source string
 *
 * @param path path to shader file
 * @return std::string source string
 */
std::string readShaderFile(const std::string path)
{
    std::ifstream shaderFile(path, std::ios::binary | std::ios::ate);
    if (!shaderFile) throw std::exception(std::format("Shader file: Failed to open {}!", path).c_str());

    auto        size = shaderFile.tellg();
    std::string source(size, '\0');
    shaderFile.seekg(0);
    shaderFile.read(&source[0], size);

    return source;
}

void loadShader()
{
    // Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto sourceStr = readShaderFile("assets/shader/VertexShader.txt");
    auto source    = sourceStr.c_str();
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);
    // Check if shader compile failed
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    // Fragment Shader
    sourceStr = readShaderFile("assets/shader/FragmentShader.txt");
    source    = sourceStr.c_str();
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &source, NULL);
    glCompileShader(fragmentShader);
    // Check if shader compile failed
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    // Shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

TriRenderer::TriRenderer()
{
    loadShader();

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void TriRenderer::render()
{
    glUseProgram(shaderProgram);

    // 更新uniform颜色
    float timeValue           = glfwGetTime();
    float greenValue          = sin(timeValue) / 2.0f + 0.5f;
    int   vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
