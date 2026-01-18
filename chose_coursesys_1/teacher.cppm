export module domain.teacher;

import std;
import domain.score;

using std::string;
using std::vector;

export class Teacher {
private:
    string teacherId;
    string name;
    string department;
    vector<string> teachingCourses; // 教授的课程ID列表

public:
    Teacher(string id, string name, string dept);
    string getTeacherId() const;                      //    获取教师ID
    string getName() const;                           //    获取姓名
    string getDepartment() const;                     //    部门
    bool addTeachingCourse(const string& courseId);   //    添加教授的课程
    bool removeTeachingCourse(const string& courseId);//    优化：参数统一用 string（和类内 using 匹配）
    const vector<string>& getTeachingCourses() const ;//    获取教授的课程列表
    bool isTeachingCourse(const string& courseId) const ;// 检查是否教授某门课程

    std::unique_ptr<Score> inputScore(const string& studentId,
                                     const string& courseId,
                                     float scoreValue);//   录入/修改学生成绩（基础版本）

    vector<std::unique_ptr<Score>> batchInputScores(
        const vector<std::tuple<string, string, float>>& scoreData);// 批量录入成绩

    struct CourseStatistics {
        string courseId;
        int studentCount;
        float averageScore;
        float maxScore;
        float minScore;
    };// 获取课程统计信息（简单版本）

    CourseStatistics getCourseStatistics(const string& courseId) const;// 生成课程统计（模拟数据）
};

Teacher::Teacher(string id, string name, string dept)
    : teacherId(std::move(id)), name(std::move(name)), department(std::move(dept)) {}

//获取教师ID
string Teacher::getTeacherId() const { return teacherId; }

//获取姓名
string Teacher::getName() const { return name; }

//获取部门（实现不动，只改声明）
string Teacher::getDepartment() const { return department; }

//添加教授的课程
bool Teacher::addTeachingCourse(const string& courseId) {
    // 检查是否已经在教授此课程
    for (const auto& cid : teachingCourses) {
        if (cid == courseId) {
            return false; // 已经在教授此课程
        }
    }
    teachingCourses.push_back(courseId);
    return true;
}

//移除教授的课程
bool Teacher::removeTeachingCourse(const string& courseId) {
    for (auto it = teachingCourses.begin(); it != teachingCourses.end(); ++it) {
        if (*it == courseId) {
            teachingCourses.erase(it);
            return true;
        }
    }
    return false;
}

//获取教授的课程列表
const vector<string>& Teacher::getTeachingCourses() const {
    return teachingCourses;
}

//检查是否教授某门课程
bool Teacher::isTeachingCourse(const string& courseId) const {
    for (const auto& cid : teachingCourses) {
        if (cid == courseId) {
            return true;
        }
    }
    return false;
}

//录入/修改学生成绩（基础版本）
std::unique_ptr<Score> Teacher::inputScore(const string& studentId,
                                 const string& courseId,
                                 float scoreValue) {
    //验证教师是否教授此课程
    if (!isTeachingCourse(courseId)) {
        return nullptr; // 不教授此课程，无权录入成绩
    }

    try {
        return std::make_unique<Score>(studentId, courseId, scoreValue);
    } catch (const std::invalid_argument& e) {
        return nullptr; // 成绩无效
    }
}

//批量录入成绩
vector<std::unique_ptr<Score>> Teacher::batchInputScores(
    const std::vector<std::tuple<string, string, float>>& scoreData) {

    vector<std::unique_ptr<Score>> scores;

    for (const auto& [studentId, courseId, scoreValue] : scoreData) {
        if (auto score = inputScore(studentId, courseId, scoreValue)) {
            scores.push_back(std::move(score));
        }
    }

    return scores;
}

//生成课程统计（模拟数据）
Teacher::CourseStatistics Teacher::getCourseStatistics(const string& courseId) const {
    if (!isTeachingCourse(courseId)) {
        throw std::runtime_error("不教授此课程");
    }

    //模拟数据 - 实际中应该从数据库查询
    return CourseStatistics{
        courseId,
        25,    // 学生人数
        78.5f, // 平均分
        98.0f, // 最高分
        45.0f  // 最低分
    };
}
