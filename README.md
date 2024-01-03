## 简介

* c++学习的地址：www.learncpp.com

## 入门

### 简介

* OpenGL仅仅只是一个规范，并不是一个API
* 渲染模式分为立即渲染模式和核心渲染模式，前者就是固定渲染管线，后者开放更多细节交给开发者使用
* OpenGL自身是一个巨大的状态机，一些列的变量描述OpenGL此时应该如何运行。这个状态通常被称为上下文，**OpenGL本身提供了一些状态设置函数以及状态使用函数，前者可以修改上下文，后者可以利用上下文进行一系列操作。**
* OpenGL中的对象大多采用结构体表示。
* 当我们需要对一个对象进行操作时，我们对需要使用的对象，使用一个id来保存它的引用。然后我们将对象绑定到上下文的目标位置，对对象结束操作之后，解绑这个对象，设置的内容还会存在于ObjectID所引用到的对象中。

#### 环境配置

为了可以再不同的电脑上开发，这边还是将windows下的环境配置文件上传了。

* 构建GLFW，[源码地址](http://www.glfw.org/download.html)，可以下载编译的库以及二进制文件
* 下载CMake，使用CMake GUI，选择源代码目录以及存放编译结果的目标文件目录，选择GLFW源码目录，然后新建一个build文件夹，选中作为目标目录。之后Configure点击一下，生成器选择是Visual Studio16的工具链。再次点击Configure，保存之后，点击Generate，生成的工程文件就会在你的build文件夹中。
* 打开build目录，打开sln工程，直接点击build solution，在build/src/Debug下就有glfw3.lib。
* 创建工程后选择x64平台，在属性的VC++的目录下添加include目录以及lib目录。
* 除此之外，在链接器部分的Input部分，在Additional Dependencies添加glfw3.lib，除此之外，还需要添加opengl32.lib。
* 配置[GLAD](https://glad.dav1d.de/), 选择语言为c/c++，选择3.3的opengl版本，profile设置core，选中Generate a loader，点击生成即可。GLAD会返回两个include文件夹和一个glad.c，需要将glad.c添加到你的工程中。

添加指令

```cpp
#inlcude <glad/glad.h>
```

### 创建窗口

* 刚开始初始化glfw

* 之后设置好glfw的版本号码（自己在这里用的是3.3）

* 然后设置opengl的渲染模式，core_profile

* 创建窗口，并且将当前window的context设置成本线程的context.

* 然后去gladLoadGLLoader加载函数的实际地址

* 之后就是调整视口，窗口调整回调以及renderLoop的逻辑

具体可以看代码中的内容。

### 绘制三角形

三个名词:

* Vertex Array Object, VAO

* Vertex Buffer Object, VBO

* Element Buffer Object, EBO或者称为Index Buffer Object, IBO

#### 可配置阶段

顶点着色器，集合着色器和片段着色器

#### 顶点输入

绘制图形之前，我们需要先给OpenGL输入一些顶点数据，openGL的NDC只处理在[-1.0 ， 1.0]之间的数据（一般来说，vertex shader会将之前的空间坐标系转换至NDC中），之后再做一次视口变换即可。

* 我们通过VBO来管理这个内存，他会在GPU显存中存储大量顶点，使用VBO的好处是我们可以一次性发送大批数据到显卡上，而不是每个顶点发送一次。

#### 创建着色器

* 使用glCreateShader(\<SHADER_TYPE\>)来创建shader

* 使用`glShadaerSource`来替换shader中的代码

* `glCompileShader`用于编译shader。

#### 链接着色器

* 说是链接，其实是单独的顶点着色器与片段着色器需要一个shaderprogram才可以使用。

下面的例子自行体会

```cpp
unsigned int shaderProgram;
shaderProgram = glCreateProgram();

glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);
// 激活该程序对象
glUseProgram(ShaderProgram);
```

#### 链接顶点属性

需要指定OpenGL如何理解顶点数据。

* 使用`glVertexAttribPointer`指定顶点数组的一系列属性

* 完成之后，需要使用glEnableVertexAttribArray来指定

* The zeroing behavior does not match x86 (i.e. this instruction zeroes when an index is out-of-range instead of when the most significant bit is 1); use a constant swizzle amount (or i8x16.shuffle) to avoid 3 extra x86 instructions in some runtimes.

## 高级OpenGL篇

这边插一句，因为中间的笔记都写在纸质部分，因此这里直接从高级数据部分开始，但是中间缺失的笔记部分也会从纸质转到电子文档。

### 高级数据

* OpenGL中的数据无外乎是一个管理特定内的对象，将其绑定到一个Buffer Target上时才为其赋予了意义。例如，绑定缓冲到GL_ARRAY_BUFFER，他就表示一个顶点数组缓冲，绑定的target不同，处理的方式也不一样。
* 目前为止，我们填充Vertex buffer object的方式还是glBufferData来进行填充，如果将data参数设置为空，这相当于预留一定大小的内存，但是之后才去填充的情况时非常有用。
* glBufferSubData，与前者不同的是它还需要一个偏移量来表示从哪里开始填充数据。

```cpp
glBufferSubData(GL_ARRAY_BUFFER, 24, sizeof(data), &data);
```

* 讲数据导入缓冲的另一种方式是请求缓冲内存的指针。直接将数据复制到缓冲当中。使用glMapBuffer函数。

```cpp
float data[] = {
    0.5f, 1.0f, -0.35f
    ...
};
glBindBuffer(GL_ARRAY_BUFFER, buffer);
// 获取指针
void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

memcpy(ptr, data, sizeof(data));
glUnmapBuffer(GL_ARRAY_BUFFER);
```

#### 分批顶点属性

我们之前的顶点数据分布方式是每个顶点的位置，法向量，纹理坐标等排列在一起，但是有的时候这些数据是一个个大大的区块单独存储的，这是后就体现出glBufferSubData的好处，配合新的glVertexAttribPointer的理解顶点数据的方式就可以处理这种数据排布。

```cpp
float positions[] = { ... };
float normals[] = { ... };
float tex[] = { ... };
// 填充缓冲
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions);
glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(normals), &normals);
glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(normals), sizeof(tex), &tex);

// 新的数据处理方式
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(positions)));  
glVertexAttribPointer(
  2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(positions) + sizeof(normals)));
```

#### 复制缓冲

* 其实就是当我们有一个缓冲之后，想要将当前缓冲的数据分享到另外一个缓冲。

```cpp
void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);
```

但如果我们想读写数据的两个不同缓冲都为顶点数组缓冲该怎么办呢？我们不能同时将两个缓冲绑定到同一个缓冲目标上。正是出于这个原因，OpenGL提供给我们另外两个缓冲目标，叫做GL_COPY_READ_BUFFER和GL_COPY_WRITE_BUFFER。我们接下来就可以将需要的缓冲绑定到这两个缓冲目标上，并将这两个目标作为`readtarget`和`writetarget`参数。

```cpp
float vertexData[] = { ... };
glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));
```

或者是单独使用GL_COPY_WRITE_BUFFER, 源target类型还是GL_ARRAY_BUFFER。

```cpp
float vertexData[] = { ... };
glBindBuffer(GL_ARRAY_BUFFER, vbo1);
glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));
```

