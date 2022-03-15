#include <stdio.h>
#include<stdlib.h>

#ifndef EDH
#define EDH


/*Task Struct Definition*/
struct task {
	int task,
	arrival,
	 c,                                  //computation time of the task
	 period,
	 deadline,
	 energy,
	 next_end,                            // Deadline for the next cycle of the task p[i]
	 next_ar,                             // Arrival for the next cycle of the task p[i]
	 start_cal,                           //Current arrival time of the task
	 end_cal,                             // Currnet deadline of the task
	 ongoing,                             // Variable to check the completion of jobs
	 pending_computation,                             // Remaining Computational time of unfinished jobs
	 pending_energy;                      // Remaining energy of unfinished jobs (Premption case)
};
#define TURE 1
#define FALSE 0
#define true 1
#define false 0


/*Macros defined in trace function (States)*/
#define WAITING		0
#define CHECK_ENERGY	1
#define CHECK_SLACK_ENERGY    2
#define EXECUTE_JOB	3
#define CHECK_JOB_FINISH  4
#define EXECUTE_CYCLE_BY_CYCLE  5
#define CHECK_SLACK_TIME  6

// const int MAX = 10;
struct task p[10];


/**Variable Initialzation**/
extern int jobs;
extern int E0;                               //Initial energy in Battery at time 0
extern int Emax;                             //Maximum energy capacity of the battery
extern int pp;                               //The energy production power from the environment
extern int lcm;
extern int periodic;
extern int emin;                            // The maximum instantaneous consumption power
extern int premption_count;
extern int  ideal_count;
extern int energy_replenished;
extern int checker;

/*Variables for calculating Feasibility table*/
extern int old_arrive;                     // Checking the same arrival time  of tasks in taskset
extern int hw;                               // Procesor demand of the jobset
extern int sst_w;                            //Static slack time
extern int gw;                               // Energy Demand of the jobset
extern int sse;                              //Static slack energy
extern int is_feasibility_pass;



/*Fucntion Decalaration*/
int LCM();
void printJobs();
void inputTask();



int find_min_end();
int find_min_start();
void calculate_line(int t1, int t2);      //Calculating the Static slack time and Static slack energy for feasibilty table
void feasibilityChecks();                 //Function for checking the test for processor utilzation,energy utilzation  & Feasibilitytest
void generate_ftable();                   //Calculating the time intervals for checking feasibilty based on arrival time and deadline of tasks


void sort();                              //Sorting the priority of  task according to the earliest deadline first
void calculate_trace_updated();           //Scheduling the trace
void display_output(int time1[],int time2[],int task_id[]);
void schedule_output();




#endif //EDH_H_
