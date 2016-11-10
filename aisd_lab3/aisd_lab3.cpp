/* 
                    �������� �����  ��-21
    18. �   ���������   ���������   ����������   ��   ���������
�����������  ������ ����� ���������.  � ������ �� ��� ��������
�������� ������� ������������� ����������,  ���������� �  ����
�������  �������  ��  �������,  �������  �����  �����  � �����
������� �����������. ���������� ����� ���� ����������� � �����
�������������   �������,   ��   ��������,   �  ������������  �
����������������  �����������  ��������.  ��   ������   ������
�������� ����� ������������� ������� �������, ������� ��������
������  ���������,  �  ���  ������  ���������  ������   ������
�����������  �  ���  �����������.  ���������  ���������  �����
������������  ����������  �����������.  ������  ������   �����
��������� (12).*/

#include "stdafx.h"   
#include <fstream>
#include <vector>
#include <iostream>
#include <map>

const int DL = 20;          // ������������ �����

using namespace std;

typedef std::map<std::string, std::vector<string>> Audience;

struct Tree
{
    char name[DL];
    int urov;
    Tree *fath;         // ���� � �������� ������
    Tree *left;
    Tree *right;
};

int read_from_file(FILE *F, Tree **r); 
void SetIdenticalAudience(Tree *corps, Tree *faculty, Audience & audience);
bool IsfileOpen(FILE *fileCorps, FILE *fileFaculty);
int main(int argc, char* argv[])
{
    Audience audience;
    Tree *corps = 0;
    Tree *faculty = 0;
    FILE *fileCorps, *fileFaculty;
    setlocale(LC_ALL, "rus");
    if (argc != 3)
    {
        printf("\nNumber parameters is wrong");
        getchar();
        return -1;
    }
    fileCorps = fopen(argv[1], "r");
    fileFaculty = fopen(argv[2], "r");
    if (IsfileOpen(fileCorps, fileFaculty))
    {
        read_from_file(fileCorps, &corps);
        read_from_file(fileFaculty, &faculty);
        SetIdenticalAudience(corps, faculty, audience);
    }
    for (auto aud : audience)
    {
        std::cout << aud.first.c_str() << ":" << std::endl;
        for (auto cath : aud.second)
        {
            std::cout << "--" << cath.c_str() << std::endl;
        }
    }
    std::cout << "";

}

bool IsDigit(char ch)
{
    return ch >= '0' &&  ch <= '9';
}
bool IsfileOpen(FILE *fileCorps, FILE *fileFaculty)
{
    if ((fileCorps == NULL) && (fileFaculty == NULL))
    {
        printf("\nCan not open file!!!");
        getchar();
        return false;
    }
    return true;
}

int read_from_file(FILE *F, Tree **r)
{
    char buf[DL];
    int i, k, m, len;
    Tree *p, *q, *t;       // *root = 0 
    m = 0;                 // ������� �������
    t = 0;
    while (!feof(F))
    {
        k = fscanf(F, "%s", buf);
        len = strlen(buf);
        if (len == 0) break;            // ���� ����� ����� � ��������� ������
        k = 0;
        while (buf[k] == '.') k++;     // k-������� �������
        p = new Tree;
        strncpy(p->name, &buf[k], len - k + 1);  // 0-(k-1) - �����, (k-1)-(len-1) - ���, \0 - ����� ������
        buf[0] = '\0';                  // ���� ����� ����� � ��������� ������
        p->urov = k;
        p->left = 0;
        p->right = 0;
        if (k == 0)                    // ������
        {
            *r = p;
            t = *r;
            continue;
        }
        if (k > m)                  // ������� �� ��������� �������
        {
            t->left = p;
            p->fath = t;
        }
        else if (k == m)            // ��� �� �������
        {
            t->right = p;
            p->fath = t->fath;  // ���� ��� ��
        }
        else        // ������ �� ������ �� m-k �������
        {
            q = t;
            for (i = 0; i < m - k; i++) q = q->fath;
            // q - ���������� ������� ���� �� ������
            p->fath = q->fath;   // ���� ��� ��, ��� � �����
            q->right = p;
        }
        m = k;      // ������� �������
        t = p;      // ������� �������
    }
    fclose(F);
    return 0;
}

void SearchFaculty(Tree *corps, Tree *faculty, Audience & audience)
{
    if (faculty)
    {
        if (std::string(corps->name) == std::string(faculty->name))
        {
            auto it = audience.find(std::string(corps->fath->name));
            if (it != audience.end())
            {
                auto itVec = std::find(it->second.begin(), it->second.end(), std::string(faculty->fath->name));
                if (itVec == it->second.end())
                {
                    it->second.push_back(std::string(faculty->fath->name));
                }
            }
        }
        SearchFaculty(corps, faculty->left, audience);
        SearchFaculty(corps, faculty->right, audience);
    }
}

void SetIdenticalAudience(Tree *corps, Tree *faculty, Audience & audience)
{
    if (corps)
    {
        if (IsDigit(corps->name[0]))
        {
            auto it = audience.find(std::string(corps->fath->name));
            if (it == audience.end())
            {
                std::vector<std::string> cathedra;
                audience.emplace(string(corps->fath->name), cathedra);
            }
            SearchFaculty(corps, faculty, audience);
        }
        SetIdenticalAudience(corps->left, faculty, audience);
        SetIdenticalAudience(corps->right, faculty, audience);
    }
}
