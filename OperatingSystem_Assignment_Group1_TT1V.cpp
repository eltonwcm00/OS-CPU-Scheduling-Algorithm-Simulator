//OperatingSystem TSN2101
//Group 1
//Tew Jing Lai 1181203035
//Elton Wong Chun Meng 1181203056
//Reynard Kok Jin Yik 1181203212

#include<iostream>
#include<algorithm>
using namespace std;

/** ==================================================== **/
/** 					Structure                         **/
/** ==================================================== **/

struct Global {
    char processname;
    int bursttime, arrivaltime, priority, totalArray[];
    int currenttime = 0, waitingtime = 0, respondtime = 0;
} global[500], global2nd[500], global3rd[500];

/** ==================================================== **/
/** 					Global Variable                  **/
/** ==================================================== **/

float totalTurnAround = 0, averageTurnAround = 0, averagewaitingtime = 0, totalwaitingtime = 0, averagerespondtime = 0, avgTT = 0, avgWT = 0, avgRT = 0;

/** ==================================================== **/
/** 					Function Prototype               **/
/** ==================================================== **/

// Prompt messasge
int insertNoProcess();
void insertProcess(int n);

// Non-preemptive && Preemptive
int non_preemptive_sjfDisplay(int n, int totalArray[]);
int preemptive_sjfDisplay(int nop, int queueTime);
int preemptive_priorityDisplay(int nop, int queueTime); 

bool bursttimeSort(Global global, Global globalCompare);
bool arrivaltimeSort(Global global, Global globalCompare);

// Preemptive
bool btimeOppSort(Global global, Global globalCompare);

// Preemptive Priority
bool PrioritySORT(Global global, Global globalCompare);

// Display Gantt Chart
void displayNP_SJFStats(int n, int totalArray[]);
void displayP_SJFStats(int n, int remainingTime, int totalArray[]);
void displayP_PriorityStats(int n, int remaintime, int totalArray[]);

/** ==================================================== **/
/** 					Function Definition              **/
/** ==================================================== **/

int insertNoProcess() {
    
    int n;
    
	cout << "Enter number of processes\n";
    cin >> n;
    
    return n;   
}

//Getting input from user
void insertProcess(int n) {
	
    for(int i=0; i<n; i++) {
        cout << "Please enter process name, arrival time and burst time for #"<<i+1<<endl;
        cin >> global[i].processname;
        cin >> global[i].arrivaltime;
        cin >> global[i].bursttime;

        global[i].waitingtime =- global[i].arrivaltime+1;
    }
}

//Getting input from user (PRIORITY)
void insertProcessPRIORITY(int n) {
	
    for(int i=0; i<n; i++) {
        cout << "Please enter process name, priority, arrival time and burst time for #"<<i+1<<endl;
        cin >> global[i].processname;
        cin >> global[i].priority;
        cin >> global[i].arrivaltime;
        cin >> global[i].bursttime;

        global[i].waitingtime =- global[i].arrivaltime+1;
    }
}

//Sorting burst time
bool bursttimeSort(Global global, Global globalCompare) {
    
	return global.bursttime < globalCompare.bursttime; 
}

//Sorting arrival time
bool arrivaltimeSort(Global global, Global globalCompare) {
    
    if(global.arrivaltime == globalCompare.arrivaltime)
    {
    	return global.bursttime < globalCompare.bursttime;
	}
    
	return global.arrivaltime < globalCompare.arrivaltime; 
}

//Sorting opposition burst time (preemptive sjf)
bool btimeOppSort(Global global, Global globalCompare) {
    
    if(global.bursttime != globalCompare.bursttime) {
        return global.bursttime > globalCompare.bursttime;
    }
    return global.bursttime < globalCompare.bursttime;
}

//Sorting priority (preemptive priority) 
bool prioritySort(Global global,Global global2nd){
    return global.priority < global2nd.priority; 
}

//Sorting PRIORITY AFTER ARRIVAL TIME ( PRE PRIORITY )
bool PrioritySORT(Global global, Global globalCompare)
{
	if(global.arrivaltime == globalCompare.arrivaltime)
	{
		if(global.priority == globalCompare.priority)
		{
			return global.bursttime < globalCompare.bursttime;
		}
		return global.priority < globalCompare.priority;
	}
	return global.arrivaltime < globalCompare.arrivaltime;
}

//Non-Preemptive algorithm
int non_preemptive_sjfDisplay(int n) {
	
	int totaltime = 0, i;
    int j, totalArray[n];
	
	//Decide ascending or descending order
    sort(global, global+n, bursttimeSort);
    sort(global, global+n, arrivaltimeSort);
    
    for(i=0; i<n ; i++) {

        j = i;
        while(global[j].arrivaltime <= totaltime && j!= n){
            j++;
        }

        sort(global+i, global+j, bursttimeSort);

        totalArray[i] = totaltime;
        totaltime += global[i].bursttime;
    }
    
    totalArray[i] = totaltime;
    
    cout<<"\nP.Name\tAT\tBT\tTA\tWT\n";
    
    for (i=0; i<n; i++){

        cout << global[i].processname << "\t"; 

        cout << global[i].arrivaltime << "\t";

        cout << global[i].bursttime << "\t";

        cout << totalArray[i] - global[i].arrivaltime + global[i].bursttime << "\t"; 
        totalTurnAround += totalArray[i] - global[i].arrivaltime + global[i].bursttime;

        cout << totalArray[i]-global[i].arrivaltime << "\t"; 
        totalwaitingtime += totalArray[i]-global[i].arrivaltime;

        cout <<"\n"; 
    }
    
    displayNP_SJFStats(n, totalArray);
}

//Preemptive algorithm
int k=0, full=0, remaining=0;

int preemptive_sjfDisplay(int nop, int queueTime) {
    
    int n = nop, queue;
    
    sort(global, global+n, arrivaltimeSort);

    int totalTime = 0;
    
    int j, i, totalArray[n];
    int allTime = 0;
    bool moveLast = false;

    for(i=0; i<n; i++) {
        allTime += global[i].bursttime;
    } 

    allTime += global[0].arrivaltime;

    for(i=0; totalTime<=allTime;) {
        j = i;
        
        while(global[j].arrivaltime<=totalTime && j!=n){
            global2nd[remaining] = global[j]; //53
            j++;
            remaining++;
        }

        if(remaining == full) {
            global3rd[k].processname = 'i';
            global3rd[k].bursttime = global[j].arrivaltime - totalTime;
            global3rd[k].arrivaltime = totalTime;

            totalTime += global3rd[k].bursttime;

            k++;
            continue;
        }
        
        i = j;

        if(moveLast == true) {
             sort(global2nd+full ,global2nd+remaining, bursttimeSort);    
        }

        j = full;

        if(global2nd[j].bursttime>queueTime) {
            global3rd[k] = global2nd[j];
            global3rd[k].bursttime = queueTime;

            k++;

            global2nd[j].bursttime = global2nd[j].bursttime-queueTime;
            
            totalTime += queueTime;  
            moveLast = true;

            for(queue=0; queue<n; queue++) {
                if(global2nd[j].processname != global[queue].processname) {
                    global[queue].waitingtime += queueTime;
                }
            }
            
        } else {
            global3rd[k] = global2nd[j];
            
            k++;
            full++;

            totalTime += global2nd[j].bursttime;  
            moveLast = false;

            for(queue=0; queue<n; queue++) {
                if(global2nd[j].processname != global[queue].processname) {
                    global[queue].waitingtime += global2nd[j].bursttime;
                }
            }

            if(full == remaining && i >= n) {
                break;
            }
        }
    }

    //
    totalArray[i] = totalTime;
    totalTime += global[i].bursttime;

    for(i=0; i<k-1; i++) {
        if(global3rd[i].processname == global3rd[i+1].processname){
            global3rd[i].bursttime += global3rd[i+1].bursttime;
            
            for(j=i+1;j<k-1;j++) {
                global3rd[j] = global3rd[j+1];
            }
            
            k--;
            i--;
        }
    }

    //
    int remainingTime = 0;
    for(j=0; j<n; j++) {
        remainingTime = 0;
        
        for(i=0; i<k; i++){
            if(global3rd[i].processname == global[j].processname){
                global[j].respondtime = remainingTime;
                break;
            }
            remainingTime += global3rd[i].bursttime;
        }
    }
    
    cout<<"\nGantt Chart\n";
    
    remainingTime=0;

    for (int i=0; i<k; i++){
        
        if(i!= k) {
            cout<<"|  "<<'P'<< global3rd[i].processname << "   "; 
        }

        remainingTime += global3rd[i].bursttime;
        
        for(int j=0; j<n; j++){
            
            if(global[j].processname == global3rd[i].processname) {
                global[j].currenttime = remainingTime;
            }
        } 
    }
    cout<<"\n";
    
    remainingTime=0;

    for (int i=0; i<k+1; i++) {
        cout << remainingTime << "\t";
        totalArray[i] = remainingTime;
        remainingTime += global3rd[i].bursttime; 
    }
	
    //
    cout<<"\nP.Name  AT\tBT\tCT\tTAT\tWT\tRT\n";

    for (i=0; i<nop && global[i].processname!='i'; i++) {
        if(global[i].processname=='\0') {
             break;
        }

        cout <<'P'<< global[i].processname << "\t"; 
        cout << global[i].arrivaltime << "\t";
        cout << global[i].bursttime << "\t";
        cout << global[i].currenttime << "\t"; 
        cout << global[i].waitingtime + global[i].currenttime - remainingTime + global[i].bursttime << "\t"; 
        
        averageTurnAround += global[i].waitingtime + global[i].currenttime - remainingTime + global[i].bursttime;
        cout << global[i].waitingtime + global[i].currenttime - remainingTime << "\t"; 
        
        averagewaitingtime += global[i].waitingtime + global[i].currenttime - remainingTime;
        cout << global[i].respondtime - global[i].arrivaltime << "\t";  
        
        averagerespondtime += global[i].respondtime - global[i].arrivaltime;
        cout <<"\n"; 
    }
    
 	 displayP_SJFStats(n, remainingTime, totalArray);
    
}



//Pre-emptive Priority Algorithm



int preemptive_priorityDisplay(int nop, int queueTime) 
{	
int k=0,full=0,remain=0;

	int n=nop,q;
    sort(global,global+n,PrioritySORT);
    int totaltime=0;
    int i,j,totalArray[n];
    int alltime=0;
    
    bool moveLast=false;
    
    for(i=0;i<n;i++){
        alltime+=global[i].bursttime;
    }
    
    alltime+=global[0].arrivaltime;
    
    for(i=0;totaltime<=alltime;)
	{
        j=i;
        while(global[j].arrivaltime<=totaltime&&j!=n)
		{
            global2nd[remain]=global[j];
            j++;
            remain++;
        }
        
        if(remain==full)
		{
            global3rd[k].processname='i';
            global3rd[k].bursttime=global[j].arrivaltime-totaltime;
            global3rd[k].arrivaltime=totaltime;
            totaltime+= global3rd[k].bursttime;
            k++;
            continue;
        }
        
		i=j;
        
        if(moveLast==true){
            sort(global2nd+full, global2nd+remain, prioritySort);
        }

        j=full;
        
        if(global2nd[j].bursttime>queueTime)
		{
            global3rd[k]=global2nd[j];
            global3rd[k].bursttime=queueTime;
            k++;
            global2nd[j].bursttime=global2nd[j].bursttime-queueTime;
            totaltime+=queueTime;  
            moveLast=true;
            for(q=0;q<n;q++)
			{
                if(global2nd[j].processname!=global[q].processname)
				{
                    global[q].waitingtime+=queueTime;
                }
            }
        }
        else
		{
            global3rd[k]=global2nd[j];
            k++;
            full++;
            totaltime+=global2nd[j].bursttime;  
            moveLast=false;
            for(q=0;q<n;q++)
			{
                if(global2nd[j].processname!=global[q].processname)
				{
                    global[q].waitingtime+=global2nd[j].bursttime;
                }
            }
        }
        if(full==remain && i>=n)
        break;
    }
    
    totalArray[i]=totaltime;
    totaltime+=global[i].bursttime;
    
    for(i=0;i<k-1;i++)
	{
        if(global3rd[i].processname==global3rd[i+1].processname)
		{
            global3rd[i].bursttime+=global3rd[i+1].bursttime;
            for(j=i+1;j<k-1;j++)
                global3rd[j]=global3rd[j+1];
            k--;
            i--;
        }
    }
    
    int remaintime=0;
    for(j=0;j<n;j++)
	{
        remaintime=0;
        for(i=0;i<k;i++)
		{
            if(global3rd[i].processname==global[j].processname)
			{
                global[j].respondtime=remaintime;
                break;
            }
            remaintime+=global3rd[i].bursttime;
        }
    }
    
    cout<<"\nGantt Chart\n";
    remaintime=0;
    
	for (i=0; i<k; i++)
	{
        if(i!=k)
	        cout<<"|  "<<'P'<< global3rd[i].processname << "   "; 
	        remaintime+=global3rd[i].bursttime;
        
        for(j=0;j<n;j++)
		{
            if(global[j].processname==global3rd[i].processname)
                global[j].currenttime=remaintime;
        } 
    }
    
    cout<<"\n";
    
	remaintime=0;
    
	for (i=0; i<k+1; i++)
	{
        cout << remaintime << "\t";
        totalArray[i]=remaintime;
        remaintime+=global3rd[i].bursttime; 
    }

    cout<<"\n";
    cout<<"\n";
    cout<<"P.Name Priority AT\tBT\tCT\tTAT\tWT\tRT\n";

    for (i=0; i<nop && global[i].processname!='i'; i++)
	{
        if(global[i].processname=='\0')
        {
        	break;
		}   
        cout <<'P'<< global[i].processname << "\t"; 
        cout << global[i].priority << "\t";
        cout << global[i].arrivaltime << "\t";
        cout << global[i].bursttime << "\t";
        cout << global[i].currenttime << "\t"; 
        cout << global[i].waitingtime+global[i].currenttime-remaintime+global[i].bursttime << "\t"; 
        
		avgTT+=global[i].waitingtime+global[i].currenttime-remaintime+global[i].bursttime;
        cout << global[i].waitingtime+global[i].currenttime-remaintime << "\t"; 
        
		avgWT+=global[i].waitingtime+global[i].currenttime-remaintime;
        cout << global[i].respondtime-global[i].arrivaltime << "\t";  
        
		avgRT+=global[i].respondtime-global[i].arrivaltime;
        cout <<"\n"; 
    }
    
    cout<<"\n";
	
	cout<<"\nTotal Turnaround time: " << (float)avgTT <<endl; 
	cout<<"Average Turnaround time: "<<(float)avgTT/(float)n<<endl;
	cout<<"Total Waiting time: "<<(float)avgWT <<endl;
	cout<<"Average Waiting time: "<<(float)avgWT/(float)n<<endl; 
    
//    displayP_PriorityStats(n, remaintime, totalArray);
}


//Display overall Non-preemptive SJF stats
void displayNP_SJFStats(int n, int totalArray[]) {
	
	cout<<"\nGantt Chart\n";

    for (int i=0; i<n; i++){
         cout <<"|   "<< global[i].processname << "   "; 
    }

     cout<<"\n";
    
     for (int i=0; i<n+1; i++){
         cout << totalArray[i] << "\t"; 
     }
    
     cout<<"\n";
    	
     cout<<"\nTotal Turnaround time: " << (float)totalTurnAround <<endl;
     cout<<"Average Turnaround time: " << (float)totalTurnAround / (float)n <<endl;
     cout<<"Total Waiting time: " << (float)totalwaitingtime <<endl;
     cout<<"Average Waiting time: " << (float)totalwaitingtime / (float)n <<endl;
}

//Display overall Preemptive SJF stats
void displayP_SJFStats(int n, int remainingTime, int totalArray[]) {
    
    cout<<"\n";

    cout<<"\nTotal Turnaround time: " << (float)averageTurnAround <<endl;
    cout<<"Average Turnaround time: "<<(float)averageTurnAround/(float)n<<endl;
    cout<<"Total Waiting time: " << (float)averagewaitingtime <<endl;
    cout<<"Average Waiting time: "<<(float)averagewaitingtime/(float)n<<endl;
}


int main() {
	
	int choice = 0, nop, n, queueTime;
	
	do {
		
		cout << "========================================" << "\n";
		cout << "Process Scheduling Algorithm Stimulation" << "\n";
		cout << "========================================" << "\n\n";
		cout << "1. Non-Preemptive Shortest Job First"<<endl;
		cout << "2. Preemptive Shortest Job First"<<endl;
		cout << "3. Preemptive Priority"<<endl;	
		cout << "\n\nPlease select which process scheduling algorithm you would like to stimulate" << "\n\n";
		
		cin >> choice;
		
	} while(choice <=0 || choice >=4);
	
	switch(choice) {
		case 1 : cout << "\nNon-Preemptive Shortest Job First\n";
				 cout << "================================="<<endl;

                 n = insertNoProcess();  
				  			
				 insertProcess(n);
    			 non_preemptive_sjfDisplay(n);
    			
				 break;
				 
		case 2 : cout << "\nPreemptive Shortest Job First\n";
				 cout << "================================="<<endl;
				 
				 n = insertNoProcess();  
				  			
				 insertProcess(n);
    			 preemptive_sjfDisplay(n, queueTime);
    			 
				 break;
				 
		// Reynard punya part
		case 3 : cout << "\nPreemptive Priority\n";
				 cout << "================================="<<endl;
				 
				 n = insertNoProcess();
				 
				 insertProcessPRIORITY(n);
				 preemptive_priorityDisplay(n, queueTime);
				 
				 break;
	}
	
}
