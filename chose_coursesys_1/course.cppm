export module domain.course;

import std;

using std::string;
using std::move;
using std::vector;
using std::move;

export class Course {
private:
    string courseId;
    string courseName;
    int maxCapacity;
    int currentCapacity;

public:
    Course(string id, string name, int maxCap);
    string getCourseId() const;               // 获取课程ID
    string getCourseName() const;             //获取课程名
    int getMaxCapacity() const;               //获取最大容量
    int getCurrentCapacity() const;           //获取当前容量
    bool isFull() const;                      //检查是否已满
    bool enrollStudent();                     //学生选课（增加当前容量）
    bool dropStudent();                       //学生退课（减少当前容量）
    int getRemainingCapacity() const ;        //获取剩余容量
    bool setCapacity(int newCapacity);        //修改课程容量（基础版本)
};

Course::Course(string id, string name, int maxCap)
    : courseId(move(id)), courseName(move(name)),
      maxCapacity(maxCap), currentCapacity(0)
{
    if (maxCap <= 0) {
        throw std::invalid_argument("课程容量必须大于0");
    }
}
//获取课程ID
string Course::getCourseId() const { return courseId; }

//获取课程名
string Course::getCourseName() const { return courseName; }

//获取最大容量
int Course::getMaxCapacity() const { return maxCapacity; }

//获取当前容量
int Course::getCurrentCapacity() const { return currentCapacity; }

//检查是否已满
bool Course::isFull() const { return currentCapacity >= maxCapacity; }


//学生选课（增加当前容量）
bool Course::enrollStudent() {
    if (isFull()) {
        return false;
    }
    currentCapacity++;
    return true;
}

//学生退课（减少当前容量）
bool Course::dropStudent() {
    if (currentCapacity <= 0) {
        return false;
    }
    currentCapacity--;
    return true;
}

//获取剩余容量
int Course::getRemainingCapacity() const {
    return maxCapacity - currentCapacity;
}

//修改课程容量（基础版本）
bool Course::setCapacity(int newCapacity) {
    if (newCapacity < currentCapacity) {
        return false; //新容量不能小于当前选课人数
    }
    maxCapacity = newCapacity;
    return true;
}
