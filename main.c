/**
 * This is a programming project for the ITELOG subject.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants

#define MAX_HISTORY_STACK 25
#define MAX_CART_SIZE 300
#define MAX_QUANTITY 99
#define MAX_CASH 99999999

/*==============================*
 *          PRODUCTS            *
 *==============================*/

const char *hotCoffeeFlavors[] = {
    "Vanilla",
    "Caramel",
    "Classic",
    "Hazelnut",
    "Mocha",
    "Pumpkin Spice",
    "Cinnamon",
    "Matcha",
};

const char *icedCoffeeFlavors[] = {
    "Vanilla",
    "Caramel Macchiato",
    "Americano",
    "Hazelnut",
    "Mocha",
    "Pumpkin Spice",
    "Cinnamon",
    "Cafe Mocha",
    "Salted Caramel Latte",
    "White Chocolate Mocha",
    "Matcha",
};

const char *icedCoffeeSizes[] = {
    "12oz",
    "18oz",
    "20oz",
};

const float hotCoffeePrice = 30.0;

const float icedCoffeePrices[] = {
    50.0,
    80.0,
    120.0,
};

const char *milkTeaFlavors[] = {
    "Mocha",
    "Okinawa",
    "Taro",
    "Matcha",
    "Chocolate",
    "Wintermelon",
    "Honey",
    "Pearl Milk Tea",
    "Honey",
    "Cookies and Cream",
};

const char *milkTeaSizes[] = {
    "18oz",
    "20oz",
};

const float milkTeaPrices[] = {
    100.0,
    150.0,
};

/**
 * Gets the index of the `size`
 *
 * @param size the size you want the index of
 * @param sizes the array of sizes
 * @param sizeCount the count of the array
 *
 * @return
 * The index of the size else `-1`
 */
int getSizeIndex(const char *size, const char *sizes[], unsigned int sizeCount)
{
    int index = -1;

    for (int i = 0; i < sizeCount; i++)
    {
        if (strcmp(sizes[i], size) == 0)
        {
            index = i;
            break;
        }
    }

    return index;
}

/**
 * Gets the product's price.
 *
 * @param name the product's name
 * @param size the product's size
 *
 * @return
 * The product's price else -1
 */
float getProductPrice(const char *name, const char *size)
{

    if (strcmp(name, "Hot Coffee") == 0)
    {
        return hotCoffeePrice;
    }
    else if (strcmp(name, "Iced Coffee") == 0)
    {
        int index = getSizeIndex(size, icedCoffeeSizes, 3);
        return icedCoffeePrices[index];
    }
    else if (strcmp(name, "Milk Tea") == 0)
    {
        int index = getSizeIndex(size, milkTeaSizes, 2);
        return milkTeaPrices[index];
    }

    return -1;
}

/*==============================*
 *             CART             *
 *==============================*/

/**
 * The object for storing the cart item
 */
typedef struct
{
    int quantity;

    char name[30];
    char flavor[30];
    char size[10];
} CartItem;

/**
 * The object for storing the user info and currently ordering item
 */
typedef struct
{
    int quantity;
    // the index of the product you want to edit
    int cartProductIndex;

    float cash;

    // The current order is stored here
    // I can use the `CartItem` struct here...
    char name[30];
    char flavor[30];
    char size[10];
} UserInfo;

int cartSize = 0;
CartItem cart[MAX_CART_SIZE];

/**
 * Gets the Item's full name.
 *
 * Merges the `name`,`flavor`, and `size` into a single string.
 *
 * @param cartItem the item you want to get the full name
 *
 * @return
 * The item's full name (eg. `"Iced CoffeeVanilla12oz"`)
 */
char *getItemFullName(CartItem cartItem)
{
    char *fullName = malloc(strlen(cartItem.name) + strlen(cartItem.flavor) + strlen(cartItem.size) + 1);

    strcpy(fullName, cartItem.name);
    strcat(fullName, cartItem.flavor);
    strcat(fullName, cartItem.size);

    return fullName;
}

/**
 * Find the item index by using the name
 *
 * @param name the item's full name
 *
 * @return
 * The index of the item. if not found returns `-1`.
 */
int findItemIndexByName(const char *name)
{
    int index = -1;
    for (int i = 0; i < cartSize; i++)
    {

        char *productName = getItemFullName(cart[i]);
        if (strcmp(productName, name) == 0)
        {
            free(productName);
            index = i;
            break;
        }

        free(productName);
    }

    return index;
}

/**
 * Add the item to the cart
 *
 * Automatically detects if there's an existing item and merges it.
 *
 * @param cartItem the item to add
 */
void addToCart(CartItem cartItem)
{

    if (cartItem.quantity <= 0)
        return;

    if (cartItem.quantity > MAX_QUANTITY)
        cartItem.quantity = MAX_QUANTITY;

    int duplicate = findItemIndexByName(getItemFullName(cartItem));

    if (duplicate == -1)
    {

        if (cartSize < MAX_CART_SIZE)
        {
            cart[cartSize] = cartItem;
            cartSize++;
        }
    }
    else
    {
        cart[duplicate].quantity += cartItem.quantity;

        if (cart[duplicate].quantity > MAX_QUANTITY)
            cart[duplicate].quantity = MAX_QUANTITY;
    }
}

/**
 * Removes the item from the cart using the index
 *
 * @param index the item index
 */
void removeFromCart(int index)
{
    if (index >= 0 && index < cartSize)
    {

        for (int i = index; i < cartSize - 1; i++)
        {
            cart[i] = cart[i + 1];
        }

        cartSize--;
    }
}

/***
 * Removes the item from the cart using the name
 *
 * @param name the item's full name
 */
void removeFromCartByName(const char *name)
{
    int index = findItemIndexByName(name);

    if (index != -1)
    {
        removeFromCart(index);
    }
}

/**
 * Sets the item from `index` to the desired quantity.
 *
 * @param index the item index
 * @param quantity the desired quantity (`0` to `MAX_QUANTITY`). 0 removes the item from the cart.
 */
void setCartItemQuantityByIndex(int index, int quantity)
{

    if (index >= 0 && index < cartSize)
    {
        // removes the item when the quantity is 0.
        if (quantity <= 0)
        {
            removeFromCart(index);
            return;
        }

        // limits the quantity
        if (quantity > MAX_QUANTITY)
            quantity = MAX_QUANTITY;

        cart[index].quantity = quantity;
    }
}

/**
 * Gets the total price of the cart.
 *
 * @return
 * The price of all the items in the cart
 */
float getCartTotalPrice()
{
    float total;

    for (int i = 0; i < cartSize; i++)
    {
        CartItem item = cart[i];

        float price = getProductPrice(item.name, item.size);

        float subtotal = (float)item.quantity * price;

        total += subtotal;
    }

    return total;
}

/**
 * Lists all the items in the cart.
 */
void listCartItems()
{
    printf("+-----+-----------------------------------------------------------------+------------+\n");
    printf("|  #  |    Name                         Size        Price      Quantity |  Subtotal  |\n");
    printf("+-----+-----------------------------------------------------------------+------------+\n");
    for (int i = 0; i < cartSize; i++)
    {
        CartItem item = cart[i];

        float price = getProductPrice(item.name, item.size);

        float subtotal = (float)item.quantity * price;

        printf("| %3d | %-32s%-10s %6.2f   x %5d     |%10.2f  |\n", i + 1, item.name, item.size, price, item.quantity, subtotal);
        printf("|     | └ %-32s                              |            |\n", item.flavor);
    }

    float total = getCartTotalPrice();
    printf("+-----+-----------------------------------------------------------------+------------+\n");
    printf("|                                                            Total: %15.2f  |\n", total);
}

/**
 * Shows the receipt.
 *
 * @param cashAmount the cash you need
 *
 * @return
 * The change of the transaction.
 */
float showReceipt(float cashAmount)
{

    float total = getCartTotalPrice();

    float change = cashAmount - total;
    printf("+------------------------------------------------------------------------------------+\n");
    printf("|                                   Yummy Tea Cafe                                   |\n");
    printf("+------------------------------------------------------------------------------------+\n");
    printf("|                                      Receipt                                       |\n");
    listCartItems();
    printf("|                                                            Cash:  %15.2f  |\n", cashAmount);
    printf("|                                                            Change: %14.2f  |\n", change);
    printf("+------------------------------------------------------------------------------------+\n");

    return change;
}

/**
 * Shows the cart
 */
void showCart()
{

    printf("+------------------------------------------------------------------------------------+\n");
    printf("|                                        Cart                                        |\n");
    listCartItems();
    printf("+------------------------------------------------------------------------------------+\n");
}

/*==============================*
 *          NAVIGATION          *
 *==============================*/
typedef enum
{
    MENU_EVENT_NO_EVENT = -1,
    MENU_EVENT_QUIT,
    MENU_EVENT_SET_PRODUCT_NAME,
    MENU_EVENT_SET_FLAVOR,
    MENU_EVENT_SET_SIZE,
    MENU_EVENT_SET_QUANTITY,
    MENU_EVENT_SET_CASH,
    MENU_EVENT_SET_CART_EDIT_INDEX,
    MENU_EVENT_ADD_TO_CART,
    MENU_EVENT_RESET,
} MENU_EVENTS;

typedef struct
{
    unsigned int id;
    int numberValue;
    float floatValue;
    char *stringValue;
} MenuEvent;

typedef struct
{
    char *name;
    MenuEvent (*action)();
} MenuPage;

MenuPage history[MAX_HISTORY_STACK];
int historySize = 0;

/**
 * Pops the topmost history from the stack
 */
void historyPop()
{

    if (historySize > 0)
    {
        history[historySize--];
    }
}

/**
 * Pops the history until it reaches the `name`
 *
 * @param name the `MenuPage` name;
 */
void historyPopUntil(char *name)
{
    MenuPage menuPage;
    menuPage.name = ""; // prevents Segmentation Error

    while (strcmp(menuPage.name, name) != 0)
    {
        menuPage = history[historySize - 1];

        historyPop();
    }
}

/**
 * Pushes the `MenuPage` to the top of the history
 *
 * @param menuPage the page you want to push
 */
void historyPush(MenuPage menuPage)
{
    if (historySize < MAX_HISTORY_STACK - 1)
    {
        history[++historySize] = menuPage;
    }
}

/*==============================*
 *            MENUS             *
 *==============================*/

// function prototypes

MenuEvent showMainMenu(UserInfo info);
MenuEvent showSelectCoffeeTypeMenu(UserInfo info);
MenuEvent showSelectHotCoffeeFlavorMenu(UserInfo info);
MenuEvent showSelectHotCoffeeSizeMenu(UserInfo info);
MenuEvent showSelectIcedCoffeeFlavorMenu(UserInfo info);
MenuEvent showSelectIcedCoffeeSizeMenu(UserInfo info);
MenuEvent showSelectMilkTeaFlavorMenu(UserInfo info);
MenuEvent showSelectMilkTeaSizeMenu(UserInfo info);
MenuEvent showSetQuantityMenu(UserInfo info);
MenuEvent showNavigationMenu(UserInfo info);
MenuEvent showPurchaseMenu(UserInfo info);
MenuEvent showReceiptMenu(UserInfo info);
MenuEvent showCartMenu(UserInfo info);
MenuEvent showCartEditMenu(UserInfo info);

// Pages

MenuPage mainMenuPage = {"Main Menu", showMainMenu};
MenuPage coffeeTypePage = {"Coffee Type", showSelectCoffeeTypeMenu};
MenuPage hotCoffeeFlavorsPage = {"Hot Coffee Flavors", showSelectHotCoffeeFlavorMenu};
MenuPage hotCoffeeSizesPage = {"Hot Coffee Sizes", showSelectHotCoffeeSizeMenu};
MenuPage icedCoffeeFlavorsPage = {"Iced Coffee Flavors", showSelectIcedCoffeeFlavorMenu};
MenuPage icedCoffeeSizesPage = {"Iced Coffee Sizes", showSelectIcedCoffeeSizeMenu};
MenuPage milkTeaFlavorsPage = {"Milk Tea Flavors", showSelectMilkTeaFlavorMenu};
MenuPage milkTeaSizesPage = {"Milk Tea Sizes", showSelectMilkTeaSizeMenu};
MenuPage setQuantityPage = {"Set Quantity", showSetQuantityMenu};
MenuPage navigationPage = {"Navigation", showNavigationMenu};
MenuPage purchasePage = {"Purchase", showPurchaseMenu};
MenuPage receiptPage = {"Receipt", showReceiptMenu};
MenuPage cartPage = {"Cart", showCartMenu};
MenuPage cartEditPage = {"Cart Edit", showCartEditMenu};

/**
 * Clears the terminal by printing 50 newlines.
 */
void clearTerminal()
{
    for (int i = 0; i < 50; i++)
    {
        printf("\n");
    }
}

/**
 * Prints the menu name in a specific format
 *
 * @param menuName the name of the menu
 */
void showMenuName(const char *menuName)
{
    printf("+-------------------------------------------------------------+\n");
    printf("| > %-57s |\n", menuName, "");
    printf("+-------------------------------------------------------------+\n");
}

/**
 * Prints the cart choice with the total items and cost of the cart
 */
void cartChoiceDisplay()
{
    printf("|    >  Cart  [ %3d Items , Total:  %12.2f ]            |\n", cartSize, getCartTotalPrice());
}

/**
 * Shows the menu with the lists of flavors.
 *
 * @param menuName the name of the menu
 * @param menuItems the array of flavors
 * @param menuItemCount the count of the flavors in the array
 */
void showMenuFlavorsItems(const char *menuName, const char *menuItems[], unsigned int menuItemCount)
{

    showMenuName(menuName);
    printf("|                                                             |\n");
    // printf("|      >  None                                                |\n");
    for (int i = 0; i < menuItemCount; i++)
    {
        printf("|      >  %-25s %-25s |\n", menuItems[i], "");
    }
    printf("|                                                             |\n");
    cartChoiceDisplay();
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");
}

/**
 * Prints the current order.
 *
 * @param info the user info
 * @param removeTop set to 1 to remove the top
 * @param removeBottom set to 1 to remove the bottom
 */
void showCurrentOrder(UserInfo info, int removeTop, int removeBottom)
{

    float price = getProductPrice(info.name, info.size);
    float subtotal = 0;
    float addonSubtotal = 0;

    if (price < 0)
        price = 0;

    if (price > 0)
    {
        subtotal = price * info.quantity;
    }

    float total = subtotal + addonSubtotal;

    if (removeTop == 0)
        printf("+-------------------------------------------------------------+\n");

    printf("|   Current Order:      %36.2f  |\n", total);
    printf("|   %-14s         %4s %7.2f x %2d      %10.2f  |\n", info.name, info.size, price, info.quantity, subtotal);
    printf("|   └ %-22s                                  |\n", info.flavor);
    if (removeBottom == 0)
        printf("+-------------------------------------------------------------+\n");
}

/**
 * Checks if the inputted flavor is valid
 *
 * @param flavor the flavor you want to check
 * @param flavors the array of flavors
 * @param count the count the of flavors in the array
 *
 * @return `"Invalid"` if invalid else the flavor name
 */
char *isValidFlavor(const char *flavor, const char *flavors[], unsigned int count)
{
    char *isValid = "Invalid";

    for (int i = 0; i < count; i++)
    {
        char *cflavor = strdup(flavors[i]);
        char *flavorLower = strlwr(cflavor);

        if (strcmp(flavor, flavorLower) == 0)
        {
            isValid = strdup(flavors[i]);
            free(cflavor);
            break;
        }
    }

    // if (strcmp(flavor, "none") == 0)
    // {
    //     isValid = "";
    // }

    return isValid;
}

/**
 * Checks if the inputted size is valid
 *
 * @param size the size you want to check
 * @param sizes the array of sizes
 * @param count the count the of sizes in the array
 *
 * @return `"Invalid"` if invalid else the size
 */
char *isValidSize(const char *size, const char *sizes[], unsigned int count)
{
    char *isValid = "Invalid";

    for (int i = 0; i < count; i++)
    {
        char *csize = strdup(sizes[i]);
        char *sizeLower = strlwr(csize);

        if (strcmp(size, sizeLower) == 0)
        {
            isValid = strdup(sizes[i]);
            free(csize);
            break;
        }
    }

    return isValid;
}

/**
 * Gets the inputted integer.
 *
 * @param label
 *
 * @return
 * The inputted integer
 */
int getInputInteger(const char *label)
{
    int num;
    printf("  Enter %s > ", label);
    scanf("%d", &num);

    return num;
}

/**
 * Gets the inputted string and turns it into lowercase
 *
 * @param label
 *
 * @return
 * the lowercase inputted string
 */
char *getInputString(const char *label)
{
    char buffer[30];
    printf("  Enter %s > ", label);
    fgets(buffer, 30, stdin);

    buffer[strcspn(buffer, "\n")] = 0;

    return strdup(strlwr(buffer));
}

MenuEvent showCartEditMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    CartItem item = cart[info.cartProductIndex];

    float price = getProductPrice(item.name, item.size);
    float subtotal = price * item.quantity;

    float total = subtotal;

    printf("+------------------------------------------------------------------------------------+\n");
    printf("| > Edit Item                                                                        |\n");
    printf("+------------------------------------------------------------------------------------+\n");
    printf("|                                                                                    |\n");
    printf("|     Item                                                                           |\n");
    printf("|       └ %-15s            %4s            %10.2f  x  %2d %13.2f |\n", item.name, item.size, price, item.quantity, subtotal);
    printf("|         └ %-25s                                                |\n", item.flavor);
    printf("|                                                                                    |\n");
    printf("|     Edit Commands                                                                  |\n");
    if (strcmp(item.name, "Hot Coffee") == 0)
    {
        printf("|                 > Flavor           > Quantity           > Remove                   |\n");
    }
    else
    {
        printf("|          > Flavor           > Size           > Quantity           > Remove         |\n");
    }
    printf("|                                                                                    |\n");
    printf("|    <  Back                                                                         |\n");
    printf("|                                                                                    |\n");
    printf("+------------------------------------------------------------------------------------+\n");

    char *input = getInputString("Command");

    if (strcmp(input, "back") == 0)
    {

        historyPop();
    }
    else if (strcmp(input, "remove") == 0)
    {
        char *inputAgree = getInputString("choice: Are you sure? (y/N)");

        if (strcmp(inputAgree, "y") == 0 || strcmp(inputAgree, "yes") == 0)
        {
            removeFromCart(info.cartProductIndex);
            event.id = MENU_EVENT_SET_CART_EDIT_INDEX;
            historyPop();
        }

        free(inputAgree);
    }
    else if (strcmp(input, "quantity") == 0)
    {
        int quantity = -1;

        while (quantity < 0)
        {
            quantity = getInputInteger("Quantity (Max 99)");

            if (quantity > MAX_QUANTITY)
                quantity = MAX_QUANTITY;
            if (quantity <= 0)
                quantity = 1;

            cart[info.cartProductIndex].quantity = quantity;
        }
    }
    else if (strcmp(input, "flavor") == 0)
    {
        if (strcmp(item.name, "Hot Coffee") == 0)
        {
            showMenuName("Hot Coffee Flavors");
            printf("|                                                             |\n");
            // printf("|      >  None                                                |\n");
            for (int i = 0; i < 8; i++)
            {
                printf("|      >  %-25s %-25s |\n", hotCoffeeFlavors[i], "");
            }
            printf("|                                                             |\n");
            printf("+-------------------------------------------------------------+\n");
        }
        else if (strcmp(item.name, "Iced Coffee") == 0)
        {
            showMenuName("Iced Coffee Flavors");
            printf("|                                                             |\n");
            // printf("|      >  None                                                |\n");
            for (int i = 0; i < 11; i++)
            {
                printf("|      >  %-25s %-25s |\n", icedCoffeeFlavors[i], "");
            }
            printf("|                                                             |\n");
            printf("+-------------------------------------------------------------+\n");
        }
        else if (strcmp(item.name, "Milk Tea") == 0)
        {
            showMenuName("Milk Tea Flavors");
            printf("|                                                             |\n");
            // printf("|      >  None                                                |\n");
            for (int i = 0; i < 10; i++)
            {
                printf("|      >  %-25s %-25s |\n", milkTeaFlavors[i], "");
            }
            printf("|                                                             |\n");
            printf("+-------------------------------------------------------------+\n");
        }

        char *isValid = "Invalid";

        do
        {
            char *inputFlavor = getInputString("Flavor");

            if (strcmp(item.name, "Hot Coffee") == 0)
            {
                isValid = isValidFlavor(inputFlavor, hotCoffeeFlavors, 8);
            }
            else if (strcmp(item.name, "Iced Coffee") == 0)
            {
                isValid = isValidFlavor(inputFlavor, icedCoffeeFlavors, 11);
            }
            else if (strcmp(item.name, "Milk Tea") == 0)
            {
                isValid = isValidFlavor(inputFlavor, milkTeaFlavors, 10);
            }

            free(inputFlavor);
        } while (strcmp(isValid, "Invalid") == 0);

        strcpy(cart[info.cartProductIndex].flavor, isValid);

        free(isValid);
    }
    else if (strcmp(input, "size") == 0)
    {
        if (strcmp(item.name, "Iced Coffee") == 0)
        {
            showMenuName("Iced Coffee Sizes");
            printf("|                                                             |\n");
            for (int i = 0; i < 3; i++)
            {
                printf("|      >  %-5s  -   %6.2f                                   |\n", icedCoffeeSizes[i], icedCoffeePrices[i]);
            }
            printf("|                                                             |\n");
            printf("+-------------------------------------------------------------+\n");
        }
        else if (strcmp(item.name, "Milk Tea") == 0)
        {
            showMenuName("Milk Tea Sizes");
            printf("|                                                             |\n");
            for (int i = 0; i < 2; i++)
            {
                printf("|      >  %-5s  -   %6.2f                                   |\n", milkTeaSizes[i], milkTeaPrices[i]);
            }
            printf("|                                                             |\n");
            printf("+-------------------------------------------------------------+\n");
        }

        char *isValid = "Invalid";

        do
        {
            char *inputSize = getInputString("Size");

            if (strcmp(item.name, "Iced Coffee") == 0)
            {
                isValid = isValidSize(inputSize, icedCoffeeSizes, 3);
            }
            else if (strcmp(item.name, "Milk Tea") == 0)
            {
                isValid = isValidSize(inputSize, milkTeaSizes, 2);
            }

            free(inputSize);
        } while (strcmp(isValid, "Invalid") == 0);

        strcpy(cart[info.cartProductIndex].size, isValid);

        free(isValid);
    }

    free(input);

    return event;
}

MenuEvent showCartMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showCart();
    printf("|                                                                                    |\n");
    printf("|    >  Purchase  -  Purchase everything                                             |\n");
    printf("|    >  Edit      -  Edit an item                                                    |\n");
    printf("|    <  Back                                                                         |\n");
    printf("|                                                                                    |\n");
    printf("+------------------------------------------------------------------------------------+\n");

    char *input = getInputString("Command");

    if (strcmp(input, "edit") == 0)
    {
        while (1) // infinite loop
        {
            char *inputIndex = getInputString("Product Number (-1 to exit)");
            int index = 0;

            if (sscanf(inputIndex, "%d", &index) == 1)
            {
                if (index > 0 && index <= cartSize)
                {
                    event.id = MENU_EVENT_SET_CART_EDIT_INDEX;
                    event.numberValue = index - 1;
                    historyPush(cartEditPage);
                    break;
                }
                else if (index == -1)
                {
                    break;
                }
                else
                {
                    printf("       ERROR!: Invalid Input\n");
                }
            }

            free(inputIndex);
        }
    }
    else if (strcmp(input, "back") == 0)
    {
        historyPop();
        event.id = MENU_EVENT_SET_CART_EDIT_INDEX;
        event.numberValue = -1;
    }
    else if (strcmp(input, "purchase") == 0)
    {
        historyPush(purchasePage);
    }

    free(input);

    return event;
}

MenuEvent showReceiptMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showReceipt(info.cash);
    printf("|                                                                                    |\n");
    printf("|   Grateful for your business!                                                      |\n");
    printf("|                                                                                    |\n");
    printf("|   <<  Restart  -   Back to main menu                                               |\n");
    printf("|    x  Exit                                                                         |\n");
    printf("|                                                                                    |\n");
    printf("+------------------------------------------------------------------------------------+\n");

    char *input = getInputString("Command");

    if (strcmp(input, "restart") == 0)
    {
        historyPopUntil("Main Menu");
        event.id = MENU_EVENT_RESET;
    }
    else if (strcmp(input, "exit") == 0)
    {
        event.id = MENU_EVENT_QUIT;
    }

    free(input);

    return event;
}

MenuEvent showPurchaseMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    if (cartSize <= 0)
    {
        historyPop();
        return event;
    }

    float change = showReceipt(info.cash);
    printf("|                                                                                    |\n");
    printf("|    >  Purchase                                                                     |\n");
    printf("|    <  Back                                                                         |\n");
    printf("|                                                                                    |\n");
    printf("+------------------------------------------------------------------------------------+\n");

    char *input = getInputString("Cash");

    if (strcmp(input, "purchase") == 0)
    {
        if (change >= 0)
        {
            historyPush(receiptPage);
        }
    }
    else if (strcmp(input, "back") == 0)
    {
        historyPop();
    }

    float cash = 0;
    if (sscanf(input, "%f", &cash) == 1)
    {
        if (cash < 0)
            cash = 0;
        if (cash > MAX_CASH)
            cash = MAX_CASH;

        event.id = MENU_EVENT_SET_CASH;
        event.floatValue = cash;
    }

    free(input);

    return event;
}

MenuEvent showNavigationMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Navigate To");
    printf("|                                                             |\n");
    printf("|    Added to cart, buy another?                              |\n");
    printf("|       << Restart   -   Select different product             |\n");
    if (strcmp(info.name, "Hot Coffee") == 0 || strcmp(info.name, "Iced Coffee") == 0)
    {
        printf("|       << Type      -   Select different coffee type         |\n");
    }
    printf("|       << Flavor    -   Select different flavor              |\n");
    printf("|       << Size      -   Select different size                |\n");
    printf("|                                                             |\n");
    cartChoiceDisplay();
    printf("|                                                             |\n");
    printf("|    >  Purchase    -   Purchase everything in the cart       |\n");
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    char *input = getInputString("Command");

    if (strcmp(input, "back") == 0)
    {
        historyPop();
    }
    else if (strcmp(input, "restart") == 0)
    {
        historyPopUntil("Main Menu");
        event.id = MENU_EVENT_RESET;
    }
    else if (strcmp(input, "type") == 0)
    {
        event.id = MENU_EVENT_SET_QUANTITY;
        event.numberValue = 1;

        if (strcmp(info.name, "Hot Coffee") == 0 || strcmp(info.name, "Iced Coffee") == 0)
        {
            historyPopUntil("Coffee Type");
        }
    }
    else if (strcmp(input, "flavor") == 0)
    {
        event.id = MENU_EVENT_SET_QUANTITY;
        event.numberValue = 1;

        char flavorPageName[30];
        strcpy(flavorPageName, info.name);
        strcat(flavorPageName, " Flavors");

        historyPopUntil(flavorPageName);
    }
    else if (strcmp(input, "size") == 0)
    {
        event.id = MENU_EVENT_SET_QUANTITY;
        event.numberValue = 1;

        char sizePageName[30];
        strcpy(sizePageName, info.name);
        strcat(sizePageName, " Sizes");

        historyPopUntil(sizePageName);
    }
    else if (strcmp(input, "cart") == 0)
    {
        historyPush(cartPage);
    }
    else if (strcmp(input, "purchase") == 0)
    {
        historyPush(purchasePage);
    }

    free(input);

    return event;
}

MenuEvent showSetQuantityMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    int quantity = 1;

    showMenuName("Set Quantity");
    printf("|                                                             |\n");
    showCurrentOrder(info, 1, 1);
    printf("|                                                             |\n");
    cartChoiceDisplay();
    printf("|                                                             |\n");
    printf("|    >  Buy   -   Buy the item                                |\n");
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    char *input = getInputString("Quantity (Max 99)");

    if (strcmp(input, "cart") == 0)
    {
        historyPush(cartPage);
    }
    else if (strcmp(input, "back") == 0)
    {
        historyPop();
    }
    else if (strcmp(input, "buy") == 0)
    {
        event.id = MENU_EVENT_ADD_TO_CART;
        historyPush(navigationPage);
    }

    if (sscanf(input, "%d", &quantity) == 1)
    {
        event.id = MENU_EVENT_SET_QUANTITY;
    }

    if (quantity >= MAX_QUANTITY)
        quantity = MAX_QUANTITY;
    else if (quantity <= 0)
        quantity = 1;

    event.numberValue = quantity;

    free(input);

    return event;
}

MenuEvent showSelectMilkTeaSizeMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Milk Tea Sizes");
    printf("|                                                             |\n");
    for (int i = 0; i < 2; i++)
    {
        printf("|      >  %-5s  -   %6.2f                                   |\n", milkTeaSizes[i], milkTeaPrices[i]);
    }
    printf("|                                                             |\n");
    cartChoiceDisplay();
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    showCurrentOrder(info, 0, 0);

    char *input = getInputString("Enter Size");

    if (strcmp(input, "cart") == 0)
    {
        historyPush(cartPage);
    }
    else if (strcmp(input, "back") == 0)
    {
        historyPop();
    }

    for (int i = 0; i < 2; i++)
    {
        char *size = strdup(milkTeaSizes[i]);
        char *sizeLower = strlwr(size);

        if (strcmp(input, sizeLower) == 0)
        {
            historyPush(setQuantityPage);
            event.id = MENU_EVENT_SET_SIZE;
            event.stringValue = strdup(milkTeaSizes[i]);
            free(size);
            break;
        }
    }

    free(input);

    return event;
}

MenuEvent showSelectIcedCoffeeSizeMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Iced Coffee Sizes");
    printf("|                                                             |\n");
    for (int i = 0; i < 3; i++)
    {
        printf("|      >  %-5s  -   %6.2f                                   |\n", icedCoffeeSizes[i], icedCoffeePrices[i]);
    }
    printf("|                                                             |\n");
    cartChoiceDisplay();
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");
    showCurrentOrder(info, 0, 0);

    char *input = getInputString("Enter Size");

    if (strcmp(input, "cart") == 0)
    {
        historyPush(cartPage);
    }
    else if (strcmp(input, "back") == 0)
    {
        historyPop();
    }

    for (int i = 0; i < 3; i++)
    {
        char *size = strdup(icedCoffeeSizes[i]);
        char *sizeLower = strlwr(size);

        if (strcmp(input, sizeLower) == 0)
        {
            historyPush(setQuantityPage);
            event.id = MENU_EVENT_SET_SIZE;
            event.stringValue = strdup(icedCoffeeSizes[i]);
            free(size);
            break;
        }
    }

    free(input);

    return event;
}

// just here so I can return the MENU_EVENT_SET_SIZE event
MenuEvent showSelectHotCoffeeSizeMenu(UserInfo info)
{

    historyPop(); // removes itself
    historyPush(setQuantityPage);

    MenuEvent event;
    event.id = MENU_EVENT_SET_SIZE;
    event.stringValue = "";
    return event;
}

MenuEvent showSelectMilkTeaFlavorMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuFlavorsItems("Milk Tea Flavors", milkTeaFlavors, 10);
    showCurrentOrder(info, 0, 0);

    char *input = getInputString("Enter Milk Tea Flavor");

    if (strcmp(input, "cart") == 0)
    {
        historyPush(cartPage);
    }
    else if (strcmp(input, "back") == 0)
    {
        event.id = MENU_EVENT_RESET;
        historyPop();
    }

    for (int i = 0; i < 10; i++)
    {

        char *flavor = strdup(milkTeaFlavors[i]);
        char *flavorLower = strlwr(flavor);

        if (strcmp(input, flavorLower) == 0)
        {
            historyPush(milkTeaSizesPage);
            event.id = MENU_EVENT_SET_FLAVOR;
            event.stringValue = strdup(milkTeaFlavors[i]);
            free(flavor);
            break;
        }
    }

    free(input);

    return event;
}

MenuEvent showSelectIcedCoffeeFlavorMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuFlavorsItems("Iced Coffee Flavors", icedCoffeeFlavors, 11);
    showCurrentOrder(info, 0, 0);

    char *input = getInputString("Enter Iced Coffee Flavor");

    if (strcmp(input, "cart") == 0)
    {
        historyPush(cartPage);
    }
    else if (strcmp(input, "back") == 0)
    {
        historyPop();
    }

    for (int i = 0; i < 11; i++)
    {

        char *flavor = strdup(icedCoffeeFlavors[i]);
        char *flavorLower = strlwr(flavor);

        if (strcmp(input, flavorLower) == 0)
        {
            historyPush(icedCoffeeSizesPage);
            event.id = MENU_EVENT_SET_FLAVOR;
            event.stringValue = strdup(icedCoffeeFlavors[i]);
            free(flavor);
            break;
        }
    }

    free(input);

    return event;
}

MenuEvent showSelectHotCoffeeFlavorMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuFlavorsItems("Hot Coffee Flavors", hotCoffeeFlavors, 8);
    showCurrentOrder(info, 0, 0);

    char *input = getInputString("Enter Hot Coffee Flavor");

    if (strcmp(input, "cart") == 0)
    {
        historyPush(cartPage);
    }
    else if (strcmp(input, "back") == 0)
    {
        historyPop();
    }

    for (int i = 0; i < 8; i++)
    {

        char *flavor = strdup(hotCoffeeFlavors[i]);
        char *flavorLower = strlwr(flavor);

        if (strcmp(input, flavorLower) == 0)
        {
            historyPush(hotCoffeeSizesPage);
            event.id = MENU_EVENT_SET_FLAVOR;
            event.stringValue = strdup(hotCoffeeFlavors[i]);
            free(flavor);
            break;
        }
    }

    free(input);

    return event;
}

MenuEvent showSelectCoffeeTypeMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Coffee Type");
    printf("|                                                             |\n");
    printf("|    What Coffee would you like to buy?                       |\n");
    printf("|      >  Hot                                                 |\n");
    printf("|      >  Iced                                                |\n");
    printf("|                                                             |\n");
    cartChoiceDisplay();
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");
    showCurrentOrder(info, 0, 0);

    char *input = getInputString("Coffee Type");

    if (strcmp(input, "hot") == 0)
    {
        historyPush(hotCoffeeFlavorsPage);
        event.id = MENU_EVENT_SET_PRODUCT_NAME;
        event.stringValue = "Hot Coffee";
    }
    else if (strcmp(input, "iced") == 0)
    {
        historyPush(icedCoffeeFlavorsPage);
        event.id = MENU_EVENT_SET_PRODUCT_NAME;
        event.stringValue = "Iced Coffee";
    }
    else if (strcmp(input, "cart") == 0)
    {
        historyPush(cartPage);
    }
    else if (strcmp(input, "back") == 0)
    {
        event.id = MENU_EVENT_RESET;
        historyPop();
    }

    free(input);

    return event;
}

MenuEvent showMainMenu(UserInfo info)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Yummy Tea Cafe");
    printf("|                                                             |\n");
    printf("|    What product would you like to buy?                      |\n");
    printf("|      >  Coffee                                              |\n");
    printf("|      >  Milk Tea                                            |\n");
    printf("|                                                             |\n");
    cartChoiceDisplay();
    printf("|    x  Exit                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    showCurrentOrder(info, 0, 0);

    char *input = getInputString("Product Name");

    if (strcmp(input, "coffee") == 0)
    {
        historyPush(coffeeTypePage);
    }
    else if (strcmp(input, "milk tea") == 0)
    {
        historyPush(milkTeaFlavorsPage);
        event.id = MENU_EVENT_SET_PRODUCT_NAME;
        event.stringValue = "Milk Tea";
    }
    else if (strcmp(input, "cart") == 0)
    {
        historyPush(cartPage);
    }
    else if (strcmp(input, "exit") == 0)
    {
        event.id = MENU_EVENT_QUIT;
    }

    free(input);

    return event;
}

/*==============================*
 *             MAIN             *
 *==============================*/

int main()
{

    int running = 1;

    historyPush(mainMenuPage);

    UserInfo currentItem = {1, -1, 0.0, "", "", ""};

    while (running != 0)
    {

        clearTerminal();

        MenuEvent event = history[historySize].action(currentItem);

        switch (event.id)
        {
        case MENU_EVENT_QUIT:
            running = 0;
            break;
        case MENU_EVENT_SET_PRODUCT_NAME:
            strcpy(currentItem.name, event.stringValue);
            break;
        case MENU_EVENT_SET_FLAVOR:
            strcpy(currentItem.flavor, event.stringValue);
            break;
        case MENU_EVENT_SET_SIZE:
            strcpy(currentItem.size, event.stringValue);
            // this strcpy sets the quantity to 0 for some reason
            // FIXED: the data type order matters in struct putting the int
            // after the char[] causes the problem.
            // NOTE TO SELF: put ints first in structs
            // currentItem.quantity = backupQuantity;
            break;
        case MENU_EVENT_SET_QUANTITY:
            currentItem.quantity = event.numberValue;
            break;
        case MENU_EVENT_ADD_TO_CART:
            CartItem cartItem;

            strcpy(cartItem.name, currentItem.name);
            strcpy(cartItem.flavor, currentItem.flavor);
            strcpy(cartItem.size, currentItem.size);
            cartItem.quantity = currentItem.quantity;
            addToCart(cartItem);
            break;
        case MENU_EVENT_SET_CASH:
            currentItem.cash = event.floatValue;
            break;
        case MENU_EVENT_SET_CART_EDIT_INDEX:
            currentItem.cartProductIndex = event.numberValue;
            break;
        case MENU_EVENT_RESET:
            strcpy(currentItem.name, "");
            strcpy(currentItem.flavor, "");
            strcpy(currentItem.size, "");
            currentItem.quantity = 1;
            currentItem.cartProductIndex = -1;
            currentItem.cash = 0.0;
            break;
        default:
            break;
        }
    };

    return 0;
}