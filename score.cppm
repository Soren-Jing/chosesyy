
// File: score.cppm
// Created:Lufengying 2024051604030 1276809756@qq.com        2026-01-17
// Version: 1.0      License: AGPLv3
export module registrar.score;
import std;
using std::string;
using std::move;
using std::invalid_argument;

export class Score {
private:
    string studentId;
    string courseId;
    float score;

public:
    // 构造函数：增强成绩范围校验
    Score(string sid, string cid, float s)
        : studentId(move(sid)), courseId(move(cid)), score(s) {
        if (s < 0 || s > 100) {
            throw invalid_argument("成绩必须在0-100之间");
        }
    }

    // 获取学生ID
    string getStudentId() const { return studentId; }
    // 获取课程ID
    string getCourseId() const { return courseId; }
    // 获取成绩
    float getScore() const { return score; }

    // 设置成绩：增强校验
    void setScore(float s) {
        if (s < 0 || s > 100) {
            throw invalid_argument("成绩必须在0-100之间");
        }
        score = s;
    }

    // 获取成绩等级：优化逻辑（修复原代码语法错误）
    string getGrade() const {
        if (score >= 90) return "A";
        if (score >= 80) return "B";
        if (score >= 70) return "C";
        if (score >= 60) return "D";
        return "F";
    }
};
