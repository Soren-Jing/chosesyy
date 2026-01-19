export module domain.student;

import std;
import domain.score;
import domain.person;  // 导入基类模块

using std::string;
using std::move;
using std::vector;

export class Student : public Person {  //继承Person
private:
    string major;
    vector<string> selectedCourses;

public:
    //构造函数初始化Person部分
    Student(string id, string name, string major, int age = 18, string gender = "未知");

    //保留兼容接口
    string getStudentId() const;
    string getMajor() const;
    string getName() const;
    bool selectCourse(const string& courseId);
    bool dropCourse(const string& courseId);
    const vector<string>& getSelectedCourses() const;
    bool hasSelectedCourse(const string& courseId) const;
    vector<Score> queryScore() const;
};

//构造
Student::Student(string id, string name, string major, int age, string gender)
    : Person(move(id), move(name), age, move(gender)), major(move(major)) {}

//分别获取id,name,和主修
string Student::getStudentId() const { return getId(); }
string Student::getName() const { return Person::getName(); }
string Student::getMajor() const { return major; }

//学生选课功能
//遍历selectedCourses列表
//若已存在则返回false，表示选课失败。若不存在则将课程ID添加到列表末尾，返回true表示选课成功
bool Student::selectCourse(const string& courseId) {
    auto it = std::find(selectedCourses.begin(), selectedCourses.end(), courseId);
    if (it != selectedCourses.end()) return false;
    selectedCourses.push_back(courseId);
    return true;
}

//学生退课功能
//同理遍历selectedCourses列表
//若找到则调用vector::erase删除该元素，返回true表示退课成功。若未找到则返回false表示退课失败，也可能是学生未选此课
bool Student::dropCourse(const string& courseId) {
    auto it = std::find(selectedCourses.begin(), selectedCourses.end(), courseId);
    if (it != selectedCourses.end()) {
        selectedCourses.erase(it);
        return true;
    }
    return false;
}

// 获取已选课程列表
const vector<string>& Student::getSelectedCourses() const {
    return selectedCourses;
}

// 检查是否已选某门课程
bool Student::hasSelectedCourse(const string& courseId) const {
    return std::find(selectedCourses.begin(), selectedCourses.end(), courseId)
           != selectedCourses.end();
}

// 查询学生已选课程的成绩
// 生成模拟成绩。基础分60 + （学生ID+课程ID的哈希值对40取模），确保成绩范围在60~99之间
// 调用Score的构造函数，创建成绩对象并添加到结果列表
vector<Score> Student::queryScore() const {
    vector<Score> scores;
    for (const auto& courseId : selectedCourses) {
        float mockScore = 60.0f + (std::hash<string>{}(getStudentId() + courseId) % 40);
        scores.emplace_back(getStudentId(), courseId, mockScore);
    }
    return scores;
}
