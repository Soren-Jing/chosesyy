import std;
import domain.student;
import domain.course;
import domain.score;
import domain.teacher;
import domain.database;

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
    print("课程管理系统 - 基础测试\n");
    print("=====================\n\n");

    try {
        // 1. 创建学生
        print("1. 创建学生:\n");
        auto student1 = std::make_unique<Student>("S001", "张三", "计算机科学");
        auto student2 = std::make_unique<Student>("S002", "李四", "软件工程");

        print("  学生1: {} ({})\n", student1->getName(), student1->getStudentId());
        print("  学生2: {} ({})\n", student2->getName(), student2->getStudentId());

        // 2. 创建课程
        print("\n2. 创建课程:\n");
        auto course1 = std::make_unique<Course>("C001", "C++程序设计", 2); // 容量设为2便于测试
        auto course2 = std::make_unique<Course>("C002", "数据结构", 3);

        print("  课程1: {} (容量: {}, 剩余: {})\n",
              course1->getCourseName(), course1->getMaxCapacity(), course1->getRemainingCapacity());
        print("  课程2: {} (容量: {}, 剩余: {})\n",
              course2->getCourseName(), course2->getMaxCapacity(), course2->getRemainingCapacity());

        // 3. 测试选课功能
        print("\n3. 测试选课:\n");

        // 学生1选课
        bool success = student1->selectCourse("C001");
        if (success && course1->enrollStudent()) {
            print("  ✓ 学生 {} 成功选修 {}\n", student1->getName(), course1->getCourseName());
        } else {
            print("  ✗ 选课失败\n");
        }

        // 学生2选同一门课
        success = student2->selectCourse("C001");
        if (success && course1->enrollStudent()) {
            print("  ✓ 学生 {} 成功选修 {}\n", student2->getName(), course1->getCourseName());
        }

        // 检查课程是否已满
        print("  课程 {} 状态: {} (当前人数: {})\n",
              course1->getCourseName(),
              (course1->isFull() ? "已满" : "未满"),
              course1->getCurrentCapacity());

        // 4. 测试成绩功能
        print("\n4. 测试成绩功能:\n");

        // 创建成绩
        Score score1("S001", "C001", 92.5f);
        print("  成绩创建: 学生 {} 课程 {} 成绩: {} 等级: {}\n",
              score1.getStudentId(), score1.getCourseId(), score1.getScore(), score1.getGrade());

        // 测试成绩有效性检查
        try {
            Score invalidScore("S001", "C001", 150.0f);
            print("  ✗ 应该抛出异常但没有\n");
        } catch (const std::invalid_argument& e) {
            print("  ✓ 成功捕获无效成绩异常: {}\n", e.what());
        }

        // 5. 测试查询学生成绩（模拟）
        print("\n5. 测试查询成绩:\n");
        auto scores = student1->queryScore();
        print("  学生{} 的成绩:\n",student1->getName());
        for (const auto& score : scores) {
            print("    - 课程: {}, 成绩: {}, 等级: {}\n",
                       score.getCourseId(), score.getScore(), score.getGrade());
        }

        // 6. 测试退课
        print("\n6. 测试退课:\n");
        if (student1->dropCourse("C001") && course1->dropStudent()) {
            print("  ✓ 学生 {} 成功退选 {}\n", student1->getName(), course1->getCourseName());
            print("  课程当前人数: {}\n", course1->getCurrentCapacity());
        }
        // 测试新增的教师功能
        testTeacherFunctions();

        // 测试新增的数据库功能
        //testDatabaseFunctions();
        print("\n===============================\n");
        print("基础测试完成!\n");

    } catch (const std::exception& e) {
        std::cerr << "程序错误: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
