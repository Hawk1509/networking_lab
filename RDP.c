
#include<stdio.h>
#include<ctype.h>
#include<string.h>
char input[100];
int i, l;

int E()
{
    if (T())
    {
        if (Ed())
            return (1);
        else
            return (0);
    }
    else
        return (0);
}
int Ed()
{
    if (input[i] == '+')
    {
        i++;
        if (T())
        {
            if (Ed())
                return (1);
            else
                return (0);
        }
        else
            return (0);
    }
    else
        return (1);
}
int T()
{
    if (F())
    {
        if (Td())
            return (1);
        else
            return (0);
    }
    else
        return (0);
}
int Td()
{
    if (input[i] == '*')
    {
        i++;
        if (F())
        {
            if (Td())
                return (1);
            else
                return (0);
        }
        else
            return (0);
    }
    else
        return (1);
}
int F()
{
    if (input[i] == '(')
    {
        i++;
        if (E())
        {
            if (input[i] == ')')
            {
                i++;
                return (1);
            }
            else
                return (0);
        }
        else
            return (0);
    }
    else if (input[i] == 'a')
    {
        i++;
        return (1);
    }
    else
        return (0);
}

void main()
{
   
    printf("Enter the expression:");
    scanf("%s", input);
    if (E())
    {
        if (input[i + 1] == '\0')
            printf("Expression is accepted");
        else
            printf("Expression is not accepted");
    }
    else
        printf("Expression is not accepted");
}

