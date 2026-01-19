// 教师模块：定义教师类，继承自Person基类，包含授课管理、成绩录入、课程统计等功能
export module domain.teacher;

import std;
import domain.score;
import domain.person;

using std::string;
using std::vector;

// 教师类，继承自Person基类，实现教师专属的授课和成绩管理功能
export class Teacher : public Person {
private:
    string department;             // 教师所属部门
    vector<string> teachingCourses;// 教师所教授的课程ID列表，用于管理授课范围

public:
    Teacher(string id, string name, string dept, int age = 30, string gender = "未知"); // 构造函数
    string getTeacherId() const;                                 // 获取教师ID
    string getName() const;                                     //获取教师姓名（覆盖父类同名方法，确保返回正确的姓名）
    string getDepartment() const;                               //同理
    bool addTeachingCourse(const string& courseId);             // 为教师添加一门授课课程
    bool removeTeachingCourse(const string& courseId);          // 为教师移除一门授课课程
    const vector<string>& getTeachingCourses() const;           // 获取教师的所有授课课程ID列表
    bool isTeachingCourse(const string& courseId) const;        // 检查教师是否教授某门课程
    std::unique_ptr<Score> inputScore(const string& studentId,
                                     const string& courseId,
                                     float scoreValue);          // 为单个学生录入某门课程的成绩
    vector<std::unique_ptr<Score>> batchInputScores(
        const vector<std::tuple<string, string, float>>& scoreData);    // 批量录入成绩，支持一次性录入多条学生-课程-成绩记录

    // 课程统计信息结构体：存储某门课程的学生人数、平均分、最高分、最低分
    struct CourseStatistics {
        string courseId;       // 课程ID
        int studentCount;      // 选课学生人数
        float averageScore;    // 课程平均分
        float maxScore;        // 课程最高分
        float minScore;        // 课程最低分
    };
    CourseStatistics getCourseStatistics(const string& courseId) const;    // 获取某门课程的统计信息
};

Teacher::Teacher(string id, string name, string dept, int age, string gender)
    : Person(move(id), move(name), age, move(gender)), department(move(dept)) {}

//获取教师ID
string Teacher::getTeacherId() const { return getId(); }

//获取教师姓名
//调用父类Person的getName()方法，确保姓名的一致性
string Teacher::getName() const { return Person::getName(); }

// 获取教师所属部门
string Teacher::getDepartment() const { return department; }

// 为教师添加授课课程
//遍历teachingCourses列表，检查课程ID是否已存在；不存在则添加并返回true，否则返回false
bool Teacher::addTeachingCourse(const string& courseId) {
    for (const auto& cid : teachingCourses) {
        if (cid == courseId) return false;
    }
    teachingCourses.push_back(courseId);
    return true;
}

//为教师移除授课课程
//遍历列表找到目标课程ID的迭代器，通过erase方法删除元素；找到则返回true，否则返回false
bool Teacher::removeTeachingCourse(const string& courseId) {
    for (auto it = teachingCourses.begin(); it != teachingCourses.end(); ++it) {
        if (*it == courseId) {
            teachingCourses.erase(it);
            return true;
        }
    }
    return false;
}

//获取教师的所有授课课程ID列表
//返回const引用，确保函数调用方只能读取列表，不能修改
const vector<string>& Teacher::getTeachingCourses() const {
    return teachingCourses;
}

//检查教师是否教授指定课程
//遍历列表，匹配到课程ID则返回true，否则返回false
bool Teacher::isTeachingCourse(const string& courseId) const {
    for (const auto& cid : teachingCourses) {
        if (cid == courseId) return true;
    }
    return false;
}

// 为单个学生录入成绩
/*先通过isTeachingCourse检查教师是否有权教授该课程，无权则返回空指针
尝试创建Score对象，若成绩值非法则捕获并返回空指针若成功创建则返回指向Score对象的unique_ptr*/
std::unique_ptr<Score> Teacher::inputScore(const string& studentId,
                                 const string& courseId,
                                 float scoreValue) {
    // 权限校验：教师必须教授该课程才能录入成绩
    if (!isTeachingCourse(courseId)) return nullptr;
    try {
        // 创建Score对象，Score构造函数会校验成绩是否在0-100范围内
        return std::make_unique<Score>(studentId, courseId, scoreValue);
    } catch (const std::invalid_argument& e) {
        // 成绩非法时返回空指针
        return nullptr;
    }
}

//批量录入成绩
//对每条数据调用inputScore方法，若返回非空指针则加入结果列表
//最终返回所有成功录入的Score智能指针列表
vector<std::unique_ptr<Score>> Teacher::batchInputScores(
    const vector<std::tuple<string, string, float>>& scoreData) {

    vector<std::unique_ptr<Score>> scores;
    for (const auto& [studentId, courseId, scoreValue] : scoreData) {
        // 复用单个录入逻辑，确保权限和数据校验一致
        if (auto score = inputScore(studentId, courseId, scoreValue)) {
            scores.push_back(std::move(score));
        }
    }
    return scores;
}

// 获取课程统计信息
//先校验教师是否教授该课程，无权则抛出运行时异常
Teacher::CourseStatistics Teacher::getCourseStatistics(const string& courseId) const {
    if (!isTeachingCourse(courseId)) {
        throw std::runtime_error("不教授此课程");
    }
    return CourseStatistics{
        courseId,
        25,    // 模拟：选课学生人数
        78.5f, // 模拟：课程平均分
        98.0f, // 模拟：课程最高分
        45.0f  // 模拟：课程最低分
    };
}
