#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#define KEY_POWER 116
#define KEY_VOLUMEDOWN 114
#define KEY_VOLUMEUP 115

// Função que abre o device de input
int open_input_device() {
    for (int i = 0; i < 10; i++) {
        char path[32];
        sprintf(path, "/dev/input/event%d", i);

        int fd = open(path, O_RDONLY);
        if (fd >= 0) {
            printf("[RECOVERY] Usando %s\n", path);
            return fd;
        }
    }
    return -1;
}

// Função que lê a tecla pressionada
int read_key(int fd) {
    struct input_event ev;
    while (read(fd, &ev, sizeof(ev)) > 0) {
        if (ev.type == EV_KEY && ev.value == 1) { // pressionado
            return ev.code;
        }
    }
    return -1;
}

// Função que efetivamente formata /data
void wipe_data() {
    printf("\n*** INICIANDO WIPE REAL ***\n");

    // desmonta /data
    system("umount /data");

    // formata a partição inteira
    system("mke2fs -t ext4 /dev/block/mmcblk0p28");

    printf("Wipe completo.\n");
}

// Função do menu de confirmação
void do_wipe_data(int fd) {
    const char* options[] = {
        "No","No","No","No","No","No","No","No",
        "No","No","No","No","No","No","No","No",
        "!!! YES — delete all user data !!!"
    };

    int selection = 0;
    int key;

    while (1) {
        printf("\nConfirme:\n");
        for (int i = 0; i < 17; i++) {
            if (i == selection) printf(" ➜ %s\n", options[i]);
            else                printf("   %s\n", options[i]);
        }

        printf("\nVOL UP = cima | VOL DOWN = baixo | POWER = selecionar\n");

        key = read_key(fd);

        if (key == KEY_VOLUMEUP && selection > 0) selection--;
        if (key == KEY_VOLUMEDOWN && selection < 16) selection++;

        if (key == KEY_POWER) {
            if (selection == 16) {
                wipe_data();  // chama o wipe real
            } else {
                printf("\nCancelado.\n");
            }
            break;
        }
    }
}

// Função do menu principal
void start_recovery_menu() {
    int fd = open_input_device();
    if (fd < 0) {
        printf("ERRO: Nenhum /dev/input/event encontrado!\n");
        return;
    }

    int key;
    int selection = 0;

    const char* items[] = {
        "Reboot system now",
        "Wipe data / Factory reset",
        "Wipe cache partition",
        "Exit"
    };

    while (1) {
        printf("\n======== DroidPureC Recovery ========\n");
        for (int i = 0; i < 4; i++) {
            if (i == selection) printf(" ➜ %s\n", items[i]);
            else                printf("   %s\n", items[i]);
        }

        key = read_key(fd);

        if (key == KEY_VOLUMEUP && selection > 0) selection--;
        if (key == KEY_VOLUMEDOWN && selection < 3) selection++;

        if (key == KEY_POWER) {
            if (selection == 0)
                system("reboot"); // reboot real
            else if (selection == 1)
                do_wipe_data(fd); // chama menu de wipe
            else if (selection == 2)
                printf("\n(Wipe cache — placeholder)\n");
            else if (selection == 3)
                break;
        }
    }
}

int main() {
    start_recovery_menu();
    return 0;
}
