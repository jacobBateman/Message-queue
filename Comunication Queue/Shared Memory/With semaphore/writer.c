#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 128
#define SHM_KEY 1234
#define SEM_KEY 5678

struct shared_data {
    char message[SHM_SIZE];
};

int main() {
    int shmid, semid;
    struct shared_data *shm_addr;
    const char *messages[] = {"CIEPLO", "ZIMNO"};
    int index = 0;
    union semun sem_union;

    // Tworzenie segmentu pamięci współdzielonej
    shmid = shmget(SHM_KEY, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Dołączanie segmentu pamięci współdzielonej
    shm_addr = (struct shared_data *)shmat(shmid, NULL, 0);
    if (shm_addr == (struct shared_data *)-1) {
        perror("shmat");
        exit(1);
    }

    // Tworzenie semafora
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Inicjalizacja semafora
    sem_union.val = 1;
    if (semctl(semid, 0, SETVAL, sem_union) == -1) {
        perror("semctl");
        exit(1);
    }

    while (1) {
        sem_wait(semid, 0); // Zamykanie semafora
        // Wpisywanie wiadomości do pamięci współdzielonej
        strcpy(shm_addr->message, messages[index]);
        printf("Wysłano: %s\n", messages[index]);
        index = (index + 1) % 2; // Naprzemienne przełączanie między 0 a 1
        sem_signal(semid, 0); // Otwieranie semafora
        sleep(1); // Opóźnienie, aby nie zapełniać CPU
    }

    return 0;
}
