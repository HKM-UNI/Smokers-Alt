#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "smokers.h"
#include "offerhandlers.h"
#define PERMUTATION_TIME 3
#define SMOKING_TIME 5

void quit_all(int sig);
void meet_the_smokers(int sig);
void meet_the_agent(int sig);

pid_t agent_pid;
Smoker smoker;
int smoker_pids[3];
int smokers_ready = 0;

int main()
{
    signal(SIGUSR1, meet_the_smokers);
    signal(SIGINT, quit_all);

    agent_pid = getpid();
    printf("Agent PID: %d\n", agent_pid);

    pid_t pid = fork();
    if (pid == 0) {
        create_smoker(&smoker, getpid(), "Smoker 1", PAPER);
    } else {
        smoker_pids[0] = pid;
        if ((pid = fork()) == 0) {
            create_smoker(&smoker, getpid(), "Smoker 2", MATCHES);
        } else {
            smoker_pids[1] = pid;
            if ((pid = fork()) == 0) {
                create_smoker(&smoker, getpid(), "Smoker 3", TOBACCO);
            } else {
                smoker_pids[2] = pid;
            }
        }
    }
    pid = getpid();

    if (pid == agent_pid) {
        smokers_ready = 1;

        for (int i=0; i<3; i++) {
            printf("Smoker%d PID: %d\n", i+1, smoker_pids[i]);
        }
        printf("\n------\n\n");
        signal(SIGUSR1, meet_the_smokers);
        kill(agent_pid, SIGUSR1);
    } else {
        signal(SIGUSR2, meet_the_agent);
    }

    while (1) {
        sleep(10);
    }
}

void meet_the_smokers(int sig) {
    if (!smokers_ready) return;

    CigaretteMaterial offers[2];
    release_offers(offers);

    printf("[Agent]: New offers are ready:\n"
           "\t- %s\n\t- %s\n\n",
           material_name(offers[0]), material_name(offers[1]));
    fflush(stdout);
    sleep(PERMUTATION_TIME);

    for (int i=0; i<3; i++) {
        kill(smoker_pids[i], SIGUSR2);
    }
}

void meet_the_agent(int sig) {
    CigaretteMaterial offers[2] = { PAPER, MATCHES };
    look_offers(offers);

    if (smoker.material != offers[0] && smoker.material != offers[1]) {
        printf("[%s]: has the %s, is now smoking...\n", smoker.name, material_name(smoker.material));
        fflush(stdout);
        sleep(SMOKING_TIME);

        printf("[%s]: has finished\n\n", smoker.name);
        fflush(stdout);
        sleep(PERMUTATION_TIME);
        
        kill(getppid(), SIGUSR1);
    }
}

void quit_all(int sig) {
    kill(0, SIGQUIT);
}
