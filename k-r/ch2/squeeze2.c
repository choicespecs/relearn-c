#include <stdio.h>
#include <string.h>


void squeeze2(char s1[], char s2[])
{
    int i,j,k;
    for (k = 0; s2[k] != '\0'; k++)
        for (i = j = 0; s1[i] != '\0'; i++)
            if (s1[i]  != s2[k])
                s1[j++] = s1[i];
        s1[j] = '\0';
}

int main(void)
{
    char s1[5] = {'s','s', 't', 'k', '\0'};
    char s2[5] = {'d','a', 't','k', '\0'};
    squeeze2(s1, s2);
    printf("%s\n", s1);
    return 0;
}


