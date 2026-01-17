// File: student.cppm
// Created:Lufengying 2024051604030 127680976@qq.com        2026-01-17
// Version: 1.0      License: AGPLv3

export module registrar.student;
import std;
import registrar.course; // 引入Course类，实现双向关联
import registrar.score;

using std::string;
using std::move;
using std::vector;
using std::find;

export class Student {
private:
    string studentId;
    string name;
    string major;
    vector<string> selectedCourses; // 存储已选课程ID
    vector<Score> scores; // 新增：存储学生成绩（替代临时生成）

public:
    // 构造函数：基础初始化
    Student(string id, string name, string major)
        : studentId(move(id)), name(move(name)), major(move(major)) {}

    // 获取学生ID
    string getStudentId() const { return studentId; }
    // 获取姓名
    string getName() const { return name; }
    // 获取专业
    string getMajor() const { return major; }
    // 获取已选课程列表
    const vector<string>& getSelectedCourses() const { return selectedCourses; }
    // 获取成绩列表
    const vector<Score>& getScores() const { return scores; }

    // 选课：新增“课程容量校验”+双向关联
    bool selectCourse(Course& course) { // 参数改为Course对象，实现双向操作
        // 1. 检查是否已选该课程
        if (hasSelectedCourse(course.getCourseId())) {
            return false;
        }
        // 2. 检查课程是否已满
        if (course.isFull()) {
            return false;
        }
        // 3. 双向更新（学生选课+课程人数增加）
        selectedCourses.push_back(course.getCourseId());
        course.enrollStudent();
        return true;
    }

    // 退课：新增双向关联
    bool dropCourse(Course& course) { // 参数改为Course对象
        auto it = find(selectedCourses.begin(), selectedCourses.end(), course.getCourseId());
        if (it == selectedCourses.end()) {
            return false; // 未选该课程
        }
        // 双向更新（学生退课+课程人数减少）
        selectedCourses.erase(it);
        course.dropStudent();
        return true;
    }

    // 检查是否已选课程
    bool hasSelectedCourse(const string& courseId) const {
        return find(selectedCourses.begin(), selectedCourses.end(), courseId) != selectedCourses.end();
    }

    // 录入成绩：新增（替代临时生成）
    bool addScore(const Score& score) {
        // 检查是否是已选课程的成绩
        if (!hasSelectedCourse(score.getCourseId())) {
            return false;
        }
        scores.push_back(score);
        return true;
    }
};
