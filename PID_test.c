#include<stdio.h>//PID算法模拟（增量式）
#include<stdlib.h>
struct _pid_t
{
    float SetSpeed;//设定速度
    float ActualSpeed;
    float err;
    float err_next;
    float err_last;
    float Kp,Ki,Kd;
}pid;
 void PID_init()
 {
     pid.SetSpeed=0.0;
     pid.ActualSpeed=0.0;
     pid.err=0.0;
     pid.err_last=0.0;
     pid.err_next=0.0;
     pid.Kp=0.21;
     pid.Ki=0.061;
     pid.Kd=0.35;
 }
 float PID_realize(float speed)
 {
     pid.SetSpeed=speed;
     pid.err=pid.SetSpeed-pid.ActualSpeed;
     float incrementSpeed=pid.Kp*(pid.err-pid.err_next)+pid.Ki*pid.err+pid.Kd*(pid.err-2*pid.err_next+pid.err_last);
     pid.ActualSpeed+=incrementSpeed;
     pid.err_last=pid.err_next;
     pid.err_next=pid.err;
     return pid.ActualSpeed;
 }
 int main()
 {
     FILE *fp;
     char c='\n';
     char cc=':';
     //fp=fopen("c:/data.txt","a");
    int i=1;
     PID_init();
     int count=0;
     while(count<1000)
     {
          float speed=PID_realize(200.0);
        // printf("%f\n",speed);
        fp=fopen("c:/data.txt","a");
        fprintf(fp,"%d%c",i,cc);
         fprintf(fp,"%f%c",speed,c);
         printf("%d:",i);
         printf("%f\n",speed);
         count++;
         i++;
     }

 }
