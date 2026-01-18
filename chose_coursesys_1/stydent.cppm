export module domain.student;

import std;
import domain.score;

using std::string;
using std::move;
using std::vector;
using std::move;
export class Student {
private:
    string studentId;
    string name;
    string major;
    vector<string> selectedCourses; //存储课程ID

public:
    Student(string id, string name, string major);
    string getStudentId() const;                         //获取学生ID
    string getName() const;                              //获取姓名
    string getMajor() const;                             //获取专业
    bool selectCourse(const string& courseId) ;          //选课
    bool dropCourse(const string& courseId);             //退课
    const vector<string>& getSelectedCourses() const;    //获取已选课程列表
    bool hasSelectedCourse(const string& courseId) const;//检查是否选了某门课
    vector<Score> queryScore() const;                    //查询成绩（模拟版本，后续会完善）
};

Student::Student(string id, string name, string major)
    : studentId(move(id)), name(move(name)), major(move(major)) {}

//获取学生ID
string Student::getStudentId() const { return studentId; }

//获取姓名
string Student::getName() const { return name; }

//获取专业
string Student::getMajor() const { return major; }

//选课
bool Student::selectCourse(const string& courseId) {
    //检查是否已选该课程
    auto it = std::find(selectedCourses.begin(), selectedCourses.end(), courseId);
    if (it != selectedCourses.end()) {
        return false; // 已选过此课程
    }
    selectedCourses.push_back(courseId);
    return true;
}

//退课
bool Student::dropCourse(const string& courseId) {
    auto it = std::find(selectedCourses.begin(), selectedCourses.end(), courseId);
    if (it != selectedCourses.end()) {
        selectedCourses.erase(it);
        return true;
    }
    return false;
}

//获取已选课程列表
const vector<string>& Student::getSelectedCourses() const {
    return selectedCourses;
}

//检查是否选了某门课
bool Student::hasSelectedCourse(const string& courseId) const {
    return std::find(selectedCourses.begin(), selectedCourses.end(), courseId)
           != selectedCourses.end();
}

//查询成绩（模拟版本，后续会完善）
vector<Score> Student::queryScore() const {
    vector<Score> scores;
    //为每门已选课程创建一个模拟成绩
    for (const auto& courseId : selectedCourses) {
        // 使用学生ID和课程ID生成一个确定性的模拟成绩
        float mockScore = 60.0f + (std::hash<string>{}(studentId + courseId) % 40);
        scores.emplace_back(studentId, courseId, mockScore);
    }
    return scores;
}
