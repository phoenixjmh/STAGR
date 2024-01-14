#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "Physics.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Model.h"

#include <vector>
class Physics;




struct SceneData {
private:
    glm::mat4 LightSpaceMatrix;

    glm::mat4 light_projection;

    float lightConstant = 1;
    float lightLinear = 0.09f;
    float lightQuadratic = 0.032f;


public:
    glm::vec3 light_color;
    glm::vec3 light_position;

    glm::mat4 GetLightSpaceMatrix()
    {
        float near_plane = 1.0f, far_plane = 20.0f;
        light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

        glm::mat4 light_view = glm::lookAt(light_position, glm::vec3(0.0f, 0.0f, 0.0f)
            , glm::vec3(0.0f, 1.0f, 0.0f));
        LightSpaceMatrix = light_projection * light_view;
        return LightSpaceMatrix;
    }
};


struct Camera {
    glm::vec3 camera_position;
    glm::vec3 camera_front;
    glm::vec3 camera_up;
    glm::vec3 camera_target;
    glm::vec3 camera_direction;
    glm::vec3 up;
    glm::vec3 camera_right;

    Camera() : camera_position(glm::vec3(0.0, 0.0, 30.0f)), camera_front(glm::vec3(0, 0, -1)),
        camera_up(glm::vec3(0, 1, 0)), camera_target(glm::vec3(0, 0, 0)),
        camera_direction(glm::normalize(camera_position - camera_target)),
        up(glm::vec3(0, 1, 0)),
        camera_right(glm::normalize(glm::cross(up, camera_direction)))
    {}
};

class Renderer {
public:
    Renderer() : NO_LIGHTING(false) { Init(); }
    Camera camera;
    GLFWwindow* GetWindow() { return window; }
    void InitScene();
    void DrawScene(float alpha);
    void DepthPass(float alpha);
    void BeginDraw();
    void Present();
    void ModelMove(float scale, glm::vec3 position);
    void DrawPlane(float size, glm::vec3 position);
    void DrawLight(float size);
    void create_depth_map();
    void DrawObject(float size, glm::vec3 position, unsigned model_id, Shader* active_shader);
    void Clean();
    bool NO_LIGHTING;
private:
    SceneData m_sceneData;

    glm::mat4 ModelMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 ModelViewProjection;
    Shader* m_light_shader = nullptr;
    Shader* m_shader = nullptr;
    Shader* m_depth;
    Shader* NOLIGHTING = nullptr;
    unsigned int depthMapFBO;
    unsigned int depthMap;

    GLFWwindow* window;
    unsigned int m_width;
    int m_height;
    void Init();
    int glad_init();
    int create_window(const int width, const int height);
    void createModels();
    void createShaders();
    void init_mvp();
    Model getModelFromID(unsigned int& ID);
    std::vector<Model> m_Models;
};

//Helpers

inline void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity
    , GLsizei length, const GLchar* message
    , const void* userParam)
{
    std::cout << "SOURCE::" << source << " | TYPE:: " << type << " | LOG::" << message << "\n";
}