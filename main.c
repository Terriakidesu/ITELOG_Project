#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY_STACK 50
#define MAX_CART_SIZE 500
#define MAX_QUANTITY 99

/*==============================*
 *          PRODUCTS            *
 *==============================*/

const int addonPrice = 10.0;

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

const char *coffeeAddons[] = {
    "Milk",
    "Cream",
    "Sweeteners",
    "Cinnamon",
    "Flavored Syrups",
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

const char *milkTeaAddons[] = {
    "Tapioca Pearls",
    "Egg Pudding",
    "Creamy Custard",
    "Popping Boba",
    "Oreo",
    "Chocolate Pudding",
};

const char *milkTeaSizes[] = {
    "18oz",
    "20oz",
};

const float milkTeaPrices[] = {
    100.0,
    150.0,
};

int getSizeIndex(const char *sizes[], const char *size, unsigned int sizeCount)
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

float getProductPrice(const char *name, const char *size)
{

    if (strcmp(name, "Hot Coffee") == 0)
    {
        return hotCoffeePrice;
    }
    else if (strcmp(name, "Iced Coffee") == 0)
    {
        int index = getSizeIndex(icedCoffeeSizes, size, 3);
        return icedCoffeePrices[index];
    }
    else if (strcmp(name, "Milk Tea") == 0)
    {
        int index = getSizeIndex(milkTeaSizes, size, 2);
        return milkTeaPrices[index];
    }

    return -1;
}

/*==============================*
 *             CART             *
 *==============================*/

typedef struct
{
    int quantity;

    char name[30];
    char addon[30];
    char size[10];
} CartItem;

int cartSize = 0;
CartItem cart[MAX_CART_SIZE];

char *getProductFullName(CartItem cartItem)
{
    char *fullName = malloc(strlen(cartItem.name) + strlen(cartItem.addon) + strlen(cartItem.size) + 1);

    strcpy(fullName, cartItem.name);
    strcat(fullName, cartItem.addon);
    strcat(fullName, cartItem.size);

    return fullName;
}

int findItemIndexByName(const char *name)
{
    int index = -1;
    for (int i = 0; i < cartSize; i++)
    {
        char *productName = getProductFullName(cart[i]);
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

void addToCart(CartItem cartItem)
{

    if (cartItem.quantity <= 0)
        return;

    if (cartItem.quantity > MAX_QUANTITY)
        cartItem.quantity = MAX_QUANTITY;

    int duplicate = findItemIndexByName(getProductFullName(cartItem));

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
    }
}

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

void removeFromCartByName(const char *name)
{
    int index = findItemIndexByName(name);

    if (index != -1)
    {
        removeFromCart(index);
    }
}

void setCartItemQuantityByIndex(int index, int quantity)
{

    if (index >= 0 && index < cartSize)
    {

        if (quantity <= 0)
        {
            removeFromCart(index);
            return;
        }

        if (quantity > MAX_QUANTITY)
            quantity = MAX_QUANTITY;

        cart[index].quantity = quantity;
    }
}

float getCartTotalPrice()
{
    float total;

    for (int i = 0; i < cartSize; i++)
    {
        CartItem item = cart[i];

        float price = getProductPrice(item.name, item.size);

        float subtotal = (float)item.quantity * price;
        float addonSubtotal = 0;

        if (strcmp(item.addon, "") != 0)
        {
            addonSubtotal = (float)addonPrice * item.quantity;
        }

        total += subtotal + addonSubtotal;
    }

    return total;
}

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

        if (strcmp(item.addon, "") == 0)
            continue;

        float addonSubtotal = (float)addonPrice * item.quantity;
        printf("|     | └ %-32s%-8s %6.2f   x %5d     |%10.2f  |\n", item.addon, "", (float)addonPrice, item.quantity, addonSubtotal);
    }

    float total = getCartTotalPrice();
    printf("+-----+-----------------------------------------------------------------+------------+\n");
    printf("|                                                            Total: %15.2f  |\n", total);
}

void showReceipt(float cashAmount)
{

    float total = getCartTotalPrice();

    float change = cashAmount - total;

    if (change < 0)
        return;

    printf("+------------------------------------------------------------------------------------+\n");
    printf("|                                      Receipt                                       |\n");
    listCartItems();
    printf("|                                                            Cash:  %15.2f  |\n", cashAmount);
    printf("|                                                            Change: %14.2f  |\n", change);
    printf("+------------------------------------------------------------------------------------+\n");
}

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
    MENU_EVENT_ADD_TO_CART,
    MENU_EVENT_RESET_PRODUCT,
} MENU_EVENTS;

typedef struct
{
    unsigned int id;
    int numberValue;
    char *stringValue;
} MenuEvent;

typedef struct
{
    char *name;
    MenuEvent (*action)();
} MenuPage;

MenuPage history[MAX_HISTORY_STACK];
int historySize = 0;

int historyPop()
{

    if (historySize > 0)
    {
        history[historySize--];
        return historySize;
    }

    return -1;
}

int historyPopUntil(char *name)
{
    MenuPage menuPage;

    while (strcmp(menuPage.name, name) != 0)
    {
        menuPage = history[historySize - 1];

        historyPop();
    }
}

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

MenuEvent showMainMenu(CartItem item);
MenuEvent showSelectCoffeeTypeMenu(CartItem item);
MenuEvent showSelectHotCoffeeFlavorMenu(CartItem item);
MenuEvent showSelectHotCoffeeSizeMenu(CartItem item);
MenuEvent showSelectIcedCoffeeFlavorMenu(CartItem item);
MenuEvent showSelectIcedCoffeeSizeMenu(CartItem item);
MenuEvent showSelectMilkTeaFlavorMenu(CartItem item);
MenuEvent showSelectMilkTeaSizeMenu(CartItem item);
MenuEvent showSetQuantityMenu(CartItem item);
MenuEvent showNavigationMenu(CartItem item);
MenuEvent showCartMenu(CartItem item);

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
MenuPage cartPage = {"Cart", showCartMenu};

void showMenuName(const char *menuName)
{
    printf("+-------------------------------------------------------------+\n");
    printf("| > %-57s |\n", menuName, "");
    printf("+-------------------------------------------------------------+\n");
}

void showMenuFlavorsItems(const char *menuName, const char *menuItems[], unsigned int menuItemCount)
{

    showMenuName(menuName);
    printf("|                                                             |\n");
    printf("|      >  None                                                |\n");
    for (int i = 0; i < menuItemCount; i++)
    {
        printf("|      >  %-25s %-25s |\n", menuItems[i], "");
    }
    printf("|                                                             |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");
}

void showCurrentOrder(CartItem item, int removeTop, int removeBottom)
{

    float price = getProductPrice(item.name, item.size);
    float subtotal = 0;
    float addonSubtotal = 0;

    if (price < 0)
        price = 0;

    if (strcmp(item.addon, "") != 0)
    {
        addonSubtotal = addonPrice * item.quantity;
    }

    if (price > 0)
    {
        subtotal = price * item.quantity;
    }

    float total = subtotal + addonSubtotal;

    if (removeTop == 0)
        printf("+-------------------------------------------------------------+\n");

    printf("|   Current Order:      %36.2f  |\n", total);
    printf("|   %-14s         %4s %7.2f x %2d      %10.2f  |\n", item.name, item.size, price, item.quantity, subtotal);
    if (strcmp(item.addon, "") != 0)
    {

        printf("|   └ %-22s    %7.2f x %2d      %10.2f  |\n", item.addon, (float)addonPrice, item.quantity, addonSubtotal);
    }
    if (removeBottom == 0)
        printf("+-------------------------------------------------------------+\n");
}

int getIntegerInput(const char *label)
{
    int inp = 0;
    printf("  Enter %s > ", label);
    scanf("%d", &inp);
    return inp;
}

char *getInputString(const char *label)
{
    char buffer[30];
    printf("  Enter %s > ", label);
    fgets(buffer, 30, stdin);

    buffer[strcspn(buffer, "\n")] = 0;

    // return strlwr(buffer);
    return strdup(strlwr(buffer));
}

MenuEvent showCartMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    return event;
}

MenuEvent showNavigationMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Navigate To");
    printf("|                                                             |\n");
    printf("|    Added to cart, buy another?                              |\n");
    printf("|       << Start     -   Select different product             |\n");
    if (strcmp(item.name, "Hot Coffee") == 0 || strcmp(item.name, "Iced Coffee") == 0)
    {
        printf("|       << Type      -   Select different coffee type         |\n");
    }
    printf("|       << Flavor    -   Select different flavor              |\n");
    printf("|       << Size      -   Select different size                |\n");
    printf("|                                                             |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    char *input = getInputString("Command");

    if (strcmp(input, "back") == 0)
    {
        historyPop();
    }
    else if (strcmp(input, "start") == 0)
    {
        historyPopUntil("Main Menu");
        event.id = MENU_EVENT_RESET_PRODUCT;
    }
    else if (strcmp(input, "type") == 0)
    {
        if (strcmp(item.name, "Hot Coffee") == 0 || strcmp(item.name, "Iced Coffee") == 0)
        {
            historyPopUntil("Coffee Type");
        }
    }
    else if (strcmp(input, "flavor") == 0)
    {
        char flavorPageName[30];
        strcpy(flavorPageName, item.name);
        strcat(flavorPageName, " Flavors");

        historyPopUntil(flavorPageName);
    }
    else if (strcmp(input, "size") == 0)
    {
        char sizePageName[30];
        strcpy(sizePageName, item.name);
        strcat(sizePageName, " Sizes");

        historyPopUntil(sizePageName);
    }
    else if (strcmp(input, "cart") == 0)
    {
    }

    return event;
}

MenuEvent showSetQuantityMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    int quantity = 1;

    showMenuName("Set Quantity");
    printf("|                                                             |\n");
    showCurrentOrder(item, 1, 1);
    printf("|                                                             |\n");
    printf("|    >  Buy                                                   |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    char *input = getInputString("Quantity (Max 99)");

    if (strcmp(input, "cart") == 0)
    {
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

    free(input);

    if (quantity >= MAX_QUANTITY)
        quantity = MAX_QUANTITY;
    else if (quantity <= 0)
        quantity = 1;

    event.numberValue = quantity;

    // free(input);

    return event;
}

MenuEvent showSelectMilkTeaSizeMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Milk Tea Sizes");
    printf("|                                                             |\n");
    for (int i = 0; i < 2; i++)
    {
        printf("|      >  %-25s %-25s |\n", milkTeaSizes[i], "");
    }
    printf("|                                                             |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    showCurrentOrder(item, 0, 0);

    char *input = getInputString("Enter Size");

    if (strcmp(input, "cart") == 0)
    {
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

MenuEvent showSelectIcedCoffeeSizeMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Iced Coffee Sizes");
    printf("|                                                             |\n");
    for (int i = 0; i < 3; i++)
    {
        printf("|      >  %-25s %-25s |\n", icedCoffeeSizes[i], "");
    }
    printf("|                                                             |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");
    showCurrentOrder(item, 0, 0);

    char *input = getInputString("Enter Size");

    if (strcmp(input, "cart") == 0)
    {
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
MenuEvent showSelectHotCoffeeSizeMenu(CartItem item)
{

    historyPop(); // removes itself
    historyPush(setQuantityPage);

    MenuEvent event;
    event.id = MENU_EVENT_SET_SIZE;
    event.stringValue = "";
    return event;
}

MenuEvent showSelectMilkTeaFlavorMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuFlavorsItems("Milk Tea Flavors", milkTeaFlavors, 10);
    showCurrentOrder(item, 0, 0);

    char *input = getInputString("Enter Milk Tea Flavor");

    if (strcmp(input, "cart") == 0)
    {
    }
    else if (strcmp(input, "back") == 0)
    {
        event.id = MENU_EVENT_RESET_PRODUCT;
        historyPop();
    }
    else if (strcmp(input, "none") == 0)
    {
        historyPush(milkTeaSizesPage);
        event.id = MENU_EVENT_SET_FLAVOR;
        event.stringValue = "";
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

MenuEvent showSelectIcedCoffeeFlavorMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuFlavorsItems("Iced Coffee Flavors", icedCoffeeFlavors, 11);
    showCurrentOrder(item, 0, 0);

    char *input = getInputString("Enter Iced Coffee Flavor");

    if (strcmp(input, "cart") == 0)
    {
    }
    else if (strcmp(input, "back") == 0)
    {
        historyPop();
    }
    else if (strcmp(input, "none") == 0)
    {
        historyPush(icedCoffeeSizesPage);
        event.id = MENU_EVENT_SET_FLAVOR;
        event.stringValue = "";
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

MenuEvent showSelectHotCoffeeFlavorMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuFlavorsItems("Hot Coffee Flavors", hotCoffeeFlavors, 8);
    showCurrentOrder(item, 0, 0);

    char *input = getInputString("Enter Hot Coffee Flavor");

    if (strcmp(input, "cart") == 0)
    {
    }
    else if (strcmp(input, "back") == 0)
    {
        historyPop();
    }
    else if (strcmp(input, "none") == 0)
    {
        historyPush(hotCoffeeSizesPage);
        event.id = MENU_EVENT_SET_FLAVOR;
        event.stringValue = "";
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

MenuEvent showSelectCoffeeTypeMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Coffee Type");
    printf("|                                                             |\n");
    printf("|    What Coffee would you like to buy?                       |\n");
    printf("|      >  Hot                                                 |\n");
    printf("|      >  Iced                                                |\n");
    printf("|                                                             |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");
    showCurrentOrder(item, 0, 0);

    char *input = getInputString("Product Name");

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
    }
    else if (strcmp(input, "back") == 0)
    {
        event.id = MENU_EVENT_RESET_PRODUCT;
        historyPop();
    }

    free(input);

    return event;
}

MenuEvent showMainMenu(CartItem item)
{
    MenuEvent event;
    event.id = MENU_EVENT_NO_EVENT;

    showMenuName("Yummy Tea Cafe");
    printf("|                                                             |\n");
    printf("|    What product would you like to buy?                      |\n");
    printf("|      >  Coffee                                              |\n");
    printf("|      >  Milk Tea                                            |\n");
    printf("|                                                             |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    x  Exit                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    showCurrentOrder(item, 0, 0);

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

    CartItem currentItem = {1, "", "", ""};

    while (running != 0)
    {

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
            strcpy(currentItem.addon, event.stringValue);
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
            addToCart(currentItem);
            break;
        case MENU_EVENT_RESET_PRODUCT:
            strcpy(currentItem.name, "");
            strcpy(currentItem.addon, "");
            strcpy(currentItem.size, "");
            currentItem.quantity = 1;
            break;
        default:
            break;
        }
    }

    return 0;
}