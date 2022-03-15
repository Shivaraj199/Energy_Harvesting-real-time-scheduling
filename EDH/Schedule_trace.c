#include "EDH.h"

int out_time1[100];
int out_time2[100];
int out_taskid[100];
int checker=1;


void inputTask() {
	int i;
	for (i = 0; i < jobs; i++)
	{

		printf("\n--------------------------------\n");

		p[i].task = i + 1;
		printf("Enter Arrival Time of Task%d: ", p[i].task);
		//scanf("%d", &p[i].arrival);
		printf("Enter Computational Time of Task%d: ", p[i].task);
		//scanf("%d", &p[i].c);
		printf("Enter Period of Task%d, if non-periodic please provide the maximum deadline as the period: ", p[i].task);
		//scanf("%d", &p[i].period);
		printf("Enter Deadline of Task%d: ", p[i].task);
		//scanf("%d", &p[i].deadline);
		printf("Enter Energy Consumption of Task%d: ", p[i].task);
		//scanf("%d", &p[i].energy);

	}
}

/*Displaying the table with the Input parametrs for the job*/
void printJobs()
{
	int i;
	printf("\n---------------------------------------------------------------------------------------------------------------\n");
	printf("%10s |%15s |%15s |%25s |%15s |%15s\n", "Task", "Arrival Time", "Period (Pi)", "Computational Time (Ci)",
		"Deadline (Di)", "Energy (Ei)");
	printf("---------------------------------------------------------------------------------------------------------------\n");
	for (i = 0; i < jobs; i++) {
		printf("%9s%d |%15d |%15d |%25d |%15d |%15d\n", "Task", p[i].task, p[i].arrival, p[i].period, p[i].c, p[i].deadline, p[i].energy);
	}
}


void feasibilityChecks()
{
	if (is_feasibility_pass)
		printf("TRACE: SSEw, SSTw  feasibility test passed! Hence taskset is feasible.\n");
	else
	{
		printf("TRACE: SSEw, SSTw  feasibility test failed, exiting taskset\n");
		//exit(0);
	}

	float processor_util = 0.0;                                             // Calculatingthe processor utilzation
	for (int i = 0; i < jobs; i++)
		processor_util += (float)p[i].c / p[i].period;
	printf("TRACE: processor utilization %f hence %s\n", processor_util, (processor_util < 1.0) ? "satisfied" : "unsatisfied.");

	float energy_util = 0.0;
	for (int i = 0; i < jobs; i++)
		energy_util += (float)p[i].energy / p[i].period;                       // Calcualting the energy Ultization
	printf("TRACE: energy utilization %f hence %s\n", energy_util, (energy_util < 2.0) ? "satisfied" : "unsatisfied.");


}


void display_output(int time1[],int time2[],int task_id[])
{
		out_time1[checker-1]=time1[checker-1];
		out_time2[checker-1]=time2[checker-1];
		out_taskid[checker-1]=task_id[checker-1];
	checker++;
}
void schedule_output(){
int overall[lcm];
	for (int i=0;i<lcm;i++)
	{
        overall[i]='+';
    }
	overall[out_time1[0]]=out_taskid[0]+'0';
    for (int i=1;i<lcm;i++)
	{
        if((out_time1[i])!=0)
		{
			overall[out_time1[i]]=out_taskid[i]+'0';
        }
	}
	printf("                                   ");
	printf("\n| ");
	printf("%c",overall[0]);
    for (int i=1;i<lcm;i++)
	{
		printf(" | ");
		printf("%c",overall[i]);
    }
	printf(" |");
	printf("\n");
	for (int i=0;i<lcm;i++)
	{
		printf("____");
	}
	printf("\n");
	for (int i=0;i<=lcm;i++)
	{
		printf("%d",i);
		if(i>9)
		{
			printf("  ");
		}
		else
			printf("   ");
	}
printf("                                   \n\n");
	
printf("					Number of times Battery is fully replenished =%d\n",energy_replenished);
float energy_full_percent=(float)energy_replenished/lcm*100.0;

printf("					Battery Full Charged percentage=%.2f%%\n\n",energy_full_percent);
	if (premption_count==0)
	{
		printf("					No of Pre-emption=%d\n",premption_count);	
	}
	else
	{
		printf("					No of Pre-emption=%d\n",premption_count-1);	
		int premption = premption_count-1;
		float premption_percent=(float)premption/lcm*100.0;
		printf("					Pre-emption percentage=%.2f%%\n\n",premption_percent);
	}
float ideal_percentage =(float)ideal_count/lcm*100.0;

printf("					Percentage processor is ideal =%.2f%%\n",ideal_percentage);

}
