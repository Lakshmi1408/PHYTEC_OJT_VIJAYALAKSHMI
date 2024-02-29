#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_FOOD_ITEMS 5
#define NUM_JUICE_ITEMS 4

struct MenuItem {
    char name[50];
    float price;
};

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void displayMenu(struct MenuItem menu[], int numItems) {
    printf("\n------------------ MENU ------------------\n");
    for (int i = 0; i < numItems; i++) {
        printf("%d. %s Rs.%.2f\n", i + 1, menu[i].name, menu[i].price);
    }
    printf("------------------------------------------\n");
}

float calculateTotal(float prices[], int quantities[], int numItems) {
    float total = 0;
    for (int i = 0; i < numItems; i++) {
        total += prices[i] * quantities[i];
    }
    return total;
}

void addFoodOrder(struct MenuItem menu[], int quantities[], float prices[]) {
    displayMenu(menu, NUM_FOOD_ITEMS);
    int choice, quantity;
    printf("Enter the item number (0 to cancel): ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 0) {
        printf("Order canceled.\n");
        return;
    }

    if (choice >= 1 && choice <= NUM_FOOD_ITEMS) {
        printf("Enter the quantity: ");
        scanf("%d", &quantity);
        clearInputBuffer();
        quantities[choice - 1] += quantity;
        prices[choice - 1] = menu[choice - 1].price;
        printf("%s (x%d) added to the order.\n", menu[choice - 1].name, quantity);
    } else {
        printf("Invalid item number.\n");
    }
}

void addJuiceOrder(struct MenuItem menu[], int quantities[], float prices[]) {
    displayMenu(menu, NUM_JUICE_ITEMS);
    int choice, quantity;
    printf("Enter the item number (0 to cancel): ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 0) {
        printf("Order canceled.\n");
        return;
    }

    if (choice >= 1 && choice <= NUM_JUICE_ITEMS) {
        printf("Enter the quantity: ");
        scanf("%d", &quantity);
        clearInputBuffer();
        quantities[choice - 1] += quantity;
        prices[choice - 1] = menu[choice - 1].price;
        printf("%s (x%d) added to the order.\n", menu[choice - 1].name, quantity);
    } else {
        printf("Invalid item number.\n");
    }
}

int main() {
	printf("***********************WELCOME****************************"); 
    struct MenuItem foodMenu[NUM_FOOD_ITEMS] = {
        {"Burger", 90.0},
        {"Pizza", 120.0},
        {"Pasta", 90.0},
        {"Salad", 70.0},
        {"Steak", 200.0}
    };

    struct MenuItem juiceMenu[NUM_JUICE_ITEMS] = {
        {"Orange Juice", 40.0},
        {"Apple Juice", 50.0},
        {"Lemonade", 30.0},
        {"Iced Tea", 20.0}
    };

    int foodQuantities[NUM_FOOD_ITEMS] = {0};
    int juiceQuantities[NUM_JUICE_ITEMS] = {0};
    float foodPrices[NUM_FOOD_ITEMS] = {0};
    float juicePrices[NUM_JUICE_ITEMS] = {0};

    while (1) {
        printf("\n--------MAIN MENU------- \n");
        printf("1. Food Menu\n");
        printf("2. Juice Menu\n");
        printf("3. Show Total Bill for Food\n");
        printf("4. Show Total Bill for Juice\n");
        printf("5. Show Total Bill for Both\n");
        printf("6. Exit\n");
        printf("-----------------------------------------------\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                addFoodOrder(foodMenu, foodQuantities, foodPrices);
                break;

            case 2:
                addJuiceOrder(juiceMenu, juiceQuantities, juicePrices);
                break;

            case 3:
                printf("\nTotal Food Bill: Rs.%.2f\n", calculateTotal(foodPrices, foodQuantities, NUM_FOOD_ITEMS));
                break;

            case 4:
                printf("\nTotal Juice Bill: Rs.%.2f\n", calculateTotal(juicePrices, juiceQuantities, NUM_JUICE_ITEMS));
                break;

            case 5:
                printf("\nTotal Food Bill: Rs.%.2f\n", calculateTotal(foodPrices, foodQuantities, NUM_FOOD_ITEMS));
                printf("Total Juice Bill: Rs.%.2f\n", calculateTotal(juicePrices, juiceQuantities, NUM_JUICE_ITEMS));
                printf("Total Bill : Rs.%.2f\n",
                       calculateTotal(foodPrices, foodQuantities, NUM_FOOD_ITEMS) +
                       calculateTotal(juicePrices, juiceQuantities, NUM_JUICE_ITEMS));
                break;

            case 6:
                printf("Exiting. Thank you!\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

