/* NOTES - PLEASE READ:
Code might not run if the #include <sys/utsname.h> is not compatible with the OS.
If code fail to run, please comment the specified header and also some codes in int main()

The following line of code that needs to be deleted/comment are:
-Line 17
-Line 612 until 614

Code was tested in vsCode using WSL:Ubuntu
Thank you for the understanding.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/utsname.h> ///PLEASE COMMENT THIS LINE, IF CODE DOESN'T RUN (header to print system)
#include <time.h>        // header to print time

#define clear() printf("\033[H\033[J") // function to clear screen (transition)

const int MAX_BUCKETS = 7; // Max elements to be stored in the hash table
int CURRENT_CUSTOMER = 0;  // Total Customer Right Now
int CURRENT_MENU = 0;      // Total Menu Right Now

// Linked list Functions to store all dish
struct Node
{ // Double linked list
    char namaMenu[50];
    int harga;
    int stok;
    Node *next, *prev;
};

Node *head, *tail; // Head Menu and Tail Menu

Node *createNode(char namaMenu[], int harga, int stok)
{ // Function to create newNode - newMenu
    Node *newNode = (Node *)malloc(sizeof(Node));

    strcpy(newNode->namaMenu, namaMenu);
    newNode->harga = harga;
    newNode->stok = stok;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

void readNode(Node *currentNode, int counter)
{ // Function coordinate with printMenu()
    printf("%.2d. ", counter);
    printf("%-15s     ", currentNode->namaMenu);
    printf("%.3d       ", currentNode->stok);
    printf("Rp%d", currentNode->harga);
}

void printMenu()
{ // Function to print all menu in Linked List
    int counter = 1;
    for (Node *temp = head; temp != NULL; temp = temp->next)
    {
        readNode(temp, counter);
        counter++;
        puts("");
    }
}

// Function to run "1. Add Dish"
void addDish(char namaMenu[], int harga, int stok)
{ // Function to add dish (pushHead - Algorithm)
    Node *temp = createNode(namaMenu, harga, stok);
    if (!head)
    { // if there's no head - empty data
        head = tail = temp;
    }
    else
    {
        temp->next = head;
        head->prev = temp;
        head = temp;
    }

    CURRENT_MENU++;
}

// Function to run "2. Remove Dish"
void popTail()
{ // Function to remove last Menu (popTail - Algorithm)
    if (!head)
    { // if there's no node in LL
        return;
    }
    else if (head == tail)
    { // if there's only 1 node
        free(head);
        head = tail = NULL;
    }
    else
    { // if there's more than 1 node, we need a new tail candidate
        Node *newTail = tail->prev;
        tail->prev = newTail->next = NULL;
        free(tail);
        tail = newTail;
    }
}

void popHead()
{ // Function to remove first Menu (popHead - Algorithm)
    if (!head)
    { // if there's no node in LL
        return;
    }
    else if (head == tail)
    { // if there's only 1 node
        free(head);
        head = tail = NULL;
    }
    else
    { // if there's more than 1 node, we need a new head candidate
        Node *newHead = head->next;
        head->next = newHead->prev = NULL;
        free(head);
        head = newHead;
    }
}

int removeDish(char namaMenu[])
{ // Function to remove dish (popMid - Algorithm)
    if (!head)
    {             // if there's no node in LL
        return 0; // do nothing
    }
    else if (strcmp(head->namaMenu, namaMenu) == 0)
    {              // if removed dish is in first position (head),
        popHead(); // use popHead() function
        return 1;
    }
    else if (strcmp(tail->namaMenu, namaMenu) == 0)
    {              // if removed dish is in last position (tail),
        popTail(); // use popTail() function
        return 1;
    }
    else
    { // if query is in the middle of the LL

        Node *current = head; // start from beggining (head)
        while (current && strcmp(current->namaMenu, namaMenu) != 0)
        { // check if current node name == target removed name
            if (current == tail)
            {             // if removed dish not found until last menu,
                return 0; // return 0
            }
            current = current->next;
        }

        current->prev->next = current->next;
        current->next->prev = current->prev;
        current->prev = current->next = NULL;
        free(current);
        current = NULL;
        return 1;
    }

    CURRENT_MENU--;
    // return 0 - dish not found / doesn't exist
    // return 1 - dish is removed succesfully
}

int checkDuplicateMenu(char namaMenu[])
{ // function to check if the menu is unique or not in the linked list
    if (!head)
    {
        return 1;
    }
    else
    {
        Node *current = head; // start from beggining (head)
        while (current)
        { // check if there is still element
            if (strcmp(current->namaMenu, namaMenu) == 0)
            {             // if dish wanted to add is a duplicate
                return 0; // return 0
            }
            current = current->next;
        }
        return 1;
    }

    // return 0 - dish is duplicate
    // return 1 - dish is unique
}

// Hash functions to store customer's name (chaining method)
struct cusNode
{ // Double Linked List to store names and orders
    char cusName[50];
    cusNode *nextCus;
    Node *orderList;
} *cusHead[MAX_BUCKETS], *cusTail[MAX_BUCKETS];

cusNode *createNodeCus(const char *name)
{
    cusNode *newNode = (cusNode *)malloc(sizeof(cusNode));
    strcpy(newNode->cusName, name);
    newNode->nextCus = NULL;
    newNode->orderList = NULL;

    return newNode;
}

unsigned long djb2(const char *str)
{ // hashing algorithm of DJB2 HASHING
    unsigned long key = 5381;

    for (int i = 0; i < strlen(str); i++)
    {
        int ascii = int(str[i]);
        key = (key << 5) + key + ascii;
    }

    return key % MAX_BUCKETS;
}

// Function for 3. Add Customer
void insertCustomer(const char *str)
{ // function to insert name into hash table
    cusNode *newNode = createNodeCus(str);
    int index = djb2(str);

    if (!cusHead[index])
    { // if LL is empty at the specified index
        cusHead[index] = cusTail[index] = newNode;
    }
    else
    { // push tail
        cusTail[index]->nextCus = newNode;
        cusTail[index] = newNode;
    }

    CURRENT_CUSTOMER++;
}

// Function for 6. Order
void insertOrder(const char *nama, const char *menu, int jumlah)
{
    cusNode *curr;

    bool isFoundCustomer = false;

    for (int i = 0; i < MAX_BUCKETS; i++)
    { // loop until find the customer's specified index in the hash table
        if (!cusHead[i])
        {
            continue;
        }
        else
        { // loop through the index to find any possible match name
            curr = cusHead[i];
            while (curr)
            {
                if (strcmp(curr->cusName, nama) == 0)
                {
                    isFoundCustomer = true;
                    break;
                }
                curr = curr->nextCus;
            }

            if (isFoundCustomer)
            {
                break;
            }
        }
    }

    // sudah dapat posisi customer sekarang

    Node *temp = head;
    while (temp)
    {
        if (strcmp(temp->namaMenu, menu) == 0)
        {
            break;
        }
        temp = temp->next;
    }

    // temp sudah di posisi data menu yang ingin diambil

    Node *order = createNode(temp->namaMenu, temp->harga, jumlah);

    temp->stok -= order->stok; // kurangin stok sekarang sama yang di order
    if (temp->stok == 0)
    { // kalau stok udah abis, remove menu
        removeDish(temp->namaMenu);
    }

    if (curr->orderList == NULL)
    {
        curr->orderList = order;
    }
    else
    {
        Node *position = curr->orderList;

        while (position)
        {
            if (position->next == NULL)
            {
                break;
            }
            position = position->next;
        }

        position->next = order;
    }
}

void traverseHash(int i)
{ // function to traverse(loop) the hash table specified index
    cusNode *curr = cusHead[i];
    while (curr)
    {
        printf("%s -> ", curr->cusName);
        curr = curr->nextCus;
    }
    puts("Null");
}

void readCustomer()
{ // function to print the current hashing table (to check program)
    for (int i = 0; i < MAX_BUCKETS; i++)
    {
        printf("Index %d: ", i);
        if (cusHead[i])
        {
            traverseHash(i);
        }
        else
        {
            puts("NULL");
        }
    }
}

// Searching customers in the hash table
int traverseSearch(int i, const char *str)
{ // function to traverse and compare customer's names
    cusNode *curr = cusHead[i]->nextCus;
    while (curr)
    {
        if (strcmp(curr->cusName, str) == 0)
        {
            return 1;
        }
        curr = curr->nextCus;
    }
    return 0;
}

// Function for 4. Search Customer
int searchCustomer(const char *str)
{ // function to check and compare names in the hash table
    for (int i = 0; i < MAX_BUCKETS; i++)
    {
        if (!cusHead[i])
        { // if index has no data (NULL)
            continue;
        }
        else if (strcmp(cusHead[i]->cusName, str) == 0)
        { // if customer's name immediately match with the head element
            return 1;
        }
        else
        { // loop through the index to find any possible match name
            if (traverseSearch(i, str))
            {
                return 1;
            }
        }
    }

    return 0;
}

// Display all customers in the hash table
int traverseView(int i, int counter)
{ // function to traverse all customer's at the specified index hash table
    cusNode *curr = cusHead[i];
    while (curr)
    {
        printf("%2d. %s\n", counter, curr->cusName);
        counter++;
        curr = curr->nextCus;
    }

    return counter;
}

// Function for 5. View Warteg
void viewWarteg()
{ // function to check all customer's list
    int totalCus = 1;
    for (int i = 0; i < MAX_BUCKETS; i++)
    {
        if (!cusHead[i])
        {
            continue;
        }
        else
        {
            totalCus = traverseView(i, totalCus);
        }
    }
}

int traverseSearchIndex(int i, int counter, int index, char *copy)
{
    int a = counter;
    cusNode *curr = cusHead[i];
    while (curr)
    {
        if (a == index)
        {
            strcpy(copy, curr->cusName);
            return 0;
        }
        a++;
        curr = curr->nextCus;
    }

    return a;
}

int searchCusIndex(int index, char *copy)
{
    int totalCus = 1;
    for (int i = 0; i < MAX_BUCKETS; i++)
    {
        if (!cusHead[i])
        {
            continue;
        }
        else
        {
            totalCus = traverseSearchIndex(i, totalCus, index, copy);
            if (totalCus == 0)
            {
                return 1;
            }
        }
    }

    return 0;
}

void popHeadCustomer(int headNow)
{
    if (cusHead[headNow] == cusTail[headNow])
    {
        free(cusHead[headNow]);
        cusHead[headNow] = cusTail[headNow] = NULL;
    }
    else
    {
        cusNode *temp = cusHead[headNow];
        temp = temp->nextCus;
        cusHead[headNow]->nextCus = NULL;
        free(cusHead[headNow]);
        cusHead[headNow] = temp;
    }
}

void popTailCustomer(int headNow)
{
    if (cusHead[headNow] == cusTail[headNow])
    {
        free(cusHead[headNow]);
        cusHead[headNow] = cusTail[headNow] = NULL;
    }
    else
    {
        cusNode *temp = cusHead[headNow];

        while (temp->nextCus != cusTail[headNow])
        {
            temp = temp->nextCus;
        }

        temp->nextCus = NULL;
        free(cusTail[headNow]);
        cusTail[headNow] = temp;
    }
}

void removeCustomer(cusNode *curr, int headNow)
{ // remove customer by pop mid algorithm
    if (!cusHead[headNow])
    { // if there is nothing
        return;
    }
    else if (strcmp(cusHead[headNow]->cusName, curr->cusName) == 0)
    { // if element in head(pop head)
        popHeadCustomer(headNow);
    }
    else if (strcmp(cusTail[headNow]->cusName, curr->cusName) == 0)
    { // if element in tail (pop tail)
        popTailCustomer(headNow);
    }
    else
    {
        cusNode *temp = cusHead[headNow];

        while (temp && strcmp(temp->nextCus->cusName, curr->cusName) != 0)
        {
            temp = temp->nextCus;
        }

        cusNode *targetRemove = temp->nextCus;

        temp->nextCus = targetRemove->nextCus;
        free(targetRemove);
        targetRemove = NULL;
    }

    CURRENT_CUSTOMER--;
}

// Function for 7. Payment
void printAllOrder(const char *name)
{
    cusNode *curr;

    bool isFoundCustomer = false;

    int headNow;

    for (int i = 0; i < MAX_BUCKETS; i++)
    { // loop until find the customer's specified index in the hash table
        if (!cusHead[i])
        {
            continue;
        }
        else
        { // loop through the index to find any possible match name
            curr = cusHead[i];
            while (curr)
            {
                if (strcmp(curr->cusName, name) == 0)
                {
                    isFoundCustomer = true;
                    break;
                }
                curr = curr->nextCus;
            }

            if (isFoundCustomer)
            {
                headNow = i;
                break;
            }
        }
    }

    // found the customer's data

    Node *position = curr->orderList;
    int counter = 1;
    int sumHarga = 0;

    while (position)
    {
        printf("[%d] %s x%d\n", counter, position->namaMenu, position->stok);
        sumHarga += position->harga * position->stok;

        counter++;
        position = position->next;
    }

    printf("Total: Rp%d\n", sumHarga);

    removeCustomer(curr, headNow);
}

int validasiOrder(char namaMenu[], int stok)
{
    if (!head)
    {
        return 0;
    }
    else
    {
        Node *current = head;
        while (current)
        {
            if (strcmp(current->namaMenu, namaMenu) == 0 && stok <= current->stok)
            {
                return 1;
            }
            current = current->next;
        }
        return 0;
    }
}

int main()
{
    while (true)
    {
        // print user's operating system
        struct utsname system;                  // PLEASE COMMENT THIS LINE, IF CODE DOESN'T RUN
        uname(&system);                         // PLEASE COMMENT THIS LINE, IF CODE DOESN'T RUN
        printf("System: %s\n", system.sysname); // PLEASE COMMENT THIS LINE, IF CODE DOESN'T RUN

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
        do
        {
            printf(">> ");
            scanf("%d", &menu);
            getchar();
        } while (menu < 1 || menu > 8);
        clear();

        switch (menu)
        {
        case 1:
        { // 1. Add Dish
            char namaMenu[20] = {0};
            int hargaMenu;
            int stokMenu;

            // input name of the dish
            bool validNamaMenu;
            do
            {
                printf("Insert the name of the dish [Lowercase letters]: ");
                scanf("%[^\n]", namaMenu);
                getchar();
                validNamaMenu = true;
                for (int i = 0; i < strlen(namaMenu); i++)
                {
                    if (isupper(namaMenu[i]))
                    { // validation to check lower case letter
                        validNamaMenu = false;
                        break;
                    }
                    else if (!isalpha(namaMenu[i]) && !isspace(namaMenu[i]))
                    { // validation to check only alphabet and space
                        validNamaMenu = false;
                        break;
                    }

                    validNamaMenu = checkDuplicateMenu(namaMenu); // check if menu is unique
                }
            } while (!validNamaMenu);

            // input price of the dish
            bool ValidHargaMenu;
            do
            {
                printf("Insert the price of the dish [1000..50000]: ");
                scanf("%d", &hargaMenu);
                ValidHargaMenu = true;

                if (hargaMenu < 1000 || hargaMenu > 50000)
                { // validation to check price
                    ValidHargaMenu = false;
                }
            } while (!ValidHargaMenu);

            // input the stock of the dish
            bool validStockMenu;
            do
            {
                printf("Insert the quantity of the dish [1..999]: ");
                scanf("%d", &stokMenu);
                validStockMenu = true;

                if (stokMenu < 1 || stokMenu > 999)
                { // validation to check stock number
                    validStockMenu = false;
                }
            } while (!validStockMenu);

            addDish(namaMenu, hargaMenu, stokMenu);

            puts("The dish has been added!");
            puts("Press enter to continue...");
            getchar();
            getchar();
            clear();
            break;
        }
        case 2:
        { // 2. Remove Dish
            int headline = 30;
            char namaResto[20] = "Bude's Menu";
            printf("%*s\n", (int)(headline / 2 + strlen(namaResto) / 2), namaResto);
            printf("==============================\n");
            printf("No.      Name         Quantity     Price  \n");
            if (head == NULL)
            {
                printf("\nNo dish is available.\n\n");
                printf("==============================\n");
            }
            else
            {
                printMenu();
                printf("==============================\n");
                printf("Insert dish's name to be deleted: ");
                char targetDish[20] = {0};
                scanf("%[^\n]", targetDish);
                getchar();

                int check = removeDish(targetDish);

                if (check == 1)
                { // true(1) - dish is removed
                    puts("The dish has been removed!");
                }
                else
                { // false(0) - dish not found
                    puts("Invalid input, dish not found!");
                }
            }
            puts("Press enter to continue...");
            getchar();
            clear();
            break;
        }
        case 3:
        { // 3. Add Customer
            bool validCusName;
            do
            {
                printf("Insert the customer's name [Without space]: ");
                char tempCusName[50] = {0};
                scanf("%[^\n]", tempCusName);
                getchar();
                validCusName = true;

                for (int i = 0; i < strlen(tempCusName); i++)
                {
                    if (!isalpha(tempCusName[i]) || isspace(tempCusName[i]))
                    { // validation to check only alphabet and no space
                        validCusName = false;
                        break;
                    }
                }

                if (validCusName)
                {
                    insertCustomer(tempCusName); // if valid, insert to hashtable
                }
            } while (!validCusName);

            puts("Customer has been added!");
            puts("Press enter to continue...");
            getchar();
            clear();
            break;
        }
        case 4:
        { // 4. Search Customer
            bool validSearchCus;
            do
            {
                printf("Insert the customer's name to be searched: ");
                char tempCheckCus[50] = {0};
                scanf("%[^\n]", tempCheckCus);
                getchar();
                validSearchCus = true;

                for (int i = 0; i < strlen(tempCheckCus); i++)
                {
                    if (!isalpha(tempCheckCus[i]) || isspace(tempCheckCus[i]))
                    { // validation to check only alphabet and no space
                        validSearchCus = false;
                        break;
                    }
                }

                if (validSearchCus)
                {
                    bool cusFound = searchCustomer(tempCheckCus); // if valid, search customer in hashtable
                    if (cusFound)
                    { // if target customer is found
                        printf("%s is present.\n", tempCheckCus);
                    }
                    else
                    {
                        printf("%s is not present.\n", tempCheckCus);
                    }
                }

            } while (!validSearchCus);

            puts("Press enter to continue...");
            getchar();
            clear();
            break;
        }
        case 5:
        { // 5. View Warteg
            // readCustomer(); // checking customers

            puts("Customer's List");

            if (CURRENT_CUSTOMER == 0)
            {
                printf("\nNo customer is available right now.\n\n");
                puts("Press enter to continue...");
                getchar();
                clear();
                break;
            }

            viewWarteg();

            puts("Press enter to continue...");
            getchar();
            clear();
            break;
        }
        case 6:
        { // 6. Order
            if (CURRENT_MENU == 0)
            {
                puts("No dish is available right now.");
                puts("Press enter to continue...");
                getchar();
                clear();
                break;
            }

            if (CURRENT_CUSTOMER == 0)
            {
                puts("No customer is available right now.");
                puts("Press enter to continue...");
                getchar();
                clear();
                break;
            }

            char customerName[50] = {0};
            int orderTotal;

            bool validOrderCus;
            do
            {
                char tempCheckOrderName[50] = {0};
                printf("Insert the customer's name: ");
                scanf("%[^\n]", tempCheckOrderName);
                getchar();
                validOrderCus = true;

                for (int i = 0; i < strlen(tempCheckOrderName); i++)
                {
                    if (!isalpha(tempCheckOrderName[i]) || isspace(tempCheckOrderName[i]))
                    { // validation to check only alphabet and no space
                        validOrderCus = false;
                        break;
                    }
                }

                if (validOrderCus)
                {
                    bool cusOrderFound = searchCustomer(tempCheckOrderName); // if valid, search customer in hashtable
                    if (!cusOrderFound)
                    { // if target customer is not found
                        validOrderCus = false;
                    }
                    else
                    {
                        strcpy(customerName, tempCheckOrderName);
                    }
                }
            } while (!validOrderCus);

            do
            {
                printf("Insert the amount of dish: ");
                scanf("%d", &orderTotal);
            } while (orderTotal <= 0);

            // addOrder(customerName, orderTotal);

            for (int i = 1; i <= orderTotal; i++)
            {
                char namaMenuOrder[20] = {0};
                int tempJumlahOrder;
                bool isValidOrder;
                do
                {
                    char tempNamaMenu[20] = {0};
                    char namaMenuFormatted[20] = {0};
                    printf("[%d] Insert the dish's name and quantity: ", i);
                    getchar();

                    // Format input is - [nama makanan] x2
                    // Example = telur goreng x5
                    scanf("%[^x]x%d", tempNamaMenu, &tempJumlahOrder);

                    for (int j = 0; j < strlen(tempNamaMenu) - 1; j++)
                    {
                        namaMenuFormatted[j] = tempNamaMenu[j];
                    }

                    isValidOrder = validasiOrder(namaMenuFormatted, tempJumlahOrder);

                    if (isValidOrder)
                    {
                        strcpy(namaMenuOrder, namaMenuFormatted);
                    }
                } while (!isValidOrder);

                insertOrder(customerName, namaMenuOrder, tempJumlahOrder);
            }

            getchar();
            puts("Order success!");
            puts("Press enter to continue...");
            getchar();
            clear();
            break;
        }
        case 7:
        { // 7. Payment
            if (CURRENT_CUSTOMER == 0)
            {
                puts("No customer is available right now.");
                puts("Press enter to continue...");
                getchar();
                clear();
                break;
            }

            int indexSearch;

            do
            {
                printf("Insert the customer's index: ");
                scanf("%d", &indexSearch);
                getchar();
            } while (indexSearch <= 0 || indexSearch > CURRENT_CUSTOMER);

            char namaCustomer[20] = {0};
            searchCusIndex(indexSearch, namaCustomer);

            printf("\n%s\n", namaCustomer);
            printAllOrder(namaCustomer);

            puts("Press enter to continue...");
            getchar();
            clear();
            break;
        }
        case 8:
        { // 8. Exit Warteg
            puts("Please expand your terminal to full screen!");
            puts("Press enter to continue...");
            getchar();

            FILE *fp = fopen("splash-screen.txt", "r");

            char line[256];
            while (fgets(line, sizeof(line), fp))
            {
                printf("%s", line);
            }

            fclose(fp);
            exit(0);
        }
        }
    }

    return 0;
}