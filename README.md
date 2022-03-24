# Notebook

-------

本项目为浙江大学2022下学期OOP课程作业，请勿star/watch/pull request。

## 1. 编译依赖库

本项目默认使用vcpkg包管理工具，详见https://github.com/microsoft/vcpkg

① 参考文档初始化vcpkg

② 选择合适的triplet，Windows(msvc)推荐x64-windows-static-md，Windows(MinGW)推荐x64-mingw，linux推荐x64-linux。

③ ./vcpkg install sqlite3 gtest --triplet=[triplet]

也可以使用其他工具。

## 2. 生成CMake Cache

如使用vcpkg，请指定vcpkg根目录增加以下开关：

```shell
-DVCPKG_DIR=[vcpkg dir] -DVCPKG_TARGET_TRIPLET=[triplet]
```

如使用其他工具，需添加如下开关：

```shell
-DCMAKE_TOOLCHAIN_FILE=[toolchain]
```

以下为示例：

#### 1. Visual Studio 2019

```shell
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_CONFIGURATION_TYPES:STRING="Debug" -S . -B build
```

#### 2. mingw-gcc

```shell
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -S . -B build
```

#### 3. gnu-cc/clang

```shell
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -S . -B build
```

#### 4. Ninja

```shell
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -S . -B build
```

Tip: Other build types: RelWithDebInfo, Release, MinSizeRel

## 3. 编译

Build：

```shell
cmake --build ./build --target notebook
```

clean: 

```shell
cmake --build ./build --target clean
```

## 其他信息：
如果对编译过程有疑问，请参考github ci文件。
