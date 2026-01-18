// File: domain/teacher.cppm
export module domain.teacher;

import std;
import domain.score;
import domain.person;  //导入基类模块

using std::string;
using std::vector;

export class Teacher : public Person {  //继承Person
private:
    string department;
    vector<string> teachingCourses;

public:
    //构造函数初始化Person部分
    Teacher(string id, string name, string dept, int age = 30, string gender = "未知");
    //保留兼容接口
    string getTeacherId() const;
    string getName() const;
    string getDepartment() const;

    bool addTeachingCourse(const string& courseId);
    bool removeTeachingCourse(const string& courseId);
    const vector<string>& getTeachingCourses() const;
    bool isTeachingCourse(const string& courseId) const;
    std::unique_ptr<Score> inputScore(const string& studentId,
                                     const string& courseId,
                                     float scoreValue);
    vector<std::unique_ptr<Score>> batchInputScores(
        const vector<std::tuple<string, string, float>>& scoreData);

    struct CourseStatistics {
        string courseId;
        int studentCount;
        float averageScore;
        float maxScore;
        float minScore;
    };
    CourseStatistics getCourseStatistics(const string& courseId) const;
};

//实现部分（移除了name成员访问）
Teacher::Teacher(string id, string name, string dept, int age, string gender)
    : Person(move(id), move(name), age, move(gender)), department(move(dept)) {}

string Teacher::getTeacherId() const { return getId(); }
string Teacher::getName() const { return Person::getName(); }
string Teacher::getDepartment() const { return department; }

bool Teacher::addTeachingCourse(const string& courseId) {
    // 修改：空值校验，抛出异常
    if (courseId.empty()) {
        throw std::invalid_argument("课程ID不能为空");
    }
    for (const auto& cid : teachingCourses) {
        if (cid == courseId) return false;
    }
    teachingCourses.push_back(courseId);
    return true;
}

bool Teacher::removeTeachingCourse(const string& courseId) {
    for (auto it = teachingCourses.begin(); it != teachingCourses.end(); ++it) {
        if (*it == courseId) {
            teachingCourses.erase(it);
            return true;
        }
    }
    return false;
}

const vector<string>& Teacher::getTeachingCourses() const {
    return teachingCourses;
}

bool Teacher::isTeachingCourse(const string& courseId) const {
    for (const auto& cid : teachingCourses) {
        if (cid == courseId) return true;
    }
    return false;
}

std::unique_ptr<Score> Teacher::inputScore(const string& studentId,
                                 const string& courseId,
                                 float scoreValue) {
    if (!isTeachingCourse(courseId)) return nullptr;
    try {
        return std::make_unique<Score>(studentId, courseId, scoreValue);
    } catch (const std::invalid_argument& e) {
        return nullptr;
    }
}

vector<std::unique_ptr<Score>> Teacher::batchInputScores(
    const vector<std::tuple<string, string, float>>& scoreData) {

    vector<std::unique_ptr<Score>> scores;
    for (const auto& [studentId, courseId, scoreValue] : scoreData) {
        if (auto score = inputScore(studentId, courseId, scoreValue)) {
            scores.push_back(std::move(score));
        }
    }
    return scores;
}

Teacher::CourseStatistics Teacher::getCourseStatistics(const string& courseId) const {
    if (!isTeachingCourse(courseId)) {
        throw std::runtime_error("不教授此课程");
    }
    return CourseStatistics{
        courseId,
        25,    // 学生人数
        78.5f, // 平均分
        98.0f, // 最高分
        45.0f  // 最低分
    };
}
