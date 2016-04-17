#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>

#include "library.h"
#include "philosopher.h"

static void thinking(int philosopher);
static void taking_forks(int philosopher);
static void philosopher_wants_to_eat(int philosopher);
static void eating(int philosopher);
static void return_fork(int philosopher);

void philosopher(int philosopher)
{
	//int x = 0;

	printf("[%d] I'm Philosopher %s!\n", philosopher, philosophers_names[philosopher]);
	
	//do {
		
		thinking(philosopher);	
		taking_forks(philosopher);
		eating(philosopher);
		return_fork(philosopher);
	
	//} while (x < TIMES);
	
	pthread_exit(NULL);
}

static void thinking(int philosopher)
{
	printf("[%d] Philosopher %s is thinking: FRASE DO FILOSOFO\n", philosopher, philosophers_names[philosopher]);
	usleep(MICROSECONDS);
}

static void taking_forks(int philosopher)
{
	pthread_mutex_lock(&mutex);

	printf("[%d] Philosopher %s is hungly!\n", philosopher, philosophers_names[philosopher]);
	status[philosopher] = HUNGLY;
	
	philosopher_wants_to_eat(philosopher);
	
	pthread_mutex_unlock(&mutex);	
	pthread_mutex_lock(&(philosophers_mutex[philosopher]));
}

static void philosopher_wants_to_eat(int philosopher)
{
	printf("[%d] Philosopher %s wants to eat!\n", philosopher, philosophers_names[philosopher]);

	if ((status[philosopher] == HUNGLY) && 
	    (status[(philosopher + NUMBER_OF_PHILOSOPHERS - 1) % NUMBER_OF_PHILOSOPHERS] != EATING) && 
	    (status[(philosopher + NUMBER_OF_PHILOSOPHERS) % NUMBER_OF_PHILOSOPHERS] != EATING)) {
	
		printf("[%d] Philosopher %s can eat now!\n", philosopher, philosophers_names[philosopher]);
		status[philosopher] = EATING;
		pthread_mutex_unlock(&(philosophers_mutex[philosopher]));
	} 
}

static void eating(int philosopher)
{
	printf("[%d] Philosopher %s is eating for %d microseconds!\n", philosopher, philosophers_names[philosopher], MICROSECONDS);
	usleep(MICROSECONDS);
}

static void return_fork(int philosopher)
{
	pthread_mutex_lock(&mutex);
	printf("[%d] Philosopher %s is thinking again!\n", philosopher, philosophers_names[philosopher]);
	status[philosopher] = THINKING;
	
	philosopher_wants_to_eat((int)(philosopher + NUMBER_OF_PHILOSOPHERS - 1) % NUMBER_OF_PHILOSOPHERS);
	philosopher_wants_to_eat((int)(philosopher + NUMBER_OF_PHILOSOPHERS) % NUMBER_OF_PHILOSOPHERS);
	pthread_mutex_unlock(&mutex);
}




