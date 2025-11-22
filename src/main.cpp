#include "../include/my_memory_resource.hpp"
#include "../include/queue.hpp"
#include <iostream>
#include <string>

struct Student {
    int id;
    std::string surname;
    int labs_count;

    Student(int i = 0, std::string s = "", int l = 0) : id(i), surname(s), labs_count(l) {}

    friend std::ostream &operator<<(std::ostream &os, const Student &stud) {
        os << "Студент №" << stud.id << " " << stud.surname << " (лабы: " << stud.labs_count << ")";
        return os;
    }
};

int main() {
    MyMemoryResource resource;

    auto student_queue = Queue<Student, std::pmr::polymorphic_allocator<Student>>(std::pmr::polymorphic_allocator<Student>{&resource});

    student_queue.push(Student(1, "Иванов", 5));
    student_queue.push(Student(2, "Петров", 3));
    student_queue.push(Student(3, "Сидоров", 7));

    std::cout << "Очередь студентов на проверку лаб:\n";
    std::cout << "Всего в очереди: " << student_queue.get_size() << " студентов\n\n";
    std::cout << "Первый в очереди: " << student_queue.front() << std::endl;
    std::cout << "Последний в очереди: " << student_queue.back() << std::endl;

    std::cout << "\nОбход очереди через итератор:\n";
    int position = 1;
    for (const auto& student : student_queue) {
        std::cout << position << ". " << student << std::endl;
        position++;
    }
    
    while (!student_queue.is_empty()) {
        Student current = student_queue.front();
        std::cout << "Проверяем: " << current << std::endl;
        student_queue.pop();
        std::cout << "Осталось в очереди: " << student_queue.get_size() << " студентов\n";
        
        if (!student_queue.is_empty()) {
            std::cout << "Следующий: " << student_queue.front() << "\n" << std::endl;
        }
    }

    std::cout << "Все студенты проверены.\n";

    return 0;
}
