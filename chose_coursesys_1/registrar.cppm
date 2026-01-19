export module domain.registrar;

import std;
import domain.student;
import domain.course;
import domain.teacher;
import domain.score;
import domain.person;
import database;
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
    //void displayAllRegistrations() const;                                     //展示所有学生选课情况
    const unordered_map<string, unique_ptr<Course>>& getAllCourses() const ;  //获取所有课程信息
    const unordered_map<string, unique_ptr<Student>>& getAllStudents() const ;//获取所有学生信息
    bool printStudentRecord(const std::string& studentId) const;              //打印单个学生的完整选课记录
                                                                              //如果学生不存在返回f，成功打印返回t
    const Person* findPerson(const string& id) const;
    // 展示所有学生选课情况
    void displayAllRegistrations() const {
            std::println("\n=== 学生选课总览 ===");
            for (const auto& [studentId, student] : students) {
                std::print("学生 {} ({}, 年龄:{}, 性别:{}): ",
                          student->getName(), studentId,
                          student->getAge(), student->getGender());  // 使用Person接口
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

        const auto& getAllStudents()  { return students; }
        const auto& getAllCourses()  { return courses; }
        const auto& getAllTeachers()  { return teachers; }

};


// 添加学生
void Registrar::addStudent(unique_ptr<Student> student) {
    students[student->getStudentId()] = std::move(student);//因为是指针所以只能进行资源转移
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
// 核心修改：选课同步到数据库
    bool Registrar::registerCourse(const std::string& studentId, const std::string& courseId) {
        auto itStudent = students.find(studentId);
        auto itCourse = courses.find(courseId);
        if (itStudent == students.end() || itCourse == courses.end()) return false;

        Student* student = itStudent->second.get();
        Course* course = itCourse->second.get();
        if (course->isFull() || student->hasSelectedCourse(courseId)) return false;

        // 1. 先写入数据库
        if (!addEnrollmentToDB(studentId, courseId)) return false;

        // 2. 再更新内存状态
        bool studentOk = student->selectCourse(courseId);
        bool courseOk = course->enrollStudent();

        // 3. 失败则回滚数据库
        if (!(studentOk && courseOk)) {
            removeEnrollmentFromDB(studentId, courseId);
            return false;
        }
        return true;
    }

    // 核心修改：退课同步到数据库
    bool Registrar::dropCourse(const std::string& studentId, const std::string& courseId) {
        auto itStudent = students.find(studentId);
        auto itCourse = courses.find(courseId);
        if (itStudent == students.end() || itCourse == courses.end()) return false;

        Student* student = itStudent->second.get();
        Course* course = itCourse->second.get();
        if (!student->hasSelectedCourse(courseId)) return false;

        // 1. 先删除数据库记录
        if (!removeEnrollmentFromDB(studentId, courseId)) return false;

        // 2. 再更新内存状态
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
bool Registrar::printStudentRecord(const string& studentId) const {
    auto it = students.find(studentId);
    if (it == students.end()) return false;

    const Student* stu = it->second.get();
    auto courses = getStudentCourses(studentId);

    std::println("-------- 选课记录 --------");
    std::println("学号: {}  姓名: {}  专业: {}  年龄: {}  性别: {}",
                stu->getStudentId(), stu->getName(),
                stu->getMajor(), stu->getAge(), stu->getGender());

    if (courses.empty()) {
        std::println("  当前未选修任何课程。");
    } else {
        std::println("  已选课程 ({} 门):", courses.size());
        for (const Course* c : courses) {
            std::println("    · {:<20} [ID: {}]  容量: {}/{}",
                        c->getCourseName(), c->getCourseId(),
                        c->getCurrentCapacity(), c->getMaxCapacity());
        }
    }
    std::println("--------------------------");
    return true;
}

// 新增：通过Person基类统一查询
const Person* Registrar::findPerson(const string& id) const {
   if (auto it = students.find(id); it != students.end()) return it->second.get();
   if (auto it = teachers.find(id); it != teachers.end()) return it->second.get();
   return nullptr;
}
