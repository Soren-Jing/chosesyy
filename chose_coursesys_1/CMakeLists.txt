cmake_minimum_required(VERSION 4.1.1)

#https://github.com/Kitware/CMake/blob/v4.1.1/Help/dev/experimental.rst

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD ON)

project(chose_coursesys_1 LANGUAGES CXX)

# set(MODULE_LIB_DIR1 /opt/utils/modules)  # 模块库的目录的设置
add_executable(chose_coursesys_1
    #math.cpp                              # 模块实现单元
    main.cpp

)

set(MODULE_INTERFACE_FILES
    #${MODULE_LIB_DIR1}/utils.cppm         #添加模块库的模块接口单元
    #math.cppm
    course.cppm
    stydent.cppm
    score.cppm# 模块接口单元
)

source_group("Module Interfaces" FILES ${MODULE_INTERFACE_FILES})
target_sources(chose_coursesys_1  PRIVATE FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS                              # 配置BASE_DIRS，用于组织构建输出目录，以便构建不同目录中(同名的)模块文件.
        #${MODULE_LIB_DIR1}
        ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${MODULE_INTERFACE_FILES}
)
target_compile_features(chose_coursesys_1 PRIVATE cxx_std_23)












































# target_link_directories(chose_coursesys_1 PRIVATE /opt/utils/lib)  # -L /opt/utils/lib 设置链接路径
# target_link_libraries(chose_coursesys_1 PRIVATE utils)             # -lutils           设置链接库名

# set_target_properties(chose_coursesys_1 PROPERTIES
#     INSTALL_RPATH_USE_LINK_PATH  True                           # 使用链接路径作为安装文件的RPath，等价于"-Wl,-rpath=/opt/utils/lib"
# )

#add_library(chose_coursesys_1 SHARED
#  utils.cpp
#)

#set_target_properties( chose_coursesys_1 PROPERTIES
#    PUBLIC_HEADER utils.cppm
#    VERSION   2.5.1
#    SOVERSION 2
#    LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/lib"
#)

#install(TARGETS chose_coursesys_1
#    LIBRARY DESTINATION /opt/utils/lib
#    PUBLIC_HEADER DESTINATION  /opt/utils/modules
#)

#add_custom_command(TARGET chose_coursesys_1
#    POST_BUILD
#    COMMAND cmake --install "${CMAKE_CURRENT_BINARY_DIR}"
#)
