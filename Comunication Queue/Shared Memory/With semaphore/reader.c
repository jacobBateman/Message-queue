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

    // Dołączanie segmentu pamięci współdzielonej
    shmid = shmget(SHM_KEY, sizeof(struct shared_data), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shm_addr = (struct shared_data *)shmat(shmid, NULL, 0);
    if (shm_addr == (struct shared_data *)-1) {
        perror("shmat");
        exit(1);
    }

    // Pobieranie semafora
    semid = semget(SEM_KEY, 1, 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    while (1) {
        sem_wait(semid, 0); // Zamykanie semafora
        // Odczytywanie wiadomości z pamięci współdzielonej
        if (strcmp(shm_addr->message, "CIEPLO") == 0 || strcmp(shm_addr->message, "ZIMNO") == 0) {
            printf("Odebrano: %s\n", shm_addr->message);
        } else {
            printf("Błąd: niepoprawna wiadomość: %s\n", shm_addr->message);
        }
        sem_signal(semid, 0); // Otwieranie semafora
        sleep(1); // Opóźnienie, aby synchronizować się z nadawcą
    }

    return 0;
}
