# 引入包含函数定义的文件
include(Function.cmake)
cmake_minimum_required(VERSION 3.9)
project(AutoGenerateUIStyle)
# 目标名 
set(TARGET_NAME AutoGenerateUIStyle)
# 设置为启动项目
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})

# 是否x86
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(PLATFORM_CPU "x64")
else()
    SET(PLATFORM_CPU "x86")
endif()
MESSAGE(STATUS "PLATFORM_CPU: ${PLATFORM_CPU}")

# Qt，开启Qt代码自动生成
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# 设置QTDIR
if(NOT DEFINED ENV{QT_DIR})
    if(${PLATFORM_CPU} STREQUAL "x86")
        SET(QTDIR "C:/Qt/Qt5.9/5.9.4/msvc2015")
    else()
        SET(QTDIR "C:/Qt/Qt5.9/5.9.4/msvc2015_64")
    endif()
    MESSAGE(STATUS "cus_QTDIR: ${QTDIR}")
else()
    SET(QTDIR $ENV{QT_DIR})
    MESSAGE(STATUS "env_QTDIR: ${QTDIR}")
endif()

# 设置CMAKE_PREFIX_PATH，供find_package()使用
set(CMAKE_PREFIX_PATH ${QTDIR})
message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
# 解决节数超过对象文件格式限制
add_compile_options(-bigobj)
# 常规-多处理器编译 【重点】
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") 
# 默认开启从父级或项目默认设置继承,否则props无法引用lib【重点】
set(CMAKE_CXX_STANDARD_LIBRARIES "%(AdditionalDependencies)")
# 重定向输出目录
set(OUTPUT_MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_CPU})
set(OUTPUT_EXE_DIR ${OUTPUT_MAIN_DIR}/${CMAKE_CONFIGURATION_TYPES})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_MAIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_MAIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_MAIN_DIR})
# 项目编译设置
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
# 如果有自定义环境变量(),则加入Od使Release下生成PDB
if(DEFINED ENV{MOZI_BUILD_INOD})
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Od")
MESSAGE(STATUS "Build in mode /Od")
endif()

# 添加include path
include_directories(Base)
include_directories(UIModule)

# 所有源文件
file(GLOB_RECURSE SRC_FILE "*.h*" "*.c*" "*.ui" "*.qrc" "*.ts")
# 定义要排除的目录名，防止build里面moc预处理完的cpp再次加入编译
set(EXCLUDE_DIRS "build")
list(FILTER SRC_FILE EXCLUDE REGEX "/(${EXCLUDE_DIRS})/.*")

## 排除生成的moc、qrc和ui文件
foreach(file ${SRC_FILE})
    list(APPEND TARGET_SRC ${file})
endforeach(file ${SRC_FILE})
list(FILTER TARGET_SRC EXCLUDE REGEX "moc_|qrc_|ui_")

# 添加VS过滤器
source_group_by_dir(TARGET_SRC)

# 设置使用到的Qt模块 
set(QT_MODULES Core Gui Widgets)
find_package(Qt5 COMPONENTS ${QT_MODULES} REQUIRED)

add_executable(${TARGET_NAME} WIN32 ${TARGET_SRC})

# 链接使用到的Qt模块
foreach(MODULE ${QT_MODULES})
    target_link_libraries(${TARGET_NAME} Qt5::${MODULE})
endforeach(MODULE ${QT_MODULES})

# 自动解析props,把所有存在的dll拷贝过来
foreach(file ${VS_PROPS})
    list(APPEND props_list ${file})
endforeach(file ${VS_PROPS})
