// File: course.cppmn
// Created:Lufengying 2024051604030 1276809756@qq.com
// Version: 1.0      License: AGPLv3
export module registrar.course;
import std;
using std::string;
using std::move;
using std::invalid_argument;

export class Course {
private:
    string courseId;
    string courseName;
    int maxCapacity;
    int currentCapacity;

public:
    // 构造函数：增强参数校验
    Course(string id, string name, int maxCap)
        : courseId(move(id)), courseName(move(name)), maxCapacity(maxCap), currentCapacity(0) {
        if (maxCap <= 0) {
            throw invalid_argument("课程容量必须大于0");
        }
    }

    // 获取课程ID
    string getCourseId() const { return courseId; }
    // 获取课程名
    string getCourseName() const { return courseName; }
    // 获取最大容量
    int getMaxCapacity() const { return maxCapacity; }
    // 获取当前容量
    int getCurrentCapacity() const { return currentCapacity; }
    // 检查课程是否已满
    bool isFull() const { return currentCapacity >= maxCapacity; }
    // 获取剩余容量
    int getRemainingCapacity() const { return maxCapacity - currentCapacity; }

    // 学生选课：新增课程容量校验+双向关联支持
    bool enrollStudent() {
        if (isFull()) {
            return false; // 课程已满，选课失败
        }
        currentCapacity++;
        return true;
    }

    // 学生退课：新增容量下限校验+双向关联支持
    bool dropStudent() {
        if (currentCapacity <= 0) {
            return false; // 无学生可选退
        }
        currentCapacity--;
        return true;
    }

    // 修改课程容量：新增“新容量≥当前人数”校验
    bool setCapacity(int newCapacity) {
        if (newCapacity < currentCapacity) {
            throw invalid_argument("新容量不能小于当前选课人数");
        }
        maxCapacity = newCapacity;
        return true;
    }
};
