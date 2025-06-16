#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#define MAX_NUM_SLICES 40 //40
#define GRAVITY 9.81
#define FLOOR -2.0
#define CEILING 5.0
#define WEIGHT 1.5
#define LETTER_HEIGHT 2
#define SCALE 0.75
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define FPS 60.0

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

unsigned logo;

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);

void formPrism(float x, float y, float width, float height, bool testing, float slantHorizontal = 0.0f, float slantVertical = 0.0f, bool isFrustum = false);
void formArch(float xc, float yc, float xa, float ya, float phia, float xb, float yb, float phib, bool testing, float widthCoef = 1.0f);
void formLogo();
void a(bool testing);
void b(bool testing);
void v(bool testing);
void g(bool testing);
void d(bool testing);
void dj(bool testing);
void e(bool testing);
void zh(bool testing);
void z(bool testing);
void i(bool testing);
void j(bool testing);
void k(bool testing);
void l(bool testing);
void lj(bool testing);
void m(bool testing);
void n(bool testing);
void nj(bool testing);
void o(bool testing);
void p(bool testing);
void r(bool testing);
void s(bool testing);
void t(bool testing);
void ch(bool testing);
void u(bool testing);
void f(bool testing);
void h(bool testing);
void c(bool testing);
void tch(bool testing);
void dzh(bool testing);
void sh(bool testing);
void space(bool testing);

void writeWord(const std::wstring& word, bool testing = false);
int numLetters(const std::wstring& text);

float* vertices;

int location = 0;
std::vector<int> locations = {};
float offset = -12;

std::map<int, int> archLocations = {};
std::vector<int> wordLengths = {};

std::vector<int> arches = {};

unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Tekstura nije ucitana! Putanja teksture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}

unsigned loadTexture(const std::string& fileName, bool filterLinear) {
    unsigned texture = loadImageToTexture(("res/" + fileName + ".png").c_str());
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (filterLinear) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    return texture;
}

int main(void)
{
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    const char wTitle[] = "Slagalica";
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    int screenWidth = mode->width;
    int screenHeight = mode->height;

    window = glfwCreateWindow(screenWidth, screenHeight, wTitle, monitor, NULL);
    
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    
    glfwMakeContextCurrent(window);

    
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    logo = loadTexture("logo", true);
    assert(logo != 0);
    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    unsigned int texShader = createShader("tex.vert", "tex.frag");

    glUniform1i(glGetUniformLocation(texShader, "foreground"), 0);

    const std::vector<std::wstring> words = { L"СЛАГАЛИЦА", L"МОЈ БРОЈ"};
    int wordIndex = 0;
    location = MAX_NUM_SLICES + 6;
    for (std::wstring word : words) {
        writeWord(word, true);
        wordLengths.push_back(location);
    }
    const int verticesSize = location * 6;
    location = MAX_NUM_SLICES + 6;
    vertices = new float[verticesSize];
    formLogo();
    vertices[24] = 0;
    vertices[25] = FLOOR - 0.75;
    vertices[26] = 0;
    vertices[27] = 0.4;
    vertices[28] = 0.5;
    vertices[29] = 1.0;
    for (int i = 5; i < location; ++i) {
        float phi = 2 * M_PI * i / MAX_NUM_SLICES;
        vertices[i * 6] = 20 * sin(phi);
        vertices[i * 6 + 1] = FLOOR - 0.75;
        vertices[i * 6 + 2] = 20 * cos(-phi);
        vertices[i * 6 + 3] = 0.1;
        vertices[i * 6 + 4] = 0.2;
        vertices[i * 6 + 5] = 1.0;
    }
    for (std::wstring word : words) {
        offset = -12;
        writeWord(word);
    }

    unsigned int VAO;
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    bool enterPressed = false;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(SCALE, SCALE, SCALE));
    unsigned int modelLoc = glGetUniformLocation(unifiedShader, "uM");
    
    glm::mat4 view;
    view = glm::lookAt(glm::vec3(6.0f, 5.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int viewLoc = glGetUniformLocation(unifiedShader, "uV");
    
    
    glm::mat4 projectionO = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, -10.0f, 50.0f);
    unsigned int projectionLoc = glGetUniformLocation(unifiedShader, "uP");

    unsigned int colorLoc = glGetUniformLocation(unifiedShader, "white");

    glUseProgram(unifiedShader);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));
    glBindVertexArray(VAO);

    glClearColor(0.0, 0.0, 0.7, 1.0);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    bool white = true;

    float initialTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double time = glfwGetTime();
        
        while (glfwGetTime() - time < 1 / FPS) {}

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            glEnable(GL_DEPTH_TEST);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            glEnable(GL_CULL_FACE);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            glDisable(GL_CULL_FACE);
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            white = true;
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            white = false;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            model = glm::rotate(model, glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !enterPressed)
        {
            enterPressed = true;
            wordIndex = (wordIndex + 1) % words.size();
            initialTime = glfwGetTime();
        }
        else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
        {
            enterPressed = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ////
        glUseProgram(texShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, logo);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        ////
        glUseProgram(unifiedShader);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(colorLoc, false);
        glDrawArrays(GL_TRIANGLE_FAN, 4, MAX_NUM_SLICES + 2);
        glUniform1i(colorLoc, white);
        int i = MAX_NUM_SLICES + 6;
        if (wordIndex > 0) i = wordLengths[wordIndex - 1];
        
        while (i < wordLengths[wordIndex]) {
            for (int j = 0; j < locations.size(); ++j) {
                if (i >= locations[j]) continue;
                int letterIndex = j;
                for (int k = 0; k < wordIndex; ++k) {
                    letterIndex -= numLetters(words[k]);
                }
                float elapsed = std::max(0.0, glfwGetTime() - initialTime - letterIndex / 8.0);
                float uY = std::min(5.0, std::max(FLOOR, CEILING - elapsed * elapsed * WEIGHT * GRAVITY));
                float sY = 1;
                if (uY == FLOOR) {
                    sY = 1 - (sin(elapsed * 20) + 1) / 10.0 / pow(elapsed, 3);
                    uY += SCALE * (sY * LETTER_HEIGHT / 2.0 - 1);
                }
                glm::mat4 letterModel = glm::scale(glm::translate(model, glm::vec3(0, uY, 0)), glm::vec3(1, sY, 1));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(letterModel));
                break;
            }
            if (archLocations.count(i) > 0) {
                glDrawArrays(GL_TRIANGLE_FAN, i, 8);
                int end = archLocations[i] * 10;
                for(int j = 0; j < end; j+= 10)
                    glDrawArrays(GL_TRIANGLE_FAN, i + 8 + j, 10);
                glDrawArrays(GL_TRIANGLE_FAN, i + 8 + end, 8);
                i += end + 16;
            }
            else {
                glDrawArrays(GL_TRIANGLE_FAN, i, 8);
                glDrawArrays(GL_TRIANGLE_FAN, i + 8, 8);
                i += 16;
            }
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(unifiedShader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);
    
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource)
{
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

void formPrism(float x, float y, float width, float height, bool testing, float slantHorizontal, float slantVertical, bool isFrustum) {
    if (!testing)
        for (int i = 0; i < 16; ++i) {
            float deviation = abs(7.5 - i);
            bool isUp = (i == 8 || abs(4.5 - deviation) <= 1);
            bool isRight = (i != 0 && (abs(3.5 - deviation) >= 2));
            bool isBack = (i == 8 || i == 15 || (abs(2.5 - deviation) <= 1));
            vertices[location * 6 + i * 6] = x + slantHorizontal * isUp + width * isRight - isFrustum * 2 * slantHorizontal * (isUp && isRight);
            vertices[location * 6 + i * 6 + 1] = y + height * isUp + slantVertical * isRight;
            vertices[location * 6 + i * 6 + 2] = 0.15 - 0.3 * isBack;
            vertices[location * 6 + i * 6 + 3] = isUp;
            vertices[location * 6 + i * 6 + 4] = isRight;
            vertices[location * 6 + i * 6 + 5] = isBack;
        }
    location += 16;
}

void formArch(float xc, float yc, float xIn, float yIn, float phiIn, float xOut, float yOut, float phiOut, bool testing, float widthCoef) {
    const int numSlices = std::ceil(MAX_NUM_SLICES * std::max(phiIn, phiOut) / 360.0);
    int numLocations = 16 + 10 * (numSlices - 1);
    // maksimalno 406 za max_n = 40, polukrug 206
    if (!testing)
        for (int i = 0; i < numLocations; ++i) {
            int gen = std::min(std::max(1001222100 / int(pow(10, 9 - (i - 8 + 10 * (i < 8)) % 10)) % 10 + ((i - 8) / 10 - (i < 8)), 0), numSlices) - (i > numLocations - 3);
            bool isOddFan = ((i + 2) / 10) % 2;
            bool isOut = ((i + 3) % 10 > 6) ^ isOddFan ^ (abs(1.5 - abs(numLocations - 2 - i)) < 1);
            bool isBack = (i % 10 > 2) ^ isOddFan ^ (abs(3.5 - abs(3 - i)) < 1);
        
            float phi = gen * (isOut * phiOut + !isOut * phiIn) * M_PI / 180 / numSlices;
            float x = isOut * xOut + !isOut * xIn;
            float y = isOut * yOut + !isOut * yIn;

            vertices[location * 6 + i * 6] = xc + ((x - xc) * cos(phi) - (y - yc) * sin(phi)) * widthCoef;
            vertices[location * 6 + i * 6 + 1] = yc + (x - xc) * sin(phi) + (y - yc) * cos(phi);
            vertices[location * 6 + i * 6 + 2] = 0.15 - 0.3 * isBack;
            vertices[location * 6 + i * 6 + 3] = isOddFan;
            vertices[location * 6 + i * 6 + 4] = isOut;
            vertices[location * 6 + i * 6 + 5] = isBack;
            archLocations[location] = numSlices - 1;
        }
    location += numLocations;
}

void a(bool testing) {
    formPrism(offset, -0.75, 0.35, 22 / 13.0, testing, 0.55);
    formPrism(1.45 + offset, -0.75, 0.35, 22 / 13.0, testing, -0.55);
    formPrism(0.55 + offset, -7 / 52.0, 0.7, 4 / 13.0, testing, 0.1, 0, true);
    formPrism(0.55 + offset, 49 / 52.0, 0.7, 4 / 13.0, testing, 0.1, 0, true);
    if (!testing) offset += 2.2;
}
void b(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 1, 1, 0.25, testing);
    formPrism(0.3 + offset, 0.125, 0.6, 0.25, testing);
    formPrism(0.3 + offset, -0.75, 0.6, 0.25, testing);
    formArch(0.9 + offset, -0.1875, 0.9 + offset, -0.5, 180, 0.9 + offset, -0.75, 180, testing);
    if (!testing) offset += 1.85;
}
void v(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 1, 0.6, 0.25, testing);
    formPrism(0.3 + offset, 0.125, 0.6, 0.25, testing);
    formPrism(0.3 + offset, -0.75, 0.6, 0.25, testing);
    formArch(0.9 + offset, -0.1875, 0.9 + offset, -0.5, 180, 0.9 + offset, -0.75, 180, testing);
    formArch(0.9 + offset, 0.69, 0.9 + offset, 0.375, 180, 0.9 + offset, 0.125, 180, testing);
    if (!testing) offset += 1.85;
    //TODO: fix z fighting
}
void g(bool testing) {
    formPrism(offset, -0.75, 0.3, 1.75, testing);
    formPrism(offset, 1, 1.1, 0.25, testing);
    if (!testing) offset += 1.5;
}
void d(bool testing) {

}
void dj(bool testing) {

}
void e(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 1, 1, 0.25, testing);
    formPrism(0.3 + offset, 0.125, 0.8, 0.25, testing);
    formPrism(0.3 + offset, -0.75, 1, 0.25, testing);
    if (!testing) offset += 1.7;
}
void zh(bool testing) {
    formPrism(offset, -0.75, 0.4, 1, testing, 0.7);
    formPrism(0.7 + offset, 0.25, 0.4, 1, testing, -0.7);
    formPrism(1.1 + offset, -0.75, 0.3, 2, testing);
    formPrism(1.4 + offset, 0.25, 0.4, 1, testing, 0.7);
    formPrism(0.9 + offset, 0.05, 0.7, 0.4, testing);
    formPrism(2.1 + offset, -0.75, 0.4, 1, testing, -0.7);
    if (!testing) offset += 2.9;
    //TODO: fix z fighting
}
void z(bool testing) {

}
void i(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(1.2 + offset, -0.75, 0.3, 2, testing);
    formPrism(offset + 0.3, -0.75, 0.9, 0.5, testing, 0, 1.5);
    if (!testing) offset += 1.9;
}
void j(bool testing) {
    formArch(offset, -0.2, offset, -0.45, 90, offset, -0.75, 90, testing);
    formPrism(offset + 0.25, -0.2, 0.3, 1.45, testing);
    if (!testing) offset += 0.95;
}
void k(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 0.25, 0.4, 1, testing, 0.7);
    formPrism(0.3 + offset, 0.05, 0.2, 0.4, testing);
    formPrism(1 + offset, -0.75, 0.4, 1, testing, -0.7);
    if (!testing) offset += 1.8;
    //TODO: fix z fighting
}
void l(bool testing) {
    formArch(offset, -0.2, offset, -0.45, 90, offset, -0.75, 90, testing);
    formPrism(offset + 0.25, -0.2, 0.3, 1.45, testing);
    formPrism(1.25 + offset, -0.75, 0.3, 2, testing);
    formPrism(0.55 + offset, 1, 0.7, 0.25, testing);
    if (!testing) offset += 1.95;
}
void lj(bool testing) {

}
void m(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(1.9 + offset, -0.75, 0.3, 2, testing);
    formPrism(offset + 0.3, 0.75, 0.8, 0.5, testing, 0, -1.5);
    formPrism(offset + 1.1, -0.75, 0.8, 0.5, testing, 0, 1.5);
    if (!testing) offset += 2.5;
    //TODO: fix z fighting
}
void n(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(1.2 + offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 0.125, 0.9, 0.25, testing);
    if (!testing) offset += 1.9;
}
void nj(bool testing) {

}
void o(bool testing) {
    formArch(offset + 0.9, 0.25, offset + 0.9 + 0.7 / sqrt(2), 0.25 + 0.7 / sqrt(2), 360, offset + 0.9 + 1 / sqrt(2), 0.25 + 1 / sqrt(2), 360, testing, 0.9);
    if (!testing) offset += 2;
}
void p(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(1.2 + offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 1, 0.9, 0.25, testing);
    if (!testing) offset += 1.9;
}
void r(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 1, 0.6, 0.25, testing);
    formPrism(0.3 + offset, 0.125, 0.6, 0.25, testing);
    formArch(0.9 + offset, 0.69, 0.9 + offset, 0.375, 180, 0.9 + offset, 0.125, 180, testing);
    if (!testing) offset += 1.85;
}
void s(bool testing) {
    formArch(offset + 0.9, 0.25, offset + 0.9 + 0.7 / sqrt(2), 0.25 + 0.7 / sqrt(2), 270, offset + 0.9 + 1 / sqrt(2), 0.25 + 1 / sqrt(2), 270, testing, 0.9);
    if (!testing) offset += 1.9;
}
void t(bool testing) {
    formPrism(offset + 0.6, -0.75, 0.3, 1.75, testing);
    formPrism(offset, 1, 1.5, 0.25, testing);
    if (!testing) offset += 1.9;
}
void ch(bool testing) {

}
void u(bool testing) {

}
void f(bool testing) {

}
void h(bool testing) {
    formPrism(offset, -0.75, 0.35, 2, testing, 1.15);
    formPrism(1.15 + offset, -0.75, 0.35, 2, testing, -1.15);
    if (!testing) offset += 1.9;
    //TODO: fix z fighting
}
void c(bool testing) {
    formPrism(offset, -0.3, 0.3, 1.55, testing);
    formPrism(1.2 + offset, -0.3, 0.3, 1.55, testing);
    formPrism(offset, -0.55, 1.7, 0.25, testing);
    formPrism(1.4 + offset, -0.75, 0.3, 0.2, testing);
    if (!testing) offset += 2.1;
}
void tch(bool testing) {

}
void dzh(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(1.2 + offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, -0.75, 0.9, 0.25, testing);
    formPrism(0.6 + offset, -1.25, 0.3, 0.5, testing);
    if (!testing) offset += 1.9;
}
void sh(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(2 + offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, -0.75, 1.7, 0.25, testing);
    formPrism(1 + offset, -0.5, 0.3, 1.75, testing);
    if(!testing) offset += 2.7;
}

void space(bool testing) {
    if (!testing) offset += 1.5;
}

void writeWord(const std::wstring& word, bool testing) {
    for (wchar_t letter : word) {
        switch (letter) {
        case L'А':
            a(testing);
            break;
        case L'Б':
            b(testing);
            break;
        case L'В':
            v(testing);
            break;
        case L'Г':
            g(testing);
            break;
        case L'Д':
            d(testing);
            break;
        case L'Ђ':
            dj(testing);
            break;
        case L'Е':
            e(testing);
            break;
        case L'Ж':
            zh(testing);
            break;
        case L'З':
            z(testing);
            break;
        case L'И':
            i(testing);
            break;
        case L'Ј':
            j(testing);
            break;
        case L'К':
            k(testing);
            break;
        case L'Л':
            l(testing);
            break;
        case L'Љ':
            lj(testing);
            break;
        case L'М':
            m(testing);
            break;
        case L'Н':
            n(testing);
            break;
        case L'Њ':
            nj(testing);
            break;
        case L'О':
            o(testing);
            break;
        case L'П':
            p(testing);
            break;
        case L'Р':
            r(testing);
            break;
        case L'С':
            s(testing);
            break;
        case L'Т':
            t(testing);
            break;
        case L'Ћ':
            ch(testing);
            break;
        case L'У':
            u(testing);
            break;
        case L'Ф':
            f(testing);
            break;
        case L'Х':
            h(testing);
            break;
        case L'Ц':
            c(testing);
            break;
        case L'Ч':
            tch(testing);
            break;
        case L'Џ':
            dzh(testing);
            break;
        case L'Ш':
            sh(testing);
            break;
        case L' ':
            space(testing);
            break;
        }
        if (letter != L' ' && !testing) locations.push_back(location);
    }
}

int numLetters(const std::wstring& text) {
    int count = 0;
    for (wchar_t ch : text) {
        if (ch != L' ') ++count;
    }
    return count;
}

float convertX(int pixels, int screenWidth) {
    return pixels * 2.0f / screenWidth - 1;
}

float convertY(int pixels, int screenHeight) {
    return 1 - pixels * 2.0f / screenHeight;
}

void formLogo() {
    for (int i = 0; i < 4; i++) {
        if ((i + 1) % 4 < 2) {
            vertices[i * 6] = 0.69375;
            vertices[i * 6 + 3] = 0.0;
        }
        else {
            vertices[i * 6] = 0.984375;
            vertices[i * 6 + 3] = 1.0;
        }

        if (i % 4 < 2) {
            vertices[i * 6 + 1] = 0.588889;
            vertices[i * 6 + 4] = 0.0;
        }
        else {
            vertices[i * 6 + 1] = 0.972222;
            vertices[i * 6 + 4] = 1.0;
        }
        vertices[i * 6 + 2] = 0;
        vertices[i * 6 + 5] = 0;
    }
}