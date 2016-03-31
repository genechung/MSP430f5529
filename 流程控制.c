#include<stdio.h>
int main()
{
    int a,b;
    int i,j;
    for(i=1;i<=9;i++)
    {
        printf("%d",i);
        if(i%3==0)
            printf("\n");
        else
        {
            printf("  ");
        }

    }
    while(1)
    {
        loop1:scanf("%d",&a);
        if(a==1)
        {
            printf("进入1\n");
            loop:scanf("%d",&b);
            if(b==0)
            {
                printf("退出1\n");
                goto loop1;
            }
            else
            {
             goto loop;
            }
        }
        else if(a==2)
        {
             printf("进入2\n");
           loop2:scanf("%d",&b);
           if(b==0)
           {
            printf("退出2\n");
            goto loop1;
           }
           else
           {
               goto loop2;
           }

        }
        else if(a==0)
        {
            printf("退出程序");
            break;
        }

    }


}
