export module domain.secretary;

import std;
import domain.student;
import domain.course;
import domain.teacher;

// 排课表条目
struct ScheduleEntry {
    std::string teacherId;
    std::string courseId;
    std::string timeSlot;          // 如 "Mon08:00-09:40"
    std::string classroom;
};

export class Secretary : public Student, public Course {
public:
    // 构造函数：给 Student 和 Course 部分传 dummy 值
    Secretary(const std::string& secId, const std::string& secName)
        : Student(secId, secName, "教学秘书"),
          Course("SEC-DUMMY", "SecretaryDummy", 999) {}
    bool scheduleCourse(const std::string& teacherId,
                        const std::string& courseId,
                        const std::string& timeSlot,
                        const std::string& classroom);//核心排课接口

    bool cancelSchedule(const std::string& teacherId,
                        const std::string& courseId,
                        const std::string& timeSlot);

    void printSchedule() const;

private:
    std::vector<ScheduleEntry> schedule;                 //简单顺序表
    bool timeConflict(const std::string& teacherId,
                      const std::string& timeSlot) const;//工具：检查教师在该时间段是否已有排课
};

bool Secretary::timeConflict(const std::string& teacherId,
                             const std::string& timeSlot) const {
    for (const auto& e : schedule)
        if (e.teacherId == teacherId && e.timeSlot == timeSlot)
            return true;
    return false;
}

bool Secretary::scheduleCourse(const std::string& teacherId,
                               const std::string& courseId,
                               const std::string& timeSlot,
                               const std::string& classroom) {
    if (timeConflict(teacherId, timeSlot)) return false;
    schedule.push_back({teacherId, courseId, timeSlot, classroom});
    return true;
}

bool Secretary::cancelSchedule(const std::string& teacherId,
                               const std::string& courseId,
                               const std::string& timeSlot) {
    auto it = std::find_if(schedule.begin(), schedule.end(),
                           [&](const ScheduleEntry& e) {
                               return e.teacherId == teacherId
                                   && e.courseId == courseId
                                   && e.timeSlot == timeSlot;
                           });
    if (it == schedule.end()) return false;
    schedule.erase(it);
    return true;
}

void Secretary::printSchedule() const {
    std::println("\n======== 教学秘书排课表 ========");
    if (schedule.empty()) {
        std::println("  当前无排课记录。");
    } else {
        for (std::size_t i = 0; i < schedule.size(); ++i) {
            const auto& e = schedule[i];
            std::println("{:02d}. 教师={} 课程={} 时间={} 教室={}",
                         i + 1, e.teacherId, e.courseId, e.timeSlot, e.classroom);
        }
    }
    std::println("================================");
}
