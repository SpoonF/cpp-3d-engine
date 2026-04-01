cmake_minimum_required(VERSION 3.10)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем include директории
include_directories(${CMAKE_SOURCE_DIR}/include)

# Находим все .cpp файлы
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Для отладки
set(CMAKE_BUILD_TYPE Debug)