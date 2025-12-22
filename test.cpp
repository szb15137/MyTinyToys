#include "mmpool.h"
#include <string>
#include <iostream>

class Student
{
private:
    /* data */
    char grade;
    int age;
public:
    Student(char grade_, int age_)
        : grade(grade_), age(age_){};
    ~Student() = default;
    void set_grade(char g) { grade = g; };
    char get_grade(){ return grade; };
};

int main(){
    MyMmPool pool(5, sizeof(Student));
    void* ptr1 = pool.allocate();
    std::cout << "Pool Memory Address: " << ptr1 << std::endl;
    Student* student1 = new (ptr1) Student('A', 20);
    std::cout << "Student Address: " << student1 << std::endl;
}
