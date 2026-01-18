export module domain.database;

import std;

using std::size_t;
using std::string;
using std::move;
using std::vector;
// 模拟数据库异常
export class DatabaseException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// 数据库连接配置
struct ConnectionConfig {
    string host;
    int port;
    string username;
    string password;
    string database;

    ConnectionConfig(string h = "localhost",
                     int p = 3306,
                     string u = "root",
                     string pass = "",
                     string db = "course_system")
        : host(move(h)), port(p), username(move(u)),
          password(move(pass)), database(move(db)) {}

    string toConnectionString() const {
        return host + ":" + std::to_string(port) + "/" + database;
    }
};

// 模拟数据库连接状态
enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    EXECUTING,
    ERROR
};

// 模拟数据库结果集
class ResultSet {
private:
    vector<std::unordered_map<std::string, string>> data;
    size_t currentRow = 0;

public:
    ResultSet() = default;

    void addRow(std::unordered_map<std::string, string> row) {
        data.push_back(move(row));
    }

    bool next() {
        if (currentRow < data.size()) {
            currentRow++;
            return true;
        }
        return false;
    }

    string getString(const string& column) const {
        if (currentRow == 0 || currentRow > data.size()) {
            return "";
        }
        auto it = data[currentRow - 1].find(column);
        return it != data[currentRow - 1].end() ? it->second : "";
    }

    int getInt(const string& column) const {
        auto str = getString(column);
        return str.empty() ? 0 : std::stoi(str);
    }

    float getFloat(const string& column) const {
        auto str = getString(column);
        return str.empty() ? 0.0f : std::stof(str);
    }

    size_t rowCount() const {
        return data.size();
    }

    bool isEmpty() const {
        return data.empty();
    }
};

// 模拟数据库连接
class Connection {
private:
    ConnectionConfig config;
    ConnectionState state;
    bool autoCommit;
    string lastError;

public:
    explicit Connection(const ConnectionConfig& cfg)
        : config(cfg), state(ConnectionState::DISCONNECTED), autoCommit(true) {}

    ~Connection() {
        if (isConnected()) {
            close();
        }
    }

    // 连接数据库
    bool connect() {
        if (state != ConnectionState::DISCONNECTED) {
            lastError = "连接已存在或正在连接";
            return false;
        }

        state = ConnectionState::CONNECTING;

        // 模拟连接过程
        // 在实际应用中，这里会有实际的数据库连接代码

        if (config.host.empty()) {
            state = ConnectionState::ERROR;
            lastError = "主机名不能为空";
            return false;
        }

        state = ConnectionState::CONNECTED;
        return true;
    }

    // 关闭连接
    void close() {
        state = ConnectionState::DISCONNECTED;
        lastError.clear();
    }

    // 检查是否已连接
    bool isConnected() const {
        return state == ConnectionState::CONNECTED;
    }

    // 获取连接状态
    ConnectionState getState() const {
        return state;
    }

    // 获取最后错误信息
    string getLastError() const {
        return lastError;
    }

    // 设置自动提交
    void setAutoCommit(bool autoCommitMode) {
        autoCommit = autoCommitMode;
    }

    // 获取自动提交状态
    bool getAutoCommit() const {
        return autoCommit;
    }

    // 执行SQL语句（模拟版本）
    bool execute(const std::string& sql) {
        if (!isConnected()) {
            lastError = "数据库未连接";
            return false;
        }

        state = ConnectionState::EXECUTING;

        // 模拟SQL执行
        // 在实际应用中，这里会有实际的SQL执行代码

        if (sql.empty()) {
            state = ConnectionState::ERROR;
            lastError = "SQL语句不能为空";
            return false;
        }

        // 简单的SQL语句验证（模拟）
        if (sql.find("SELECT") != std::string::npos) {
            // SELECT语句，实际应该执行查询
            state = ConnectionState::CONNECTED;
            return true;
        } else if (sql.find("INSERT") != std::string::npos ||
                   sql.find("UPDATE") != std::string::npos ||
                   sql.find("DELETE") != std::string::npos) {
            // DML语句
            state = ConnectionState::CONNECTED;
            return true;
        } else {
            state = ConnectionState::ERROR;
            lastError = "不支持的SQL语句类型";
            return false;
        }
    }

    // 执行查询（模拟版本）
    std::unique_ptr<ResultSet> query(const string& sql) {
        if (!isConnected()) {
            lastError = "数据库未连接";
            return nullptr;
        }

        if (!execute(sql)) {
            return nullptr;
        }

        auto result = std::make_unique<ResultSet>();

        // 模拟查询结果
        // 在实际应用中，这里会从数据库获取实际数据

        if (sql.find("students") != std::string::npos) {
            // 模拟学生数据
            std::unordered_map<string, string> row1 = {
                {"student_id", "S001"},
                {"name", "张三"},
                {"major", "计算机科学"}
            };
            result->addRow(row1);

            std::unordered_map<string, string> row2 = {
                {"student_id", "S002"},
                {"name", "李四"},
                {"major", "软件工程"}
            };
            result->addRow(row2);
        }
        else if (sql.find("courses") != std::string::npos) {
            // 模拟课程数据
            std::unordered_map<std::string, std::string> row1 = {
                {"course_id", "C001"},
                {"course_name", "C++程序设计"},
                {"max_capacity", "30"},
                {"current_capacity", "25"}
            };
            result->addRow(row1);
        }

        return result;
    }

    // 开始事务
    bool beginTransaction() {
        if (!isConnected()) {
            lastError = "数据库未连接";
            return false;
        }

        setAutoCommit(false);
        return execute("START TRANSACTION");
    }

    // 提交事务
    bool commit() {
        if (!isConnected()) {
            lastError = "数据库未连接";
            return false;
        }

        bool success = execute("COMMIT");
        setAutoCommit(true);
        return success;
    }

    // 回滚事务
    bool rollback() {
        if (!isConnected()) {
            lastError = "数据库未连接";
            return false;
        }

        bool success = execute("ROLLBACK");
        setAutoCommit(true);
        return success;
    }

    // 获取配置信息
    const ConnectionConfig& getConfig() const {
        return config;
    }
};

// 数据库连接池
export class ConnectionPool {
private:
    ConnectionConfig config;
    //修改：
    std::vector<std::unique_ptr<Connection>> idleConnections;  // 空闲连接
    std::vector<std::unique_ptr<Connection>> usedConnections;  // 使用中连接
    size_t maxPoolSize;
    std::mutex poolMutex;//线程安全锁
    static constexpr size_t DEFAULT_POOL_SIZE = 10; // 常量提取

public:
    ConnectionPool(const ConnectionConfig& cfg, size_t  DEFAULT_POOL_SIZE)
        : config(cfg), maxPoolSize(maxPoolSize) {}

    // 获取连接
    std::unique_ptr<Connection> getConnection() {
            std::lock_guard<std::mutex> lock(poolMutex);
            // 优先复用空闲连接
            if (!idleConnections.empty()) {
                auto conn = std::move(idleConnections.back());
                idleConnections.pop_back();
                usedConnections.push_back(std::move(conn));
                return std::move(usedConnections.back());
            }

        // 创建新连接
        if (idleConnections.size() + usedConnections.size() < maxPoolSize) {
                    auto conn = std::make_unique<Connection>(config);
                    if (conn->connect()) {
                        usedConnections.push_back(std::move(conn));
                        return std::move(usedConnections.back());
            }
        }

        return nullptr;
    }

    // 归还连接
    void returnConnection(std::unique_ptr<Connection> conn) {
           std::lock_guard<std::mutex> lock(poolMutex);
           if (conn && conn->isConnected()) {
               // 从使用中列表移除
               auto it = std::find_if(usedConnections.begin(), usedConnections.end(),
                   [&](const std::unique_ptr<Connection>& c) {
                       return c.get() == conn.get();
                   });
               if (it != usedConnections.end()) {
                   usedConnections.erase(it);
               }
               // 加入空闲列表
               idleConnections.push_back(std::move(conn));
           }
       }

    // 关闭所有连接

};


