module;
#include <libpq-fe.h>
export module database;

import std;



static PGconn* db_conn = nullptr;//静态全局变量
const char* connInfo = "dbname=course_system user=postgres password=your_password hostaddr=127.0.0.1 port=5432";


// 初始化数据库连接并创建选课表
export bool initDB(const char* connInfo) {
    // 建立数据库连接
    db_conn = PQconnectdb(connInfo);
    if (PQstatus(db_conn) != CONNECTION_OK) {
        std::cerr << "数据库连接失败: " << PQerrorMessage(db_conn) << std::endl;
        PQfinish(db_conn);
        db_conn = nullptr;
        return false;
    } else {
        std::print("ok");
    }

    // 创建选课记录表（仅存储选课退课记录）
    const char* createEnrollTable = R"(
        CREATE TABLE IF NOT EXISTS student_course (
            student_id VARCHAR(20) NOT NULL,
            course_id VARCHAR(20) NOT NULL,
            enroll_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
            PRIMARY KEY (student_id, course_id) -- 防止重复选课
        )
    )";
    PGresult* res = PQexec(db_conn, createEnrollTable);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "创建选课表失败: " << PQerrorMessage(db_conn) << std::endl;
        PQclear(res);
        PQfinish(db_conn);
        db_conn = nullptr;
        return false;
    }
    PQclear(res);
    std::cout << "数据库初始化成功！" << std::endl;
    return true;
}

// 添加选课记录到数据库
export bool addEnrollmentToDB(const std::string& student_id, const std::string& course_id) {
    if (!db_conn) return false;
    std::string sql = "INSERT INTO student_course (student_id, course_id) VALUES ('"
                      + student_id + "', '" + course_id + "') ON CONFLICT DO NOTHING";
    PGresult* res = PQexec(db_conn, sql.c_str());
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    if (!success) {
        std::cerr << "添加选课记录失败: " << PQerrorMessage(db_conn) << std::endl;
    }
    PQclear(res);
    return success;
}

// 从数据库删除退课记录
export bool removeEnrollmentFromDB(const std::string& student_id,
                                   const std::string& course_id) {
    if (!db_conn) return false;

    //1先查询确认存在
    const char* selectSql = "SELECT 1 FROM student_course "
                            "WHERE student_id = $1 AND course_id = $2";
    const char* values[2] = { student_id.c_str(), course_id.c_str() };

    PGresult* selRes = PQexecParams(db_conn, selectSql, 2, nullptr, values,
                                    nullptr, nullptr, 0);
    bool exists = (PQresultStatus(selRes) == PGRES_TUPLES_OK &&
                   PQntuples(selRes) > 0);
    PQclear(selRes);
    if (!exists) return false;   //没有这条记录，直接返回

    //执行删除
    const char* deleteSql = "DELETE FROM student_course "
                            "WHERE student_id = $1 AND course_id = $2";
    PGresult* delRes = PQexecParams(db_conn, deleteSql, 2, nullptr, values,
                                    nullptr, nullptr, 0);
    bool ok = (PQresultStatus(delRes) == PGRES_COMMAND_OK);
    if (!ok) std::cerr << "删除失败: " << PQerrorMessage(db_conn) << std::endl;
    PQclear(delRes);
    return ok;
}

export void closeDB() {
    if (db_conn) {                                     // 非空才释放
        PQfinish(db_conn);
        db_conn = nullptr;
    }
}
