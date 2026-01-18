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
//         * 初步实现了ui，但还没有分离模块
import std;
import domain.student;
import domain.course;
import domain.score;
import domain.teacher;
import domain.registrar;
import domain.secretary;
import domain.person;  //用于多态实现，统一人员属性接口

using std::print;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;

// 全局注册员实例，统一管理这些核心数据
Registrar registrar;

// 函数声明
void showMainMenu();  //显示系统主菜单
void studentManagementMenu(); //学生管理子菜单
void teacherManagementMenu(); //教师管理子菜单
void courseManagementMenu(); //课程管理子菜单
void enrollmentMenu(); //选课管理子菜单
void gradeManagementMenu(); //成绩管理子菜单
void scheduleManagementMenu(); //排课管理子菜单
void clearScreen(); //清频函数
void pauseScreen(); //暂停函数
string inputString(const string& prompt); //安全获取字符串输入
int inputInt(const string& prompt);
float inputFloat(const string& prompt);
bool confirmAction(const string& prompt);//操作确认函数

// 修正：testTeacherFunctions不使用参数，改为使用全局registrar
void testTeacherFunctions() {
    print("\n=== 测试教师功能 ===\n");
    auto it1 = registrar.getAllTeachers().find("T001");
    auto it2 = registrar.getAllTeachers().find("T002");
    if (it1 == registrar.getAllTeachers().end() || it2 == registrar.getAllTeachers().end()) {
        print("错误：教师不存在！\n");
        return;
    }
    //获取教师对象指针，用于后续调用
    auto* teacher1 = it1->second.get();
    auto* teacher2 = it2->second.get();
    //打印教师基本信息
    print("1. 教师信息:\n");
    print("   教师1: {} (ID: {}, 部门: {})\n", teacher1->getName(), teacher1->getTeacherId(), teacher1->getDepartment());
    print("   教师2: {} (ID: {}, 部门: {})\n", teacher2->getName(), teacher2->getTeacherId(), teacher2->getDepartment());
    //测试添加教授课程并打印结果
    print("\n2. 教授课程:\n");
    teacher1->addTeachingCourse("C001");
    teacher1->addTeachingCourse("C002");
    teacher2->addTeachingCourse("C003");
    print("   王教授课程: ");
    for (const auto& cid : teacher1->getTeachingCourses()) print("{} ", cid);
    print("\n");
    //测试单个成绩录入功能
    print("\n3. 成绩录入:\n");
    auto score = teacher1->inputScore("S001", "C001", 88.5f);
    if (score) print("   ✓ 录入成绩: {} 等级: {}\n", score->getScore(), score->getGrade());
    // 测试无权限课程的成绩录入
    score = teacher1->inputScore("S001", "C003", 92.0f);
    if (!score) print("   ✓ 正确拒绝无权限录入\n");
    //测试批量成绩录入功能
    print("\n4. 批量录入:\n");
    vector<std::tuple<string, string, float>> batchData = {
        {"S001", "C001", 85.0f},
        {"S002", "C001", 92.0f},
        {"S003", "C002", 78.5f}
    };
    auto scores = teacher1->batchInputScores(batchData);
    print("   成功录入 {} 条记录\n", scores.size());
    //测试课程成绩统计功能
    print("\n5. 课程统计:\n");
    try {
        auto stats = teacher1->getCourseStatistics("C001");
        print("   学生数: {} 平均分: {} 最高分: {} 最低分: {}\n",
              stats.studentCount, stats.averageScore, stats.maxScore, stats.minScore);
    } catch (const std::exception& e) {
        print("   错误: {}\n", e.what());
    }
}
//主函数
int main() {
    print("课程管理系统 - 主菜单\n");
    print("====================\n\n");

    // 初始化测试数据
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
        //捕获初始化过程中的异常，避免程序崩溃
        cerr << "初始化失败: " << e.what() << "\n";
    }

    // 主循环
    while (true) {
        showMainMenu();
        int choice = inputInt("请选择: ");
        //根据选择跳转对应功能的子菜单
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
                    return 0;
                }
                break;
            default: print("无效选择！\n"); pauseScreen();
        }
    }
    return 0;
}

// 主菜单
void showMainMenu() {
    clearScreen();
    print("\n========== 课程管理系统 ==========\n");
    print("1. 学生管理\n2. 教师管理\n3. 课程管理\n4. 选课管理\n5. 成绩管理\n6. 排课管理\n0. 退出系统\n");
    print("=================================\n");
}

// 学生管理
void studentManagementMenu() {
    while (true) {
        clearScreen();
        print("\n========== 学生管理 ==========\n");
        print("1. 添加学生\n2. 查看所有学生\n3. 查看学生详情\n4. 查看学生选课\n0. 返回\n");
        print("=============================\n");

        switch (inputInt("请选择: ")) {
            case 1: {
                //添加新学生
                string id = inputString("学号: ");
                string name = inputString("姓名: ");
                string major = inputString("专业: ");
                int age = inputInt("年龄: ");
                string gender = inputString("性别: ");
                try {
                    //调用注册员接口添加学生
                    registrar.addStudent(make_unique<Student>(id, name, major, age, gender));
                    print("添加成功！\n");
                } catch (const std::exception& e) { cerr << "失败: " << e.what() << "\n"; }
                pauseScreen();
                break;
            }
            case 2: {
                //查看所有学生
                print("\n所有学生:\n");
                for (const auto& [id, s] : registrar.getAllStudents()) {
                    print("{} - {} ({}岁, {})\n", id, s->getName(), s->getAge(), s->getGender());
                }
                pauseScreen();
                break;
            }
            case 3: {
                //查看单个学生
                string id = inputString("学号: ");
                if (!registrar.printStudentRecord(id)) cerr << "学生不存在！\n";
                pauseScreen();
                break;
            }
            case 4: {
                //查看学生选课记录
                string id = inputString("学号: ");
                auto courses = registrar.getStudentCourses(id);
                if (courses.empty()) print("无选课记录。\n");
                else {
                    print("\n选修课程:\n");
                    for (const auto* c : courses) print("  - {} [{}]\n", c->getCourseName(), c->getCourseId());
                }
                pauseScreen();
                break;
            }
            case 0: return; //返回上一级菜单（主菜单）
            default: print("无效选择！\n"); pauseScreen();
        }
    }
}

// 教师管理
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
                try {
                    registrar.addTeacher(make_unique<Teacher>(id, name, dept, age, gender));
                    print("添加成功！\n");
                } catch (const std::exception& e) { cerr << "失败: " << e.what() << "\n"; }
                pauseScreen();
                break;
            }
            case 2: {
                print("\n所有教师:\n");
                for (const auto& [id, t] : registrar.getAllTeachers()) {
                    print("{} - {} ({})\n", id, t->getName(), t->getDepartment());
                }
                pauseScreen();
                break;
            }
            case 3: {
                //为教师添加教授课程
                string tid = inputString("教师ID: ");
                string cid = inputString("课程ID: ");
                auto it = registrar.getAllTeachers().find(tid);
                if (it == registrar.getAllTeachers().end()) print("教师不存在！\n");
                else {
                    if (it->second->addTeachingCourse(cid)) print("添加成功！\n");
                    else print("已存在该课程。\n");
                }
                pauseScreen();
                break;
            }
            case 4: {
                //查看教师教授课程
                string tid = inputString("教师ID: ");
                auto it = registrar.getAllTeachers().find(tid);
                if (it == registrar.getAllTeachers().end()) print("教师不存在！\n");
                else {
                    print("\n教授课程:\n");
                    for (const auto& cid : it->second->getTeachingCourses()) print("  - {}\n", cid);
                }
                pauseScreen();
                break;
            }
            case 0: return;
            default: print("无效选择！\n"); pauseScreen();
        }
    }
}

// 课程管理
void courseManagementMenu() {
    while (true) {
        clearScreen();
        print("\n========== 课程管理 ==========\n");
        print("1. 添加课程\n2. 查看所有课程\n3. 查看课程详情\n4. 查看课程学生\n0. 返回\n");
        print("=============================\n");

        switch (inputInt("请选择: ")) {
            case 1: {
                //添加新课程
                string id = inputString("课程ID: ");
                string name = inputString("名称: ");
                int cap = inputInt("容量: ");
                try {
                    //调用注册员接口添加课程
                    registrar.addCourse(make_unique<Course>(id, name, cap));
                    print("添加成功！\n");
                } catch (const std::exception& e) { cerr << "失败: " << e.what() << "\n"; }
                pauseScreen();
                break;
            }
            case 2: {
                //查看所有课程
                print("\n所有课程:\n");
                for (const auto& [id, c] : registrar.getAllCourses()) {
                    print("{} - {} (容量: {}/{})\n", id, c->getCourseName(),
                          c->getCurrentCapacity(), c->getMaxCapacity());
                }
                pauseScreen();
                break;
            }
            case 3: {
                //查看单个课程情况
                string id = inputString("课程ID: ");
                auto it = registrar.getAllCourses().find(id);
                if (it == registrar.getAllCourses().end()) print("课程不存在！\n");
                else {
                    const auto& c = it->second;
                    print("\n课程详情:\n");
                    print("ID: {}\n名称: {}\n容量: {}/{}\n状态: {}\n",
                          c->getCourseId(), c->getCourseName(),
                          c->getCurrentCapacity(), c->getMaxCapacity(),
                          c->isFull() ? "已满" : "未满");
                }
                pauseScreen();
                break;
            }
            case 4: {
                //查看课程选课学生
                string cid = inputString("课程ID: ");
                auto students = registrar.getCourseStudents(cid);
                if (students.empty()) print("无学生选修。\n");
                else {
                    print("\n选修学生:\n");
                    for (const auto* s : students) print("  - {} [{}]\n", s->getName(), s->getStudentId());
                }
                pauseScreen();
                break;
            }
            case 0: return;
            default: print("无效选择！\n"); pauseScreen();
        }
    }
}
// 选课管理
void enrollmentMenu() {
    while (true) {
        clearScreen();
        print("\n========== 选课管理 ==========\n");
        print("1. 学生选课\n2. 学生退课\n3. 查看学生选课\n0. 返回\n");
        print("=============================\n");

        switch (inputInt("请选择: ")) {
            case 1: {
                //学生选课
                string sid = inputString("学号: ");
                string cid = inputString("课程ID: ");
                if (registrar.registerCourse(sid, cid)) print("选课成功！\n");
                else print("失败！可能原因：课程已满、已选过、学生或课程不存在。\n");
                pauseScreen();
                break;
            }
            case 2: {
                //学生退课
                string sid = inputString("学号: ");
                string cid = inputString("课程ID: ");
                if (registrar.dropCourse(sid, cid)) print("退课成功！\n");
                else print("失败！可能原因：未选该课程或学生/课程不存在。\n");
                pauseScreen();
                break;
            }
            case 3: {
                //查看学生选课记录
                string sid = inputString("学号: ");
                auto courses = registrar.getStudentCourses(sid);
                if (courses.empty()) print("无选课记录。\n");
                else {
                    print("\n选修课程:\n");
                    for (const auto* c : courses) print("  - {} [{}]\n", c->getCourseName(), c->getCourseId());
                }
                pauseScreen();
                break;
            }
            case 0: return;
            default: print("无效选择！\n"); pauseScreen();
        }
    }
}

// 成绩管理
void gradeManagementMenu() {
    while (true) {
        clearScreen();
        print("\n========== 成绩管理 ==========\n");
        print("1. 录入单个成绩\n2. 批量录入成绩\n3. 查看学生成绩\n0. 返回\n");
        print("=============================\n");

        switch (inputInt("请选择: ")) {
            case 1: {
                //录入单个成绩
                string sid = inputString("学号: ");
                string cid = inputString("课程ID: ");
                float score = inputFloat("成绩(0-100): ");
                string tid = inputString("教师ID: ");
                auto it = registrar.getAllTeachers().find(tid);
                if (it == registrar.getAllTeachers().end()) print("教师不存在！\n");
                else {
                    if (auto s = it->second->inputScore(sid, cid, score))
                        print("录入成功！等级: {}\n", s->getGrade());
                    else
                        print("失败！无权教授或成绩无效。\n");
                }
                pauseScreen();
                break;
            }
            case 2: {
                //批量录入成绩
                string tid = inputString("教师ID: ");
                auto it = registrar.getAllTeachers().find(tid);
                if (it == registrar.getAllTeachers().end()) { print("教师不存在！\n"); pauseScreen(); break; }
                int n = inputInt("录入条数: ");
                vector<std::tuple<string, string, float>> data;
                for (int i = 0; i < n; ++i) {
                    print("\n第{}条:\n", i+1);
                    data.push_back({inputString("  学号: "), inputString("  课程ID: "), inputFloat("  成绩: ")});
                }
                auto cnt = it->second->batchInputScores(data);
                print("成功录入{}条记录\n", cnt.size());
                pauseScreen();
                break;
            }
            case 3: {
                //查看学生成绩
                string sid = inputString("学号: ");
                auto it = registrar.getAllStudents().find(sid);
                if (it == registrar.getAllStudents().end()) print("学生不存在！\n");
                else {
                    auto scores = it->second->queryScore();
                    if (scores.empty()) print("无成绩记录。\n");
                    else {
                        print("\n成绩列表:\n");
                        for (const auto& s : scores) print("  {}: {} ({})\n", s.getCourseId(), s.getScore(), s.getGrade());
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

// 排课管理
void scheduleManagementMenu() {
    //创建秘书实例
    Secretary secretary("SEC001", "赵秘书", 30, "女");
    while (true) {
        clearScreen();
        print("\n========== 排课管理 ==========\n");
        print("1. 排课\n2. 取消排课\n3. 查看排课表\n0. 返回\n");
        print("=============================\n");

        switch (inputInt("请选择: ")) {
            case 1: {
                //排课操作
                string tid = inputString("教师ID: ");
                string cid = inputString("课程ID: ");
                string time = inputString("时间(如Mon08:00-09:40): ");
                string room = inputString("教室: ");
                if (secretary.scheduleCourse(tid, cid, time, room)) print("排课成功！\n");
                else print("失败！时间冲突。\n");
                pauseScreen();
                break;
            }
            case 2: {
                //取消排课
                string tid = inputString("教师ID: ");
                string cid = inputString("课程ID: ");
                string time = inputString("时间: ");
                if (secretary.cancelSchedule(tid, cid, time)) print("取消成功！\n");
                else print("失败！记录不存在。\n");
                pauseScreen();
                break;
            }
            case 3:
                secretary.printSchedule();
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

// 辅助函数
//清屏
void clearScreen() { std::cout << "\033[2J\033[1;1H"; }
//暂停屏幕
void pauseScreen() {
    print("\n按回车键继续...");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
//安全获取字符串输入，带提示信息
string inputString(const string& prompt) {
    print("{}", prompt);
    string s; cin >> s; return s;
}
//安全获取整数输入，带格式校验
int inputInt(const string& prompt) {
    while (true) {
        print("{}", prompt);
        int v; if (cin >> v) { cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return v; }
        cin.clear(); cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        print("错误：请输入整数。\n");
    }
}
//安全获取浮点型输入，带格式校验
float inputFloat(const string& prompt) {
    while (true) {
        print("{}", prompt);
        float v; if (cin >> v) { cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return v; }
        cin.clear(); cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        print("错误：请输入数字。\n");
    }
}
//操作确认，返回用户是否确认
bool confirmAction(const string& prompt) {
    print("{} (y/n): ", prompt);
    char c; cin >> c; cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return c=='y' || c=='Y';
}
