cmake_minimum_required(VERSION 2.8)	

#Имя проекта
project(imEngine)

#Библиотеки
#find_package()

#Директория где искать *.h
include_directories(imCore/)

#Флаги
add_definitions(-std=c++11)		#C++11
add_definitions(-Wall)			#Все предупреджения

#Список исходников
set(SRC_LIST main.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})

