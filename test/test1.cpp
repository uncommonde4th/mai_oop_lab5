#include <gtest/gtest.h>
#include "../include/my_memory_resource.hpp"
#include "../include/queue.hpp"
#include <string>

struct Student {
    int id;
    std::string surname;
    int labs_count;

    Student(int i = 0, std::string s = "", int l = 0) 
        : id(i), surname(s), labs_count(l) {}

    bool operator==(const Student& other) const {
        return id == other.id && surname == other.surname && labs_count == other.labs_count;
    }
};

TEST(StudentQueueTest, BasicOperations) {
    MyMemoryResource resource;
    auto queue = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    EXPECT_TRUE(queue.is_empty());
    EXPECT_EQ(queue.get_size(), 0);
    
    queue.push(Student(1, "Иванов", 5));
    queue.push(Student(2, "Петров", 3));
    
    EXPECT_FALSE(queue.is_empty());
    EXPECT_EQ(queue.get_size(), 2);
}

TEST(StudentQueueTest, PushAndPop) {
    MyMemoryResource resource;
    auto queue = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    Student student1(1, "Иванов", 5);
    Student student2(2, "Петров", 3);
    
    queue.push(student1);
    queue.push(student2);
    
    EXPECT_EQ(queue.front(), student1);
    EXPECT_EQ(queue.back(), student2);
    
    queue.pop();
    EXPECT_EQ(queue.front(), student2);
    EXPECT_EQ(queue.get_size(), 1);
    
    queue.pop();
    EXPECT_TRUE(queue.is_empty());
}

TEST(StudentQueueTest, FrontAndBack) {
    MyMemoryResource resource;
    auto queue = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    queue.push(Student(1, "Иванов", 5));
    queue.push(Student(2, "Петров", 3));
    queue.push(Student(3, "Сидоров", 7));
    
    EXPECT_EQ(queue.front().id, 1);
    EXPECT_EQ(queue.front().surname, "Иванов");
    EXPECT_EQ(queue.front().labs_count, 5);
    
    EXPECT_EQ(queue.back().id, 3);
    EXPECT_EQ(queue.back().surname, "Сидоров");
    EXPECT_EQ(queue.back().labs_count, 7);
}

TEST(StudentQueueTest, IteratorFunctionality) {
    MyMemoryResource resource;
    auto queue = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    queue.push(Student(1, "Иванов", 5));
    queue.push(Student(2, "Петров", 3));
    queue.push(Student(3, "Сидоров", 7));
    
    auto it = queue.begin();
    auto end_it = queue.end();
    EXPECT_EQ(it->id, 1);
    EXPECT_EQ(it->surname, "Иванов");
    ++it;
    EXPECT_EQ(it->id, 2);
    EXPECT_EQ(it->surname, "Петров");
    it++;
    EXPECT_EQ(it->id, 3);
    EXPECT_EQ(it->surname, "Сидоров");
    ++it;
    EXPECT_TRUE(it == end_it);
}

TEST(StudentQueueTest, RangeBasedForLoop) {
    MyMemoryResource resource;
    auto queue = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    queue.push(Student(1, "Иванов", 5));
    queue.push(Student(2, "Петров", 3));
    queue.push(Student(3, "Сидоров", 7));
    
    EXPECT_EQ(queue.get_size(), 3);

    int count = 0;
    for (const auto& student : queue) {
        EXPECT_TRUE(student.id >= 1 && student.id <= 3);
        EXPECT_FALSE(student.surname.empty());
        EXPECT_GE(student.labs_count, 0);
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(StudentQueueTest, ClearOperation) {
    MyMemoryResource resource;
    auto queue = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    queue.push(Student(1, "Иванов", 5));
    queue.push(Student(2, "Петров", 3));
    queue.push(Student(3, "Сидоров", 7));
    
    EXPECT_EQ(queue.get_size(), 3);
    EXPECT_FALSE(queue.is_empty());
    
    queue.clear();
    
    EXPECT_EQ(queue.get_size(), 0);
    EXPECT_TRUE(queue.is_empty());
}

TEST(StudentQueueTest, MoveSemantics) {
    MyMemoryResource resource;
    auto queue = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    Student temp(1, "Иванов", 5);
    queue.push(std::move(temp));
    
    EXPECT_EQ(queue.front().id, 1);
    EXPECT_EQ(queue.front().surname, "Иванов");
    EXPECT_EQ(queue.front().labs_count, 5);
}

TEST(StudentQueueTest, MultipleQueuesSameResource) {
    MyMemoryResource resource;
 
    auto queue1 = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    auto queue2 = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    queue1.push(Student(1, "Иванов", 5));
    queue2.push(Student(2, "Петров", 3));
    
    EXPECT_EQ(queue1.front().id, 1);
    EXPECT_EQ(queue2.front().id, 2);
    EXPECT_EQ(queue1.get_size(), 1);
    EXPECT_EQ(queue2.get_size(), 1);
}

TEST(StudentQueueTest, EmptyQueueBehavior) {
    MyMemoryResource resource;
    auto queue = Queue<Student, std::pmr::polymorphic_allocator<Student>>(
        std::pmr::polymorphic_allocator<Student>{&resource});
    
    EXPECT_TRUE(queue.is_empty());
    EXPECT_EQ(queue.get_size(), 0);
    
    EXPECT_EQ(queue.begin(), queue.end());
}

TEST(StudentQueueTest, StudentComparison) {
    Student student1(1, "Иванов", 5);
    Student student2(1, "Иванов", 5);
    Student student3(2, "Петров", 3);
    
    EXPECT_EQ(student1, student2);
    EXPECT_FALSE(student1 == student3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
