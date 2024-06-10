#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 128
#define SHM_KEY 1234

int main() {
    int shmid;
    char *shm_addr;
    const char *messages[] = {"CIEPLO", "ZIMNO"};
    int index = 0;

    // Tworzenie segmentu pamięci współdzielonej
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Dołączanie segmentu pamięci współdzielonej
    shm_addr = (char *)shmat(shmid, NULL, 0);
    if (shm_addr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    while (1) {
        // Wpisywanie wiadomości do pamięci współdzielonej
        strcpy(shm_addr, messages[index]);
        printf("Wysłano: %s\n", messages[index]);
        index = (index + 1) % 2; // Naprzemienne przełączanie między 0 a 1
        sleep(1); // Opóźnienie, aby nie zapełniać CPU
    }

    return 0;
}
