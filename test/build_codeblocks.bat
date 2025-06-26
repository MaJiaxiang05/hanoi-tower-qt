@echo off
echo ========================================
echo     汉诺塔游戏 CodeBlocks 构建脚本
echo ========================================

REM 设置Qt环境变量（根据你的Qt安装路径调整）
set QT_DIR=C:\Qt\6.5.0\mingw_64
set QT_TOOLS=C:\Qt\Tools\mingw1120_64
set PATH=%QT_DIR%\bin;%QT_TOOLS%\bin;%PATH%

echo 设置Qt环境路径: %QT_DIR%

REM 检查Qt是否正确安装
echo 检查Qt安装...
moc -v
if %errorlevel% neq 0 (
    echo 错误: 未找到moc工具，请检查Qt安装路径
    pause
    exit /b 1
)

REM 创建构建目录
if not exist "build" mkdir build
if not exist "moc" mkdir moc

echo.
echo 开始处理MOC文件...

REM 处理包含Q_OBJECT的头文件
echo 处理 mainwindow.h...
moc src\mainwindow.h -o moc\moc_mainwindow.cpp
if %errorlevel% neq 0 (
    echo 错误: 处理mainwindow.h失败
    pause
    exit /b 1
)

echo 处理 hanoitower.h...
moc src\hanoitower.h -o moc\moc_hanoitower.cpp
if %errorlevel% neq 0 (
    echo 错误: 处理hanoitower.h失败
    pause
    exit /b 1
)

echo 处理 gamecontroller.h...
moc src\gamecontroller.h -o moc\moc_gamecontroller.cpp
if %errorlevel% neq 0 (
    echo 错误: 处理gamecontroller.h失败
    pause
    exit /b 1
)

echo.
echo ✅ MOC文件生成成功！
echo.
echo 生成的文件:
echo   - moc\moc_mainwindow.cpp
echo   - moc\moc_hanoitower.cpp  
echo   - moc\moc_gamecontroller.cpp
echo.
echo 📝 下一步操作:
echo   1. 打开CodeBlocks
echo   2. 将moc目录下的.cpp文件添加到项目中
echo   3. 编译运行项目
echo.
echo 💡 提示: 如果修改了包含Q_OBJECT的头文件，需要重新运行此脚本
echo.
pause 