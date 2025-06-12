#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

// thread sayısı 1 iken static dynamic guided
// thread sayısı 2 iken  static dynamic guided
//...

#define NUM_PHILOSOPHERS 8
#define DINNER_TIME 100

omp_lock_t *forks;
int *dinned_times;

int num_threads = 8;
int schedule_type = 1; // 0: static, 1: dynamic, 2: guided

int random_usleep_time(int base)
{
    return base / 2 + rand() % base;
}

void dine(int philosopher_id)
{
    int left = philosopher_id;
    int right = (philosopher_id + 1) % NUM_PHILOSOPHERS;

    srand(time(NULL) ^ (philosopher_id * 7919));

    while (dinned_times[philosopher_id] < DINNER_TIME)
    {
        printf("Philosopher %d is thinking\n", philosopher_id);
        usleep(random_usleep_time(500000));

        if (philosopher_id == NUM_PHILOSOPHERS - 1)
        {
            omp_set_lock(&forks[right]);
            omp_set_lock(&forks[left]);
        }
        else
        {
            omp_set_lock(&forks[left]);
            omp_set_lock(&forks[right]);
        }

        printf("Philosopher %d starts eating\n", philosopher_id);
        usleep(random_usleep_time(500000));
        printf("Philosopher %d stops eating\n", philosopher_id);

#pragma omp atomic
        dinned_times[philosopher_id]++;

        omp_unset_lock(&forks[left]);
        omp_unset_lock(&forks[right]);
    }

    printf("Philosopher %d finished dining all %d meals.\n", philosopher_id, DINNER_TIME);
}

int main(int argc, char *argv[])
{
    // Komut satırından thread sayısı ve schedule tipi al
    if (argc >= 2)
        num_threads = atoi(argv[1]);

    if (argc >= 3)
        schedule_type = atoi(argv[2]);

    forks = malloc(NUM_PHILOSOPHERS * sizeof(omp_lock_t));
    dinned_times = malloc(NUM_PHILOSOPHERS * sizeof(int));

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        omp_init_lock(&forks[i]);
        dinned_times[i] = 0;
    }

    omp_set_num_threads(num_threads);

    printf("Start the Dinner!!\n");
    printf("Using %d threads.\n", num_threads);
    if (schedule_type == 0)
        printf("Scheduling type: static\n");
    else if (schedule_type == 1)
        printf("Scheduling type: dynamic\n");
    else if (schedule_type == 2)
        printf("Scheduling type: guided\n");

    double start_time = omp_get_wtime();

    if (schedule_type == 0)
    {
#pragma omp parallel for schedule(static)
        for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        {
            printf("Philosopher %d begins dining...\n", i);
            dine(i);
        }
    }
    else if (schedule_type == 1)
    {
#pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        {
            printf("Philosopher %d begins dining...\n", i);
            dine(i);
        }
    }
    else if (schedule_type == 2)
    {
#pragma omp parallel for schedule(guided)
        for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        {
            printf("Philosopher %d begins dining...\n", i);
            dine(i);
        }
    }

    double end_time = omp_get_wtime();

    printf("End the Dinner!!\n");
    printf("Total elapsed time: %f seconds\n", end_time - start_time);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        printf("Philosopher %d ate %d meals.\n", i, dinned_times[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        omp_destroy_lock(&forks[i]);
    }

    free(forks);
    free(dinned_times);

    return 0;
}