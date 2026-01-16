//student.cppm
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
    vector<string> selectedCourses; // 存储课程ID

public:
    Student(string id, string name, string major)
        : studentId(move(id)), name(move(name)), major(move(major)) {}

    // 获取学生ID
    string getStudentId() const { return studentId; }

    // 获取姓名
    string getName() const { return name; }

    // 获取专业
    string getMajor() const { return major; }

    // 选课
    bool selectCourse(const string& courseId) {
        // 检查是否已选该课程
        auto it = std::find(selectedCourses.begin(), selectedCourses.end(), courseId);
        if (it != selectedCourses.end()) {
            return false; // 已选过此课程
        }
        selectedCourses.push_back(courseId);
        return true;
    }

    // 退课
    bool dropCourse(const string& courseId) {
        auto it = std::find(selectedCourses.begin(), selectedCourses.end(), courseId);
        if (it != selectedCourses.end()) {
            selectedCourses.erase(it);
            return true;
        }
        return false;
    }

    // 获取已选课程列表
    const vector<string>& getSelectedCourses() const {
        return selectedCourses;
    }

    // 检查是否选了某门课
    bool hasSelectedCourse(const string& courseId) const {
        return std::find(selectedCourses.begin(), selectedCourses.end(), courseId)
               != selectedCourses.end();
    }

    // 查询成绩（模拟版本，后续会完善）
    vector<Score> queryScore() const {
        vector<Score> scores;
        // 为每门已选课程创建一个模拟成绩
        for (const auto& courseId : selectedCourses) {
            // 使用学生ID和课程ID生成一个确定性的模拟成绩
            float mockScore = 60.0f + (std::hash<string>{}(studentId + courseId) % 40);
            scores.emplace_back(studentId, courseId, mockScore);
        }
        return scores;
    }
};

