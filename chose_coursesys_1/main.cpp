import std;
import domain.student;
import domain.course;
import domain.score;

int main() {
    std::cout << "课程管理系统 - 基础测试\n";
    std::cout << "=====================\n\n";

    try {
        // 1. 创建学生
        std::cout << "1. 创建学生:\n";
        auto student1 = std::make_unique<Student>("S001", "张三", "计算机科学");
        auto student2 = std::make_unique<Student>("S002", "李四", "软件工程");

        std::cout << "  学生1: " << student1->getName() << " (" << student1->getStudentId() << ")\n";
        std::cout << "  学生2: " << student2->getName() << " (" << student2->getStudentId() << ")\n";

        // 2. 创建课程
        std::cout << "\n2. 创建课程:\n";
        auto course1 = std::make_unique<Course>("C001", "C++程序设计", 2); // 容量设为2便于测试
        auto course2 = std::make_unique<Course>("C002", "数据结构", 3);

        std::cout << "  课程1: " << course1->getCourseName()
                  << " (容量: " << course1->getMaxCapacity()
                  << ", 剩余: " << course1->getRemainingCapacity() << ")\n";
        std::cout << "  课程2: " << course2->getCourseName()
                  << " (容量: " << course2->getMaxCapacity()
                  << ", 剩余: " << course2->getRemainingCapacity() << ")\n";

        // 3. 测试选课功能
        std::cout << "\n3. 测试选课:\n";

        // 学生1选课
        bool success = student1->selectCourse("C001");
        if (success && course1->enrollStudent()) {
            std::cout << "  ✓ 学生 " << student1->getName() << " 成功选修 " << course1->getCourseName() << "\n";
        } else {
            std::cout << "  ✗ 选课失败\n";
        }

        // 学生2选同一门课
        success = student2->selectCourse("C001");
        if (success && course1->enrollStudent()) {
            std::cout << "  ✓ 学生 " << student2->getName() << " 成功选修 " << course1->getCourseName() << "\n";
        }

        // 检查课程是否已满
        std::cout << "  课程 " << course1->getCourseName() << " 状态: "
                  << (course1->isFull() ? "已满" : "未满")
                  << " (当前人数: " << course1->getCurrentCapacity() << ")\n";

        // 4. 测试成绩功能
        std::cout << "\n4. 测试成绩功能:\n";

        // 创建成绩
        Score score1("S001", "C001", 92.5f);
        std::cout << "  成绩创建: 学生 " << score1.getStudentId()
                  << " 课程 " << score1.getCourseId()
                  << " 成绩: " << score1.getScore()
                  << " 等级: " << score1.getGrade() << "\n";

        // 测试成绩有效性检查
        try {
            Score invalidScore("S001", "C001", 150.0f);
            std::cout << "  ✗ 应该抛出异常但没有\n";
        } catch (const std::invalid_argument& e) {
            std::cout << "  ✓ 成功捕获无效成绩异常: " << e.what() << "\n";
        }

        // 5. 测试查询学生成绩（模拟）
        std::cout << "\n5. 测试查询成绩:\n";
        auto scores = student1->queryScore();
        std::cout << "  学生 " << student1->getName() << " 的成绩:\n";
        for (const auto& score : scores) {
            std::cout << "    - 课程: " << score.getCourseId()
                     << ", 成绩: " << score.getScore()
                     << ", 等级: " << score.getGrade() << "\n";
        }

        // 6. 测试退课
        std::cout << "\n6. 测试退课:\n";
        if (student1->dropCourse("C001") && course1->dropStudent()) {
            std::cout << "  ✓ 学生 " << student1->getName() << " 成功退选 " << course1->getCourseName() << "\n";
            std::cout << "  课程当前人数: " << course1->getCurrentCapacity() << "\n";
        }

        std::cout << "\n===============================\n";
        std::cout << "基础测试完成!\n";

    } catch (const std::exception& e) {
        std::cerr << "程序错误: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
