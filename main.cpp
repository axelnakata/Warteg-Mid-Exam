#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <time.h>
#include <ctype.h>



int main(){
    // print user's operating system
    struct utsname system;
    uname(&system);
    printf("System: %s\n", system.sysname);

    // print current day, date,and time
    time_t tm;
    time(&tm);
    printf("%s", ctime(&tm));

    // List down all menu option
    puts("1. Add Dish");
    puts("2. Remove Dish");
    puts("3. Add Customer");
    puts("4. Search Customer");
    puts("5. View Warteg");
    puts("6. Order");
    puts("7. Payment");
    puts("8. Exit Warteg");
    int menu;
    do{
        printf(">> ");
        scanf("%d", &menu);
        getchar();
    }while(menu < 1 || menu > 8);


    switch(menu){
        case 1:{
            char namaMenu[50] = {0};
            int hargaMenu;
            int stokMenu;

            // input name of the dish
            bool validNamaMenu;
            do{
                printf("Insert the name of the dish [Lowercase letters]: ");
                scanf("%[^\n]", namaMenu);
                getchar();
                validNamaMenu = true;
                for(int i = 0; i < strlen(namaMenu); i++){
                    if(isupper(namaMenu[i])){
                        validNamaMenu = false;
                        break;
                    }
                }
            }while(!validNamaMenu);

            // input price of the dish
            bool ValidHargaMenu;
            do{
                printf("Insert the price of the dish [1000..50000]: ");
                scanf("%d", &hargaMenu);
                ValidHargaMenu = true;

                if(hargaMenu < 1000 || hargaMenu > 50000){
                    ValidHargaMenu = false;
                }
            }while(!ValidHargaMenu);

            // input the stock of the dish
            bool validStockMenu;
            do{
                printf("Insert the quantity of the dish [1..999]: ");
                scanf("%d", &stokMenu);
                validStockMenu = true;

                if(stokMenu < 1 || stokMenu > 999){
                    validStockMenu = false;
                }
            }while(!validStockMenu);
        }

        puts("The dish has been added!");
        puts("Press enter to continue...");
    }


    return 0;
}