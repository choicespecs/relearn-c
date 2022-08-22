#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>
#include<pthread.h>

bool isInteger(const char* string) {

    while(*string != '\0') {
        if (!isdigit(*string)) {
            return false;
        }
        string++;
    }
    return true;
}

int main(int argc, char* argv[]) {
    int students = 0;
    if (argc != 2) {
        printf("only define # of students as argument\n");
        exit(EXIT_FAILURE);
    }
    if (!isInteger(argv[1])) {
        printf("not a valid value for students\n");
        exit(EXIT_FAILURE);
    }
    students = atoi(argv[1]);
    int student_id[students];
    printf("Created %i students\n", students);
    for (int i = 0; i < students; i++)
        student_id[i] = i + 1;

    for (int i = 0; i < students; i++) 
        printf("student: %i | id: %i\n", i, student_id[i]);
}
