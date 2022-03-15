#include <stdio.h>
#include<stdlib.h>
#include "EDH.h"

int main()
{

	/*
		printf("Enter Number Of Tasks: ");
		scanf("%d", &jobs);
		printf("Enter Max capacity of Energy units of Energy reservoir: ");
		scanf("%d", &Emax);
		printf("Enter Rechargeable Power constant: ");
		scanf("%d", &pp);
		printf("Enter the Energy at time(0): ");
		scanf("%d", &E0);
		printf("Enter e-max value, if N/A enter the value 0: ");
		scanf("%d", &emin);

		inputTask();
	*/
    
	int example = 1;
	///// example 1/////
	if (example == 1)
	{
		jobs = 2;
		p[0].task = 1; p[0].arrival = 0;	p[0].c = 3;		p[0].deadline = 6;		p[0].period = 6;		p[0].energy = 8;
		p[1].task = 2; p[1].arrival = 0;	p[1].c = 2;		p[1].deadline = 8;		p[1].period = 8;		p[1].energy = 5;

		E0 = 4;
		pp = 2;
		Emax = 4;
		emin = 0;
	}
	else if (example == 2)
	{
		///// example 2/////
		jobs = 3;
		p[0].task = 1; p[0].arrival = 0;	p[0].c = 1;		p[0].deadline = 3;		p[0].period = 5;		p[0].energy = 30;
		p[1].task = 2; p[1].arrival = 0;	p[1].c = 2;		p[1].deadline = 7;		p[1].period = 10;		p[1].energy = 80;
		p[2].task = 3; p[2].arrival = 0;	p[2].c = 3;		p[2].deadline = 12;		p[2].period = 20;		p[2].energy = 180;
		E0 = 200;
		pp = 10;
		Emax = 200;
		emin = 0;
	}
	else if (example == 3)
	{
		jobs = 3;
		p[0].task = 1; p[0].arrival = 4;	p[0].c = 1;		p[0].deadline = 6;		p[0].period = 9;		p[0].energy = 5;
		p[1].task = 2; p[1].arrival = 0;	p[1].c = 1;		p[1].deadline = 2;		p[1].period = 9;		p[1].energy = 1;
		p[2].task = 3; p[2].arrival = 0;	p[2].c = 4;		p[2].deadline = 9;		p[2].period = 9;		p[2].energy = 8;
		E0 = 5;
		pp = 1;
		periodic = false;
		Emax = 5;
		emin = 0;
	}
	else if (example == 4)
	{
		jobs = 5;
		p[0].task = 1; p[0].arrival = 0;	p[0].c = 3;		p[0].deadline = 18;		p[0].period = 24;		p[0].energy = 9;
		p[1].task = 2; p[1].arrival = 4;	p[1].c = 2;		p[1].deadline = 12;		p[1].period = 24;		p[1].energy = 12;
		p[2].task = 3; p[2].arrival = 5;	p[2].c = 3;		p[2].deadline = 24;		p[2].period = 24;		p[2].energy = 7;
		p[3].task = 4; p[3].arrival = 0;	p[3].c = 4;		p[3].deadline = 16;		p[3].period = 24;		p[3].energy = 10;
		p[4].task = 5; p[4].arrival = 8;	p[4].c = 3;		p[4].deadline = 20;		p[4].period = 24;		p[4].energy = 9;

		E0 = 10;
		pp = 2;
		periodic = false;
		Emax = 10;
		emin = 0;
	}
	else if (example == 5)
	{
		jobs = 2;
		p[0].task = 1; p[0].arrival = 0;	p[0].c = 1;		p[0].deadline = 8;		p[0].period = 8;		p[0].energy = 2;
		p[1].task = 2; p[1].arrival = 2;	p[1].c = 3;		p[1].deadline = 6;		p[1].period = 8;		p[1].energy = 8;

		E0 = 4;
		pp = 1;
		periodic = false;
		Emax = 6;
		emin = 3;
	}
	else if (example == 6)
	{
		jobs = 3;
		p[0].task = 1; p[0].arrival = 0;	p[0].c = 3;		p[0].deadline = 7;		p[0].period = 20;		p[0].energy = 6;
		p[1].task = 2; p[1].arrival = 0;	p[1].c = 2;		p[1].deadline = 4;		p[1].period = 5;		p[1].energy = 2;
		p[2].task = 3; p[2].arrival = 0;	p[2].c = 1;		p[2].deadline = 8;		p[2].period = 10;		p[2].energy = 2;
		E0 = 4;
		pp = 1;
		Emax = 4;
		emin = 0;
	}



	lcm = LCM();
	printJobs();
	generate_ftable();
	feasibilityChecks();
	calculate_trace_updated();
	printf("Scheduling trace:        \n" );
	schedule_output();
	printf("\n");

}
