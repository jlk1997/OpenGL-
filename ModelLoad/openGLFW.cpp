#include"Base.h"
#include"Shader.h"
#include"ffImage.h"
#include"Camera.h"
#include"ffModel.h"

uint _texid = 0;
float _lastTime = 0;

Shader          _shader;
FF::ffModel*    _model;


Camera          _camera;

glm::mat4 _projMatrix(1.0f);
int       _width = 400;
int       _height = 300;

void rend()
{
   /* float _currentTime = glfwGetTime();
    float _deltaTime = _currentTime - _lastTime;
    _lastTime = _currentTime;*/

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    //更新摄像机位置
    _camera.update();
    //设置相应变换矩阵
    _projMatrix = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 1000.0f);
    glm::mat4 _modelMatrix(1.0f);
    _modelMatrix = glm::translate(_modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(0.01));

    _shader.start();
    //传递MVP变换到shader
    _shader.setMatrix("_modelMatrix", _modelMatrix);
    _shader.setMatrix("_viewMatrix", _camera.getMatrix());
    _shader.setMatrix("_projMatrix", _projMatrix);
    _shader.setFloat("myMaterial.m_shiness", 32.0f);
    _shader.setVec3("view_pos", _camera.getPosition());

    
    _shader.setVec3("myLight.m_pos", glm::vec3(0.0f , 1.0f , 1.0f));
    _shader.setVec3("myLight.m_ambient", glm::vec3(0.2f , 0.2f , 0.2f));
    _shader.setVec3("myLight.m_diffuse", glm::vec3(1.0f , 1.0f , 1.0f));
    _shader.setVec3("myLight.m_specular", glm::vec3(1.0f, 1.0f, 1.0f));

    //电光源衰减系数
    _shader.setFloat("myLight.c", 1.0f);
    _shader.setFloat("myLight.l", 0.09f);
    _shader.setFloat("myLight.q", 0.032f);

    //model的draw是调用了每一个mesh的draw方法
    _model->draw(_shader);

    _shader.end();
}

void initShader()
{
    _shader.initShader("../ModelLoad/shader/vPointShader.shader", "../ModelLoad/shader/fPointShader.shader");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        _camera.move(CAMERA_MOVE::MOVE_FRONT);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        _camera.move(CAMERA_MOVE::MOVE_BACK);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        _camera.move(CAMERA_MOVE::MOVE_LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        _camera.move(CAMERA_MOVE::MOVE_RIGHT);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    _camera.onMouseMove(xpos, ypos);
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwWindowHint(GLFW_FLOATING, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(_width, _height, "OpenGL Core", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
        glfwSetWindowPos(window, 600, 100);

    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, _width, _height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    _camera.lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    _camera.setSpeed(0.05f);
     
    _model = new FF::ffModel("../ModelLoad/res/ren/看表贴图.fbx");
    initShader();
 
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        rend();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}