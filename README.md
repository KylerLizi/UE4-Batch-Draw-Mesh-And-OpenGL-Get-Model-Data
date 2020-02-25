# UE4-Batch-Draw-Mesh-And-OpenGL-Get-Model-Data
## 2019.02.01-2019.06.01(毕设开发周期，期间还在忙国庆仿真)
![Draw9300Meshes](https://github.com/ColorGalaxy/UE4-Batch-Draw-Mesh-And-OpenGL-Get-Model-Data/raw/master/Screenshot/Draw9300Meshes.png)<br>
　本人毕设题目是`基于UE4的矿山爆破仿真系统`。导师会提供给我矿山模型的`13万条顶点坐标与三角面索引数据集`，我使用该Demo在UE4中绘制了这9300个碎石组成了矿山模型。（画面简陋到现在的我无法直视，现在想想还是`Houdini`做比较适合）<br>
# UE4部分使用说明
详细的技术细节我总结在了CSDN博客，这里只做简单的说明与展示：<br>
* [【UE4 C++】由点面数据，批量绘制ProceduralMesh并转化为StaticMesh资产](https://blog.csdn.net/qq_31788759/article/details/104092692)<br>
* [【UE4 Blueprint】ProceduralMeshComponent绘制自定义模型并转为StaticMesh](https://blog.csdn.net/qq_31788759/article/details/104088427)<br>
* [【UE4】如何使用Edit Scripting Utilities在蓝图中为大量静态网格体设置自动凸包碰撞](https://blog.csdn.net/qq_31788759/article/details/88824558)<br>
* [【UE4 C++】无法打开源文件"ProceduralMeshComponent.h"的解决办法](https://blog.csdn.net/qq_31788759/article/details/104088427)<br>
* [【UE4 C++】RawMesh.h引用报错](https://blog.csdn.net/qq_31788759/article/details/104092656)<br>
![Instructions](https://github.com/ColorGalaxy/UE4-Batch-Draw-Mesh-And-OpenGL-Get-Model-Data/raw/master/Screenshot/Instructions.png)<br>
在关卡蓝图中配置好`数据表格`与输出的静态网格体`名称`，运行按下`1或2键`对应生成一或多个网格体资源。<br>
　注：运行绘制结束后记得在内容浏览器中对创建的模型`重置化本地ID`，因为它们暂时还是临时资源。<br>
![UEDrawMeshes](https://github.com/ColorGalaxy/UE4-Batch-Draw-Mesh-And-OpenGL-Get-Model-Data/raw/master/Screenshot/UEDrawMeshes.png)<br>
### 文件/文件夹说明
* Content
  * Blueprint　　　　绘制单个/多个Mesh、批量添加凸包碰撞的蓝图<br>
  * Data　　　　　　存放单个/多个Mesh的顶点与面索引的数据表格、结构体<br>
  * Mesh　　　　　　绘制模型资源保存路径，可在PMConvertSM.cpp中配置<br>
* Source　　　　　　　UE4 C++ ProceduralMesh转换为StaticMesh代码<br>
# VS项目部分使用说明
　详细说明：[【OpenGL C++ UE4】获取模型顶点及面索引数据，并优化存储结构供UE4绘制](https://blog.csdn.net/qq_31788759/article/details/104476953)<br>
　在`ModelLoad-UseAssimp.cpp`中`"Model/Teapot.obj"`配置读取模型的路径，`#define SHOW_WIREFRAME`配置是否开启模型线框显示。<br>
　在`Mesh.h`中配置是否开启Mesh数据导出，若声明`#define EXPORT_MODEL_DATA`则导出，注释仅读取并在窗口中绘制模型。<br>
### 文件/文件夹说明<br>
* VSProject_GetMeshInfoCSV<br>
  * GetMeshInfo<br>
    * Config　　　　　　需要的OpenGL第三方库环境，GLFW、GLAD、ASSIMP、GLM、stb_image<br>
    * MeshData　　　　　输出的顶点与面索引CSV表格，用于导入UE4<br>
    * Model　　　　　　　存放待读取obj模型，这里有一个三棱锥和茶壶作示例<br>
    * ShaderSource　　　OpenGL渲染模型的顶点与片段着色器<br>
    * Camera.h　　　　　窗口摄像机控制逻辑<br>
    * Mesh.h　　　　　　处理网格信息，网格代表的是单个的可绘制实体<br>
    * Model.h　　　　　　处理模型信息，模型包含多个网格，甚至是多个物体<br>
    * Shader.h　　　　　编译着色器项目<br>
    * MeshInfoCSV.h　　重复顶点数据去除、索引更新、打印适用于导入UE4的CSV表格<br>
    * ModelLoad-UseAssimp.cpp　　　　　main函数所在，管理整个项目各个模块<br>
    
