# HW5 简单的服务端

---

## 任务介绍

> 为贪吃蛇🐍游戏添加多人游戏功能：多个玩家👯‍♂️可以分别控制贪吃蛇在游戏区域里驰骋，避免碰到自己、同类或障碍物，尽可能地吃更多的食物🍅以生长！



---

## 开发环境

- OpenGL3
- GLFW
- GLAD
- C++

环境的搭建主要是参考了 https://learnopengl.com/，可参考我[之前的博客](https://www.jianshu.com/p/ae3bccbd6019)。



---

## 功能介绍
- 必须连接到服务器才能开始游戏，成功上线服务器返回“WELCOME USERNAME”的欢迎信息😁
- 当没有新的玩家上线时，玩家可以进行单人游戏，当游戏已经开始时第二个玩家无法上线，服务器返回“BUSY”🙃
- 当两个以上玩家同时上线时，可以进行多人游戏，客户端界面出现不同颜色的蛇🐍，分别由不同玩家进行控制👯‍♂️
- 当某一玩家关闭连接，该玩家游戏失败😵
- 除了单机贪吃蛇的基本规则外，当蛇头接触到同类时，该玩家游戏失败😵
- 生存时间长的玩家获胜，用你的身体挡住对手的路吧😛

*实际上这是简化的服务器，更像是“房间”的概念。*



---

## 知识介绍

1. 关于网络拓扑，上述提到的是简单的Client-Server模型，游戏运行在“权威”服务器下，客户端想进行一个操作时，会发送动作请求的数据包给服务器，服务器统一处理这些请求并进行状态更新，再把更新结果反馈给客户端。
![输入图片说明](https://images.gitee.com/uploads/images/2019/0602/114027_b2fd1f31_1194012.png "屏幕截图.png")
2. 关于传输层协议，采用UDP进行传输可以使数据包的传输更加灵活，UDP socket是无连接的，必须先调用Bind函数保留一个端口，再使用ReceiveFrom取回传入的数据；UDP socket是无状态的，需要自定义流量控制以及拥塞控制等规则，以确保所有数据能够按预期进行处理。

```cpp
class UDPSocket {
public:

	~UDPSocket();

	int Bind( const SocketAddress& inToAddress );
	int SendTo( const void* inToSend, int inLength, const SocketAddress& inToAddress );
	int ReceiveFrom( void* inToReceive, int inMaxLength, SocketAddress& outFromAddress );

private:
	friend class SocketUtil;
	UDPSocket( SOCKET inSocket ) : mSocket( inSocket ) {}
	SOCKET mSocket;

};
```
3. 关于动作队列，客户端以一定的频率轮询键盘，保存到动作队列中，并将所有或者部分（最新若干个）动作发送到服务端，如何判断动作执行的先后顺序需要为动作增加时间戳，而模拟步长跟服务器、客户端的帧步长怎么设置也是需要关心的。

```cpp
void RoboCatServer::Update() {
	// Code omitted

	ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
	if( client )
	{
		MoveList& moveList = client->GetUnprocessedMoveList();
		for( const Move& unprocessedMove : moveList )
		{
			const InputState& currentState = unprocessedMove.GetInputState();
			float deltaTime = unprocessedMove.GetDeltaTime();
			ProcessInput( deltaTime, currentState );
			SimulateMovement( deltaTime );
		}

		moveList.Clear();
	}
}
```



---
## 实验完成

###  实验目标

1. 完成多人贪吃蛇游戏的C-S框架搭建。
2. 多客户端能够分别连接服务器并有正确的响应（连接成功、连接失败）。
3. 服务器能够正确处理客户端的动作请求，客户端能够正确更新蛇的移动状态。
4. 蛇的碰撞检测🐍、奖励生成🎁、胜负判断🎉等内容的实现。



### 实验实现



#### ★ Server

服务器的实现主要分为以下几个部分：`创建UDP套接口`、`创建socket`、`绑定套接口`、`数据传输`，其中，`数据传输`又包括`捕获客户端地址`和`接收数据`两个部分。

实现服务器，我们首先要创建一个UDP套接口和socket。

```CPP
struct sockaddr_in server_addr; 
bzero(&server_addr, sizeof(server_addr)); 
server_addr.sin_family = AF_INET; 
server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
server_addr.sin_port = htons(SERVER_PORT); 

int server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
if(server_socket_fd == -1) 
{ 
  perror("Create Socket Failed:"); 
  exit(1); 
} 
```

然后我们绑定了两个用户和服务器的套接口。

```cpp
if(-1 == (bind(server_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))) 
{ 
  perror("Server Bind Failed:"); 
  exit(1); 
} 

  int player_tag[2] = {0, 0};
  char p[2] = {'1','2'};
  int isStart = 0;

//instructions
char player1_instructions[10]; 
  bzero(player1_instructions, 10); 
  char player2_instructions[10]; 
  bzero(player2_instructions, 10);
```

我们接下来就可以进行数据传输了。

```cpp
while(1) 
{  
  /* 定义地址 */
  struct sockaddr_in client_addr; 
  socklen_t client_addr_length = sizeof(client_addr); 

  /* 接收数据 */
  char buffer[BUFFER_SIZE]; 
  bzero(buffer, BUFFER_SIZE); 

  if(recvfrom(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &client_addr_length) == -1) 
  { 
    perror("Receive Data Failed:"); 
    exit(1); 
  }

  if(strcmp(buffer,"connect UDP") == 0){
    printf("connect successfully\n");
    memset(buffer, 0, BUFFER_SIZE);
    for(int i = 0;i < 2;i++) {
      if(player_tag[i] == 0) {
        player_tag[i] = 1;
        char message[15] = "Your id is  .";
        message[11] = p[i];
        strcpy(buffer, message);
        sendto(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
        memset(buffer, 0, BUFFER_SIZE);
        break;
      }
    }
  }

  if(player_tag[0] == 1 && player_tag[1] == 1 && isStart < 10) { 
    printf("All player is online.Start game!\n");
    isStart++;
    memset(buffer, 0, BUFFER_SIZE);
    char s[10] = "Start";
          strncpy(buffer, s, strlen(s)>BUFFER_SIZE?BUFFER_SIZE:strlen(s)); 
    if(sendto(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr,sizeof(client_addr)) < 0) 
    { 
      perror("Response Failed:"); 
      exit(1); 
    }
  }

  if(buffer[0] == '1') {
      if(buffer[1] == 'a' || buffer[1] == 's' || buffer[1] == 'd' || buffer[1] == 'w') {
        printf("Receive message from cilent1: %s\n", buffer);
      }
      if(buffer[1] == 'q') {
        printf("player1 quit!\n");
        player_tag[0] = 0;
        isStart = 0;
      }
      strcpy(player1_instructions, buffer);
      memset(buffer, 0, BUFFER_SIZE);
          strncpy(buffer, player2_instructions, strlen(player2_instructions)>BUFFER_SIZE?BUFFER_SIZE:strlen(player2_instructions)); 
    if(sendto(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr,sizeof(client_addr)) < 0) 
    { 
      perror("Response Failed:"); 
      exit(1); 
    }
  }
  if(buffer[0] == '2') {
    if(buffer[1] == 'a' || buffer[1] == 's' || buffer[1] == 'd' || buffer[1] == 'w') {
        printf("Receive message from cilent2: %s\n", buffer);
      }
      if(buffer[1] == 'q') {
        printf("player2 quit!\n");
        player_tag[1] = 0;
        isStart = 0;
      }
    strcpy(player2_instructions, buffer);
    memset(buffer, 0, BUFFER_SIZE);
          strncpy(buffer, player1_instructions, strlen(player1_instructions)>BUFFER_SIZE?BUFFER_SIZE:strlen(player1_instructions)); 
    if(sendto(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr,sizeof(client_addr)) < 0) 
    { 
      perror("Response Failed:"); 
      exit(1); 
    }
  }
}  
```

最后关闭socket，这样服务器就创建好了！🎉🎁🎈

```c++
close(server_socket_fd);
```



#### ★ Class Snake

🐍蛇类是由蛇的移动方向`Snake_Movement`和蛇身`Snake`组成的。

```cpp
enum Snake_Movement {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake
{
public:
    list<point> body;
    Snake_Movement Snake_dir;
    Snake();
    ~Snake();
    void Move();
    void Turn(Snake_Movement direction);
    int eat_food(point location);
    int collision();
};
```

`body` list 中存蛇身list，蛇身是载入的正方体模型。`Snake_dir`是蛇的移动方向。`Move()`函数定义了蛇的移动规则，包括各个方向的移动和画面移出功能的实现。`Turn()`定了蛇移动的转弯规则。`eat_food()`函数让蛇在吃到食物后增长一个块。`collision()`进行碰撞检测。



#### ★ Class Food

为了方便，食物也定义为一个正方形的结构体。食物类🍎的实现依赖于一个点类结构体和食物结构体。

```cpp
struct point {
    int x;
    int y;
    point();
    point(int _x, int _y)();
    bool operator == (const point &p)();
};

class Food
{
public:
    point loca;
    bool Eaten;
    Food();
    ~Food();
    int Produce(list<point> L);
};
```

其中，`Eaten`表示食物是否被吃了，`Produce`函数用来生成新的事物。



#### ★ Client

客户端的实现是这个实验的关键部分。主要包括以下几个部分：`创建服务端地址`、`创建socket`、`glfw窗口创建`、`渲染循环`、`处理输入`、`判断终止`。

创建服务端地址是为了跟客户端匹配，这样Server才能处理该Client的请求，并通过socket进行连接。

``` cpp
struct sockaddr_in server_addr; 
bzero(&server_addr, sizeof(server_addr)); 
server_addr.sin_family = AF_INET; 
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
server_addr.sin_port = htons(8000); 
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
```

接下来我们要创建一个glfw的窗口，并将我们窗口的上下文设置为当前线程的主上下文。

```cpp
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
```

对于每一帧进行渲染，首先根据`cube.vs`和`cube.fs`来渲染cube模型。分别查找两个套接字的地址，然后检测连接。当第一个Client连上了之后，`buffer[0]`会被置为`Y`，表示连接；当两个Client都连上了之后，`buffer[0]`会被置为`S`，表示可以开始游戏了。然后根据输入的字符，决定移动的方向。

```cpp
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
```

渲染后需要绘制游戏元素，墙🧱、食物🍎、蛇🐍的绘制方法如下所示。

```cpp
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
```

我们还需要设计一个`processInput()`的函数来对输入进程进行处理。

```cpp
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
```

游戏开始前，我们还要设定一下Game Over😵。

```cpp
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
```

这下，游戏🎮终于可以开始了！控制移动🐍、检测碰撞💥、食物奖励🍎一个都不落下！

``` cpp
if(isStart == true) {
  
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
```

接下来让我们看看实验的结果吧！！！😊



### 实验结果

★ **C-S框架搭建**🏠：游戏要开始时，启动server，两个客户端尚未连接。我们暂时给Client1的蛇命名为**白素贞**，Client2的蛇明明为**小青**。

![游戏开始](https://i.loli.net/2019/06/17/5d077a7980b9f14639.png)

★ **连接响应**🔗：白素贞和小青在服务器的帮助下，千里传音，互有回响。如下两图分别显示白素贞和小青的连接成功通知。

![素贞连接成功](https://i.loli.net/2019/06/17/5d077fe19161065101.png)

![小青连接成功](https://i.loli.net/2019/06/17/5d0780592836e65934.png)

★ **状态更新**：👯‍♂️两条蛇开始游戏。对于每一个客户端而言，自己是白色的，而对方是黄色的。

![状态更新](https://i.loli.net/2019/06/17/5d077aea73c8962028.png)

★ **奖励生成**🎁：白素贞吃了仙果法力增长，变得更长。（*由于UDP传输有丢包现象，所以有时候会出现一定延迟，此时可能出现的情况是小青短时间内无法收到素贞已经增长法力的消息*）

![奖励生成](https://i.loli.net/2019/06/17/5d0781a8ce4e450916.png)

★ **碰撞检测**💥：小青和白素贞相遇，最终同归于尽，游戏退出，小青获胜。（*由于UDP的时延问题，两条蛇并没有同时感知到碰撞消息，而且碰撞了之后过一段时间游戏才会结束*）

![碰撞检测](https://i.loli.net/2019/06/17/5d07820ba797f99520.png)

★ **GIF效果**：以下是游戏的GIF截图。

![1.gif](https://upload-images.jianshu.io/upload_images/3220531-4cbd9f7e3ab0271b.gif?imageMogr2/auto-orient/strip)

![2.gif](https://upload-images.jianshu.io/upload_images/3220531-2e65f08125fcbac2.gif?imageMogr2/auto-orient/strip)


★ **玩家体验**👯‍♂️：我将游戏的玩家体验上传到了我的[Youtube频道——多玩家贪吃蛇](https://youtu.be/4twxTL2pCLo)，欢迎观看！🎉



---

## 参考资料

- 《网络多人游戏架构与编程》



---