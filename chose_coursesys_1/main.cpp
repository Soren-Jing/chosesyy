// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:YIjunchen  LUfengying Chen    2026-01-17 17:51:42
// Description:
//     a system of select course
//     [v0.1.2]     2026-01-17 17:49:04
//         * 初步实现学生教师和分数
//     [v0.1.2]     2026-01-18 13:39:57
//         *初步实现了教学秘书类和排课功能
import std;
import domain.student;
import domain.course;
import domain.score;
import domain.teacher;
import domain.database;
import domain.registrar;
import domain.secretary;
using std::print;

void testTeacherFunctions() {
    print("\n=== 测试教师功能 ===\n");

    // 创建教师,原来domain空间下面的Teacher
    auto teacher1 = std::make_unique<Teacher>("T001", "王教授", "计算机学院");
    auto teacher2 = std::make_unique<Teacher>("T002", "李教授", "软件学院");

    print("1. 创建教师:\n");
    print("   教师1: {} ({}, 部门: {})\n",
               teacher1->getName(), teacher1->getTeacherId(), teacher1->getDepartment());
    print("   教师2: {} ({}, 部门: {})\n",
               teacher2->getName(), teacher2->getTeacherId(), teacher2->getDepartment());

    // 添加教授课程
    print("\n2. 设置教授课程:\n");
    teacher1->addTeachingCourse("C001");
    teacher1->addTeachingCourse("C002");
    teacher2->addTeachingCourse("C003");

    print("   王教授教授课程: ");
    for (const auto& courseId : teacher1->getTeachingCourses()) {
        print("{} ", courseId);
    }
    print("\n");

    // 测试成绩录入
    print("\n3. 测试成绩录入:\n");
    auto score = teacher1->inputScore("S001", "C001", 88.5f);
    if (score) {
        print("   ✓ 成功录入成绩: {}, 等级: {}\n", score->getScore(), score->getGrade());
    }

    // 测试无权限录入成绩
    score = teacher1->inputScore("S001", "C003", 92.0f);
    if (!score) {
        print("   ✓ 正确拒绝无权限课程成绩录入\n");
    }

    // 测试批量录入
    print("\n4. 测试批量成绩录入:\n");
    std::vector<std::tuple<std::string, std::string, float>> batchData = {
        {"S001", "C001", 85.0f},
        {"S002", "C001", 92.0f},
        {"S003", "C002", 78.5f}
    };

    auto scores = teacher1->batchInputScores(batchData);
    print("   成功录入 {} 条成绩记录\n", scores.size());

    // 测试课程统计
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
/*
void testDatabaseFunctions() {
    print("\n=== 测试数据库功能 ===\n");

    // 创建数据库连接配置
    database::ConnectionConfig config(
        "localhost",    // host
        3306,           // port
        "course_admin", // username
        "password123",  // password
        "course_db"     // database
    );

    print("1. 数据库配置:\n");
    print("   连接字符串: {}\n", config.toConnectionString());
    print("   用户名: {}\n", config.username);

    // 测试数据库连接
    print("\n2. 测试数据库连接:\n");
    auto connection = std::make_unique<database::Connection>(config);

    if (connection->connect()) {
        print("   ✓ 数据库连接成功\n");
        print("   连接状态: 已连接\n");

        // 测试SQL执行
        print("\n3. 测试SQL执行:\n");
        if (connection->execute("SELECT * FROM students")) {
            print("   ✓ SELECT语句执行成功\n");
        }

        if (connection->execute("INSERT INTO students VALUES ('S003', '王五', '网络工程')")) {
            print("   ✓ INSERT语句执行成功\n");
        }

        // 测试查询
        print("\n4. 测试查询功能:\n");
        auto result = connection->query("SELECT * FROM students");
        if (result && !result->isEmpty()) {
            print("   ✓ 查询成功，返回 {} 条记录\n", result->rowCount());

            // 显示查询结果
            while (result->next()) {
                print("     学生ID: {}, 姓名: {}, 专业: {}\n",
                      result->getString("student_id"),
                      result->getString("name"),
                      result->getString("major"));
            }
        }

        // 测试事务
        print("\n5. 测试事务功能:\n");
        connection->beginTransaction();
        print("   事务开始\n");

        connection->execute("UPDATE courses SET current_capacity = 26 WHERE course_id = 'C001'");
        print("   执行更新操作\n");

        // 这里可以选择提交或回滚
        connection->commit();
        print("   事务提交\n");

        // 关闭连接
        connection->close();
        print("\n   数据库连接已关闭\n");

    } else {
        print("   ✗ 数据库连接失败: {}\n", connection->getLastError());
    }

    // 测试连接池
    print("\n6. 测试数据库连接池:\n");
    database::ConnectionPool pool(config, 3);

    auto conn1 = pool.getConnection();
    auto conn2 = pool.getConnection();

    print("   获取2个连接，连接池大小: {}\n", pool.getConnectionCount());

    if (conn1 && conn2) {
        print("   ✓ 成功从连接池获取连接\n");

        pool.returnConnection(std::move(conn1));
        pool.returnConnection(std::move(conn2));

        print("   归还连接后，连接池大小: {}\n", pool.getConnectionCount());

        pool.closeAll();
        print("   关闭所有连接\n");
    }
}
*/

int main() {
    print("课程管理系统 - 代管者模式测试\n");
        print("==============================\n\n");

        try {
            // 创建代管者实例（Facade）
            Registrar registrar;

            // 1. 添加学生
            print("1. 添加学生:\n");
            registrar.addStudent(std::make_unique<Student>("S001", "张三", "计算机科学"));
            registrar.addStudent(std::make_unique<Student>("S002", "李四", "软件工程"));
            registrar.addStudent(std::make_unique<Student>("S003", "王五", "网络工程"));

            // 2. 添加课程（容量设为2便于测试满员情况）
            print("\n2. 添加课程:\n");
            registrar.addCourse(std::make_unique<Course>("C001", "C++程序设计", 2));
            registrar.addCourse(std::make_unique<Course>("C002", "数据结构", 3));
            registrar.addCourse(std::make_unique<Course>("C003", "数据库原理", 2));

            // 显示初始状态
            registrar.displayAllRegistrations();

            // 3. 测试选课功能
            print("\n3. 测试选课:\n");

            // 学生1选课成功
            if (registrar.registerCourse("S001", "C001")) {
                print("  ✓ 张三成功选修 C++程序设计\n");
            }

            // 学生2选同一门课成功
            if (registrar.registerCourse("S002", "C001")) {
                print("  ✓ 李四成功选修 C++程序设计\n");
            }

            // 学生3选已满课程失败
            if (!registrar.registerCourse("S003", "C001")) {
                print("  ✓ 王五选课失败（C++程序设计已满）\n");
            }

            // 学生3选其他课程成功
            if (registrar.registerCourse("S003", "C002")) {
                print("  ✓ 王五成功选修 数据结构\n");
            }

            // 重复选课失败
            if (!registrar.registerCourse("S001", "C001")) {
                print("  ✓ 张三重复选课失败（已选过C001）\n");
            }

            // 4. 展示选课结果
            print("\n4. 选课结果:\n");
            registrar.displayAllRegistrations();
            // 查看学号 S001 的选课情况
            if (!registrar.printStudentRecord("S001"))
                std::cerr << "未找到该学生！\n";
            // 5. 测试退课
            print("\n5. 测试退课:\n");
            if (registrar.dropCourse("S001", "C001")) {
                print("  ✓ 张三成功退选 C++程序设计\n");
            }

            // 再次展示
            print("\n6. 退课后的状态:\n");
            registrar.displayAllRegistrations();

            // 7. 测试查询功能
            print("\n7. 测试查询功能:\n");

            // 查询学生选课
            auto s1Courses = registrar.getStudentCourses("S001");
            print("  张三的选课: ");
            for (const auto* course : s1Courses) {
                print("{} ", course->getCourseName());
            }
            print("\n");

            // 查询课程学生
            auto c1Students = registrar.getCourseStudents("C001");
            print("  C++程序设计的学生: ");
            for (const auto* student : c1Students) {
                print("{} ", student->getName());
            }
            print("\n");

            // 8. 原有测试功能
             testTeacherFunctions();
            //9. 测试教学秘书排课
            print("\n8. 测试教学秘书排课功能:\n");
            Secretary secretary("SEC001", "赵秘书");

            // 先给教师排两门课
            secretary.scheduleCourse("T001", "C001", "Mon08:00-09:40", "A101");
            secretary.scheduleCourse("T001", "C002", "Tue10:00-11:40", "B203");
            // 冲突排课（同一教师同一时间）
            bool conflict = secretary.scheduleCourse("T001", "C003", "Mon08:00-09:40", "A102");
            print("  冲突排课结果: {}\n", conflict ? "成功" : "失败（时间冲突）");

            secretary.printSchedule();

            // 撤排一门
            secretary.cancelSchedule("T001", "C002", "Tue10:00-11:40");
            print("\n  撤排后:\n");
            secretary.printSchedule();

            print("\n==============================\n");
            print("代管者模式测试完成!\n");

        } catch (const std::exception& e) {
            std::cerr << "程序错误: " << e.what() << "\n";
            return 1;
        }


    return 0;
}
