//partners of the project
//Mahmoud sihab    1182143
//bara ammer       
//Mohamed Shqierat 1190702 



// /*
//  * The server process ( Parent Process )
//  */


//the program runs through many obstecles like , not finishing executing the whole 
//program after executing the execlp() function 







#include "local.h"              //external header file the contains most of the libraryes + fifo var
#include <math.h>
#include <string.h>

int X_cordinate_generator();   //function that generates x coordinate based on the randomness  of time seed
int Y_cordinate_generator();   //function that generates y coordinate based on the randomness  of time seed
void signal_catcher(int);       
int EnemeID(int);              //function that finds nearest enemy of given soldier pid



struct Soldier
{
  pid_t soldier_ID;
  int x_cordinate;
  int y_cordinate;            //bulding block of the every child (soldier) proces
  char team;
  pid_t enemy_soldier_ID;
};
struct Soldier *soldier_record[10];   //array of struct ( children ) 10 children 


int fd, counter_team_1 = 0, conter_team_2 = 0;   // file decriptor for fifo 
              // team counters to count for next round until it reach 5 to end the game 

char string1[100];
char string2[100];
char string3[100];            //diffrent strings to store in them arguments to pass
char string4[100];            //them to execlp() function to run soldier program
char string5[100];
char string6[100];


int main(void)            //main program starts here 
{
  
  for(int k = 0 ; k < 10 ; k++)
  {
  soldier_record[k] = malloc(sizeof(struct Soldier));       //dynamicaly allocating every child in heap
  }


  if( mkfifo(myfifo, 0777) == -1)             //creating fifo with full permission 
  {
     if(errno != EEXIST)
            {
            printf("ERROR in making pipe\n");       //error handling in case of unable 
            return 1;                               //to create fifos neglecting the already
            }                                        // existing senario (EEXIST)
  }   



  int i, j, status, value,  counter_of_all_team_1 = 0, counter_of_all_team_2 = 0;
  pid_t myPid , pid ;
  int process_status;
  myPid = getpid();
  printf("My process ID is %d\n", getpid());



   if (sigset(2, signal_catcher) == (int)SIG_ERR)
  {
    perror("Sigset can not set SIGQUIT");
    exit(SIGQUIT);
  }


  while (counter_of_all_team_1 < 5 || counter_of_all_team_2 < 5)  // while loop for the whole game 
  {


    for (j = 0; j < 10; j++)
    {
      soldier_record[i]->x_cordinate = X_cordinate_generator();

      soldier_record[i]->y_cordinate = Y_cordinate_generator();

    }


    for (i = 0; i < 10; i++)          // loop for creating children and comunicating with the 
    {                                 //Soldier File 
      pid  = fork();


      if (pid == -1)
      {
        printf("Failed to create chid \n");
         exit(-1);
      }

      if (pid == 0 )                //in child proces
      {
        soldier_record[i]->soldier_ID = getpid();       //assigning pid for the child in struct 
        if (i < 5)
        {
          soldier_record[i]->team = 'R';                //randomly  assigning every child 
                                                        // with team , every team has 5 players  
        }
        else
        {
          soldier_record[i]->team = 'B';
         
        }


        soldier_record[i]->enemy_soldier_ID = EnemeID(i);         //finding the nearest enemy location 
                                                                  //to send signals (shoting and receving )
        printf("I am the child  => PID = %d\n", getpid());         


        sprintf(string1,"%d",soldier_record[i]->enemy_soldier_ID);
        sprintf(string2,"%d",soldier_record[i]->team) ;
        sprintf(string3,"%d",soldier_record[i]->x_cordinate);           //casting soldiers 
        sprintf(string4,"%d",soldier_record[i]->y_cordinate) ;           //attributes to strings
        sprintf(string5,"%d",soldier_record[i]->enemy_soldier_ID);        //so the execlp function
        sprintf(string6,"%d",myPid);                                       //accepts them as arguments


        execlp("./Soldier","./Soldier", string1,  string2 , string3, string4, string5 , string6, NULL);
        perror("exec failure!");          //passing argument into the soldier executable file 
      }

      else 
      { 
        if( i  < 9)                         //if the process is parent , continue to child process
          continue;


                                //when the children process finishes 
        while (counter_team_1 < 5 && conter_team_2 < 5)   //event of the game , waiting for one team    
        {                                                 // to lose the round 

  
          value = (0 + (rand() % (9 - 0)));
 
          kill(soldier_record[value]->soldier_ID, 2);
         }

        if (counter_team_1 == 5)
          {
          printf("blue team win the round\n");              //declearing the winner of the round
          counter_of_all_team_2++;                           //
          }                                                   
        else
        {
          printf("red team win the round\n");                 //
          counter_of_all_team_1++;                              //
        }

        /*send a signal to the child to shoot using id*/
        printf("I am the parent => PID = %d, child ID = %d\n", getpid(), pid);
        /*send a siganal to the child py random from 0 to 9 in for loop*/
        /*the for loop end when the first 5 childs die or the another*/
      }
    }
    
  }     // end of while of the whole GAME 

 
 

  if (counter_of_all_team_1 == 5)
  {
    printf("red team win the game\n");            //counter to check and declare the winner of the
  }                                                //whole game 
  else
  {
    printf("blue team win the game\n");
  }



    if (sigset(2, signal_catcher) == SIG_ERR)
  {                                               
    perror("Sigset can not set SIGQUIT");
    exit(SIGQUIT);
  }
 
     return 0;
} // end of the main program 

  

int X_cordinate_generator()
{
  int value;
  srand(NULL);
  sleep(1);                                         //x coordinate function to generate x value of evey
  value = (int)(0 + (rand() % (50 - 0 )));           //soldier ( child ) with lower limit of 0 , upper limit to 50 
  return value;
}
int Y_cordinate_generator()
{
  int value;
  srand(time(NULL));
  sleep(1);
  value = (int)(0 + (rand() % (50 - 0 )));        //y coordinate function to generate x value of evey
  return value;                                    //soldier ( child ) with lower limit of 0 , upper limit to 50 
}

int EnemeID(int i)
{
  char team;
  team = soldier_record[i]->team;                     //enemy soldier detector function
  int x, distance, value = 70, id;                    //that finds the nearest enemy soldier 
  int x2, x1, y2, y1;                                 // based on checking FIRST the team  
  x1 = soldier_record[i]->x_cordinate;                // if team are diffrent then 
  y1 = soldier_record[i]->y_cordinate;                 // its enemy then checks based on 
  for (x = 0; x < 10; x++)                             // the Diagonal of the whole battlefield  
                                                        // 50 X 50 (DIA = 70 )
  {
    if (x == i && team == soldier_record[i]->team)
    {
      x++;
    }
    else
    {
      x2 = soldier_record[i]->x_cordinate;
      y2 = soldier_record[i]->y_cordinate;
      distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
      if (distance > value)
      {
        value = distance;
        printf("distance ->>> %d\n" , value);
        id = soldier_record[i]->soldier_ID;
      }
    }
  }
  return id;
}

void signal_catcher(int the_sig)
{
  char theTeam;
  fd = open(myfifo, O_RDONLY);
  read(fd, theTeam, 10);                //signal catcher that catches the signal from child program ( Soldier program )
  if (theTeam == 'R')                     
  {
    counter_team_1++;
  }
  else
  {
    conter_team_2++;
  }
}
