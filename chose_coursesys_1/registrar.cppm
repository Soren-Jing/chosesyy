export module domain.registrar;

import std;
import domain.student;
import domain.course;
import domain.teacher;
import domain.score;


using std::string;
using std::vector;
using std::unique_ptr;
using std::unordered_map;
using std::print;
using std::move;

export class Registrar {
private:
    unordered_map<string, unique_ptr<Student>> students;                      //哈希表容器存放类型为teacher的智能指针
    unordered_map<string, unique_ptr<Course>> courses;                        //同理
    unordered_map<string, unique_ptr<Teacher>> teachers;                      //使用ID作为键，方便快速查找

public:
    void addStudent(unique_ptr<Student> student);                             //添加学生
    void addCourse(unique_ptr<Course> course);                                //添加课程
    void addTeacher(unique_ptr<Teacher> teacher);                             //添加教师
    bool registerCourse(const string& studentId, const string& courseId);     //统一选课接口（代管者模式核心）
    bool dropCourse(const string& studentId, const string& courseId);         //统一退课接口
    vector<const Course*> getStudentCourses(const string& studentId) const ;  //获取学生的选课列表（包含课程详情）v
    vector<const Student*> getCourseStudents(const string& courseId) const;   //获取课程的学生列表
    void displayAllRegistrations() const;                                     //展示所有学生选课情况
    const unordered_map<string, unique_ptr<Course>>& getAllCourses() const ;  //获取所有课程信息
    const unordered_map<string, unique_ptr<Student>>& getAllStudents() const ;//获取所有学生信息
    bool printStudentRecord(const std::string& studentId,                     //打印单个学生的完整选课记录
                            std::ostream& out = std::cout) const;             //如果学生不存在返回 false，成功打印返回 true
};


// 添加学生
void Registrar::addStudent(unique_ptr<Student> student) {
    students[student->getStudentId()] = std::move(student);
}

// 添加课程
void Registrar::addCourse(unique_ptr<Course> course) {
    courses[course->getCourseId()] = std::move(course);
}

// 添加教师
void Registrar::addTeacher(unique_ptr<Teacher> teacher) {
    teachers[teacher->getTeacherId()] = std::move(teacher);
}

// 统一选课接口（代管者模式核心）
// 返回: true=成功, false=失败2/2
bool Registrar::registerCourse(const string& studentId, const string& courseId) {
    // 1. 检查学生和课程是否存在
    auto itStudent = students.find(studentId);
    auto itCourse = courses.find(courseId);
    if (itStudent == students.end() || itCourse == courses.end()) {
        return false; // 学生或课程不存在
    }

    Student* student = itStudent->second.get();
    Course* course = itCourse->second.get();

    // 2. 检查课程是否已满
    if (course->isFull()) {
        return false;
    }

    // 3. 检查学生是否已选此课程
    if (student->hasSelectedCourse(courseId)) {
        return false;
    }

    // 4. 执行选课（同时更新学生和课程状态）
    bool studentOk = student->selectCourse(courseId);
    bool courseOk = course->enrollStudent();

    // 5. 确保两个操作都成功
    if (studentOk && courseOk) {
        return true;
    } else {
        // 回滚避免误操作
        if (studentOk) student->dropCourse(courseId);
        if (courseOk) course->dropStudent();
        return false;
    }
}

// 统一退课接口
bool Registrar::dropCourse(const string& studentId, const string& courseId) {
    auto itStudent = students.find(studentId);
    auto itCourse = courses.find(courseId);
    if (itStudent == students.end() || itCourse == courses.end()) {
        return false;
    }

    Student* student = itStudent->second.get();
    Course* course = itCourse->second.get();

    // 检查学生是否选了此课程
    if (!student->hasSelectedCourse(courseId)) {
        return false;
    }

    // 执行退课
    bool studentOk = student->dropCourse(courseId);
    bool courseOk = course->dropStudent();

    return studentOk && courseOk;
}

// 获取学生的选课列表（包含课程详情）
vector<const Course*> Registrar::getStudentCourses(const string& studentId) const {
    vector<const Course*> result;
    auto itStudent = students.find(studentId);
    if (itStudent == students.end()) {
        return result; // 学生不存在
    }

    // 遍历学生已选课程ID，查找对应课程详情
    for (const auto& courseId : itStudent->second->getSelectedCourses()) {
        auto itCourse = courses.find(courseId);
        if (itCourse != courses.end()) {
            result.push_back(itCourse->second.get());
        }
    }
    return result;
}

// 获取课程的学生列表
vector<const Student*> Registrar::getCourseStudents(const string& courseId) const {
    vector<const Student*> result;
    auto itCourse = courses.find(courseId);
    if (itCourse == courses.end()) {
        return result; // 课程不存在
    }

    // 遍历所有学生，检查是否选了这门课
    for (const auto& [studentId, student] : students) {
        if (student->hasSelectedCourse(courseId)) {
            result.push_back(student.get());
        }
    }
    return result;
}

// 展示所有学生选课情况
void Registrar::displayAllRegistrations() const {
    std::println("\n=== 学生选课总览 ===");
    for (const auto& [studentId, student] : students) {
        std::print("学生 {} ({}): ", student->getName(), studentId);
        auto courses = getStudentCourses(studentId);
        if (courses.empty()) {
            std::print("未选课");
        } else {
            for (const auto* course : courses) {
                std::print("{} ", course->getCourseName());
            }
        }
        std::println("");
    }
}

// 获取所有课程信息
const unordered_map<string, unique_ptr<Course>>& Registrar::getAllCourses() const {
    return courses;
}

// 获取所有学生信息
const unordered_map<string, unique_ptr<Student>>& Registrar::getAllStudents() const {
    return students;
}
// 打印单个学生的完整选课记录
// 如果学生不存在返回 false，成功打印返回 true
bool Registrar::printStudentRecord(const std::string& studentId,
                        std::ostream& out) const
{
    auto it = students.find(studentId);
    if (it == students.end()) return false;          // 学生不存在

    const Student* stu = it->second.get();
    auto           courses = getStudentCourses(studentId);

    out << "-------- 选课记录 --------\n";
    out << "学号: " << stu->getStudentId()
        << "  姓名: " << stu->getName()
        << "  专业: " << stu->getMajor() << '\n';

    if (courses.empty()) {
        out << "  当前未选修任何课程。\n";
    } else {
        out << "  已选课程 (" << courses.size() << " 门):\n";
        for (const Course* c : courses) {
            out << "    · " << std::left << std::setw(20)
                << c->getCourseName()
                << " [ID: " << c->getCourseId() << "]  "
                << "容量: " << c->getCurrentCapacity() << '/'
                << c->getMaxCapacity() << '\n';
        }
    }
    out << "--------------------------\n";
    return true;
}
