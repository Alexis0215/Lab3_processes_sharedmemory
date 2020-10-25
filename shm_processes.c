#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  StudentAcct(int []);
void  DadAct(int []);
int Turn=0;  //to see who's account we are in

int  main(int  argc, char *argv[]){
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
      
    srandom(getpid()); 
     
    if (argc != 3) {
         printf("Use: %s 0 0\n", argv[0]);
        
     }
  
     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
  
     printf("Server has received a shared memory of two integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
  
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
  
     printf("Server has attached the shared memory...\n");

     ShmPTR[0] = atoi(argv[1]); //BankAcoount (atoi= converts string to int )
     ShmPTR[1] = atoi(argv[2]);//Turn var 
    
     printf("Server has filled %d %d in shared memory...\n",
            ShmPTR[0], ShmPTR[1]); //puts 4 ints into shared mem ptr arr 

     printf("Server is about to fork a child process...\n");
  
     pid = fork(); 
    
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) { //child process 
          StudentAcct(ShmPTR); //calls studentAcct function
          exit(0);
     }
    else{
      //else call parent proc here 
      DadAcct(ShmPTR); }
  
     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

void  DadAcct(int  SharedMem[]){
    int i = 0;
    int BankAccount = SharedMem[0];
    int balance; 
    
   
    for(i=0; i<25;i++){
      //Sleep some random amount of time between 0 - 5 seconds
      sleep(random() %5); 
     
      
        while(SharedMem[1]!=0){ //Turn !=0
          //do nothing 
        }
      BankAccount = SharedMem[0];
       
       //if the account is <= 100
       if(BankAccount<=100){
        
        //Randomly generate a balance amount to give the Student between   0− 100
        balance=random() % 100; //get random amt of money 
        
          //If the random number is even
          if(balance % 2 == 0){
            BankAccount = BankAccount + balance; //deposit balance to BA
            printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, BankAccount);
          }
         //If the random number is odd
          else{
            printf("Dear old Dad: Doesn't have any money to give\n");
          }
       }
      else{
        printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", BankAccount);
      }
      
      //Copy value from local variable account back to BankAccount.
      SharedMem[0]=BankAccount;  
      //set Turn = 1
      SharedMem[1]=1;  
    }

}




void  StudentAcct(int  SharedMem[]){
    int i = 0;
    int BankAccount = SharedMem[0];
    int wbalance; 
    
   //child proc created here 
    for(i=0; i<25;i++){
      //Sleep some random amount of time between 0 - 5 seconds
      sleep(random() %5); 
      
      
        //loop while Turn != 1 do no-op
        while(SharedMem[1]!=1){
          //do nothing 
        }
      //After waking up, copy the value in BankAccount to a local variable account
      BankAccount = SharedMem[0];
      
      //Randomly generate a balance amount that the Student needs between   0− 50
     wbalance=random()%50;
      printf("Poor Student needs $%d\n", wbalance);
       
      //If the balance needed is <= the account
      if(wbalance<=BankAccount){
        //Withdraw the amount from the account, then call
        BankAccount=BankAccount-wbalance;
        printf("Poor Student: Withdraws $%d / Bank Account Balance = $%d\n", wbalance, BankAccount);
      }
      //If the balance needed is > than the account 
      else{ 
        printf("Poor Student: Not Enough Cash ($%d)\n", BankAccount );
      }
      //copy the value in BankAccount to a local variable account
      SharedMem[0]=BankAccount; 
      
      //Set Turn = 0
      SharedMem[1]=0;  
    }
}
