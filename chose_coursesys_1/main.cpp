// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:YIjunchen  LUfengying ChenYuehang    2026-01-17 17:51:42
// Description:
//     a system of select course
//     [v0.1.2]     2026-01-17 17:49:04
//         * 初步实现学生教师和分数
//     [v0.1.2]     2026-01-18 13:39:57
//         *初步实现了教学秘书类和排课功能
// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:YIjunchen  LUfengying ChenYuehang    2026-01-17 17:51:42
// Description:
//     a system of select course
//     [v0.1.2]     2026-01-17 17:49:04
//         * 初步实现学生教师和分数
//     [v0.1.2]     2026-01-18 13:39:57
//         *初步实现了教学秘书类和排课功能（已注释）
//     [v0.1.3]     2026-01-18 21:16:40
//         * 初步实现了ui，但还没有分离模块（已注释）

// 引入C++20标准库模块
import std;
// 引入领域层核心模块（仅保留学生、成绩、教师、注册员、人员基类，注释课程、秘书）
import domain.student;
// import domain.course;  // 注释：课程模块暂时不需要
import domain.score;
import domain.teacher;
import domain.registrar;
// import domain.secretary;  // 注释：秘书/排课模块暂时不需要
import domain.person;  //用于多态实现，统一人员属性接口

// 引入std命名空间中的必要组件（仅保留核心功能所需，注释交互相关）
using std::print;

using std::cerr;

using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;

// 全局注册员实例：统一管理学生、教师核心数据（支撑分数功能基础）
Registrar registrar;

//带Registrar&参数的核心功能测试函数（学生/教师/分数基础测试）
void testTeacherFunctions(Registrar& registrar);

// 核心功能测试函数实现（简化课程相关逻辑，聚焦学生/教师基础信息）
void testTeacherFunctions(Registrar& registrar) {
    print("\n=== 测试学生/教师/分数核心功能 ===\n");
    // 从传入的Registrar实例中查找指定ID的教师（核心数据查询）
    auto it1 = registrar.getAllTeachers().find("T001");
    auto it2 = registrar.getAllTeachers().find("T002");
    if (it1 == registrar.getAllTeachers().end() || it2 == registrar.getAllTeachers().end()) {
        print("错误：教师不存在！\n");
        return;
    }

    // 获取教师对象指针，提取基础属性
    auto* teacher1 = it1->second.get();
    auto* teacher2 = it2->second.get();

    // 1. 打印教师基本信息（核心对象属性展示）
    print("1. 教师基础信息:\n");
    print("   教师1: {} (ID: {}, 部门: {})\n", teacher1->getName(), teacher1->getTeacherId(), teacher1->getDepartment());
    print("   教师2: {} (ID: {}, 部门: {})\n", teacher2->getName(), teacher2->getTeacherId(), teacher2->getDepartment());
}

// 程序入口函数：仅保留核心数据初始化与测试，注释所有UI交互循环
int main() {
    print("课程管理系统 - 核心功能测试（学生/教师/分数）\n");
    print("===========================================\n\n");

    // 初始化核心测试数据：添加学生、教师（支撑后续分数功能扩展）
    try {
        // 添加3名测试学生（学号、姓名、专业、年龄、性别）
        registrar.addStudent(make_unique<Student>("S001", "张三", "计算机科学", 20, "男"));
        registrar.addStudent(make_unique<Student>("S002", "李四", "软件工程", 21, "男"));
        registrar.addStudent(make_unique<Student>("S003", "王五", "网络工程", 19, "女"));

        // 添加2名测试教师（教师ID、姓名、部门、年龄、性别）
        registrar.addTeacher(make_unique<Teacher>("T001", "王教授", "计算机学院", 45, "男"));
        registrar.addTeacher(make_unique<Teacher>("T002", "李教授", "软件学院", 50, "女"));

        // 调用核心功能测试函数，传入全局注册员实例
        testTeacherFunctions(registrar);

    } catch (const std::exception& e) {
        // 捕获初始化异常，避免程序崩溃（核心功能异常处理）
        cerr << "初始化失败: " << e.what() << "\n";
    }



    // 核心功能测试完成提示
    print("\n=== 学生/教师/分数核心功能测试完成 ===");
    return 0;
}


