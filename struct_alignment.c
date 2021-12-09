#include <stdio.h>

typedef struct student
{
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} Student;

typedef struct tagged_student1
{
    int id;
    char name[100];
    int age;
    double height;
    double weight;
    char tag;
} TStudent1;

typedef struct tagged_student2
{
    char tag;
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} TStudent2;

int main (int argc, char**argv)
{
    Student s_begin;
    Student s1;
    TStudent1 s2;
    TStudent2 s3;
    TStudent2 s_end;
  
    /* 以下に各構造体の先頭アドレスを表示するプログラムを書く */
    /* Hint: printf で %p フォーマットを指定する*/
    /* 逆順に表示（s_end, s3, s2, s1, s_begin の順）*/
    printf("%p\n",&s_end);
    printf("%p\n",&s3);
    printf("%p\n",&s2);
    printf("%p\n",&s1);
    printf("%p\n\n",&s_begin);
    // do_something!!

    /* 以下には各構造体のサイズをsizeof演算子で計算し、表示する */
    /* printf 表示には%zu を用いる*/
    printf("%zu %p\n",sizeof(s1),&s1);
    printf("%zu %p\n",sizeof(s1.id),&s1.id);
    printf("%zu %p\n",sizeof(s1.name),&s1.name);
    printf("%zu %p\n",sizeof(s1.age),&s1.age);
    printf("%zu %p\n",sizeof(s1.height),&s1.height);
    printf("%zu %p\n\n",sizeof(s1.weight),&s1.weight);
    
    printf("%zu %p\n",sizeof(s2),&s2);
    printf("%zu %p\n",sizeof(s2.id),&s2.id);
    printf("%zu %p\n",sizeof(s2.name),&s2.name);
    printf("%zu %p\n",sizeof(s2.age),&s2.age);
    printf("%zu %p\n",sizeof(s2.height),&s2.height);
    printf("%zu %p\n",sizeof(s2.weight),&s2.weight);
    printf("%zu %p\n\n",sizeof(s2.tag),&s2.tag);

    printf("%zu %p\n",sizeof(s3),&s3);
    printf("%zu %p\n",sizeof(s3.tag),&s3.tag);
    printf("%zu %p\n",sizeof(s3.id),&s3.id);
    printf("%zu %p\n",sizeof(s3.name),&s3.name);
    printf("%zu %p\n",sizeof(s3.age),&s3.age);
    printf("%zu %p\n",sizeof(s3.height),&s3.height);
    printf("%zu %p\n",sizeof(s3.weight),&s3.weight);
    // do_something!!
  
    return 0;
}
