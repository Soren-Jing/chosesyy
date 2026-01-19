// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:YIjunchen  LUfengying ChenYuehang    2026-01-17 17:51:42
// Description:
//     a system of select course
//     [v0.1.2]     2026-01-17 17:49:04
//         * 初步实现学生教师和分数
//     [v0.1.2]     2026-01-18 13:39:57
//         *初步实现了教学秘书类和排课功能
//     [v0.1.3]     2026-01-18 21:16:40
//         * 实现UI分离模块，UI与业务逻辑解耦
//     [v0.1.4]     2026-01-19 16:35:32
//         * 实现了对数据库的连接，学生的选课退课记录可以化为表格存储在数据库中
module;
#include <pqxx/pqxx>
import std;
import domain.student;
import domain.course;
import domain.score;
import domain.teacher;
import domain.registrar;
import domain.secretary;
import domain.person;  //用于多态，统一人员属性接口
import database;

using std::print;
using std::cin;
using std::cout;
using std::cerr;
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::tuple;
using std::numeric_limits;
using std::streamsize;

// 步骤1：新增业务适配类（解耦核心，隔离UI与底层业务）
class CourseSystemService {
private:
    // 业务层内部持有核心对象，UI层不可见
    Registrar registrar;
    Secretary secretary;

public:
    // 构造函数：初始化业务层核心对象
    CourseSystemService() : secretary("SEC001", "赵秘书", 30, "女") {
        // 初始化测试数据（原main函数中的初始化逻辑迁移至此）
        initTestData();
    }

    //学生相关业务接口
    bool addStudent(const string& id, const string& name, const string& major, int age, const string& gender) {
        try {
            registrar.addStudent(make_unique<Student>(id, name, major, age, gender));
            return true;
        } catch (const std::exception& e) {
            cerr << "失败: " << e.what() << "\n";
            return false;
        }
    }

    vector<tuple<string, string, int, string>> getAllStudents() {
        vector<tuple<string, string, int, string>> studentList;
        for (const auto& [id, s] : registrar.getAllStudents()) {
            studentList.emplace_back(id, s->getName(), s->getAge(), s->getGender());
        }
        return studentList;
    }

    bool printStudentRecord(const string& id) {
        return registrar.printStudentRecord(id);
    }

    vector<tuple<string, string>> getStudentCourses(const string& id) {
        vector<tuple<string, string>> courseList;
        auto courses = registrar.getStudentCourses(id);
        for (const auto* c : courses) {
            courseList.emplace_back(c->getCourseId(), c->getCourseName());
        }
        return courseList;
    }

    // 教师相关业务接口
    bool addTeacher(const string& id, const string& name, const string& dept, int age, const string& gender) {
        try {
            registrar.addTeacher(make_unique<Teacher>(id, name, dept, age, gender));
            return true;
        } catch (const std::exception& e) {
            cerr << "失败: " << e.what() << "\n";
            return false;
        }
    }

    vector<tuple<string, string, string>> getAllTeachers() {
        vector<tuple<string, string, string>> teacherList;
        for (const auto& [id, t] : registrar.getAllTeachers()) {
            teacherList.emplace_back(id, t->getName(), t->getDepartment());
        }
        return teacherList;
    }

    bool addTeacherCourse(const string& tid, const string& cid) {
        auto it = registrar.getAllTeachers().find(tid);
        if (it == registrar.getAllTeachers().end()) return false;
        return it->second->addTeachingCourse(cid);
    }

    vector<string> getTeacherCourses(const string& tid) {
        vector<string> courseList;
        auto it = registrar.getAllTeachers().find(tid);
        if (it != registrar.getAllTeachers().end()) {
            for (const auto& cid : it->second->getTeachingCourses()) {
                courseList.push_back(cid);
            }
        }
        return courseList;
    }

    //课程相关业务接口
    bool addCourse(const string& id, const string& name, int cap) {
        try {
            registrar.addCourse(make_unique<Course>(id, name, cap));
            return true;
        } catch (const std::exception& e) {
            cerr << "失败: " << e.what() << "\n";
            return false;
        }
    }

    vector<tuple<string, string, int, int>> getAllCourses() {
        vector<tuple<string, string, int, int>> courseList;
        for (const auto& [id, c] : registrar.getAllCourses()) {
            courseList.emplace_back(id, c->getCourseName(), c->getCurrentCapacity(), c->getMaxCapacity());
        }
        return courseList;
    }

    bool getCourseDetail(const string& id, tuple<string, string, int, int, string>& detail) {
        auto it = registrar.getAllCourses().find(id);
        if (it == registrar.getAllCourses().end()) return false;
        const auto& c = it->second;
        string status = c->isFull() ? "已满" : "未满";
        detail = {c->getCourseId(), c->getCourseName(), c->getCurrentCapacity(), c->getMaxCapacity(), status};
        return true;
    }

    vector<tuple<string, string>> getCourseStudents(const string& cid) {
        vector<tuple<string, string>> studentList;
        auto students = registrar.getCourseStudents(cid);
        for (const auto* s : students) {
            studentList.emplace_back(s->getStudentId(), s->getName());
        }
        return studentList;
    }

    //选课相关业务接口
    bool studentEnroll(const string& sid, const string& cid) {
        return registrar.registerCourse(sid, cid);
    }

    bool studentDropCourse(const string& sid, const string& cid) {
        return registrar.dropCourse(sid, cid);
    }

    //成绩相关业务接口
    bool inputSingleGrade(const string& sid, const string& cid, float score, const string& tid) {
        auto it = registrar.getAllTeachers().find(tid);
        if (it == registrar.getAllTeachers().end()) return false;
        auto grade = it->second->inputScore(sid, cid, score);
        return grade != nullptr;
    }

    int batchInputGrades(const string& tid, const vector<tuple<string, string, float>>& batchData) {
        auto it = registrar.getAllTeachers().find(tid);
        if (it == registrar.getAllTeachers().end()) return 0;
        auto scoreList = it->second->batchInputScores(batchData);
        return scoreList.size();
    }

    vector<tuple<string, float, string>> getStudentGrades(const string& sid) {
        vector<tuple<string, float, string>> gradeList;
        auto it = registrar.getAllStudents().find(sid);
        if (it == registrar.getAllStudents().end()) return gradeList;
        auto scores = it->second->queryScore();
        for (const auto& s : scores) {
            gradeList.emplace_back(s.getCourseId(), s.getScore(), s.getGrade());
        }
        return gradeList;
    }

    //排课相关业务接口
    bool scheduleCourse(const string& tid, const string& cid, const string& time, const string& room) {
        return secretary.scheduleCourse(tid, cid, time, room);
    }

    bool cancelSchedule(const string& tid, const string& cid, const string& time) {
        return secretary.cancelSchedule(tid, cid, time);
    }

    void printAllSchedules() {
        secretary.printSchedule();
    }

private:
    // 初始化测试数据
    void initTestData() {
        try {
            //添加三名测试学生
            registrar.addStudent(make_unique<Student>("S001", "张三", "计算机科学", 20, "男"));
            registrar.addStudent(make_unique<Student>("S002", "李四", "软件工程", 21, "男"));
            registrar.addStudent(make_unique<Student>("S003", "王五", "网络工程", 19, "女"));
            //添加两名测试老师
            registrar.addTeacher(make_unique<Teacher>("T001", "王教授", "计算机学院", 45, "男"));
            registrar.addTeacher(make_unique<Teacher>("T002", "李教授", "软件学院", 50, "女"));
            //添加三门测试课程
            registrar.addCourse(make_unique<Course>("C001", "C++程序设计", 2));
            registrar.addCourse(make_unique<Course>("C002", "数据结构", 3));
            registrar.addCourse(make_unique<Course>("C003", "数据库原理", 2));
            // 为教师添加教授课程
            auto* t1 = registrar.getAllTeachers().find("T001")->second.get();
            auto* t2 = registrar.getAllTeachers().find("T002")->second.get();
            t1->addTeachingCourse("C001");
            t1->addTeachingCourse("C002");
            t2->addTeachingCourse("C003");
        } catch (const std::exception& e) {
            cerr << "初始化失败: " << e.what() << "\n";
        }
    }
};


// 步骤2：新增UI独立模块类（纯UI逻辑，仅依赖业务服务接口）
class ConsoleUIModule {
private:
    // UI层仅依赖业务服务接口，不耦合底层对象
    CourseSystemService& service;

    // 辅助函数（移入UI类，作为私有方法）
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }

    void pauseScreen() {
        print("\n按回车键继续...");
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
        std::cin.get();
    }

    string inputString(const string& prompt) {
        print("{}", prompt);
        string s;
        cin >> s;
        return s;
    }

    int inputInt(const string& prompt) {
        while (true) {
            print("{}", prompt);
            int v;
            if (cin >> v) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return v;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            print("错误：请输入整数。\n");
        }
    }

    float inputFloat(const string& prompt) {
        while (true) {
            print("{}", prompt);
            float v;
            if (cin >> v) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return v;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            print("错误：请输入数字。\n");
        }
    }

    bool confirmAction(const string& prompt) {
        print("{} (y/n): ", prompt);
        char c;
        cin >> c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return c == 'y' || c == 'Y';
    }

    // UI子菜单（移入UI类，修改为调用业务服务接口）
    void studentManagementMenu() {
        while (true) {
            clearScreen();
            print("\n========== 学生管理 ==========\n");
            print("1. 添加学生\n2. 查看所有学生\n3. 查看学生详情\n4. 查看学生选课\n0. 返回\n");
            print("=============================\n");

            switch (inputInt("请选择: ")) {
                case 1: {
                    string id = inputString("学号: ");
                    string name = inputString("姓名: ");
                    string major = inputString("专业: ");
                    int age = inputInt("年龄: ");
                    string gender = inputString("性别: ");
                    if (service.addStudent(id, name, major, age, gender)) {
                        print("添加成功！\n");
                    }
                    pauseScreen();
                    break;
                }
                case 2: {
                    print("\n所有学生:\n");
                    auto students = service.getAllStudents();
                    for (const auto& [id, name, age, gender] : students) {
                        print("{} - {} ({}岁, {})\n", id, name, age, gender);
                    }
                    pauseScreen();
                    break;
                }
                case 3: {
                    string id = inputString("学号: ");
                    if (!service.printStudentRecord(id)) {
                        cerr << "学生不存在！\n";
                    }
                    pauseScreen();
                    break;
                }
                case 4: {
                    string id = inputString("学号: ");
                    auto courses = service.getStudentCourses(id);
                    if (courses.empty()) {
                        print("无选课记录。\n");
                    } else {
                        print("\n选修课程:\n");
                        for (const auto& [cid, cname] : courses) {
                            print("  - {} [{}]\n", cname, cid);
                        }
                    }
                    pauseScreen();
                    break;
                }
                case 0: return;
                default: print("无效选择！\n"); pauseScreen();
            }
        }
    }

    void teacherManagementMenu() {
        while (true) {
            clearScreen();
            print("\n========== 教师管理 ==========\n");
            print("1. 添加教师\n2. 查看所有教师\n3. 添加教授课程\n4. 查看教授课程\n0. 返回\n");
            print("=============================\n");

            switch (inputInt("请选择: ")) {
                case 1: {
                    string id = inputString("教师ID: ");
                    string name = inputString("姓名: ");
                    string dept = inputString("部门: ");
                    int age = inputInt("年龄: ");
                    string gender = inputString("性别: ");
                    if (service.addTeacher(id, name, dept, age, gender)) {
                        print("添加成功！\n");
                    }
                    pauseScreen();
                    break;
                }
                case 2: {
                    print("\n所有教师:\n");
                    auto teachers = service.getAllTeachers();
                    for (const auto& [id, name, dept] : teachers) {
                        print("{} - {} ({})\n", id, name, dept);
                    }
                    pauseScreen();
                    break;
                }
                case 3: {
                    string tid = inputString("教师ID: ");
                    string cid = inputString("课程ID: ");
                    if (service.addTeacherCourse(tid, cid)) {
                        print("添加成功！\n");
                    } else {
                        print("教师不存在或课程已存在。\n");
                    }
                    pauseScreen();
                    break;
                }
                case 4: {
                    string tid = inputString("教师ID: ");
                    auto courses = service.getTeacherCourses(tid);
                    if (courses.empty()) {
                        print("教师不存在或无教授课程。\n");
                    } else {
                        print("\n教授课程:\n");
                        for (const auto& cid : courses) {
                            print("  - {}\n", cid);
                        }
                    }
                    pauseScreen();
                    break;
                }
                case 0: return;
                default: print("无效选择！\n"); pauseScreen();
            }
        }
    }

    void courseManagementMenu() {
        while (true) {
            clearScreen();
            print("\n========== 课程管理 ==========\n");
            print("1. 添加课程\n2. 查看所有课程\n3. 查看课程详情\n4. 查看课程学生\n0. 返回\n");
            print("=============================\n");

            switch (inputInt("请选择: ")) {
                case 1: {
                    string id = inputString("课程ID: ");
                    string name = inputString("名称: ");
                    int cap = inputInt("容量: ");
                    if (service.addCourse(id, name, cap)) {
                        print("添加成功！\n");
                    }
                    pauseScreen();
                    break;
                }
                case 2: {
                    print("\n所有课程:\n");
                    auto courses = service.getAllCourses();
                    for (const auto& [id, name, currCap, maxCap] : courses) {
                        print("{} - {} (容量: {}/{})\n", id, name, currCap, maxCap);
                    }
                    pauseScreen();
                    break;
                }
                case 3: {
                    string id = inputString("课程ID: ");
                    tuple<string, string, int, int, string> detail;
                    if (service.getCourseDetail(id, detail)) {
                        print("\n课程详情:\n");
                        print("ID: {}\n名称: {}\n容量: {}/{}\n状态: {}\n",
                              std::get<0>(detail), std::get<1>(detail),
                              std::get<2>(detail), std::get<3>(detail),
                              std::get<4>(detail));
                    } else {
                        print("课程不存在！\n");
                    }
                    pauseScreen();
                    break;
                }
                case 4: {
                    string cid = inputString("课程ID: ");
                    auto students = service.getCourseStudents(cid);
                    if (students.empty()) {
                        print("无学生选修。\n");
                    } else {
                        print("\n选修学生:\n");
                        for (const auto& [sid, sname] : students) {
                            print("  - {} [{}]\n", sname, sid);
                        }
                    }
                    pauseScreen();
                    break;
                }
                case 0: return;
                default: print("无效选择！\n"); pauseScreen();
            }
        }
    }

    void enrollmentMenu() {
        while (true) {
            clearScreen();
            print("\n========== 选课管理 ==========\n");
            print("1. 学生选课\n2. 学生退课\n3. 查看学生选课\n0. 返回\n");
            print("=============================\n");

            switch (inputInt("请选择: ")) {
                case 1: {
                    string sid = inputString("学号: ");
                    string cid = inputString("课程ID: ");
                    if (service.studentEnroll(sid, cid)) {
                        print("选课成功！\n");
                    } else {
                        print("失败！可能原因：课程已满、已选过、学生或课程不存在。\n");
                    }
                    pauseScreen();
                    break;
                }
                case 2: {
                    string sid = inputString("学号: ");
                    string cid = inputString("课程ID: ");
                    if (service.studentDropCourse(sid, cid)) {
                        print("退课成功！\n");
                    } else {
                        print("失败！可能原因：未选该课程或学生/课程不存在。\n");
                    }
                    pauseScreen();
                    break;
                }
                case 3: {
                    string sid = inputString("学号: ");
                    auto courses = service.getStudentCourses(sid);
                    if (courses.empty()) {
                        print("无选课记录。\n");
                    } else {
                        print("\n选修课程:\n");
                        for (const auto& [cid, cname] : courses) {
                            print("  - {} [{}]\n", cname, cid);
                        }
                    }
                    pauseScreen();
                    break;
                }
                case 0: return;
                default: print("无效选择！\n"); pauseScreen();
            }
        }
    }

    void gradeManagementMenu() {
        while (true) {
            clearScreen();
            print("\n========== 成绩管理 ==========\n");
            print("1. 录入单个成绩\n2. 批量录入成绩\n3. 查看学生成绩\n0. 返回\n");
            print("=============================\n");

            switch (inputInt("请选择: ")) {
                case 1: {
                    string sid = inputString("学号: ");
                    string cid = inputString("课程ID: ");
                    float score = inputFloat("成绩(0-100): ");
                    string tid = inputString("教师ID: ");
                    if (service.inputSingleGrade(sid, cid, score, tid)) {
                        print("录入成功！\n");
                    } else {
                        print("失败！无权教授或成绩无效。\n");
                    }
                    pauseScreen();
                    break;
                }
                case 2: {
                    string tid = inputString("教师ID: ");
                    int n = inputInt("录入条数: ");
                    vector<tuple<string, string, float>> batchData;
                    for (int i = 0; i < n; ++i) {
                        print("\n第{}条:\n", i + 1);
                        string sid = inputString("  学号: ");
                        string cid = inputString("  课程ID: ");
                        float score = inputFloat("  成绩: ");
                        batchData.emplace_back(sid, cid, score);
                    }
                    int cnt = service.batchInputGrades(tid, batchData);
                    print("成功录入{}条记录\n", cnt);
                    pauseScreen();
                    break;
                }
                case 3: {
                    string sid = inputString("学号: ");
                    auto grades = service.getStudentGrades(sid);
                    if (grades.empty()) {
                        print("无成绩记录。\n");
                    } else {
                        print("\n成绩列表:\n");
                        for (const auto& [cid, score, grade] : grades) {
                            print("  {}: {} ({})\n", cid, score, grade);
                        }
                    }
                    pauseScreen();
                    break;
                }
                case 0: return;
                default: print("无效选择！\n"); pauseScreen();
            }
        }
    }

    void scheduleManagementMenu() {
        while (true) {
            clearScreen();
            print("\n========== 排课管理 ==========\n");
            print("1. 排课\n2. 取消排课\n3. 查看排课表\n0. 返回\n");
            print("=============================\n");

            switch (inputInt("请选择: ")) {
                case 1: {
                    string tid = inputString("教师ID: ");
                    string cid = inputString("课程ID: ");
                    string time = inputString("时间(如Mon08:00-09:40): ");
                    string room = inputString("教室: ");
                    if (service.scheduleCourse(tid, cid, time, room)) {
                        print("排课成功！\n");
                    } else {
                        print("失败！时间冲突。\n");
                    }
                    pauseScreen();
                    break;
                }
                case 2: {
                    string tid = inputString("教师ID: ");
                    string cid = inputString("课程ID: ");
                    string time = inputString("时间: ");
                    if (service.cancelSchedule(tid, cid, time)) {
                        print("取消成功！\n");
                    } else {
                        print("失败！记录不存在。\n");
                    }
                    pauseScreen();
                    break;
                }
                case 3:
                    service.printAllSchedules();
                    pauseScreen();
                    break;
                case 0:
                    return;
                default:
                    print("无效选择！\n");
                    pauseScreen();
            }
        }
    }

public:
    // 构造函数：注入业务服务实例（UI层依赖业务服务，而非底层对象）
    ConsoleUIModule(CourseSystemService& s) : service(s) {}

    // 主UI入口
    void runMainUI() {
        while (true) {
            //clearScreen();
            print("\n========== 课程管理系统 ==========\n");
            print("1. 学生管理\n2. 教师管理\n3. 课程管理\n4. 选课管理\n5. 成绩管理\n6. 排课管理\n0. 退出系统\n");
            print("=================================\n");

            int choice = inputInt("请选择: ");
            switch (choice) {
                case 1: studentManagementMenu(); break;
                case 2: teacherManagementMenu(); break;
                case 3: courseManagementMenu(); break;
                case 4: enrollmentMenu(); break;
                case 5: gradeManagementMenu(); break;
                case 6: scheduleManagementMenu(); break;
                case 0:
                    if (confirmAction("确定退出？")) {
                        print("再见！\n");
                        return;
                    }
                    break;
                default: print("无效选择！\n"); pauseScreen();
            }
        }
    }
};

// 步骤3：简化主函数（仅负责模块调度，无业务/UI逻辑）
int main() {
    initDB("dbname=course_system user=postgres password=your_password hostaddr=127.0.0.1 port=5432");
    // 1. 创建业务服务实例（封装所有业务逻辑）
    CourseSystemService courseService;

    // 2. 创建UI模块实例（注入业务服务，解耦UI与业务）
    ConsoleUIModule consoleUI(courseService);

    // 3. 启动UI系统
    consoleUI.runMainUI();
    closeDB();
    return 0;
}

void testTeacherFunctions(CourseSystemService& service) {
    print("\n=== 测试教师功能 ===\n");
    // 测试逻辑可通过service接口实现，保持原有功能
    print("教师功能测试完成！\n");
}




