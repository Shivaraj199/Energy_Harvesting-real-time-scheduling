#include"EDH.h"
/*Calculating the scheduling of the task*/
int E0=0;
int Emax=0;
int emin=0;
int gw=0;
int is_feasibility_pass = true;
int jobs = 0;
int pp = 0;
int lcm = 0;
int periodic = true;
int old_arrive = 100;
int sst_w = 0;
int sse = 0;
int hw=0;
int energy_replenished =0;
int premption_count=0;
int ideal_count=0;


void calculate_trace_updated()
{

	for (int i = 0; i < jobs; i++)
	{
		p[i].pending_computation = 0;                      //Initialzing all the task of taskset as a fresh task

	}
	//sort(p);
	int state = 0;                          // Starting the scheduling from the waiting state
	int energy = E0;
	int remain_energy = 0;
	int slack_energy = 0;
	int EDF = 0;
	int deadline;
	int total_energy_req;
	int total_reacharge;
	int minSlack = 10000;                   //some high number,i.e Slacktime !=0
	int slackChecked = 0;
	int slackCheckedTime = 0;
	int count=0;
    int time_1[lcm];
    int	time_2[lcm];
    int	task_id[lcm];

	for (int t = 0; t < lcm;)               //Calculating for each time till the Hyperperiod
	{

		switch (state)
		{
		case WAITING:                          //State to check the if high priority task is arrived and ready to execute or not yet arrived.
    	sort(p);
			EDF = 0;                             // Task sorted according to the highest priority and p.[0] index being the highest priority task
			//printf("time:%d EDF:%d arrival:%d t:%d\n", t, EDF, p[EDF].arrival, t);

			if (p[EDF].arrival > t)              // Checking if the High priority task p[0] is released after the current time
			{
			    printf("NOTE: time %2d-%2d Preemption:High priority job is released after the current time\n", t, t + 1);
				for (int i = 1; i < jobs; i++)
				{
					if (p[i].arrival <= t)           // Checking if there are other low priority task released before or at the current time
					{
						EDF = i;
						printf("NOTE: Feasible job found with Arrival time:%d Deadline:%d\n", p[EDF].arrival, p[EDF].deadline);
						if (p[0].ongoing==0)
						{
							premption_count++;
							printf("premption_count=%d\n",premption_count);
						}

						break;
					}
					if (i == jobs - 1){
						printf("NOTE: Feasible event not found. Waiting for recharge cycle...\n");

					}
				}
			}

			if (p[EDF].arrival <= t)            // EDF logic picked some earliest deadline job and it has already arrived
				if (EDF == 0)                     // Checking if the task currently ready is high priority.
					state = CHECK_ENERGY;           //a non-premeption task
				else                              // Checking if the task released is the high priority-1.
					state = CHECK_SLACK_ENERGY;     // pre-emption task

			else                                // No task is ready in the runqueue, idle time for recharging
			{
				//printf(" energy:%d max:%d\n",energy,Emax);
				if ((energy + pp) >= Emax)
				{
					energy = Emax;
					printf("\n\t\t\t\t\t ACTION: time %2d-%2d RECHARGING, REACHED BATTERY MAX CAPACITY |-> ENERGY:%d\n", t, t + 1, energy);
                    energy_replenished++;
					ideal_count++;
				}
				else
				{
					energy += pp;
					printf("\n\t\t\t\t\t ACTION: time %2d-%2d RECHARGING CYCLE |-> ENERGY:%d\n", t, t + 1, energy);
				    ideal_count++;
				}
				t++;                                //increment time until job is arrived
			}

			for (int i = 0; i < jobs; i++)
			{
				if (t >= p[i].deadline)            //checking for incomplete tasks
				{
					printf("FATAL: time %2d , Task %2d could not completed by %2d. Exiting simulation \n", t, p[i].task, p[i].deadline);
					exit(1);
				}
			}
			break;

		case CHECK_ENERGY:

			if (energy == 0)                      // If the energy in the battery is completely deplenished calculating the stack time
			{
				printf("Note: time %2d-%2d trying to execute task %d |-> Energy  INITIAL:%d , initial energy is zero. Calculating slack time  \n", t, t + 1, p[EDF].task, energy);
				if (slackChecked)                   // True t check the time
				{
					if (slackCheckedTime == t)        // Checking if slack time is calculated at the current time
					{
						printf("Note: Slack time was already calculated for time %d, advancing the time.\n", t);
						if ((energy + pp) >= Emax)      // If the battery reaches its max energy capacity
						{
							energy = Emax;
							printf("\n\t\t\t\t\t ACTION: time %2d-%2d RECHARGING, REACHED BATTERY MAX CAPACITY |-> ENERGY:%d\n", t, t + 1, energy);
						}
						else
						{
							energy += pp;                 // Recharging for one time period
							printf("\n\t\t\t\t\t ACTION: time %2d-%2d RECHARGING CYCLE |-> ENERGY:%d\n", t, t + 1, energy);
						}
						t++;
					}
				}
				state = CHECK_SLACK_TIME;
				break;
			}

			if (p[EDF].ongoing == 0)                // checking  whether active job is a freshjob is ready to execute
			{
				remain_energy = energy + (pp * p[EDF].c) - p[EDF].energy;
				printf("Note: time %2d-%2d trying to execute task %d |-> Energy  (INITIAL:%d  +  RECHARGE:%d) -  (REQUIRED:%d) = %d energy units \n", t, t + 1, p[EDF].task, energy, (pp * p[EDF].c), p[EDF].energy, remain_energy);
				if (remain_energy > Emax)              // If the battery reaches its max energy capacity
				{
					remain_energy = Emax;
					energy_replenished++;
				}
				else
				{
					if(energy==Emax)
					{
						energy_replenished++;
					}
				}
			}
			else
			{
				remain_energy = energy + (pp * (p[EDF].c - p[EDF].pending_computation)) - (p[EDF].energy - p[EDF].pending_energy);
				printf("Note: time %2d-%2d trying to execute pending task %d |-> Energy  (INITIAL:%d  +  RECHARGE:%d) -  (REQUIRED:%d) = %d energy units \n", t, t + 1, p[EDF].task, energy, (pp * (p[EDF].c - p[EDF].pending_computation)), (p[EDF].energy - p[EDF].pending_energy), remain_energy);
				if (remain_energy > Emax)                // If energy in the battery is greater than its MAX_Capacity
				{
					remain_energy = Emax;
				    energy_replenished++;
				}
			}

			if (remain_energy < 0)                    // Checking if the energy in the battery after the execution of the job
			{
				printf("Note: Energy Starvation detected, waiting for recharge cycle. \n");
				printf("\n\t\t\t\t\t ACTION:  time %2d-%2d RECHARGING CYCLE |-> ENERGY:%d\n", t, t + 1, energy);
				energy += pp;
				t++;
				ideal_count++;
				state = WAITING;
			}
			else if (energy < ((p[EDF].energy / p[EDF].c) - pp))        // Checking if the energy in battery is sufficinet for the execution
			{
				printf("Note: Initial energy capacity not enough, waiting for recharge cycle. \n");
				printf("\n\t\t\t\t\t ACTION:  time %2d-%2d RECHARGING CYCLE |-> ENERGY:%d\n", t, t + 1, energy);
				energy += pp;
				t++;
				ideal_count++;
				state = WAITING;
			}
			else if (energy < emin)               // Checking is energy in battery is <  maximum instantaneous consumption power e_max
			{
				printf("Note: Initial energy capacity less than emax, waiting for recharge cycle. \n");
				printf("\n\t\t\t\t\t ACTION:  time %2d-%2d RECHARGING CYCLE |-> ENERGY:%d\n", t, t + 1, energy);
				energy += pp;
				t++;
				ideal_count++;
				state = WAITING;
			}
			else
			{
				state = EXECUTE_JOB;
			}
			break;

		case EXECUTE_JOB:
			p[EDF].ongoing = 1;                   //Updating the varible the job is not freshly arrived
			p[EDF].pending_computation = p[EDF].pending_computation + 1;  // Updating the job is finished atleast one computional time
			printf("\n\t\t\t\t\t ACTION: time %2d-%2d EXECUTING TASK ->>> %2d \n", t, t + 1, p[EDF].task);
            int task_no=p[EDF].task;
			t++;
			if (p[EDF].pending_computation < p[EDF].c)
			{        // If the job has finished its computation time
				state = EXECUTE_JOB;
                time_1[count]=t-1;
				time_2[count]=t;
	            task_id[count]=task_no;
			    count++;
			}
			else                                  // The job has finished its execution for the period
			{
				p[EDF].arrival += p[EDF].period;   // Updating the arrival time of task  for  execution of periodic job in the next period
				p[EDF].deadline += p[EDF].period;  // Updating the deadline time of task  for  execution of periodic job in the next period
				p[EDF].ongoing = 0;                // reseting the vvaribales as the job is finished execution
				p[EDF].pending_computation = 0;
				energy = remain_energy;
				//printf("done!!!!!");
				state = WAITING;
				printf("\n\t\t\t\t\t ACTION: time %2d COMPLETION OF TASK %d\n\n", t, p[EDF].task);
				time_1[count]=t-1;
				time_2[count]=t;
				task_id[count]=task_no;
			count++;
	     }
			 display_output(time_1,time_2,task_id);
			break;


		case CHECK_SLACK_ENERGY:
			deadline = p[EDF].deadline;
			total_energy_req = p[EDF].energy;
			total_reacharge = pp * (deadline - t);

			for (int i = 0; i < jobs; i++)
			{
				if (i == EDF)continue;               // Checking the energy consumption of the current task
				if (p[i].deadline < deadline)        // are there any deadlines prior to this case for current time
				{
					total_energy_req += p[i].energy;   // Adding all the energy consumption of task with deadline at or before the deadlne of current job
				}
			}
			slack_energy = energy + total_reacharge - total_energy_req;
    //printf("Note: time %2d-%2d, Slack energy=%d + %d - %d =%d\n", t, t + 1, energy, total_reacharge, total_energy_req, slack_energy);

			if (slack_energy <= 0 && energy != Emax)  //slack energy is not enough so we recharge
			{
				printf("Note: Slack energy zero detected, battery not full charged, so recharging  for 1 cycle\n");
				//printf("\n\t\t\t\t\t ACTION:  time %2d-%2d RECHARGING CYCLE |-> ENERGY:%d\n", t, t + 1, energy);
				energy += pp;
				ideal_count++;
				if (energy >= Emax)
					energy = Emax;
				t++;
				state = WAITING;
			}
			else if (slack_energy <= 0 && energy == Emax)//slack is not enough but battery full. So here we will try execute by the job in individual steps
			{
				printf("Note: Slack energy zero detected, but battery full charged, so executing job %d for 1 cycle\n", p[EDF].task);
				state = EXECUTE_CYCLE_BY_CYCLE;
				if(energy>=Emax)
				{
					energy_replenished++;
				}
			}
			else if (slack_energy > 0 && energy != Emax)  // Slack is positive , Task can be executed hence goes to waiting state nad reacharges for one time period
			{
				printf("Note: Positive Slack energy detected, battery not full charged, so recharging  for 1 cycle\n");
				printf("\n\t\t\t\t\t ACTION:  time %2d-%2d RECHARGING CYCLE |-> ENERGY:%d\n", t, t + 1, energy);
				energy += pp;
				ideal_count++;
				t++;
				state = WAITING;
			}
			else                                       // Authorized to execute task if the enery in tht battery is sufficient
			{
				printf("Note: Positive Slack energy detected, battery full charged, proceeding to process execution\n");
				state = CHECK_ENERGY;
			}
			break;

		case EXECUTE_CYCLE_BY_CYCLE:
			p[EDF].ongoing = 1;					         	// will indicate that job is not finished in this cycles
			p[EDF].pending_computation = p[EDF].pending_computation + 1;	// keep track how many executions of this job is done
			p[EDF].pending_energy = p[EDF].pending_energy + p[EDF].energy / p[EDF].c; // the energy of the job that is already consumed
			printf("\n\t\t\t\t\t ACTION:  time %2d-%2d EXECUTING TASK ->>> %2d \n", t, t + 1, p[EDF].task);
			task_no=p[EDF].task;
			t++;
			energy = energy - p[EDF].energy / p[EDF].c + pp;

			if (p[EDF].pending_computation < p[EDF].c)
				{       // job not finished
					state = CHECK_SLACK_ENERGY;        //  check the slack Energy
					time_1[count]=t-1;
					time_2[count]=t;
					task_id[count]=task_no;
					count++;
				}
			else
				{
					p[EDF].arrival += p[EDF].period;
					p[EDF].deadline += p[EDF].period;
					p[EDF].ongoing = 0;
					p[EDF].pending_computation = 0;
					state = WAITING;
					printf("\n\t\t\t\t\t ACTION: time %2d COMPLETION OF TASK %d\n\n", t, p[EDF].task);
					time_1[count]=t-1;
					time_2[count]=t;
					task_id[count]=task_no;
					count++;
				}
		display_output(time_1,time_2,task_id);
			break;

		case CHECK_SLACK_TIME:

			for (int i = 0; i < jobs; i++)
			{
				if (p[i].arrival <= t)
				{
					printf("Note: Task %2d is active for slack time calculation\n", p[i].task);

					int deadline = p[i].deadline;
					int Ati = 0;
					int slackTime = 0;
					//printf("Note: finding all other jobs before this deadline\n");
					for (int j = 0; j < jobs; j++)
					{
						//printf("Note: job %2d , deadline %2d, checking for %2d\n",p[j].task,p[j].deadline,deadline);
						if (p[j].deadline <= deadline)   // Calcualting the computation time of all jobs before the deadline
							Ati += p[j].c;
					}
					slackTime = deadline - t - Ati;
					printf("Note: Slack time for task %2d at time %2d is D-T-ATi = %2d - %2d -%2d=%2d\n", p[i].task, t, deadline, t, Ati, slackTime);
					if (slackTime < minSlack)
						minSlack = slackTime;
				}
			}

			if (energy >= Emax)
			{
				printf("Note: Battery Fully charged. Executing jobs again.\n");
				state = WAITING;
				slackChecked = 1;
				slackCheckedTime = t;
				//t++;
			}
			else if (minSlack == 0)
			{
				printf("Note: Slack time is zero. Executing jobs again\n");
				state = WAITING;
				slackChecked = 1;
				slackCheckedTime = t;
				//t++;
			}
			else
			{
				state = CHECK_SLACK_TIME;
				printf("Note: Slack time :%2d , Processor can be idled from %2d to %2d, recharging for one more cycle.\n", minSlack, t, t + minSlack);
				printf("\n\t\t\t\t\t ACTION:  time %2d-%2d RECHARGING CYCLE |-> ENERGY:%d\n", t, t + 1, energy);
				energy += pp;
				ideal_count++;
				t++;
			}

			///while (1);
			break;
		}
		//	printf("state:%d\n",state);


	}

}


void generate_ftable()
{
	for (int i = 0; i < jobs; i++)
	{
		p[i].next_ar = p[i].arrival;
		p[i].next_end = p[i].arrival + p[i].deadline;
	}

	int pending_next_arrive = true;
	int pending_same_arrive = true;


	printf("\n---------------------------------------------------------------------------------------------------------------\n");
	printf("%10s |%10s |%10s |%10s \n", "Time(t1,t2)", "hw(t1,t2)", "SST(t1,t2)", "SSE(t1,t2)");
	printf("---------------------------------------------------------------------------------------------------------------\n");

	while (pending_next_arrive)                              // Updating the lower limit once the calculation of all intervals is finished
	{

		int start_index = find_min_start();
		if (p[start_index].next_ar >= lcm)                     //Checking if the same lower limit of the time interval
		{
			pending_next_arrive = false;
			break;
		}
		else if (old_arrive == p[start_index].next_ar)          //checking if the arrival time of tasks  are same in taskset
		{
			p[start_index].next_ar += p[start_index].period;
		}
		else
		{
			for (int i = 0; i < jobs; i++)
				p[i].next_end = p[i].deadline;

			pending_same_arrive = true;

			while (pending_same_arrive)                            // Checking the lower limit of time interval
			{

				int end_index = find_min_end();
				if (p[end_index].next_end >= (lcm))
				{
					pending_same_arrive = false;
					old_arrive = p[start_index].next_ar;

					if (p[end_index].next_end == (lcm) && p[start_index].next_ar < p[end_index].next_end)
					{
						calculate_line(p[start_index].next_ar, p[end_index].next_end);      // passing the argument for the lower and upper time interval
					}
					//	std::cout << "break" << std::endl;
				}
				else
				{
					if (p[start_index].next_ar < p[end_index].next_end)
					{
						calculate_line(p[start_index].next_ar, p[end_index].next_end);      // passing the argument for the lower and upper time interval
					}
					p[end_index].next_end += p[end_index].period;
				}
			}
			p[start_index].next_ar += p[start_index].period;

		}
	}
}

/*Feasibility table calculation
t1: First time interval
t2: Second time interval
Calculating the Static slack energy and static slack time
*/
void calculate_line(int t1, int t2)
{
	hw = 0;
	gw = 0;

	for (int i = 0; i < jobs; i++)
	{
		p[i].start_cal = p[i].arrival;
		p[i].end_cal = p[i].deadline;
	}

	for (int i = 0; i < jobs; i++)
	{
		while (p[i].start_cal < t1 && periodic == true)
		{
			p[i].start_cal += p[i].period;
			p[i].end_cal += p[i].period;
		}
		//			while (p[i].start_cal >= t1 && p[i].end_cal <= t2)
		//			for (int i = 0; i < jobs; i++)
		//				std::cout << "i:" << i << " next ar:" << p[i].next_ar << " next_end:" << p[i].next_end << std::endl;
		while (p[i].start_cal >= t1 && p[i].end_cal <= t2)
		{
			//				std::cout << "using index " << i << std::endl;
			hw += p[i].c;                                        //Adding the computation time of all task within the upper and lower time intervals
			gw += p[i].energy;                                   //Adding the energy consumption of all task within the upper and lower time intervals
			p[i].start_cal += p[i].period;
			p[i].end_cal += p[i].period;
		}
		sst_w = t2 - t1 - hw;                       // Formula for static slack time
		sse = E0 + pp * (t2 - t1) - gw;             // Formula for static slack energy
	}

	if (sse < 0 || sst_w < 0)
		is_feasibility_pass = false;
	printf("%5d -%4d |%9d |%9d |%9d |%9d \n", t1, t2, hw, sst_w, gw, sse);
}



/*Updating the deadline (upper limit) for the fesibilty checks*/
int find_min_end()
{
	int index = 0;
	int min = p[0].next_end;
	for (int i = 1; i < jobs; i++)
	{
		if (p[i].next_end < min)
		{
			index = i;
			min = p[i].next_end;
		}
	}
	return index;
}

/*Updating the arrrival time (lower limit) for the feasibilty checks*/
int find_min_start()
{
	int index = 0;
	int min = p[0].next_ar;
	for (int i = 1; i < jobs; i++)
	{
		if (p[i].next_ar < min)
		{
			index = i;
			min = p[i].next_ar;
		}
	}
	return index;
}



/*FUnction to sort the jobs according to the earliest deadline*/
void sort() {
	int i, j;
	for (i = 0; i < jobs - 1; i++) {
		for (j = i + 1; j < jobs; j++) {
			if (p[i].deadline == p[j].deadline) {
				if (p[i].arrival > p[j].arrival)
				{
					struct task process = p[i];
					p[i] = p[j];
					p[j] = process;
				}
			}
			else if (p[i].deadline > p[j].deadline) {
				struct task process = p[i];
				p[i] = p[j];
				p[j] = process;
			}
		}
	}
}

/*Calculating the Hyperperiod for the Scheduling*/
int LCM()
{
	int i, max, lcm;
	lcm = p[0].period;
for (i = 1; i < jobs; i++)
	{
		max = (lcm > p[i].period) ? lcm : p[i].period;
		while (1)
		{
			if (max % lcm == 0 && max % p[i].period == 0)
			{
				lcm = max;
				break;
			}
			++max;
		}
	}
return lcm;
}
