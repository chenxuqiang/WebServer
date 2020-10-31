#ifndef STUDENT_H
#define STUDENT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE  1024

struct Student {
    char name[20];
    int  id;
};

struct StudentTable {
    Student stu[TABLE_SIZE];
    int size;
};

struct QueryResponse {
    int rval;
    char name[128];
};

struct QueryRequest {
    int stuId;
    struct QueryResponse rep;
};

struct StudentTable* InitStudentTable()
{
    struct StudentTable *table = (struct StudentTable *)malloc(sizeof(struct StudentTable));
    if (table == NULL) {
        perror("malloc");
        return NULL;
    }

    strcpy(table->stu[0].name, "Alice");
    table->stu[0].id = 1;

    strcpy(table->stu[1].name, "Bob");
    table->stu[1].id = 2;

    strcpy(table->stu[2].name, "Cindy");
    table->stu[2].id = 3;

    table->size = 3;

    return table;
}

void QueryStudentTable(struct StudentTable *table, struct QueryRequest* req)
{
    for (int i = 0; i < table->size; i++) {
        if (req->stuId == table->stu[i].id) {
            strcpy(req->rep.name, table->stu[i].name);
            req->rep.rval = 1;
            return ;
        }
    }

    req->rep.rval = 0;
}

#endif