// File: domain/student.cppm
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

//实现部分（无改动）
Student::Student(string id, string name, string major, int age, string gender)
    : Person(move(id), move(name), age, move(gender)), major(move(major)) {}

string Student::getStudentId() const { return getId(); }
string Student::getName() const { return Person::getName(); }
string Student::getMajor() const { return major; }

bool Student::selectCourse(const string& courseId) {
    auto it = std::find(selectedCourses.begin(), selectedCourses.end(), courseId);
    if (it != selectedCourses.end()) return false;
    selectedCourses.push_back(courseId);
    return true;
}

bool Student::dropCourse(const string& courseId) {
    auto it = std::find(selectedCourses.begin(), selectedCourses.end(), courseId);
    if (it != selectedCourses.end()) {
        selectedCourses.erase(it);
        return true;
    }
    return false;
}

const vector<string>& Student::getSelectedCourses() const {
    return selectedCourses;
}

bool Student::hasSelectedCourse(const string& courseId) const {
    return std::find(selectedCourses.begin(), selectedCourses.end(), courseId)
           != selectedCourses.end();
}

vector<Score> Student::queryScore() const {
    vector<Score> scores;
    for (const auto& courseId : selectedCourses) {
        float mockScore = 60.0f + (std::hash<string>{}(getStudentId() + courseId) % 40);
        scores.emplace_back(getStudentId(), courseId, mockScore);
    }
    return scores;
}
