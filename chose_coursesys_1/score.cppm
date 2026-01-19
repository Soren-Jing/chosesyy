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
    Score(string sid, string cid, float s);//
    string getStudentId() const;           //获取学生ID
    string getCourseId() const;            //获取课程ID
    float getScore() const { return score; }//获取成绩
    void setScore(float s);                 // 设置成绩
    string getGrade() const;
};

//获取成绩等级
string Score::getGrade() const {
    if (score >= 90) return "A";
    if (score >= 80) return "B";
    if (score >= 70) return "C";
    if (score >= 60) return "D";
    return "F";
}

//设置成绩
void Score::setScore(float s) {
    if (s < 0 || s > 100) {
        throw std::invalid_argument("成绩必须在0-100之间");
    }
    score = s;
}

//获取课程ID
string Score::getCourseId() const { return courseId; }

//获取学生ID
string Score::getStudentId() const { return studentId; }
Score::Score(string sid, string cid, float s)
    : studentId(move(sid)), courseId(move(cid)), score(s) {
    if (score < 0 || score > 100) {
        throw std::invalid_argument("成绩必须在0-100之间");
    }
}
