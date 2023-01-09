#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
using namespace std;

sem_t S;
pthread_mutex_t mutex;
pthread_cond_t cond;
int N=0; //number of students (threads)
int cap=0; //num of seats in TA's room
int s_count=0;
 int allocation[10][3],need[10][3],Max[10][3],available[10][3];
int p,current[3];
bool executed[10],come;
void IMP(){
	come=false;
	for (int i = 0; i < 10; ++i)
	{
		executed[i]=false;
	}
	}
void * studentFunc(void * sns)
{
    int stdNum = *((int *) sns);
    cout << "Student " << stdNum << " entering....\n\n";
    pthread_mutex_lock(&mutex);
    s_count++;
    if(s_count == 1)
        sem_wait(&S); 
    if(s_count < cap)
        pthread_mutex_unlock(&mutex);
    
    
    
    cout << "Students in TA's room : " << s_count << endl << endl;
    
    pthread_mutex_lock(&mutex);
    
    s_count--;
    cout << "Student leaving TA's room" << endl<< endl;
    cout << "Remaining Student : " << s_count << endl<< endl;
    if (s_count == 0)
    {
        cout << "No student in TA's room" << endl<< endl;
        sem_post(&S); 
    }
      
    
    pthread_mutex_unlock(&mutex);
   
    pthread_exit(NULL);
}

void * TAFunc(void *)
{
    sleep(2);
    if(s_count == 0){
        cout << "TA busy with his work\n\n";
    }
    else {
        cout << "ALERT! Students are here!\n\n";
    }
    pthread_exit(NULL);
}


class aval{
private:
int t1,t2,t3;
int av1,av2,av3;
public:
aval(){
    t1=0;
    t2=0;
    t3=0;
    av1=0;
    av2=0;
    av3=0;
}
aval(int a,int b,int c){
    t1=a;
    t2=b;
    t3=c;
    
}
void addav1(int a){
    av1+=a;
}
void addav2(int a){
    av2+=a;
}
void addav3(int a){
    av3+=a;
}
int getav1(){
    return av1;
}
int getav2(){
    return av2;
}
int getav3(){
    return av3;
}

};

class proc{
private:
int r1;
int r2;
int r3;
int a1,a2,a3;
int rem1,rem2,rem3;

public:

proc(){
r1=0;
r2=0;
r3=0;
a1=0;
a2=0;
a3=0;
rem1=0;
rem2=0;
rem3=0;
}

proc(int a,int b,int c,int d,int e,int f){
    r1=a;
    r2=b;
    r3=c;
    a1=d;
    a2=e;
    a3=f;
    rem1=r1-a1;
     rem2=r2-a2;
      rem3=r3-a3;
}
int geta1(){
    return a1;
}
int geta2(){
    return a2;
}
int geta3(){
    return a3;
}
int getrem1(){
    return rem1;
}
int getrem2(){
    return rem2;
}
int getrem3(){
    return rem3;
}
void do_process(aval *a){
    a->addav1(this->a1);
      a->addav2(this->a2);
        a->addav3(this->a3);
    sleep(1);
}
};

int calc(proc*p,aval *a){

        a->addav1(-p->geta1());
        a->addav2(-p->geta2());
        a->addav3(-p->geta3());

        if(a->getav1()<0 || a->getav2()<0 || a->getav3()<0)
        return -1;
        return 0;
}
bool check(proc*p,aval*a){
    cout<<a->getav2()<<" "<<p->getrem2()<<endl;
if((a->getav1()-p->getrem1()>=0) && (a->getav2()-p->getrem2()>=0) && (a->getav3()-p->getrem3()>=0))
return 1;
else return 0;
}

void Calculate(){
	IMP();
	int i,j;
	for (i = 0; i < p; ++i)
	{	
		for (j = 0; j < p; ++j)
		{
			while(executed[j] && j<p-1){
				j++;
			}
			if (need[j][0]<=current[0]&&need[j][1]<=current[1]&&need[j][2]<=current[2])
			{
				if (!executed[j])
				{
					executed[j]=true;
					current[0]+=allocation[j][0];current[1]+=allocation[j][1];current[2]+=allocation[j][2];
					cout<<"\nProcess P"<<j+1;
					cout<<"\nCurrent: "<<current[0]<<" "<<current[1]<<" "<<current[2]<<"\n";
					cout<<"\nProcess executed without deadlock";
					come=true;
					break;
				}
			}
		}
		if (!come)
		{	
			cout<<"\n			Dead lock\n\n";
			break;
		}else{
			come=false;
		}
	}
}


void deadlockFunc()
{

	
		int keepon = 1;
	cout<<"Enter No. of processes: ";
	cin>>p;
	cout<<"\n";
	cout<<"Enter the current resources: ";
	cin>>current[0]>>current[1]>>current[2];
	for (int i = 0; i < p; ++i)
	{
		cout<<"\n\n			Process P"<<i+1<<" Details\n";
		cout<<"Enter Allocation : ";
		cin>>allocation[i][0]>>allocation[i][1]>>allocation[i][2];
		cout<<"Enter Max :";
		cin>>Max[i][0]>>Max[i][1]>>Max[i][2];
		need[i][0]=Max[i][0]-allocation[i][0];need[i][1]=Max[i][1]-allocation[i][1];need[i][2]=Max[i][2]-allocation[i][2];
	}
	cout<<"\n\n			Table for Bankers Algo\n\n";
	cout<<"Initial Resources: "<<current[0]<<" "<<current[1]<<" "<<current[2]<<"\n\n";
	cout<<"Process    Max    Allocation    Need\n";
	for (int i = 0; i < p; ++i)
	{
		cout<<"  P"<<i+1<<"    ";
		cout<<"  "<<Max[i][0]<<" "<<Max[i][1]<<" "<<Max[i][2]<<"     ";
		cout<<" "<<allocation[i][0]<<" "<<allocation[i][1]<<" "<<allocation[i][2]<<"     ";
		cout<<" "<<need[i][0]<<" "<<need[i][1]<<" "<<need[i][2];
		cout<<"\n";
	}
	cout<<"\n\n";
	Calculate();
	while(keepon){
		int val,pro;
		cout<<"\n\nSelect Below oprations:\n\n";
		cout<<"1.Change Max of process: \n";
		cout<<"2.Change Allocation of process\n";
		cout<<"3.Change Initial Resources\n";
		cout<<"4.Exit\n\n";
		cin>>val;
		if (val==1)
		{
			cout<<"\n\nEnter Process No: ";
			cin>>pro;
			cout<<"\nEnter New Max: ";
			cin>>Max[pro-1][0]>>Max[pro-1][1]>>Max[pro-1][2];
		}
		else if (val==2)
		{
			cout<<"\n\nEnter Process No: ";
			cin>>pro;
			cout<<"\nEnter New Allocation: ";
			cin>>allocation[pro-1][0]>>allocation[pro-1][1]>>allocation[pro-1][2];
		}
		else if (val==3)
		{
			cout<<"\nEnter Initial Resources: ";
			cin>>current[0]>>current[1]>>current[2];
		}
		else{
			break;
		}
		Calculate();
	}
	
}




void semaphoresFunc()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    sem_init(&S, 0, 1);

    cout << "ENTER THE NUMBER OF SEATS IN THE ROOM : ";
    cin >> cap;
    cout << "ENTER THE NUMBER OF STUDENTS : ";
    cin >> N;
    cout << endl;
    pthread_t student[N], TA;
    pthread_create(&TA, NULL, TAFunc, NULL);
    pthread_join(TA, NULL);
    for (int i = 0; i < N; i++)
    {
        int *arg = new int;
        *arg = i;
        pthread_create(&student[i], NULL, studentFunc, arg);
       
        
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(student[i], NULL);
        sleep(2);
    }
    
    cout << "Ended!" << endl;
}

 int Finishing_time = 0 ;   // It will tell the finishing time of a process

class Process
{
public:
	string Pname;  // Process name e.g. P1,P2....
	int Btime;       // Burst time
	int remaining_burst_time;  // it stores the remaining burst time required for a process after an iteration.
	int Priority;
	int arrival_time;
	double waiting_time;
	string Process_Type;
	double feedback_ratio;

	Process* next;
    
	Process()                  //default constructor
    {
        next = nullptr;
    }

};

class CPU_Scheduler    // we will use queue to implement CPU scheduling but we will not concentrating on the queue order of FIFO 
{
private:

	Process* root; // first process in the queue
	int size;          // No of processes
    Process* front;  // last process in the queue


public: 


	CPU_Scheduler()
    {
        root = nullptr;
        size = 0;
        front = nullptr;

    }

	void Enqueue(string name, int time,int Prio, int arrival)   // Inserting in the order in which they are recieved	
	{
		
		if (root == nullptr)   // if first process, then
        {

            Process* temp = new Process;
			temp->Pname = name;
			temp->Btime = time;
			temp->remaining_burst_time = time;
			temp->Priority = Prio;
			temp->arrival_time = arrival;
			temp->waiting_time = 0;


            temp->next = NULL;    
            root = temp;
            front = temp;
            size++;
        }
        else
        {

            Process* temp1 = new Process;
			temp1->Pname = name;
			temp1->Btime = time;
			temp1->remaining_burst_time = time;
			temp1->Priority = Prio;
			temp1->arrival_time = arrival;
			temp1->waiting_time = 0;

            front->next = temp1;
			temp1->next = NULL;

            front = temp1;
            size++;

        }


	}




	
	void Enqueue1(string name, int time,int Prio, int arrival, string type)   // Inserting in the order in which they are recieved	
	{
		
		if (root == nullptr)   // if first process, then
        {

            Process* temp = new Process;
			temp->Pname = name;
			temp->Btime = time;
			temp->remaining_burst_time = time;
			temp->Priority = Prio;
			temp->arrival_time = arrival;
			temp->waiting_time = 0;
			temp->Process_Type = type;
			
            temp->next = NULL;    
            root = temp;
            front = temp;
            size++;
        }
        else
        {

            Process* temp1 = new Process;
			temp1->Pname = name;
			temp1->Btime = time;
			temp1->remaining_burst_time = time;
			temp1->Priority = Prio;
			temp1->arrival_time = arrival;
			temp1->waiting_time = 0;

			temp1->Process_Type = type;
			
            front->next = temp1;
			temp1->next = NULL;

            front = temp1;
            size++;

        }


	}

	
  void FCFS()
  {
	  system("Color 0D");
	   sleep(1500);
	  cout<<endl<<"          First Come, First Serve Algorithm started:"<<endl;
		    cout<<"         ============================================  "<<endl;
			 sleep(1000);
	  Process * temp = root;

	  Finishing_time = 0;

	  while(temp!=NULL)
	  {
		  // we suppose that the project arrives and ends.

		  temp->remaining_burst_time = temp->remaining_burst_time - temp->Btime;

		  Finishing_time = Finishing_time + temp->Btime;
		  temp->waiting_time = Finishing_time - temp->Btime - temp->arrival_time;

		   sleep(800);

		 cout<<"Process  " ; 
		 
		 for(int i=0 ; temp->Pname[i]!=NULL; i++)
		 {
			 cout<<temp->Pname[i];
		 }
		 		  
		 cout<<"  has Completed its execution."<<endl<<"Arrival Time:"<<temp->arrival_time<<endl<<"Burst Time: "<<temp->Btime<<endl<<"Priority: "<<temp->Priority<<endl<<"Finishing Time: "<<Finishing_time<<endl<<"Waiting Time: "<<temp->waiting_time<<endl<<endl<<endl;
		
		  temp = temp->next;
	  }

  }


  void Priority_scheduling()
  {
	  // The process with the highest priority is executed first

	  system("Color 0D");
	   sleep(1500);
	    cout<<endl<<"       Priority Scheduling Algorithm started:"<<endl;
		      cout<<"     =========================================  "<<endl;
		Finishing_time = 0;
		 sleep(500);
	  Process * curr1 = root;
	  Process * curr2 = curr1->next;
	  Process* Lar_Pro = new Process;

	
	  int lar = 0 ;
	  int count = 1 ;

	  while(curr1->next!=NULL)
	  {
		  lar= curr1->Priority;
		  curr2 = curr1->next;
		  while(curr2!=NULL)
		  {
			  if(lar<curr2->Priority)
			  { 
				  lar = curr2->Priority;
				  // Now Swapping process to take largest process to the first location
				  Lar_Pro->Btime = curr2->Btime;
				  Lar_Pro->Pname = curr2->Pname;
				  Lar_Pro->Priority = curr2->Priority;
				  Lar_Pro->remaining_burst_time = curr2->remaining_burst_time;
				  Lar_Pro->arrival_time = curr2->arrival_time;


				  curr2->Pname = curr1->Pname;
				  curr2->Btime = curr1->Btime;
				  curr2->Priority = curr1->Priority;
				  curr2->remaining_burst_time = curr1->remaining_burst_time;
				  curr2->arrival_time = curr1->arrival_time;

				  curr1->Pname = Lar_Pro->Pname;
				  curr1->Btime = Lar_Pro->Btime;
				  curr1->Priority = Lar_Pro->Priority;
				  curr1->remaining_burst_time = Lar_Pro->remaining_burst_time;
				  curr1->arrival_time = Lar_Pro->arrival_time;

			  }

			  curr2 = curr2->next;
		  }
		  Finishing_time = Finishing_time + curr1->Btime;
		  curr1->waiting_time = Finishing_time - curr1->Btime-curr1->arrival_time;
		  curr1->remaining_burst_time = curr1->remaining_burst_time - curr1->Btime;
		   sleep(900);
		  cout<<endl<<endl<<"Number "<<count<<" highest Priority process is : ";
		  for(int i=0 ; curr1->Pname[i]!=NULL ; i++)
		  {
			  cout<<curr1->Pname[i];
		  }
		  cout<<endl<<"Priority: "<<curr1->Priority;
		  cout<<endl<<"Burst Time: "<<curr1->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<curr1->remaining_burst_time;
		  cout<<endl<<" Finishing Time is: "<<Finishing_time;
		  cout<<endl<<"Waiting Time: "<<curr1->waiting_time;
		  count++;
		   sleep(500);
			  curr1 = curr1->next;
	  }

	  // last process printing

	  Finishing_time = Finishing_time + front->Btime;
	  front->waiting_time = Finishing_time - front->Btime-front->arrival_time;
	 sleep(900);
	  cout<<endl<<endl<<"Lowest Priority Process Among all Processes is : ";
		  for(int i=0 ; front->Pname[i]!=NULL ; i++)
		  {
			  cout<< front->Pname[i];
		  }
		  cout<<endl<<"Priority: "<<front->Priority;
		  cout<<endl<<"Burst Time: "<<front->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<front->remaining_burst_time;
		  cout<<endl<<" Finishing Time is: "<<Finishing_time;
		  cout<<endl<<"Waiting Time: "<<front->waiting_time;
		  count++;
		   sleep(500);
  }


  void Round_robin()
  {
	  // first ask user about the turn around time:

	  system("Color 0D");

	  int turn_time =0;
	   sleep(1500);
	  cout<<endl<<endl<<"         Round Robin Algorithm started: "<<endl;
	              cout<<"        =============================== "<<endl;
	  sleep(800);
	  cout<<endl<<"Please enter Turn Around time for round robin algorithm: ";
	  cin>>turn_time;

	  // now each process will get some time from its remaining burst time to perform its operations.

	  Process * curr1 = root;
	  Process * pre = root ;


	  Finishing_time = 0 ;
	  int num =0;

	  while(curr1!=NULL)
	  {
		   sleep(900);
		  cout<<endl;
		  num = curr1->remaining_burst_time - turn_time;

		  if(num<=0)
		  {
			  cout<<endl<<endl<<endl;
			  for(int i=0 ; curr1->Pname[i]!=NULL ; i++)
		  {
			  cout<<curr1->Pname[i];
		  }
			  cout<<" is going to complete its execution.";
			  cout<<endl<<"Burst Time: "<<curr1->Btime;
	  	   	curr1->remaining_burst_time = curr1->remaining_burst_time - turn_time;
    		  cout<<endl<<"Remaining Burst Time: 0 ";
			   
			  Finishing_time = Finishing_time + curr1->remaining_burst_time;
		 cout<<endl<<"Finishing Time: "<<Finishing_time;
		 
		  curr1->waiting_time = Finishing_time - curr1->Btime - curr1->arrival_time;
		  cout<<endl<<"Waiting Time: "<<curr1->waiting_time;


		  // now remove that process from the queue

			    Process * temp ;
			  if(curr1== root)
			  {
				  temp = curr1;
				  root = root->next;
				  curr1 = curr1->next;
				  pre = curr1;

				  delete temp;

			  }

			  else
			  {
				  pre->next = curr1->next;
				  temp = curr1;
				  curr1 = curr1->next;

				  delete temp;
			  }
		  
		  }


		  else if(num > 0 )
		  {

		  Finishing_time = Finishing_time + turn_time;
		  curr1->remaining_burst_time = curr1->remaining_burst_time - turn_time;
		//  curr1->finish_time = curr1->finish_time + 
		   sleep(800);
		  cout<<endl<<endl<<"Process Name : ";
		  for(int i=0 ; curr1->Pname[i]!=NULL ; i++)
		  {
			  cout<<curr1->Pname[i];
		  }
		  cout<<endl<<"Status: Executing for "<<turn_time<<" seconds";
		  cout<<endl<<"Burst Time: "<<curr1->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<curr1->remaining_burst_time;
		 
		   pre = curr1;
		  curr1 = curr1->next;

		  }
		  		 
		  if(curr1== NULL)
		  {
			  curr1= root;
			  pre = curr1;
		  }

	  }

  }



//-------------------------------------------------------------------------------------------------------------------


  
  void FCFS1()
  {
	  system("Color 0E");
	   sleep(1500);
	  cout<<endl<<"          First Come, First Serve Algorithm started:"<<endl;
		    cout<<"         ============================================  "<<endl;
			 sleep(900);
	  Process * temp = root;

	  Finishing_time = 0;

	  while(temp!=NULL)
	  {
		  // we suppose that the project arrives and ends.

		  temp->remaining_burst_time = temp->remaining_burst_time - temp->Btime;
		  Finishing_time = Finishing_time + temp->Btime;
		  temp->waiting_time = Finishing_time - temp->Btime - temp->arrival_time;
		  sleep(1000);
		  cout<<"Process  " ;
		 for(int i=0 ; temp->Pname[i]!=NULL; i++)
		 {
			 cout<<temp->Pname[i];
		 }
		 		  
		 cout<<"  has Completed its execution."<<endl<<"Arrival Time:"<<temp->arrival_time<<endl<<"Burst Time: "<<temp->Btime<<endl<<"Priority: "<<temp->Priority<<endl<<"Finishing Time: "<<Finishing_time<<endl<<"Waiting Time: "<<temp->waiting_time<<endl<<endl<<endl;
		
		  temp = temp->next;
	  }

  }


  void Priority_scheduling1()
  {
	  // The process with the highest priority is executed first

	  system("Color 0D");
	   sleep(1200);
	    cout<<endl<<"       Priority Scheduling Algorithm started:"<<endl;
		      cout<<"     =========================================  "<<endl;
		Finishing_time = 0;
		 sleep(1000);
	  Process * curr1 = root;
	  Process * curr2 = curr1->next;
	  Process* Lar_Pro = new Process;

	
	  int lar = 0 ;
	  int count = 1 ;

	  while(curr1->next!=NULL)
	  {
		  lar= curr1->Priority;
		  curr2 = curr1->next;
		  while(curr2!=NULL)
		  {
			  if(lar<curr2->Priority)
			  { 
				  lar = curr2->Priority;
				  // Now Swapping process to take largest process to the first location
				  Lar_Pro->Btime = curr2->Btime;
				  Lar_Pro->Pname = curr2->Pname;
				  Lar_Pro->Priority = curr2->Priority;
				  Lar_Pro->remaining_burst_time = curr2->remaining_burst_time;
				  Lar_Pro->arrival_time = curr2->arrival_time;


				  curr2->Pname = curr1->Pname;
				  curr2->Btime = curr1->Btime;
				  curr2->Priority = curr1->Priority;
				  curr2->remaining_burst_time = curr1->remaining_burst_time;
				  curr2->arrival_time = curr1->arrival_time;

				  curr1->Pname = Lar_Pro->Pname;
				  curr1->Btime = Lar_Pro->Btime;
				  curr1->Priority = Lar_Pro->Priority;
				  curr1->remaining_burst_time = Lar_Pro->remaining_burst_time;
				  curr1->arrival_time = Lar_Pro->arrival_time;

			  }

			  curr2 = curr2->next;
		  }
		  Finishing_time = Finishing_time + curr1->Btime;
		  curr1->waiting_time = Finishing_time - curr1->Btime-curr1->arrival_time;
		  curr1->remaining_burst_time = curr1->remaining_burst_time - curr1->Btime;
		   sleep(900);
		  cout<<endl<<endl<<"Number "<<count<<" highest Priority process is : ";
		  for(int i=0 ; curr1->Pname[i]!=NULL ; i++)
		  {
			  cout<<curr1->Pname[i];
		  }
		  cout<<endl<<"Priority: "<<curr1->Priority;
		  cout<<endl<<"Burst Time: "<<curr1->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<curr1->remaining_burst_time;
		  cout<<endl<<" Finishing Time is: "<<Finishing_time;
		  cout<<endl<<"Waiting Time: "<<curr1->waiting_time;
		  count++;

			  curr1 = curr1->next;
	  }

	  // last process printing

	  Finishing_time = Finishing_time + front->Btime;
	  front->waiting_time = Finishing_time - front->Btime-front->arrival_time;
	   sleep(900); 
	  cout<<endl<<endl<<"Lowest Priority Process Among all Processes is : ";
		  for(int i=0 ; front->Pname[i]!=NULL ; i++)
		  {
			  cout<< front->Pname[i];
		  }
		  cout<<endl<<"Priority: "<<front->Priority;
		  cout<<endl<<"Burst Time: "<<front->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<front->remaining_burst_time;
		  cout<<endl<<" Finishing Time is: "<<Finishing_time;
		  cout<<endl<<"Waiting Time: "<<front->waiting_time;
		  count++;

  }


  void Round_robin1()
  {
	  // first ask user about the turn around time:

	  system("Color 0D");

	  int turn_time =0;
	   sleep(1500);
	  cout<<endl<<endl<<"         Round Robin Algorithm started: "<<endl;
	              cout<<"        =============================== "<<endl;
				   sleep(1000);
				  cout<<endl<<"Please enter Turn Around time for round robin algorithm: ";
	  cin>>turn_time;

	  // now each process will get some time from its remaining burst time to perform its operations.

	  Process * curr1 = root;
	  Process * pre = root ;

	  int choice = 0 ;

	  Finishing_time = 0 ;
	  int num =0;

	  while(curr1!=NULL)
	  {

		  num = curr1->remaining_burst_time - turn_time;

		  if(num<=0)
		  {
			  cout<<endl<<endl<<endl;
			   sleep(1000);
			  for(int i=0 ; curr1->Pname[i]!=NULL ; i++)
		  {
			  cout<<curr1->Pname[i];
		  }
			  cout<<" is going to complete its execution.";
			  cout<<endl<<"Burst Time: "<<curr1->Btime;
			  cout<<endl<<"Remaining Burst Time: "<<curr1->remaining_burst_time;
			  curr1->remaining_burst_time = curr1->remaining_burst_time - turn_time;
    		  
			   
			  Finishing_time = Finishing_time + curr1->remaining_burst_time;
		 cout<<endl<<"Finishing Time: "<<Finishing_time;
		 
		  curr1->waiting_time = Finishing_time - curr1->Btime - curr1->arrival_time;
		  cout<<endl<<"Waiting Time: "<<curr1->waiting_time;


		  // now remove that process from the queue

			    Process * temp ;
			  if(curr1== root)
			  {
				  temp = curr1;
				  root = root->next;
				  curr1 = curr1->next;
				  pre = curr1;

				  delete temp;

			  }

			  else
			  {
				  pre->next = curr1->next;
				  temp = curr1;
				  curr1 = curr1->next;

				  delete temp;
			  }
		  
		  }


		  else if(num > 0 )
		  {

		  Finishing_time = Finishing_time + turn_time;
		  curr1->remaining_burst_time = curr1->remaining_burst_time - turn_time;
		//  curr1->finish_time = curr1->finish_time + 
		   sleep(800);
		  cout<<endl<<endl<<"Process Name : ";
		  for(int i=0 ; curr1->Pname[i]!=NULL ; i++)
		  {
			  cout<<curr1->Pname[i];
		  }
		  cout<<endl<<"Status: Executing for "<<turn_time<<" seconds";
		  cout<<endl<<"Burst Time: "<<curr1->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<curr1->remaining_burst_time;
		 
		   pre = curr1;
		  curr1 = curr1->next;

		  if(choice == 3)
		  {
			   sleep(800);
			  int f=0;
			  cout<<endl<<endl<<"Some Processes are large to be executed through Round Robin and taking a lot of time"<<endl;
			  cout<<"Do you want to use another scheduling algorithm?"<<endl;
			  cout<<"If you want to use another Scheduling algorithm then Press 1, else if you want to continue then press 0:"<<endl<<">";
			  cin>>f;

			  if(f ==0)
			  {
				  choice++;
			  }

			  if(f==1)
			  {
				   sleep(800);
				  int k=0;
				  cout<<endl<<endl<<"Press 1 to continue with the FCFS,"<<endl;
				  cout<<" Press 2 to continue with Priority Scheduling,"<<endl;
				  cout<<"Press 3 to continue with Largest Job First";
				  cout<<endl<<">";
				  cin>>k;

				  if(k==1)
				  {
					   sleep(500);
					  this->FCFS1();
				  }

				  else if(k==2)
				  {
					   sleep(500);
					  this->Priority_scheduling1();
				  }

				  else if(k==3)
				  {
					   sleep(500);
					  this->Longest_Job_First_Scheduler();
				  }

				  break;
			  }

		  }

		  }
		  		 
		  if(curr1== NULL)
		  {
			  curr1= root;
			  pre = curr1;
			  choice++;
		  }

	  }

  }


//---------------------------------------------------------------------------------------------------------------------------------------------------


//  1. MultiLevel_Queue_Shceduler : 

  void MultiLevel_Queue_Shceduler()
  {
	  system("Color 09");

// In this we divide/seprate queues according to the Proocess type into several queues and then define different algorithms for every queue:

	  // So, we would divide our processses according to process type first
	  // making seprate queues for each process.
	  CPU_Scheduler system_process;
	  CPU_Scheduler interactive_process;
	  CPU_Scheduler batch_process;

	   sleep(1000);

	  cout<<endl<<endl<<"        Multi level Queue Scheduler Started: "<<endl;
	              cout<<"       ======================================  "<<endl;
	  Process * temp= root ;
	  int i=0;

	  while(temp != NULL)
	  {
		  // check what type of process is it?

		  if(temp->Process_Type[i] == 's')
		  {
			  system_process.Enqueue1(temp->Pname, temp->Btime, temp->Priority,temp->arrival_time, temp->Process_Type);
		  }

		  if(temp->Process_Type[i] == 'i')
		  {
			  interactive_process.Enqueue1(temp->Pname, temp->Btime, temp->Priority,temp->arrival_time, temp->Process_Type);
		  }
		  
		  if(temp->Process_Type[i] == 'b')
		  {
			  batch_process.Enqueue1(temp->Pname, temp->Btime, temp->Priority,temp->arrival_time, temp->Process_Type);
		  }

		  temp = temp->next;

	  }


	  // check out what queues we got.
	   sleep(1000);
	  cout<<endl<<"First of all, Making seprate queues for each Process Type:"<<endl;

	   sleep(1200);
	  cout<<endl<<endl<<"         System Processes: "<<endl;
	              cout<<"        ===================  "<<endl;
	  Process * temp5 = system_process.root;

	   sleep(1000);

	  while(temp5!=NULL)
	  {
		   sleep(500);
		   cout<<"Process Name: " ; 
		 
		   for(int i=0 ; temp5->Pname[i]!=NULL; i++)
		 {
			 cout<<temp5->Pname[i];
		 }
		cout<<endl;
		temp5= temp5->next;
	  }

	   sleep(1200);
	  cout<<endl<<endl<<"            Interactive Processes: "<<endl;
	              cout<<"           =======================  "<<endl;
	  Process * temp6 = interactive_process.root;

	   sleep(1000);

	  while(temp6!=NULL)
	  {
		   sleep(500);
		   cout<<"Process Name: " ; 
		 
		   for(int i=0 ; temp6->Pname[i]!=NULL; i++)
		 {
			 cout<<temp6->Pname[i];
		 }
		cout<<endl;
		temp6= temp6->next;
	  }
	   sleep(1200);
	  	  cout<<endl<<endl<<"            Batch Processes: "<<endl;
		              cout<<"           =================  "<<endl;
		  temp5 = batch_process.root;
		   sleep(1000);
	  while(temp5!=NULL)
	  {
		   sleep(500);
		   cout<<"Process Name: " ; 
		 
		   for(int i=0 ; temp5->Pname[i]!=NULL; i++)
		 {
			 cout<<temp5->Pname[i];
		 }
		cout<<endl;
		temp5= temp5->next;
	  }


	  // Now each algorithm will perform its execution using same or different scehduling algorithm.
	  
	  int a=0;
	  
	   sleep(1000);
	      cout<<endl<<"         System Type Process Scheduling:"<<endl<<endl;
		    	cout<<"         ===============================  "<<endl;

				 sleep(800);
	  cout<<endl<<"Click 1 to implement FCFS"<<endl<<"Click 2 to implement Priority Scheduling"<<endl<<"Click 3 to implement Round Robin"<<endl;
	  cout<<"Enter your choice: ";
	  cin>>a;
	  
	  if(a==1)
	  {
		   sleep(500);
		   system_process.FCFS();
	  }
	  
	  if(a==2)
	  {
		   sleep(500);
		   system_process.Priority_scheduling();
	  }

	  if(a==3)
	  {
		   sleep(500);
		  system_process.Round_robin();
	  }

	   sleep(1200);
  	  cout<<endl<<endl<<"         Interaction Type Process Scheduling:";
            	  cout<<"         ====================================  "<<endl;
	 sleep(800);
	cout<<endl<<endl<<"Click 1 to implement FCFS"<<endl<<"Click 2 to implement Priority Scheduling"<<endl<<"Click 3 to implement Round Robin"<<endl;
	cout<<endl<<"Enter your choice: ";
	cin>>a;
	  	 
	 if(a==1)
	  {
		   sleep(500);
		 interactive_process.FCFS();
	  }
	  
	  if(a==2)
	  {
		   sleep(500);
		  interactive_process.Priority_scheduling();
	  }

	  if(a==3)
	  {
		   sleep(500);
		  interactive_process.Round_robin();
	  }
	   sleep(1200);
	  cout<<endl<<endl<<"         Batch Type Process Scheduling:";
	              cout<<"         ==============================  "<<endl;
				   sleep(800);
    cout<<endl<<endl<<"Click 1 to implement FCFS"<<endl<<"Click 2 to implement Priority Scheduling"<<endl<<"Click 3 to implement Round Robin"<<endl;
	cout<<"Enter your choice: ";
	cin>>a;
	
	  if(a==1)
	  {
		   sleep(500);
				  batch_process.FCFS();
	  }
	  
	  if(a==2)
	  {
		   sleep(500);
		  batch_process.Priority_scheduling();
	  }

	  if(a==3)
	  {
		   sleep(500);
		  batch_process.Round_robin();
	  }


	  
  }



 // 2. MultiLevel_FeedBack_Queue_Scheduler()

  void  MultiLevel_FeedBack_Queue_Scheduler()
  {
	  // In this scheduling algorithm we divide our processes into several queues and if any process takes too much time to execute then it will be moved to 
	  // another scheduling algorithm.

	  system("Color 09");

	  CPU_Scheduler system_process;
	  CPU_Scheduler interactive_process;
	  CPU_Scheduler batch_process;
	   sleep(1500);
	  cout<<endl<<endl<<"         Multi Level FeedBack Queue Scheduling:"<<endl;
	              cout<<"         ======================================  "<<endl;
				   sleep(500);
				   Process * temp= root ;
	  int i=0;

	  while(temp != NULL)
	  {
		  // check what type of process is it?

		  if(temp->Process_Type[i] == 's')
		  {
			  system_process.Enqueue1(temp->Pname, temp->Btime, temp->Priority,temp->arrival_time, temp->Process_Type);
		  }

		  if(temp->Process_Type[i] == 'i')
		  {
			  interactive_process.Enqueue1(temp->Pname, temp->Btime, temp->Priority,temp->arrival_time, temp->Process_Type);
		  }
		  
		  if(temp->Process_Type[i] == 'b')
		  {
			  batch_process.Enqueue1(temp->Pname, temp->Btime, temp->Priority,temp->arrival_time, temp->Process_Type);
		  }

		  temp = temp->next;

	  }


	  // check out what queues we got.
	   sleep(1500);
	  cout<<endl<<"First of all, Making seprate queues for each Process Type:"<<endl;
	   sleep(500);
	    sleep(500);
	  cout<<endl<<endl<<"         System Processes: "<<endl;
	              cout<<"        ===================  "<<endl;
	  Process * temp5 = system_process.root;

	  while(temp5!=NULL)
	  {
		   sleep(800);
		   cout<<"Process Name: " ; 
		 
		   for(int i=0 ; temp5->Pname[i]!=NULL; i++)
		 {
			 cout<<temp5->Pname[i];
		 }
		cout<<endl;
		temp5= temp5->next;
	  }
	   sleep(500);
	    sleep(500);

	  cout<<endl<<endl<<"            Interactive Processes: "<<endl;
	              cout<<"           =======================  "<<endl;
	  Process * temp6 = interactive_process.root;

	  while(temp6!=NULL)
	  {
		   sleep(800);
		   cout<<"Process Name: " ; 
		 
		   for(int i=0 ; temp6->Pname[i]!=NULL; i++)
		 {
			 cout<<temp6->Pname[i];
		 }
		cout<<endl;
		temp6= temp6->next;
	  }
	   sleep(500);
	    sleep(500);
	  	  cout<<endl<<endl<<"            Batch Processes: "<<endl;
		              cout<<"           =================  "<<endl;
		  temp5 = batch_process.root;

	  while(temp5!=NULL)
	  {
		   sleep(500);
		    sleep(300);
		   cout<<"Process Name: " ; 
		 
		   for(int i=0 ; temp5->Pname[i]!=NULL; i++)
		 {
			 cout<<temp5->Pname[i];
		 }
		cout<<endl;
		temp5= temp5->next;
	  }


	  // Now each algorithm will perform its execution using same or different scehduling algorithm and many algorithms can be used during the execution.
	  
	  int a=0;
	   sleep(500);
	    sleep(500);
		 sleep(500);
	      cout<<endl<<"         System Type Process Scheduling:"<<endl<<endl;
		    	cout<<"         ===============================  "<<endl;
				 sleep(500);
	  cout<<endl<<"Click 1 to implement FCFS"<<endl<<"Click 2 to implement Priority Scheduling"<<endl<<"Click 3 to implement Round Robin"<<endl;
	  cout<<"Enter your choice: ";
	  cin>>a;
	  
	  if(a==1)
	  {
		   sleep(500);
				  system_process.FCFS1();
	  }
	  
	  if(a==2)
	  {
		   sleep(500);
		  system_process.Priority_scheduling1();
	  }

	  if(a==3)
	  {
		   sleep(500);
		  system_process.Round_robin1();
	  }
	   sleep(500);
	    sleep(500);
		 sleep(500);
  	  cout<<endl<<endl<<"         Interaction Type Process Scheduling:";
            	  cout<<"         ====================================  "<<endl;
				   sleep(500); 
				   sleep(500);
				  cout<<endl<<endl<<"Click 1 to implement FCFS"<<endl<<"Click 2 to implement Priority Scheduling"<<endl<<"Click 3 to implement Round Robin"<<endl;
	cout<<endl<<"Enter your choice: ";
	cin>>a;
	  	 
	 if(a==1)
	  { sleep(500);
		 interactive_process.FCFS1();
	  }
	  
	  if(a==2)
	  { sleep(500);
		  interactive_process.Priority_scheduling1();
	  }

	  if(a==3)
	  { sleep(500);
		  interactive_process.Round_robin1();
	  }
	   sleep(500);
	    sleep(500);
		 sleep(500);
	  cout<<endl<<endl<<"         Batch Type Process Scheduling:";
	              cout<<"         ==============================  "<<endl;
     sleep(500);
	  sleep(500);
				  cout<<endl<<endl<<"Click 1 to implement FCFS"<<endl<<"Click 2 to implement Priority Scheduling"<<endl<<"Click 3 to implement Round Robin"<<endl;
	cout<<"Enter your choice: ";
	cin>>a;
	
	  if(a==1)
	  { sleep(500);
				  batch_process.FCFS1();
	  }
	  
	  if(a==2)
	  { sleep(500);
		  batch_process.Priority_scheduling1();
	  }

	  if(a==3)
	  { sleep(500);
		  batch_process.Round_robin1();
	  }

  }

//  3. Longest_Job_First_Scheduler()

  void Longest_Job_First_Scheduler()
  {
	  system("Color 09");

	  int waiting_time = 0;
	  // LOngest Job First is the algorithm in which the longest process job is entertained first than the smallest jobs.
	  // It is Non preemptive 
	  // Longest Remaining time first is preemptive algorithm
	   sleep(500);
	   sleep(500);
	  cout<<endl<<"         Longest Job First Algorithm Started : ";
	        cout<<"         ====================================  "<<endl;
	  Process * curr1 = root;
	  Process * curr2 = curr1->next;
	  Process* Lar_Pro = new Process;

	
	  int lar = 0 ;
	  int count = 1 ;

	  while(curr1->next!=NULL)
	  {
		  lar= curr1->Btime;
		  curr2 = curr1->next;
		  while(curr2!=NULL)
		  {
			  if(lar<curr2->Btime)
			  { 
				  lar = curr2->Btime;
				  // Now Swapping process to take largest process to the first location
				  Lar_Pro->Btime = curr2->Btime;
				  Lar_Pro->Pname = curr2->Pname;
				  Lar_Pro->Priority = curr2->Priority;
				  Lar_Pro->remaining_burst_time = curr2->remaining_burst_time;


				  curr2->Pname = curr1->Pname;
				  curr2->Btime = curr1->Btime;
				  curr2->Priority = curr1->Priority;
				  curr2->remaining_burst_time = curr1->remaining_burst_time;


				  curr1->Pname = Lar_Pro->Pname;
				  curr1->Btime = Lar_Pro->Btime;
				  curr1->Priority = Lar_Pro->Priority;
				  curr1->remaining_burst_time = Lar_Pro->remaining_burst_time;

			  }

			  curr2 = curr2->next;
		  }
		  Finishing_time = Finishing_time + curr1->Btime;
		  curr1->waiting_time = Finishing_time - curr1->Btime-curr1->arrival_time;
		  curr1->remaining_burst_time = curr1->remaining_burst_time - curr1->Btime;
		   sleep(500);
		    sleep(300);
		  cout<<endl<<endl<<"Number "<<count<<" Largest Job Among all jobs is : ";
		  for(int i=0 ; Lar_Pro->Pname[i]!=NULL ; i++)
		  {
			  cout<<curr1->Pname[i];
		  }
		  cout<<endl<<"Burst Time: "<<curr1->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<curr1->remaining_burst_time;
		  cout<<endl<<" Finishing Time is: "<<Finishing_time;
		  cout<<endl<<"Waiting Time: "<<curr1->waiting_time;
		  count++;

			  curr1 = curr1->next;
	  }


	  Finishing_time = Finishing_time + front->Btime;
	  front->waiting_time = Finishing_time - front->Btime-front->arrival_time;
	    sleep(200);
	   sleep(500);
	   cout<<endl<<endl<<"Last Job Among all jobs is : ";
		  for(int i=0 ; Lar_Pro->Pname[i]!=NULL ; i++)
		  {
			  cout<< front->Pname[i];
		  }
		  cout<<endl<<"Burst Time: "<<front->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<front->remaining_burst_time;
		  cout<<endl<<" Finishing Time is: "<<Finishing_time;
		  cout<<endl<<"Waiting Time: "<<front->waiting_time;
		  count++;

  }

// 4. Highest_FeedBack_Ratio_Next_Scheduler()

  void  Highest_FeedBack_Ratio_Next_Scheduler()
  {
	  system("Color 09");
	   sleep(500);
	    sleep(500);
		 sleep(500);
	    cout<<endl<<"         Highest Feedback Ratio Next Scheduling Algorithm started:"<<endl;
              cout<<"         =========================================================  "<<endl;
	  // first of all calculate the waiting time and feedback ratio for each process
			   sleep(500);

	  Process * ptr = root;

	  int a=1;
	  while(ptr!=NULL)
	  {
		  // waiting time can be calculated in two ways i.e. 1. to use any previous Processes which have gone through any scheduling algorithm or assume
		  // waiting time for each process;

		  // I will use 2nd method .

		  Finishing_time = ptr->Btime +  a+a*3;

		  ptr->waiting_time = Finishing_time - ptr->Btime - ptr->arrival_time; 

		  ptr->feedback_ratio = (ptr->waiting_time + ptr->Btime) / ptr->Btime;

		  a++;
		  ptr= ptr->next;
	  }
	  
	  
	  // Now I can apply priority scheduling on the feedback ratio variable
	  
	Finishing_time = 0;	  
	
	  Process * curr1 = root;
	  Process * curr2 = curr1->next;
	  Process* Lar_Pro = new Process;

	
	  double lar = 0.000 ;
	  int count = 1 ;

	  while(curr1->next!=NULL)
	  {
		  lar= curr1->feedback_ratio;
		  curr2 = curr1->next;
		  while(curr2!=NULL)
		  {
			  if(lar<curr2->feedback_ratio)
			  { 
				  lar = curr2->feedback_ratio;
				  // Now Swapping process to take largest process to the first location
				  Lar_Pro->Btime = curr2->Btime;
				  Lar_Pro->Pname = curr2->Pname;
				  Lar_Pro->Priority = curr2->Priority;
				  Lar_Pro->remaining_burst_time = curr2->remaining_burst_time;
				  Lar_Pro->arrival_time = curr2->arrival_time;
				  Lar_Pro->feedback_ratio = curr2->feedback_ratio;

				  curr2->Pname = curr1->Pname;
				  curr2->Btime = curr1->Btime;
				  curr2->Priority = curr1->Priority;
				  curr2->remaining_burst_time = curr1->remaining_burst_time;
				  curr2->arrival_time = curr1->arrival_time;
				  curr2->feedback_ratio = curr1->feedback_ratio;

				  curr1->Pname = Lar_Pro->Pname;
				  curr1->Btime = Lar_Pro->Btime;
				  curr1->Priority = Lar_Pro->Priority;
				  curr1->remaining_burst_time = Lar_Pro->remaining_burst_time;
				  curr1->arrival_time = Lar_Pro->arrival_time;
				  curr1->feedback_ratio = Lar_Pro->feedback_ratio;

			  }

			  curr2 = curr2->next;
		  }
		  Finishing_time = Finishing_time + curr1->Btime;
		  curr1->waiting_time = Finishing_time - curr1->Btime-curr1->arrival_time;
		  curr1->remaining_burst_time = curr1->remaining_burst_time - curr1->Btime;
		   sleep(500);
		    sleep(300);
		  cout<<endl<<endl<<"Number "<<count<<" highest  Feed Back Priority process  is : ";
		  for(int i=0 ; curr1->Pname[i]!=NULL ; i++)
		  {
			  cout<<curr1->Pname[i];
		  }
		  cout<<endl<<"Priority: "<<curr1->Priority;
		  cout<<endl<<"Burst Time: "<<curr1->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<curr1->remaining_burst_time;
		  cout<<endl<<" Finishing Time is: "<<Finishing_time;
		  cout<<endl<<"Waiting Time: "<<curr1->waiting_time;
		  cout<<endl<<"Feed Back Ratio Time: "<<curr1->feedback_ratio;
count++;

			  curr1 = curr1->next;
	  }

	  // last process printing

	  Finishing_time = Finishing_time + front->Btime;
	  front->waiting_time = Finishing_time - front->Btime-front->arrival_time;
	   sleep(500);
	    sleep(300);
	  cout<<endl<<endl<<"Lowest Feed Back Priority Process Among all Processes is : ";
		  for(int i=0 ; front->Pname[i]!=NULL ; i++)
		  {
			  cout<< front->Pname[i];
		  }
		  cout<<endl<<"Priority: "<<front->Priority;
		  cout<<endl<<"Burst Time: "<<front->Btime;
		  cout<<endl<<"Remaining Burst Time: "<<front->remaining_burst_time;
		  cout<<endl<<" Finishing Time is: "<<Finishing_time;
		  cout<<endl<<"Waiting Time: "<<front->waiting_time;
		  cout<<endl<<"Feed Back Ratio Time: "<<curr1->feedback_ratio;
		  count++;

  }


  
  void Print1()
  {
	  system("Color 0B");
	  cout<<"         The Processes inside the CPU are: "<<endl;
	  cout<<"         =================================  "<<endl;
	  Process * curr = root;
	  string temp;

	  while(curr!=NULL)
	  {
		  sleep(700);

		 cout<<"Process Name: " ; 
		 temp = curr->Pname;
		 
		 for(int i=0 ; temp[i]!=NULL; i++)
		 {
			 cout<<temp[i];
		 }
		 
		  
		  cout<<endl<<"Burst Time: "<<curr->Btime<<endl<<"Priority: "<<curr->Priority<<endl;
		  cout<<"Process Type: ";
		  
		  for(int i=0 ; curr->Process_Type[i]!=NULL ; i++)
		  {
			  cout<<curr->Process_Type[i];
		  }
		  
		  cout<<endl<<endl;
		  curr = curr->next;
	  }


  }



  void Print()
  {
	  system("Color 0B");

	  cout<<"         The Processes inside the CPU are: "<<endl;
	  cout<<"         ==================================  "<<endl;
	  Process * curr = root;
	  string temp;

	  while(curr!=NULL)
	  {
		   sleep(700);
		 cout<<"Process Name: " ; 
		 temp = curr->Pname;
		 
		 for(int i=0 ; temp[i]!=NULL; i++)
		 {
			 cout<<temp[i];
		 }
		 
		  
		  cout<<endl<<"Burst Time: "<<curr->Btime<<endl<<"Priority: "<<curr->Priority<<endl<<endl;
		  curr = curr->next;
	  }


  }


  ~ CPU_Scheduler()
  {
	  Process * temp = root;

	  while(root!=NULL)
	  {
		  temp = root;
		  root = root->next;
		  delete temp;

	  }

  }


};


void cpuscheduling()
{
	// I assume that i have taken some different processes run and their burst time is calculated.
	
	// For Largest Job First:

		CPU_Scheduler CPU1;
	CPU1.Enqueue("P1" , 20 , 4, 0);    // name, burst time, priority, arrival time.
	CPU1.Enqueue("P2" , 30 , 10,2);
	CPU1.Enqueue("P3" , 25 , 18,6);
	CPU1.Enqueue("P4" , 40 , 2,20);
	CPU1.Enqueue("P5" , 50 , 9,25 );

	// CPU2 for multilevel queue.

	CPU_Scheduler CPU2;
	CPU2.Enqueue1("P1" , 30 , 4, 0 , "system process");    // name, burst time, priority, arrival time.
	CPU2.Enqueue1("P2" , 10 , 10,2, "system process");
	CPU2.Enqueue1("P3" , 28 , 18,6,"interactive process");
	CPU2.Enqueue1("P4" , 33 , 2,20,"system process");
	CPU2.Enqueue1("P5" , 40 , 9,25,"interactive process" );
	CPU2.Enqueue1("P6" , 58 , 8,25 ,"interactive process");
	CPU2.Enqueue1("P7" , 22 , 3 ,25 ,"batch process");
	CPU2.Enqueue1("P8" , 19 , 20,25 ,"batch process");
	CPU2.Enqueue1("P9" , 31 , 11,25 ,"batch process");

	// CPU3 for multilevel feedback queue.

	CPU_Scheduler CPU3;
	CPU3.Enqueue1("P1" , 30 , 4, 0 , "system process");    // name, burst time, priority, arrival time.
	CPU3.Enqueue1("P2" , 10 , 10,2, "system process");
	CPU3.Enqueue1("P3" , 28 , 18,6,"interactive process");
	CPU3.Enqueue1("P4" , 33 , 2,20,"system process");
	CPU3.Enqueue1("P5" , 40 , 9,25,"interactive process" );
	CPU3.Enqueue1("P6" , 58 , 8,25 ,"interactive process");
	CPU3.Enqueue1("P7" , 22 , 3 ,25 ,"batch process");
	CPU3.Enqueue1("P8" , 19 , 20,25 ,"batch process");
	CPU3.Enqueue1("P9" , 31 , 11,25 ,"batch process");


	// CPU4 for multilevel feedback Ratio queue.

	CPU_Scheduler CPU4;
	CPU4.Enqueue1("P1" , 30 , 4, 0 , "system process");    // name, burst time, priority, arrival time.
	CPU4.Enqueue1("P2" , 10 , 10,2, "system process");
	CPU4.Enqueue1("P3" , 28 , 18,6,"interactive process");
	CPU4.Enqueue1("P4" , 33 , 2,20,"system process");
	CPU4.Enqueue1("P5" , 40 , 9,25,"interactive process" );
	CPU4.Enqueue1("P6" , 58 , 8,25 ,"interactive process");
	CPU4.Enqueue1("P7" , 22 , 3 ,25 ,"batch process");
	CPU4.Enqueue1("P8" , 19 , 20,25 ,"batch process");
	CPU4.Enqueue1("P9" , 31 , 11,25 ,"batch process");


int k=0;

system("Color 0A");



while(k!=5)
{
	cout<<endl<<endl;
cout<<"                   CPU Scheduler:              "<<endl;
cout<<"                   ==============              "<<endl;
	
cout<<endl<<endl<<"Press 1 to Perform Multi Level Queue Scheduling";
cout<<endl<<"Press 2 to perform Multi Level FeedBack Queue Scheduling";
cout<<endl<<"Press 3 to perform Largest Job First Scheduling";
cout<<endl<<"Press 4 to perform Multi Level Feedback Ratio Schduling"<<endl;
cout<<"Press 5 to exit"<<endl;

cout<<endl<<">";
cin>>k;

if(k==1)
{
	CPU2.Print1();
	CPU2.MultiLevel_Queue_Shceduler();
}


else if(k==2)
{
	CPU3.Print1();
	CPU3.MultiLevel_FeedBack_Queue_Scheduler();
}

else if(k==3)
{
	CPU1.Print();
	CPU1.Longest_Job_First_Scheduler();
}

else if(k==4)
{
	CPU4.Print1();
	CPU4.Highest_FeedBack_Ratio_Next_Scheduler();
}

}


}


void secondChance (int frame, int a[], int size) {
	int frames[2][frame]; /* 2D array to store the page and bit */
	int pointer=0,h,x=0,col,row,faults=0,found=0,load=0;
	for (row=0; row<2; row++){ /* Set all array elements to 0 */
		for (col=0; col<frame; col++){
			frames[row][col] = 0;
		}
	}
	while (x < size) {
		for (h=0; h<frame; h++) { /* Check if page is in mem */
			if (frames[0][h] == a[x]){
				frames[1][h] = 1; /* set bit to 1 if in mem */
				found = 1;
			}
		}
		if (found == 0) {
			do {
				/* If bit is 0 or null, load in page */
				if (frames[1][pointer] == 0) {
					frames[0][pointer] = a[x]; /* load pages */
					frames[1][pointer] = 0; /* Set bit=0 b*/
					faults++;
					load = 1; /* load successful */
				}
				else { /* If bit is 1, set bit to 0 */
					frames[1][pointer] = 0;
				}
				pointer++; /* move pointer */
				if (pointer == frame) { pointer = 0;} /* Reset */
			} while (load != 1); 
		}
		found = 0; /* reset */
		load = 0; /* reset */
		x++;
	}
	printf("Number of page faults: %d\n",faults);
}



int num_of_pages;
int page_table_size;

class Page
{
public:
    int pageNum;
    int framenum;
    Page* nextpage;
};

class PageTable
{
public:
    Page** pageT;
    PageTable()
    {
        
        pageT = new Page * [page_table_size];
        
        for (int i = 0; i < page_table_size; i++) {
            pageT[i] = NULL;
        }
    }
    int pageTableFunc(int pageN)
    {
        return pageN % page_table_size;
    }
    void add(int key, int value)
    {
        
        int tableIndex = pageTableFunc(key);
        Page* entry = pageT[tableIndex];

       
        if (entry == NULL) {
        
            entry = new Page;
            
            entry->pageNum = key;
            entry->framenum = value;
            entry->nextpage = NULL;
            pageT[tableIndex] = entry;
           
        }



     
        else {
           
            while (entry != NULL)
            {
                entry = entry->nextpage;
            }
               

            entry = new Page;
            entry->pageNum = key;
            entry->nextpage = NULL;
          
            
        }
        cout << "Page with ID : " << key << " successfully added to page table." << endl;
        cout << "Table Index : " << tableIndex << endl;
        sleep(1000);
    }

    void find(int key)
    {
        
        int hash_val = pageTableFunc(key);
        bool flag = false;
        Page* entry = pageT[hash_val];

        
        if (entry != NULL) {
            while (entry != NULL) {
                if (entry->pageNum == key) {
                    flag = true;
                }
                if (flag) {
                    cout << "Frame number for page ID "  << key << " is : " << entry->framenum << endl;
                }
                entry = entry->nextpage;
            }
        }
        if (!flag)
            cout << "Frame number not found at page ID " << key << endl;
    }

};

void hashedPageTable()
{

	  int frameN = 2;
    char inst = 'o';
    cout << "Enter the number of PAGES to divide the process in : ";
    cin >> num_of_pages;
    cout << "Enter the page table size : ";
    cin >> page_table_size;
    cout << "Dividing process in " << num_of_pages << " pages.....\n";
    sleep(2000);
    PageTable P;
    

    for (int i = 0; i < num_of_pages; i++)
    {


        
        P.add(i, frameN);
        frameN += 2;
    }
    while (inst != 'e')
    {
        cout << "Enter page ID to get it's frame ID or 'e' to exit : ";
        cin >> inst;
        if(inst != 'e')
            P.find(inst - '0');
    }

    cout << "Ended!\n";
}

int main(){

    int getinp = 15;
    while(getinp != 0)
    {
        cout << "Enter 1 for CPU SCHEDULING.\n";
        cout << "Enter 2 for SEMAPHORES.\n";
        cout << "Enter 3 for DEADLOCKS.\n";
        cout<< "Enter 4 for Hashed Page Table."<<endl;
        cout << "Enter 0 to EXIT\n";

        cin >> getinp;

        if(getinp == 1)
        {
            cpuscheduling();
        }
        else if(getinp == 2)
        {
            semaphoresFunc();
        }   
        else if(getinp == 3)
        {
            deadlockFunc();
        }
        else if (getinp == 4)
        {
            hashedPageTable();
        }
        
    
        

    }
    return 0;
}
