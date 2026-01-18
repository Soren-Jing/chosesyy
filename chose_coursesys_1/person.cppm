export module domain.person;

import std;

export class Person {
private:
    std::string id;
    std::string name;
    int age;
    std::string gender;

public:
    Person(std::string id, std::string name, int age = 18, std::string gender = "未知");
    virtual ~Person() = default;  // 虚析构函数支持多态

    // Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getGender() const { return gender; }

    // Setters
    void setName(const std::string& n) { name = n; }
    void setAge(int a) { age = a; }
    void setGender(const std::string& g) { gender = g; }
};

Person::Person(std::string id, std::string name, int age, std::string gender)
    : id(std::move(id)), name(std::move(name)), age(age), gender(std::move(gender)) {}
