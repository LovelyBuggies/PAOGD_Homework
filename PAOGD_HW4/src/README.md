## 利用OpenGL设计贪吃蛇游戏

---

### 游戏简介
🐍先生十个贪婪的吃货。生活在丛林中的他妄图喝光所有的果汁来让自己生长。但是"人心不足蛇吞象”，他往往会误食自己或撞上石头。游戏中，玩家控制贪吃蛇在游戏区域里驰骋，避免碰到自己或障碍物，尽可能地吃更多的食物以生长！

**游戏玩法**：

- WASD控制蛇的移动
- 游戏开始，每隔一定时间会在地图空闲位置刷新一个食物，蛇触碰到食物后食物消失，蛇会增加一个单位的长度
- 当蛇触碰到自己或者障碍物，则游戏失败
- 当蛇接触到地图边界，蛇会在地图另一端重新进入地图



---

### 开发环境

- OpenGL3
- GLFW
- GLAD



---

### 提示

1. 主程序：程序窗口管理、按键响应、游戏对象管理、状态传递（维护程序状态）
![image](http://upload-images.jianshu.io/upload_images/3220531-6ac881fc4ac87a8e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
3. 精灵类：维护精灵状态，提供渲染接口
![image](http://upload-images.jianshu.io/upload_images/3220531-15276c545cbe2b83.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
3. 工具类：管理着色器、纹理的加载（也有人习惯把这部分代码耦合到精灵类中!）
![image](http://upload-images.jianshu.io/upload_images/3220531-317db2b4a037f5dd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2. 游戏类：维护游戏状态，提供更新状态的接口供主程序调用，进行碰撞检测等
![image](http://upload-images.jianshu.io/upload_images/3220531-57e3ec55b96508d1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

虽然一个完整游戏包含的内容远不止上面提到的四点，实现了这些内容，游戏的主体部分就大致完成了



---

### 目标
1. 完成贪吃蛇游戏的框架搭建。
2. 完成蛇以及食物的 **3D** 精灵加载和渲染，其中模型可以用简单的纯色几何模型实现如立方体、球体；或者网上下载合适的3D模型如[Apple-Poly](https://poly.google.com/view/5hRReRDr0v4)、[Snake-Poly](https://poly.google.com/view/2ovwPNrRijL)。
3. 实现蛇的控制。
4. 蛇的碰撞检测与响应。



---

### 环境搭建

这个项目主要是用Visual Studio搭建的环境。如何在VS上搭建openGL开发环境在此不再赘述，主要参考这个链接：[https://learnopengl-cn.github.io/01%20Getting%20started/02%20Creating%20a%20window/](https://learnopengl-cn.github.io/01 Getting started/02 Creating a window/)



---

### 实现

#### ★ 蛇身块

> 块类的定义在Block.h文件中，实现在Block.cpp文件中。

`Class Block`中，私有成员有：**位置**、**大小**、**颜色**。

`Class Block`中，公有成员如下：

- **构造函数**：初始化块位置和块大小。
- **set*()**：设置函数。
- **get*()**：获取函数。

#### ★ 果茶

> 果茶类的定义在Fruit.h文件中，实现在Fruit.cpp文件中。

`Class Fruit`中，私有成员有：**位置**、**大小**、**颜色**、**角度**。其中角度用来控制旋转。

`Class Fruit`中，公有成员如下：

- **构造函数**：初始化果茶位置和果茶大小。
- **set*()**：设置函数。
- **get*()**：获取函数。

#### ★ 磐石

> 磐石类的定义在Stone.h文件中，实现在Stone.cpp文件中。

`Class Stone`中，私有成员有：**位置**、**大小**、**颜色**。

`Class Stone`中，公有成员如下：

- **构造函数**：初始化石头位置和石头大小。
- **set*()**：设置函数。
- **get*()**：获取函数。

#### ★ 贪吃蛇

> 蛇类的定义在Snake.h文件中，实现在Snake.cpp文件中。

`Class Snake`中，私有成员如下：

-  **currentDirection**：记录当前🐍移动的方向；
- **isUpdated**：🐍是否更新过了；
- **score**：游戏得分。

`Class Snake`中，公有成员如下：

- **构造函数**：初始化的时候，蛇头被定义为黄色，蛇的移动方向向左。
- **draw()**：画出蛇的构成components，包含多个块类链表。
- **move(Direction direction)**：定义蛇的基本移动。蛇的移动速度 speed 随着分数增长逐渐变快，游戏难度增高。
- **pushSnake()**：块相连，构成蛇。
- **update(Fruit* fruit, Stone* stones[])**：更新🐍的状态，包含碰撞检测（果茶和石头），每0.1秒检查一下。
- **collisionDetection(Fruit* fruit, Stone* stones[])**：碰撞检查。
  - 喝了果茶，根据难度加分：
    - 难度越高，加分越多；
    - 产生新口（yan）味（se）的果茶（*判断条件：不能在**蛇身上**或者**磐石上**产生新的果茶*）；
    - 根据难度变长，难度越高变长越多。
  - 直面石头，smash to piece! 粉身碎骨！弹窗隐藏。
- **addBlock()**：喝了果茶，会长身体。同样，难度越高，长身体会越快，同时面临着更加危险的局面。

#### ★ 画图

> 画图类的定义在Draw.h文件中，实现在Draw.cpp文件中。

`Class Draw`中，定义了两个函数：

- **buildDLs()**：画 cube 的 list，手动定义 `cube`。
- **drawGrid()**：画网格背景
  - GL_QUADS：立体四边形作为边界修饰；
  - GL_LINES：格子线和修饰边线。

#### ★ 主文件

灯光处理。

```cpp
GLfloat lightAmbient[] = { 0.5,0.5,0.5,1 };
GLfloat lightDiffuse[] = { 1,1,1,1 };
GLfloat lightPosition[] = { 0,3,0 };
```

物体创建。

``` cpp
Fruit* fruit = new Fruit(1, 1);
Snake* snake = new Snake(0, 0, 3);
Stone* stone = new Stone(-3,-3);
Stone* stones[2] = { stone };
```

重定义窗口大小、触发事件。

```cpp
resize(int w, int h);
keyEvents(unsigned char key, int x, int y);
mouseEvents(int button, int dir, int x, int y);
```

初始化GL。

```cpp
void initGL(void) {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
}
```

画游戏系统图。

```cpp
draw(){
  Draw d;
	d.drawGrid();
	snake->draw();
	stone1->draw();
	fruit->draw();
}
```

main函数的流程如下：

1. 初始化（包括创建窗口和显示模式）；
2. 画游戏系统图；
3. 事件收集和应答；
4. 不断循环。

```cpp
int main(int argc, char **argv) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Greedy Snake");

	initGL();
	Draw d;
	d.buildDLs();

	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyEvents);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(mouseEvents);

	glutMainLoop();

	return 1;
}
```

至此，我们贪吃蛇小游戏就实现完了。🎉



---

### 效果展示

#### 游戏视频

由于文稿的限制，美好的游戏体验 🎮 无法展示出来，所以我将游戏的视频放到了 Youtube 油管上。

- https://www.youtube.com/watch?v=QntNhYvB-5o —— 游戏节选 1
- https://www.youtube.com/watch?v=UcTRJ33ivMs —— 游戏节选 2
- https://www.youtube.com/watch?v=OrmnSZH6kkI —— 游戏节选 3
- https://www.youtube.com/watch?v=cXnobgYxbBs —— 因食己而死
- https://www.youtube.com/watch?v=3mbC-D8uZ5c —— 因撞壁而亡

#### 游戏截图

![image.png](https://upload-images.jianshu.io/upload_images/3220531-311692c2d53df424.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![image.png](https://upload-images.jianshu.io/upload_images/3220531-58e2d7b7459503bc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![pic1.gif](https://upload-images.jianshu.io/upload_images/3220531-d3ad2e6026293a83.gif?imageMogr2/auto-orient/strip)

![pic2.gif](https://upload-images.jianshu.io/upload_images/3220531-6e62c44e531b4917.gif?imageMogr2/auto-orient/strip)

![pic3](https://upload-images.jianshu.io/upload_images/3220531-6886abb459d37e66.gif?imageMogr2/auto-orient/strip)

![pic4](https://upload-images.jianshu.io/upload_images/3220531-69c2d2344a3dd1d4.gif?imageMogr2/auto-orient/strip)





---

### 参考

1. [OpenGL游戏-框架设计](https://learnopengl.com/In-Practice/2D-Game/Setting-up)
2. [OpenGL游戏-精灵渲染](https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites)
3. [OpenGL游戏-碰撞检测](https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection)
4. [OpenGL模型创建](https://learnopengl.com/Getting-started/Hello-Triangle)
5. [Google Poly-3D模型库](https://poly.google.com)



---