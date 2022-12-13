#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/utsname.h> // header to print system
#include <time.h>       // header to print time

#define clear() printf("\033[H\033[J") // function to clear screen (transition)

// Linked list Functions to store all dish
struct Node{ // Double linked list
    char namaMenu[50];
    int harga;
    int stok;
    Node *next, *prev;
};

Node *head, *tail; //Head Menu and Tail Menu

Node *createNode(char namaMenu[], int harga, int stok){ // Function to create newNode - newMenu
    Node *newNode = (Node *)malloc(sizeof(Node));

    strcpy(newNode->namaMenu, namaMenu);
    newNode -> harga = harga;
    newNode -> stok = stok;
    newNode -> next = NULL;
    newNode -> prev = NULL;

    return newNode;
}

void readNode(Node *currentNode, int counter){ // Function coordinate with printMenu()
    printf("%.2d. ", counter);
    printf("%-15s     ", currentNode -> namaMenu);
    printf("%.3d       ", currentNode -> stok);
    printf("Rp%d", currentNode -> harga);
}

void printMenu(){ // Function to print all menu in Linked List
    int counter = 1;
    for(Node *temp = head; temp != NULL; temp = temp -> next){
        readNode(temp, counter);
        counter++;
        puts("");
    }
}

// Function to run "1. Add Dish"
void addDish(char namaMenu[], int harga, int stok){ //Function to add dish (pushHead - Algorithm)
    Node *temp = createNode(namaMenu, harga, stok);
    if(!head){ // if there's no head - empty data
        head = tail = temp;
    }
    else{
        temp -> next = head;
        head -> prev = temp;
        head = temp;
    }
}

// Function to run "2. Remove Dish"
void popTail(){ // Function to remove last Menu (popTail - Algorithm)
    if(!head){ // if there's no node in LL
        return;
    }
    else if(head == tail){ // if there's only 1 node
        free(head);
        head = tail = NULL;
    }
    else{ // if there's more than 1 node, we need a new tail candidate
        Node *newTail = tail -> prev;
        tail->prev = newTail -> next = NULL;
        free(tail);
        tail = newTail;
    }
}

void popHead(){ // Function to remove first Menu (popHead - Algorithm)
    if(!head){ // if there's no node in LL
        return;
    }
    else if(head == tail){ // if there's only 1 node
        free(head); 
        head = tail = NULL; 
    }
    else{ // if there's more than 1 node, we need a new head candidate
        Node *newHead = head -> next;
        head -> next = newHead -> prev = NULL;
        free(head);
        head = newHead;
    }
}

int removeDish(char namaMenu[]){ // Function to remove dish (popMid - Algorithm)
    if(!head){ // if there's no node in LL
        return 0; // do nothing
    }
    else if(strcmp(head->namaMenu, namaMenu) == 0){ // if removed dish is in first position (head),
        popHead(); //use popHead() function
        return 1;
    }
    else if(strcmp(tail->namaMenu, namaMenu) == 0){ //if removed dish is in last position (tail),
        popTail(); // use popTail() function
        return 1;
    }
    else{ // if query is in the middle of the LL

        Node *current = head; //start from beggining (head)
        while(current && strcmp(current->namaMenu, namaMenu) != 0){ //check if current node name == target removed name
            if(current == tail){ // if removed dish not found until last menu,
                return 0; // return 0
            }
            current = current -> next;
        }

        current->prev->next = current -> next; 
        current->next->prev = current -> prev;
        current->prev = current->next = NULL;
        free(current);
        current = NULL;
        return 1;
    }

    // return 0 - dish not found / doesn't exist
    // return 1 - dish is removed succesfully
}

int checkDuplicateMenu(char namaMenu[]){ // function to check if the menu is unique or not in the linked list
    if(!head){
        return 1;
    }
    else{
        Node *current = head; //start from beggining (head)
        while(current){ //check if there is still element
            if(strcmp(current->namaMenu, namaMenu) == 0){ // if dish wanted to add is a duplicate
                return 0; // return 0
            }
            current = current -> next;
        }
        return 1;
    }

    // return 0 - dish is duplicate
    // return 1 - dish is unique
}

// Hash functions to store customer's name (chaining method)
const int MAX_BUCKETS = 7; // max elements to be stored
struct cusNode { // Single Linked List to store names
    char cusName[50];
    cusNode *nextCus;
} *cusHead[MAX_BUCKETS], *cusTail[MAX_BUCKETS];

cusNode *createNodeCus(const char *name){
    cusNode *newNode = (cusNode *)malloc(sizeof(cusNode));
    strcpy(newNode -> cusName, name);
    newNode -> nextCus = NULL;

    return newNode;
}

unsigned long djb2(const char *str){ // hashing algorithm of DJB2 HASHING
    unsigned long key = 5381;

    for(int i = 0; i < strlen(str); i++){
        int ascii = int(str[i]);
        key = (key << 5) + key + ascii;
    }
    
    return key % MAX_BUCKETS;
}

void insertCustomer(const char *str){ // function to insert name into hash table
    cusNode *newNode = createNodeCus(str);
    int index = djb2(str);

    if(!cusHead[index]){ // if LL is empty at the specified index
        cusHead[index] = cusTail[index] = newNode;
    }
    else{ // push tail
        cusTail[index]->nextCus = newNode;
        cusTail[index] = newNode;
    }
}

void traverseHash(int i){ // function to traverse(loop) the hash table specified index
    cusNode *curr = cusHead[i];
    while(curr){
        printf("%s -> ", curr->cusName);
        curr = curr-> nextCus;
    }
    puts("Null");
}

void readCustomer(){ // function to print the current hashing table (to check program)
    for(int i = 0; i < MAX_BUCKETS; i++){
        printf("Index %d: ", i);
        if(cusHead[i]){
            traverseHash(i);
        }
        else{
            puts("NULL");
        }
    }
}

int traverseSearch(int i, const char *str){ // function to traverse and compare customer's names
    cusNode *curr = cusHead[i]->nextCus;
    while(curr){
        if(strcmp(curr->cusName, str) == 0){
            return 1;
        }
        curr = curr-> nextCus;
    }
    return 0;
}

int searchCustomer(const char *str){ //function to check and compare names in the hash table
    for(int i = 0; i < MAX_BUCKETS; i++){
        if(!cusHead[i]){ // if index has no data (NULL)
            continue;
        }
        else if(strcmp(cusHead[i]->cusName, str) == 0){ // if customer's name immediately match with the head element
            return 1;
        }
        else{ // loop through the index to find any possible match name
            if(traverseSearch(i, str)){
                return 1;
            }
        }
    }

    return 0;
}

int main(){
    while(true){
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
    clear();

    switch(menu){
        case 1:{ // 1. Add Dish
            char namaMenu[20] = {0};
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
                    if(isupper(namaMenu[i])){ // validation to check lower case letter
                        validNamaMenu = false;
                        break;
                    }
                    else if(!isalpha(namaMenu[i]) && !isspace(namaMenu[i])){ // validation to check only alphabet and space
                        validNamaMenu = false;
                        break;
                    }

                    validNamaMenu = checkDuplicateMenu(namaMenu); //check if menu is unique
                }
            }while(!validNamaMenu);

            // input price of the dish
            bool ValidHargaMenu;
            do{
                printf("Insert the price of the dish [1000..50000]: ");
                scanf("%d", &hargaMenu);
                ValidHargaMenu = true;

                if(hargaMenu < 1000 || hargaMenu > 50000){ // validation to check price
                    ValidHargaMenu = false;
                }
            }while(!ValidHargaMenu);

            // input the stock of the dish
            bool validStockMenu;
            do{
                printf("Insert the quantity of the dish [1..999]: ");
                scanf("%d", &stokMenu);
                validStockMenu = true;

                if(stokMenu < 1 || stokMenu > 999){ // validation to check stock number
                    validStockMenu = false;
                }
            }while(!validStockMenu);

            addDish(namaMenu, hargaMenu, stokMenu);

            puts("The dish has been added!");
            puts("Press enter to continue...");
            getchar();
            getchar();
            clear();
            break;
        }
        case 2:{ // 2. Remove Dish
            int headline = 30;
            char namaResto[20] = "Bude's Menu";
            printf ("%*s\n", (int)(headline/2 + strlen(namaResto)/2), namaResto);
            printf("==============================\n");
            printf("No.      Name         Quantity     Price  \n");
            if(head == NULL){
                printf("\nNo dish is available.\n\n");
                printf("==============================\n");
            }
            else{
                printMenu();
                printf("==============================\n");
                printf("Insert dish's name to be deleted: ");
                char targetDish[20] = {0};
                scanf("%[^\n]", targetDish);
                getchar();

                int check = removeDish(targetDish);

                if(check == 1){ //true(1) - dish is removed
                    puts("The dish has been removed!");
                }
                else{ // false(0) - dish not found
                    puts("Invalid input, dish not found!");
                }
            }
            puts("Press enter to continue...");
            getchar();
            clear();
            break;
        }
        case 3:{ // 3. Add Customer
            bool validCusName;
            do{
                printf("Insert the customer's name [Without space]: ");
                char tempCusName[50] = {0};
                scanf("%[^\n]", tempCusName);
                getchar();
                validCusName = true;

                for(int i = 0; i < strlen(tempCusName); i++){ 
                    if(!isalpha(tempCusName[i]) || isspace(tempCusName[i])){ // validation to check only alphabet and no space
                        validCusName = false;
                        break;
                    }
                }

                if(validCusName) insertCustomer(tempCusName); // if valid, insert to hashtable
            }while(!validCusName);
            
            puts("Customer has been added!");
            puts("Press enter to continue...");
            getchar();
            clear();
            break;
        }
        case 4:{ // 4. Search Customer
            readCustomer(); // checking customers
            bool validSearchCus;
            do{
                printf("Insert the customer's name to be searched: ");
                char tempCheckCus[50] = {0};
                scanf("%[^\n]", tempCheckCus);
                getchar();
                validSearchCus = true;

                for(int i = 0; i < strlen(tempCheckCus); i++){ 
                    if(!isalpha(tempCheckCus[i]) || isspace(tempCheckCus[i])){ // validation to check only alphabet and no space
                        validSearchCus = false;
                        break;
                    }
                }

                if(validSearchCus){
                    bool cusFound = searchCustomer(tempCheckCus); // if valid, search customer in hashtable
                    if(cusFound){ // if target customer is found
                        printf("%s is present.\n", tempCheckCus);
                    }
                    else{
                        printf("%s is not present.\n", tempCheckCus);
                    }
                }

            }while(!validSearchCus);

            puts("Press enter to continue...");
            getchar();
            clear();
            break;
        }
        case 8:{ // 8. Exit Warteg
            puts("Please expand your terminal to full screen!");
            puts("Press enter to continue...");
            getchar();
            exit(0);
        }
        
    }
    }


    return 0;
}