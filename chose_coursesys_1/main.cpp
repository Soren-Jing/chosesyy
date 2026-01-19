// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:YIjunchen  LUfengying ChenYuehang    2026-01-17 17:51:42
// Description:
//     a system of select course
//     [v0.1.2]     2026-01-17 17:49:04
//         * 初步实现学生教师和分数
//     [v0.1.2]     2026-01-18 13:39:57
//         *初步实现了教学秘书类和排课功能
<<<<<<< HEAD
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
=======
import std;
import domain.student;
import domain.course;
import domain.score;
import domain.teacher;
import domain.registrar;
import domain.secretary;
import domain.person;
using std::print;
using std::make_unique;
using std::vector;
using std::tuple;
using std::string;
using std::unique_ptr;
using std::make_unique;

void testTeacherFunctions() {
    print("\n=== 测试教师功能 ===\n");

    auto teacher1 = make_unique<Teacher>("T001", "王教授", "计算机学院", 45, "男");
    auto teacher2 = make_unique<Teacher>("T002", "李教授", "软件学院", 50, "女");

    print("1. 创建教师:\n");
    print("   教师1: {} ({}, 部门: {})\n",
          teacher1->getName(), teacher1->getTeacherId(), teacher1->getDepartment());
    print("   教师2: {} ({}, 部门: {})\n",
          teacher2->getName(), teacher2->getTeacherId(), teacher2->getDepartment());

    print("\n2. 设置教授课程:\n");
    try {
        teacher1->addTeachingCourse("C001");
        teacher1->addTeachingCourse("C002");
        teacher2->addTeachingCourse("C003");
        // 测试空课程ID，触发异常
        teacher1->addTeachingCourse("");
    } catch (const std::invalid_argument& e) {
                print("   异常提示: {}\n", e.what());
    }

    print("   王教授教授课程: ");
    for (const auto& courseId : teacher1->getTeachingCourses()) {
        print("{} ", courseId);
    }
    print("\n");

    print("\n3. 测试成绩录入:\n");
    auto score = teacher1->inputScore("S001", "C001", 88.5f);
    if (score) {
        print("   ✓ 成功录入成绩: {}, 等级: {}\n", score->getScore(), score->getGrade());
    }

    score = teacher1->inputScore("S001", "C003", 92.0f);
    if (!score) {
        print("   ✓ 正确拒绝无权限课程成绩录入\n");
    }

    print("\n4. 测试批量成绩录入:\n");
    vector<tuple<string, string, float>> batchData = {
        {"S001", "C001", 85.0f},
        {"S002", "C001", 92.0f},
        {"S003", "C002", 78.5f}
    };

    auto scores = teacher1->batchInputScores(batchData);
    print("   成功录入 {} 条成绩记录\n", scores.size());

    print("\n5. 测试课程统计:\n");
    try {
        auto stats = teacher1->getCourseStatistics("C001");
        print("   课程 {} 统计:\n", stats.courseId);
        print("     学生人数: {}\n", stats.studentCount);
        print("     平均分: {}\n", stats.averageScore);
        print("     最高分: {}\n", stats.maxScore);
        print("     最低分: {}\n", stats.minScore);
    } catch (const std::exception& e) {
        print("   错误: {}\n", e.what());
    }
}

int main() {
    print("课程管理系统 - 代管者模式测试\n");
    print("==============================\n\n");

    try {
        Registrar registrar;

        // 1. 添加学生
        print("1. 添加学生:\n");
>>>>>>> temp_dev1
        registrar.addStudent(make_unique<Student>("S001", "张三", "计算机科学", 20, "男"));
        registrar.addStudent(make_unique<Student>("S002", "李四", "软件工程", 21, "男"));
        registrar.addStudent(make_unique<Student>("S003", "王五", "网络工程", 19, "女"));

<<<<<<< HEAD
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


=======
        // 2. 添加教师
        registrar.addTeacher(make_unique<Teacher>("T001", "王教授", "计算机学院", 45, "男"));
        registrar.addTeacher(make_unique<Teacher>("T002", "李教授", "软件学院", 50, "女"));

        // 3. 添加课程
        print("2. 添加课程:\n");
        registrar.addCourse(make_unique<Course>("C001", "C++程序设计", 2));
        registrar.addCourse(make_unique<Course>("C002", "数据结构", 3));
        registrar.addCourse(make_unique<Course>("C003", "数据库原理", 2));

        // 初始状态
        registrar.displayAllRegistrations();

        // 4. 测试选课
        print("\n3. 测试选课:\n");
        if (registrar.registerCourse("S001", "C001")) {
            print("  ✓ 张三成功选修 C++程序设计\n");
        }
        if (registrar.registerCourse("S002", "C001")) {
            print("  ✓ 李四成功选修 C++程序设计\n");
        }
        if (!registrar.registerCourse("S003", "C001")) {
            print("  ✓ 王五选课失败（C++程序设计已满）\n");
        }
        if (registrar.registerCourse("S003", "C002")) {
            print("  ✓ 王五成功选修 数据结构\n");
        }
        if (!registrar.registerCourse("S001", "C001")) {
            print("  ✓ 张三重复选课失败（已选过C001）\n");
        }

        // 5. 选课结果
        print("\n4. 选课结果:\n");
        registrar.displayAllRegistrations();
        if (!registrar.printStudentRecord("S001")) {
            std::cerr << "未找到该学生！\n";
        }

        // 6. 测试退课
        print("\n5. 测试退课:\n");
        if (registrar.dropCourse("S001", "C001")) {
            print("  ✓ 张三成功退选 C++程序设计\n");
        }

        // 7. 退课后状态
        print("\n6. 退课后的状态:\n");
        registrar.displayAllRegistrations();

        // 8. 测试查询
        print("\n7. 测试查询功能:\n");
        auto s1Courses = registrar.getStudentCourses("S001");
        print("  张三的选课: ");
        for (const auto* course : s1Courses) {
            print("{} ", course->getCourseName());
        }
        print("\n");

        auto c1Students = registrar.getCourseStudents("C001");
        print("  C++程序设计的学生: ");
        for (const auto* student : c1Students) {
            print("{} ", student->getName());
        }
        print("\n");

        // 9. 教师功能测试
        testTeacherFunctions();

        // 10. 教学秘书排课测试
        print("\n9. 测试教学秘书排课功能:\n");
        Secretary secretary("SEC001", "赵秘书", 30, "女");
        secretary.scheduleCourse("T001", "C001", "Mon08:00-09:40", "A101");
        secretary.scheduleCourse("T001", "C002", "Tue10:00-11:40", "B203");
        bool conflict = secretary.scheduleCourse("T001", "C003", "Mon08:00-09:40", "A102");
        print("  冲突排课结果: {}\n", conflict ? "成功" : "失败（时间冲突）");
        secretary.printSchedule();
        secretary.cancelSchedule("T001", "C002", "Tue10:00-11:40");
        secretary.printSchedule();

        // 11. 多态测试
        print("\n10. 测试Person基类多态:\n");
        vector<unique_ptr<Person>> people;
        people.push_back(make_unique<Student>("S004", "钱七", "网络安全", 20, "女"));
        people.push_back(make_unique<Teacher>("T003", "周教授", "物理学院", 35, "男"));
        people.push_back(make_unique<Secretary>("SEC002", "孙秘书", 28, "女"));
        for (const auto& p : people) {
            print("  - 姓名: {}  ID: {}  年龄: {}  性别: {}\n",
                  p->getName(), p->getId(), p->getAge(), p->getGender());
        }

        print("\n==============================\n");
        print("代管者模式测试完成!\n");

    } catch (const std::exception& e) {
        std::cerr << "程序错误: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
>>>>>>> temp_dev1
