export module domain.score;

import std;

using std::string;
using std::move;
using std::vector;
using std::move;
export class Score {
private:
    string studentId;
    string courseId;
    float score;

public:
    Score(string sid, string cid, float s)
        : studentId(move(sid)), courseId(move(cid)), score(s) {
        if (score < 0 || score > 100) {
            throw std::invalid_argument("成绩必须在0-100之间");
        }
    }

    // 获取学生ID
    string getStudentId() const { return studentId; }

    // 获取课程ID
    string getCourseId() const { return courseId; }

    // 获取成绩
    float getScore() const { return score; }

    // 设置成绩
    void setScore(float s) {
        if (s < 0 || s > 100) {
            throw std::invalid_argument("成绩必须在0-100之间");
        }
        score = s;
    }

    // 获取成绩等级
    string getGrade() const {
        if (score >= 90) return "A";
        if (score >= 80) return "B";
        if (score >= 70) return "C";
        if (score >= 60) return "D";
        return "F";
    }
};


