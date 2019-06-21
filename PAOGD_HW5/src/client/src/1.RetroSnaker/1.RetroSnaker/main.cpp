#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <unistd.h>
#include "snake.h"
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<sys/types.h> 
#include<sys/socket.h> 
#include<unistd.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<errno.h> 
#include<netdb.h> 
#include<stdarg.h> 
#include<string.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, int client_socket_fd, struct sockaddr_in server_addr, char id);

const int SCR_WIDTH = 700, SCR_HEIGHT = 700;

Snake snake;
Snake rival_snake;
Food food;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void renderSqure(point x, int type);
void GameOver();
void createUDP();

int main ()
{
	char id = -1;
	int isStart = 0;

    struct sockaddr_in server_addr; 
    bzero(&server_addr, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server_addr.sin_port = htons(8000); //SERVER_PORT 8000
    socklen_t server_addr_length = sizeof(server_addr);
     
    int client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(client_socket_fd < 0) 
    { 
        perror("Create Socket Failed:"); 
        exit(1); 
    } 
 
    char buffer[1024]; 
    bzero(buffer, 1024); 
    char connect_request[15] = "connect UDP";
    strncpy(buffer, connect_request, strlen(connect_request)>1024?1024:strlen(connect_request));
    sendto(client_socket_fd, buffer, 1024,0,(struct sockaddr*)&server_addr,sizeof(server_addr));
    printf("connect request: %s\n", connect_request);
    memset(buffer, 0 , 1024);
    glfwInit();
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Snake", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader("cube.vs", "cube.fs");
    while (!glfwWindowShouldClose(window))
    {
    	char file_name[10];
    	file_name[0] = id;
        strncpy(buffer, file_name, strlen(file_name)>1024?1024:strlen(file_name));
        sendto(client_socket_fd, buffer, 1024,0,(struct sockaddr*)&server_addr,sizeof(server_addr));

    	memset(buffer, 0, 1024);
	    recvfrom(client_socket_fd, buffer, 1024,0,(struct sockaddr*)&server_addr, &server_addr_length);
	    if(buffer[0] == 'Y') {
	    	printf("My id is %c\n", buffer[11]);
	    	id = buffer[11];
	    }
	    if(buffer[0] == 'S') {
	    	printf("Start game!\n");
	    	isStart = 1;
	    }

	    if(abs(id - buffer[0]) == 1) {
	    	if(buffer[1] == 'a' || buffer[1] == 's' || buffer[1] == 'd' || buffer[1] == 'w') {
	    		printf("enermy's instrution is %c\n", buffer[1]);
	    		switch(buffer[1]) {
	    			case 'w': 
	    				rival_snake.Turn(UP);
	    				break;
	    			case 'a': 
	    				rival_snake.Turn(LEFT);
	    				break;
	    			case 's': 
	    				rival_snake.Turn(DOWN);
	    				break;
	    			case 'd': 
	    				rival_snake.Turn(RIGHT);
	    				break;
	    		}
	    	}
	    }

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

        glClearColor (0.1f, 0.1f, 0.1f, 1.0f);
        glClear (GL_COLOR_BUFFER_BIT);
        
        for (int i = -40; i < 40; i++)
        {
            renderSqure(point(39, i), 1);
            renderSqure(point(-40, i), 1);
            renderSqure(point(i, -40), 1);
            renderSqure(point(i, 39), 1);
        }
        
        renderSqure(food.loca, 1);
        if (food.Produce(snake.body))
        {
            food.Eaten = false;
        }
        
        int j = 0;
        for (auto i = snake.body.begin(); i != snake.body.end(); i++)
        {
            renderSqure(*i, 1);
            j++;
        }
        j = 0;
        for (auto i = rival_snake.body.begin(); i != rival_snake.body.end(); i++)
        {
            renderSqure(*i, 2);
            j++;
        }

        if(isStart == 1) {
        	if(deltaTime > 0.01) {
        		
	        	snake.Move();
	        	rival_snake.Move();
	        	lastFrame = currentFrame;
        	}
	        
	        processInput(window,client_socket_fd, server_addr, id);
        }
        
        if (snake.collision())
        {
            cout << "Game Over" << endl;
            break;
        }
        
        if (snake.eat_food(food.loca))
        {
            food.Eaten = true;
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        shader.setVec4("vertexColor", glm::vec4(1.0, 1.0, 1.0, 1.0));
        GameOver();
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void renderSqure(point p, int type)
{
    float x1 = p.x * 0.025;
    float x2 = x1 + 0.025;
    float y1 = p.y * 0.025;
    float y2 = y1 + 0.025;

    if(type == 1) {
	    glBegin(GL_TRIANGLES);
	    glColor3f(1.0, 1.0, 1.0);       
	    glVertex3f(x2, y2, 0.0f);     
	    glColor3f(1.0, 1.0, 1.0);      
	    glVertex3f(x2, y1, 0.0f);     
	    glColor3f(1.0, 1.0, 1.0);      
	    glVertex3f(x1, y2, 0.0f);    
	    glEnd();
	    glBegin(GL_TRIANGLES);
	    glColor3f(1.0, 1.0, 1.0);       
	    glVertex3f(x1, y2, 0.0f);     
	    glColor3f(1.0, 1.0, 1.0);       
	    glVertex3f(x1, y1, 0.0f);     
	    glColor3f(1.0, 1.0, 1.0);      
	    glVertex3f(x2, y1, 0.0f);    
	    glEnd();
	}
	if(type == 2) {
	    glBegin(GL_TRIANGLES);
	    glColor3f(1.0, 1.0, 0.0);       
	    glVertex3f(x2, y2, 0.0f);     
	    glColor3f(1.0, 1.0, 0.0);      
	    glVertex3f(x2, y1, 0.0f);     
	    glColor3f(1.0, 1.0, 0.0);      
	    glVertex3f(x1, y2, 0.0f);    
	    glEnd();
	    glBegin(GL_TRIANGLES);
	    glColor3f(1.0, 1.0, 0.0);       
	    glVertex3f(x1, y2, 0.0f);     
	    glColor3f(1.0, 1.0, 0.0);       
	    glVertex3f(x1, y1, 0.0f);     
	    glColor3f(1.0, 1.0, 0.0);      
	    glVertex3f(x2, y1, 0.0f);    
	    glEnd();
	}
}

void processInput(GLFWwindow *window, int client_socket_fd, struct sockaddr_in server_addr, char id)
{
    char file_name[10]; 
    bzero(file_name, 10); 
    
    char buffer[1024]; 
    bzero(buffer, 1024); 

    if(sendto(client_socket_fd, buffer, 1024,0,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0) 
    { 
        perror("Send File Name Failed:"); 
        exit(1); 
    } 

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        strcpy(file_name, "0q");
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        snake.Turn(UP);
        strcpy(file_name, "0w");
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        snake.Turn(DOWN);
        strcpy(file_name, "0s");
    	file_name[0] = id;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        snake.Turn(LEFT);
        strcpy(file_name, "0a");
    	file_name[0] = id;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        snake.Turn(RIGHT);
        strcpy(file_name, "0d");
    	file_name[0] = id;
    }
    
    file_name[0] = id;
    strncpy(buffer, file_name, strlen(file_name)>1024?1024:strlen(file_name));
    sendto(client_socket_fd, buffer, 1024,0,(struct sockaddr*)&server_addr,sizeof(server_addr));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GameOver()
{
    int o_firstX = -15;
    int o_firstY = -4;
    for (int i = 0; i <= 5; i++)
    {
        renderSqure(point(o_firstX + i, o_firstY), 1);
        renderSqure(point(o_firstX + i, o_firstY + 8), 1);
    }
    for (int i = 0; i <= 8; i++)
    {
        renderSqure(point(o_firstX, o_firstY + i), 1);
        renderSqure(point(o_firstX + 5, o_firstY + i), 1);
    }
    int u_firstX = -2;
    int u_firstY = -4;
    for (int i = 0; i <= 5; i++)
    {
        renderSqure(point(u_firstX + i, u_firstY), 1);
    }
    for (int i = 0; i <= 8; i++)
    {
        renderSqure(point(u_firstX, u_firstY + i), 1);
        renderSqure(point(u_firstX + 5, u_firstY + i), 1);
    }
    int t_firstX = 10;
    int t_firstY = -4;
    for (int i = 0; i <= 6; i++)
    {
        renderSqure(point(t_firstX + i, t_firstY + 8), 1);
    }
    for (int i = 0; i <= 8; i++)
    {
        renderSqure(point(t_firstX + 3, t_firstY + i), 1);
    }
}
