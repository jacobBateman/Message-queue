#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG_SIZE 128
#define MSG_TYPE 1

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    key_t key;
    int msg_id;
    struct msg_buffer message;

    // Tworzenie klucza
    key = ftok("msg_queue_key", 'A');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Tworzenie kolejki komunikatów
    msg_id = msgget(key, 0666 | IPC_CREAT);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    // Wysyłanie wiadomości do kolejki
    printf("Wpisz wiadomość: ");
    fgets(message.msg_text, MSG_SIZE, stdin);
    message.msg_type = MSG_TYPE;
    if (msgsnd(msg_id, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }
    printf("Wiadomość wysłana: %s", message.msg_text);

    return 0;
}
