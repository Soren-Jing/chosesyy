
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:Lufengying 2024051604030 1276809756@qq.com       2026-01-17 18:04:15
// Description:
import registrar.student;
import registrar.course;
import registrar.score;
import std;

int main() {
    bool success = false;
    std::print("===== 选课系统=====\n");

    try {
        // 1. 创建课程和学生对象
        Course course1("CS101", "C++编程", 50);
        Course course2("CS201", "数据结构", 40);
        Student student1("S001", "张三", "计算机科学与技术");
        Student student2("S002", "李四", "软件工程");

        // 2. 测试选课功能
        std::print("\n【选课测试】\n");
        success = student1.selectCourse(course1);
        std::print("{}，当前选课人数：{}\n",
                  (success ? " 张三选C++编程成功" : " 张三选C++编程失败"),
                  course1.getCurrentCapacity());

        success = student1.selectCourse(course1);
        std::print("{}\n",
                  (success ? " 张三选C++编程成功" : " 张三选C++编程失败（重复选课）"));

        success = student2.selectCourse(course1);
        std::print("{}，当前选课人数：{}\n",
                  (success ? " 李四选C++编程成功" : " 李四选C++编程失败"),
                  course1.getCurrentCapacity());

        // 3. 测试成绩录入
        std::print("\n【成绩录入测试】\n");
        // 合法成绩
        try {
            Score score1("S001", "CS101", 95.5f);
            success = student1.addScore(score1);
            std::print("{}：{}分（{}）\n",
                      (success ? "张三C++成绩录入成功" : "张三C++成绩录入失败"),
                      score1.getScore(), score1.getGrade());
        } catch (const std::invalid_argument& e) {
            std::print("张三成绩录入异常：{}\n", e.what());
        }

        // 未选课程成绩
        try {
            Score score2("S001", "CS201", 88.0f);
            success = student1.addScore(score2);
            std::print("{}\n",
                      (success ? "张三数据结构成绩录入成功" : "张三数据结构成绩录入失败（未选课）"));
        } catch (const std::invalid_argument& e) {
            std::print("张三成绩录入异常：{}\n", e.what());
        }

        // 非法成绩
        try {
            Score score3("S002", "CS101", 105.0f);
            student2.addScore(score3);
        } catch (const std::invalid_argument& e) {
            std::print("李四成绩录入异常：{}\n", e.what());
        }

        // 4. 测试退课功能
        std::print("\n【退课测试】\n");
        success = student1.dropCourse(course1);
        std::print("{}，当前选课人数：{}\n",
                  (success ? "张三退C++编程成功" : "张三退C++编程失败"),
                  course1.getCurrentCapacity());

        success = student1.dropCourse(course1);
        std::print("{}\n",
                  (success ? "张三退C++编程成功" : "张三退C++编程失败（未选课）"));

    } catch (const std::exception& e) {
        std::print("系统异常：{}\n", e.what());
        return 1;
    }

    std::print("\n===== 测试完成 =====\n");
    return 0;
}
