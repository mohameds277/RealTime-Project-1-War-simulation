/*
 * The client process
 */

#include "local.h"

int head=0,neck=0,chest=0,stomach=0,hands=0,legs=0,fd1;


pid_t global_argument1; 
char global_argument2;
pid_t global_argument3;


void main(int argc, char *argv)
{

  
     if (argc != 7)
   {
     printf("few arguments  requierd  \n");
     exit(-1);
   }
   printf("%d\n" , argv[0]);
   printf("%d\n" , argv[1]);
   printf("%d\n" , argv[2]);
   printf("%d\n" , argv[3]);
   printf("%d\n" , argv[4]);
   printf("%d\n" , argv[5]);
   printf("%d\n" , argv[6]);
  if ( mkfifo(myfifo, 0777) == -1  )
  {
     if(errno != EEXIST)
            {
            printf("ERROR in making pipe\n");
            return 1;
            }
  }
 
  global_argument1 = argv[1];
  global_argument3 = argv[5];



  void signal_catcher(int);  // to shoot (write)
  void signal_catcher1(int); // to receve_shot (read)

  if (sigset(2, signal_catcher) == SIG_ERR)
  { // to shoot
    perror("Sigset can not set SIGQUIT");
    exit(SIGQUIT);
  }

  if (sigset(7, signal_catcher1) == SIG_ERR)
  { // to read
    perror("Sigset can not set SIGQUIT");
    exit(SIGQUIT);
  }



}

void signal_catcher(int the_sig)
{
    int min=0,max=100;
    int sum_of_shoots=0;
    sum_of_shoots=head+neck+chest+stomach+legs+hands;
    max=max+(sum_of_shoots*5);
    int value;
    char where_the_shoot_hit;
    srand(time(NULL));
    sleep(1);
    value = (int) (min + (rand() % (max - min )));
    if(value>=0&&value<=15){
      where_the_shoot_hit='h';
    }
    else if (value>=16&&value<=30){
      where_the_shoot_hit='n';
    }
    else if (value>=31&&value<=45){
      where_the_shoot_hit='s';
    }
    else if (value>=46&&value<=60){
      where_the_shoot_hit='t';
    }
    else if (value>=61&&value<=75){
      where_the_shoot_hit='l';
    }
    else if (value>=76&&value<=90){
      where_the_shoot_hit='a';
    }
    else{
      where_the_shoot_hit='m';
    }
    
    /*put in the pipe where and send a siganal to who*/
    fd1=open(myfifo,O_WRONLY);
    write(fd1,where_the_shoot_hit,10);
    close(fd1);
    kill(atoi(global_argument1), 7);
    printf("\nSignal %d received.\n", the_sig);

    /*if (the_sig == SIGQUIT)
      exit(1);*/
}

void signal_catcher1(int the_sig)
{
  char where_i_shoot;
  fd1=open(myfifo, O_RDONLY);
  /*read where and incease it*/
  read(fd1,where_i_shoot,10);
  switch (where_i_shoot)
  {
    case 'h':head++;
    break;
    case 'n':neck++;
    break;
    case 's':chest++;
    break;
    case 't':stomach++;
    break;
    case 'l':legs++;
    break;
    case 'a':hands++;
    break;
    default:printf("the shoot was missed\n");
    break;
  }
  close(fd1);

  if(head==2||neck==3||chest==3||stomach==3||hands==5||legs==5){
    //the child will die and send a siganal to parent tp read which team is he 
    fd1=open(myfifo,O_WRONLY);
    write(fd1,global_argument1,10);
    close(fd1);
    kill(atoi(global_argument3), 2);
    printf("soldier killed\n");
    exit(-1);
  }
}
