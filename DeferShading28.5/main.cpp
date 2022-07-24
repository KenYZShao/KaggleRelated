#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor=vec4(0.8f,0.3f,0.02f,1.0f);\n"
"}\n\0";

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    //Tell GLFW what version of OpenGL we are using
    //In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Tell GLFW we are using the CORE profile
    //So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    //Greate a GLFWwindow object of 800 by 800 pixels, naming it "LearnOpenGL"
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    //Error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //Introduce the window into the current context
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Youtube referenced video added extra
    //https://youtu.be/hYZNN0MTLuc
    gladLoadGL();
    //Specify the viewport of OpenGL in the window
    // In this case the viewport gose from x=0,y-0, to x=800, y=800
    glViewport(0, 0, 800, 800);
    //Youtube added up to here
    
    //Create Vertex Shader Object and get its reference
    GLunit vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Attach Vertex Shader source to the Vertex Shadder Object
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    //Compile the Vertex Shader into machien code
    glCompileShader(vertexShader);

    //Create Fragment Shader Object and get its reference
    GLunit fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //Compile the Vertex shader into machine code
    glCompileShader(fragmentShader);

    //Create shader Program Object and get its reference
    GLunit shaderProgram = glCreateProgram();
    //Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(shaderProgram);

    //Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //Vertices coordinates
    GLfloat vertices[] =
    {
        -0.5f,-0.5f * float(sqrt(3)) / 3,0.0f,
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.0f,0.5f * float(sqrt(3)) * 2 / 3,0.0f
    };

    //Create reference containers for the Vertex Array Object and the Vertex Buffer Object
    GLuint VAO, VBO;

    //Generate the VAO and VBO with only 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Make the VAO the current Vertex Array Object by binding it
    glBindVertexArray(VAO);

    //Bind the VBO specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Introduce the vertices into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);

    //Bind both the VBO and VAO to 0 so that we don't accidently modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        //Specify the color of the background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        //Tell OpenGL which Shader Program we want to use
        glUseProgram(shaderProgram);
        //Bind the VAO so OpenGL knows to use it
        glBindVertexArray(VAO);
        //Draw the traiangle using the GL_TRIANGLES primitive
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //Swap the back buffer with the ffront buffer

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        //Take care of all GLFW events
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    //Delete all the objects we've created
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    //Delete window before ending the program
    glfwDestroyWindow(window);
    //Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}