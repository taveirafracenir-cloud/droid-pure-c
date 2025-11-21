#include <stdio.h>
#include <stdlib.h>

void do_wipe_data() {
    printf("\n======================================\n");
    printf("   Wipe data / Factory Reset\n");
    printf("======================================\n");

    const char* options[] = {
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "No",
        "!!! YES — delete all user data !!!"
    };

    int selection = 0;

    while (1) {
        printf("\nConfirme:\n");
        for (int i = 0; i < 17; i++) {
            if (i == selection)
                printf(" ➜ %s\n", options[i]);
            else
                printf("   %s\n", options[i]);
        }

        printf("\nUse UP/DOWN e ENTER: ");
        char c = getchar();
        getchar(); // consume \n

        if (c == 'w' && selection > 0) selection--;
        if (c == 's' && selection < 16) selection++;

        if (c == '\n' || c == 'e') {
            if (selection == 16) {
                printf("\n*** INICIANDO WIPE REAL ***\n");


void wipe_data() {
    printf("Wiping /data...\n");

    // desmonta /data
    system("umount /data");

    // formata a partição inteira (igual Android faz)
    system("mke2fs -t ext4 /dev/block/mmcblk0p28");

    printf("Wipe completo.\n");
}
                printf("\n(wipe executado — aqui você coloca sua lógica)\n");
            } else {
                printf("\nCancelado.\n");
            }
            break;
        }
    }
}

void start_recovery_menu() {
    while (1) {
        printf("\n======== DroidPureC Recovery ========\n");
        printf("1. Reboot system now\n");
        printf("2. Wipe data / Factory reset\n");
        printf("3. Wipe cache\n");
        printf("4. Exit\n");
        printf("Escolha: ");

        int c = getchar();
        getchar();

        if (c == '1') printf("\n(Reboot)...\n");
        else if (c == '2') do_wipe_data();
        else if (c == '3') printf("\n(Wipe cache — placeholder)\n");
        else if (c == '4') break;
    }
}

int main() {
    start_recovery_menu();
    return 0;
}
