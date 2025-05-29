#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#define MAX_NUM_SLICES 40
 
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

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);

void formPrism(float x, float y, float width, float height, bool testing, float slant = 0.0f, bool isFrustum = false);
void formArch(float xc, float yc, float xa, float ya, float phia, float xb, float yb, float phib, bool testing, bool clockwise = false);
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

void writeWord(const std::wstring& word, bool testing = false);

float* vertices;

int location = 0;
float offset = -12;

std::map<int, int> archLocations = {};

std::vector<int> arches = {};

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
    unsigned int wWidth = 1800;
    unsigned int wHeight = 900;
    const char wTitle[] = "[Generic Title]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    
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

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");

    const std::wstring word = L"БАБАКИКИ";
    writeWord(word, true);
    const int verticesSize = location * 6;
    vertices = new float[verticesSize];
    //std::cout << verticesSize / 6;
    location = 0;
    writeWord(word);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++            UNIFORME            +++++++++++++++++++++++++++++++++++++++++++++++++

    glm::mat4 model = glm::mat4(1.0f); //Matrica transformacija - mat4(1.0f) generise jedinicnu matricu
    model = glm::scale(model, glm::vec3(0.75, 0.75, 0.75));
    unsigned int modelLoc = glGetUniformLocation(unifiedShader, "uM");
    
    glm::mat4 view; //Matrica pogleda (kamere)
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
    unsigned int viewLoc = glGetUniformLocation(unifiedShader, "uV");
    
    
    glm::mat4 projectionP = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f); //Matrica perspektivne projekcije (FOV, Aspect Ratio, prednja ravan, zadnja ravan)
    glm::mat4 projectionO = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 0.1f, 50.0f); //Matrica ortogonalne projekcije (Lijeva, desna, donja, gornja, prednja i zadnja ravan)
    unsigned int projectionLoc = glGetUniformLocation(unifiedShader, "uP");


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP - PETLJA ZA CRTANJE +++++++++++++++++++++++++++++++++++++++++++++++++
    glUseProgram(unifiedShader); //Slanje default vrijednosti uniformi
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); //(Adresa matrice, broj matrica koje saljemo, da li treba da se transponuju, pokazivac do matrica)
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));
    glBindVertexArray(VAO);

    glClearColor(0.0, 0.0, 0.7, 1.0);
    glCullFace(GL_BACK);//Biranje lica koje ce se eliminisati (tek nakon sto ukljucimo Face Culling)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glUniform1f(glGetUniformLocation(unifiedShader, "uY"), -0.25);

    while (!glfwWindowShouldClose(window))
    {
        double time = glfwGetTime();
        while (glfwGetTime() - time < 1 / 60.0) {}

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        //Testiranje dubine
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            glEnable(GL_DEPTH_TEST); //Ukljucivanje testiranja Z bafera
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            glDisable(GL_DEPTH_TEST);
        }

        //Odstranjivanje lica (Prethodno smo podesili koje lice uklanjamo sa glCullFace)
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            glEnable(GL_CULL_FACE);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            glDisable(GL_CULL_FACE);
        }

        //Mijenjanje projekcija
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));
        }
        //Transformisanje trouglova
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            //model = glm::translate(model, glm::vec3(-0.01, 0.0, 0.0)); //Pomjeranje (Matrica transformacije, pomjeraj po XYZ)
            model = glm::rotate(model, glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotiranje (Matrica transformacije, ugao rotacije u radijanima, osa rotacije)
            //model = glm::scale(model, glm::vec3(0.99, 1.0, 1.0)); //Skaliranje (Matrica transformacije, skaliranje po XYZ)
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            //model = glm::translate(model, glm::vec3(0.01, 0.0, 0.0));
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(1/0.99, 1.0, 1.0));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            //model = glm::translate(model, glm::vec3(-0.01, 0.0, 0.0)); //Pomjeranje (Matrica transformacije, pomjeraj po XYZ)
            model = glm::rotate(model, glm::radians(-0.5f), glm::vec3(1.0f, 0.0f, 0.0f)); //Rotiranje (Matrica transformacije, ugao rotacije u radijanima, osa rotacije)
            //model = glm::scale(model, glm::vec3(0.99, 1.0, 1.0)); //Skaliranje (Matrica transformacije, skaliranje po XYZ)
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            //model = glm::translate(model, glm::vec3(0.01, 0.0, 0.0));
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(1/0.99, 1.0, 1.0));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Osvjezavamo i Z bafer i bafer boje
        
        int i = 0;
        while (i < location) {
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


void formPrism(float x, float y, float width, float height, bool testing, float slant, bool isFrustum) {
    if (!testing)
        for (int i = 0; i < 16; ++i) {
            float deviation = abs(7.5 - i);
            bool isUp = (i == 8 || abs(4.5 - deviation) <= 1);
            bool isRight = (i != 0 && (abs(3.5 - deviation) >= 2));
            bool isBack = (i == 8 || i == 15 || (abs(2.5 - deviation) <= 1));
            vertices[location * 6 + i * 6] = x + slant * isUp + width * isRight - isFrustum * 2 * slant * (isUp && isRight);
            vertices[location * 6 + i * 6 + 1] = y + height * isUp;
            vertices[location * 6 + i * 6 + 2] = 0.15 - 0.3 * isBack;
            vertices[location * 6 + i * 6 + 3] = 1.0;
            vertices[location * 6 + i * 6 + 4] = 1.0;
            vertices[location * 6 + i * 6 + 5] = 1.0;
        }
    location += 16;
}


void formArch(float xc, float yc, float xIn, float yIn, float phiIn, float xOut, float yOut, float phiOut, bool testing, bool clockwise) {
    const int numSlices = std::ceil(MAX_NUM_SLICES * std::max(phiIn, phiOut) / 360.0);
    //const int numSlices = 4;
    int numLocations = 16 + 10 * (numSlices - 1);
    // maksimalno 406 za max_n = 40, polukrug 206
    if (!testing)
        for (int i = 0; i < numLocations; ++i) {
            int gen = std::min(std::max(1001222100 / int(pow(10, 9 - (i - 8 + 10 * (i < 8)) % 10)) % 10 + ((i - 8) / 10 - (i < 8)), 0), numSlices) - (i > numLocations - 3);
            bool isOddFan = ((i + 2) / 10) % 2;
            bool isOut = ((i + 3) % 10 > 6) ^ isOddFan ^ (abs(1.5 - abs(numLocations - 2 - i)) < 1);
            bool isBack = (i % 10 > 2) ^ isOddFan ^ (abs(3.5 - abs(3 - i)) < 1);
        
            float phi = gen * (isOut * phiOut + !isOut * phiIn) * M_PI / 180 / numSlices * pow(-1, clockwise);
            float x = isOut * xOut + !isOut * xIn;
            float y = isOut * yOut + !isOut * yIn;

            vertices[location * 6 + i * 6] = xc + (x - xc) * cos(phi) - (y - yc) * sin(phi);
            vertices[location * 6 + i * 6 + 1] = yc + (x - xc) * sin(phi) + (y - yc) * cos(phi);
            vertices[location * 6 + i * 6 + 2] = 0.15 - 0.3 * isBack;
            vertices[location * 6 + i * 6 + 3] = 1.0;
            vertices[location * 6 + i * 6 + 4] = 1.0;
            vertices[location * 6 + i * 6 + 5] = 1.0;
        }
    else archLocations[location] = numSlices - 1;
    location += numLocations;
}

void a(bool testing) {
    formPrism(offset, -0.75, 0.35, 2, testing, 0.65);
    formPrism(0.5 + offset, -0.25, 0.8, 0.25, testing);
    formPrism(1.45 + offset, -0.75, 0.35, 2, testing, -0.65);
    if (!testing) offset += 2.2;
}
void b(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 1, 1, 0.25, testing);
    formPrism(0.3 + offset, 0.125, 0.6, 0.25, testing);
    formPrism(0.3 + offset, -0.75, 0.6, 0.25, testing);
    formArch(0.9 + offset, -0.1875, 0.9 + offset, 0.125, 180, 0.9 + offset, 0.375, 180, testing, true);
    if (!testing) offset += 1.85;
}
void v(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 1, 0.6, 0.25, testing);
    formPrism(0.3 + offset, 0.125, 0.6, 0.25, testing);
    formPrism(0.3 + offset, -0.75, 0.6, 0.25, testing);
    formArch(0.9 + offset, -0.1875, 0.9 + offset, 0.125, 180, 0.9 + offset, 0.375, 180, testing, true);
    formArch(0.9 + offset, 0.69, 0.9 + offset, 0.375, 180, 0.9 + offset, 0.125, 180, testing);
    if (!testing) offset += 1.85;
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
}
void z(bool testing) {

}
void i(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(1.2 + offset, -0.75, 0.3, 2, testing);
    formPrism(offset, -0.75, 0.3, 2, testing, 1.2);
    if (!testing) offset += 1.9;
}
void j(bool testing) {

}
void k(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 0.25, 0.4, 1, testing, 0.7);
    formPrism(0.3 + offset, 0.05, 0.2, 0.4, testing);
    formPrism(1 + offset, -0.75, 0.4, 1, testing, -0.7);
    if (!testing) offset += 1.8;
}
void l(bool testing) {

}
void lj(bool testing) {

}
void m(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(2 + offset, -0.75, 0.3, 2, testing);
    formPrism(1 + offset, -0.75, 0.3, 2, testing, 0.8);
    formPrism(1 + offset, -0.75, 0.3, 2, testing, -0.8);
    if (!testing) offset += 2.7;
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

}
void p(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(1.2 + offset, -0.75, 0.3, 2, testing);
    formPrism(0.3 + offset, 1, 0.9, 0.25, testing);
    if (!testing) offset += 1.9;
}
void r(bool testing) {

}
void s(bool testing) {

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
}
void c(bool testing) {
    formPrism(offset, -0.75, 0.3, 2, testing);
    formPrism(1.2 + offset, -0.5, 0.3, 1.75, testing);
    formPrism(0.3 + offset, -0.75, 1.4, 0.25, testing);
    formPrism(1.4 + offset, -1.25, 0.3, 0.5, testing);
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

//int getPrismCount(const std::wstring& word) {
//    int count = 0;
//    for (wchar_t letter : word) {
//        count += prismCounts.at(letter);
//    }
//    return count;
//}

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
        }
    }
}