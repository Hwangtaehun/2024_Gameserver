#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

typedef struct {
    short id;
    short salary;
    char name[50];
} Employee;

void serialize_employee(Employee* emp, unsigned char* buffer) {
    memcpy(buffer, &emp->id, sizeof(short));
    memcpy(buffer + sizeof(short), &emp->salary, sizeof(short));
    memcpy(buffer + sizeof(short) + sizeof(short), emp->name, 50);
}

void deserialize_employee(unsigned char* buffer, Employee* emp) {
    memcpy(&emp->id, buffer, sizeof(short));
    memcpy(&emp->salary, buffer + sizeof(short), sizeof(short));
    memcpy(emp->name, buffer + sizeof(short) + sizeof(short), 50);
}

//int main() {
//    Employee emp1 = { 2, 2500, "John Doe" };
//    unsigned char buffer[sizeof(int) + 50 + sizeof(float)];
//
//    // 직렬화
//    serialize_employee(&emp1, buffer);
//
//    // 역직렬화
//    Employee emp2;
//    deserialize_employee(buffer, &emp2);
//
//    printf("ID: %hd, Name: %s, Salary: %hd\n", emp2.id, emp2.name, emp2.salary);
//
//    return 0;
//}

int main() {
    char str[] = "127.0.0.1,100.0f,100.0f,100.0f";
    char* sep = strtok(str,",");
    printf("%s\n", sep);

    return 0;
}