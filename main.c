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
    char *name;
    char *addon;
    char *size;

    int quantity;
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

        if (strcmp(item.addon, "None") == 1)
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

        if (strcmp(item.addon, "None") == 0)
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
    MENU_EVENT_QUIT = -1,
    MENU_EVENT_SET_PRODUCT_NAME,
    MENU_EVENT_SET_FLAVOR,
    MENU_EVENT_SET_ADDON,
    MENU_EVENT_SET_SIZE,
    MENU_EVENT_SET_QUANTITY,
    MENU_EVENT_ADD_TO_CART,
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
    for (int i = historySize - 1; i > 0; i--)
    {
        MenuPage menuPage = history[i];

        if (!strcmp(menuPage.name, name))
        {
            break;
        }

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

MenuEvent showMainMenu();
MenuEvent showSelectCoffeeTypeMenu();
MenuEvent showSelectHotCoffeeFlavorMenu();
MenuEvent showSelectIcedCoffeeFlavorMenu();
MenuEvent showSelectMilkTeaFlavorMenu();

MenuPage mainMenuPage = {"Main Menu", showMainMenu};
MenuPage coffeeTypePage = {"Coffee Type", showSelectCoffeeTypeMenu};
MenuPage hotCoffeeFlavorsPage = {"Hot Coffee Flavors", showSelectHotCoffeeFlavorMenu};
MenuPage icedCoffeeFlavorsPage = {"Iced Coffee Flavors", showSelectIcedCoffeeFlavorMenu};
MenuPage milkTeaFlavorsPage = {"Milk Tea Flavors", showSelectMilkTeaFlavorMenu};

void showMenuName(const char *menuName)
{
    printf("+-------------------------------------------------------------+\n");
    printf("| > %-57s |\n", menuName, "");
    printf("+-------------------------------------------------------------+\n");
}

void showMenuItems(const char *menuName, const char *menuItems[], unsigned int menuItemCount)
{

    showMenuName(menuName);
    printf("|                                                             |\n");
    for (int i = 0; i < menuItemCount; i++)
    {
        printf("|      >  %-25s %-25s |\n", menuItems[i], "");
    }
    printf("|                                                             |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    <  Back                                                  |\n");
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

    return strlwr(buffer);
}

MenuEvent showSelectMilkTeaFlavorMenu()
{
    MenuEvent event;

    showMenuItems("Milk Flavors", milkTeaFlavors, 10);

    const char *input = getInputString("Enter Milk Tea Flavor");

    if (!strcmp(input, "cart"))
    {
    }
    else if (!strcmp(input, "back"))
    {
        historyPop();
    }

    for (int i = 0; i < 10; i++)
    {

        char *flavor = strdup(milkTeaFlavors[i]);
        char *flavorLower = strlwr(flavor);

        if (!strcmp(input, flavorLower))
        {
            event.id = MENU_EVENT_SET_FLAVOR;
            event.stringValue = flavor;
            free(flavor);
            break;
        }
    }

    return event;
}

MenuEvent showSelectIcedCoffeeFlavorMenu()
{
    MenuEvent event;

    showMenuItems("Iced Coffee Flavors", icedCoffeeFlavors, 11);

    const char *input = getInputString("Enter Iced Coffee Flavor");

    if (!strcmp(input, "cart"))
    {
    }
    else if (!strcmp(input, "back"))
    {
        historyPop();
    }

    for (int i = 0; i < 11; i++)
    {

        char *flavor = strdup(icedCoffeeFlavors[i]);
        char *flavorLower = strlwr(flavor);

        if (!strcmp(input, flavorLower))
        {
            event.id = MENU_EVENT_SET_FLAVOR;
            event.stringValue = flavor;
            free(flavor);
            break;
        }
    }

    return event;
}

MenuEvent showSelectHotCoffeeFlavorMenu()
{
    MenuEvent event;

    showMenuItems("Hot Coffee Flavors", hotCoffeeFlavors, 8);

    const char *input = getInputString("Enter Hot Coffee Flavor");

    if (!strcmp(input, "cart"))
    {
    }
    else if (!strcmp(input, "back"))
    {
        historyPop();
    }

    for (int i = 0; i < 8; i++)
    {

        char *flavor = strdup(hotCoffeeFlavors[i]);
        char *flavorLower = strlwr(flavor);

        if (!strcmp(input, flavorLower))
        {
            event.id = MENU_EVENT_SET_FLAVOR;
            event.stringValue = flavor;
            free(flavor);
            break;
        }
    }

    return event;
}

MenuEvent showSelectCoffeeTypeMenu()
{
    MenuEvent event;

    showMenuName("Coffee Type");
    printf("|                                                             |\n");
    printf("|    What Coffee would you like to buy?                       |\n");
    printf("|      >  Hot                                                 |\n");
    printf("|      >  Iced                                                |\n");
    printf("|                                                             |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    <  Back                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    const char *input = getInputString("Product Name");

    if (!strcmp(input, "hot"))
    {
        historyPush(hotCoffeeFlavorsPage);
        event.id = MENU_EVENT_SET_PRODUCT_NAME;
        event.stringValue = "Hot Coffee";
    }
    else if (!strcmp(input, "iced"))
    {
        historyPush(icedCoffeeFlavorsPage);
        event.id = MENU_EVENT_SET_PRODUCT_NAME;
        event.stringValue = "Iced Coffee";
    }
    else if (!strcmp(input, "cart"))
    {
    }
    else if (!strcmp(input, "back"))
    {
        historyPop();
    }
    return event;
}

MenuEvent showMainMenu()
{
    MenuEvent event;

    showMenuName("Yummy Tea Cafe");
    printf("|                                                             |\n");
    printf("|    What product would you like to buy?                      |\n");
    printf("|      >  Coffee                                              |\n");
    printf("|      >  Milk Tea                                            |\n");
    printf("|                                                             |\n");
    printf("|    >  Cart                                                  |\n");
    printf("|    x  Exit                                                  |\n");
    printf("+-------------------------------------------------------------+\n");

    const char *input = getInputString("Product Name");

    if (!strcmp(input, "coffee"))
    {
        historyPush(coffeeTypePage);
    }
    else if (!strcmp(input, "milk tea"))
    {
        historyPush(milkTeaFlavorsPage);
    }
    else if (!strcmp(input, "cart"))
    {
    }
    else if (!strcmp(input, "exit"))
    {
        event.id = MENU_EVENT_QUIT;
    }

    return event;
}

/*==============================*
 *             MAIN             *
 *==============================*/

int main()
{

    int running = 1;

    MenuPage mainMenu = {
        "Main Menu",
        showMainMenu,
    };
    historyPush(mainMenu);

    CartItem currentItem;

    while (running != 0)
    {

        MenuEvent event = history[historySize].action();

        switch (event.id)
        {
        case MENU_EVENT_QUIT:
            running = 0;
            break;
        case MENU_EVENT_SET_PRODUCT_NAME:
            currentItem.name = event.stringValue;
            break;
        case MENU_EVENT_SET_FLAVOR:
            currentItem.name = event.stringValue;
            break;
        case MENU_EVENT_SET_ADDON:
            currentItem.addon = event.stringValue;
            break;
        case MENU_EVENT_SET_SIZE:
            currentItem.size = event.stringValue;
            break;
        case MENU_EVENT_SET_QUANTITY:
            currentItem.quantity = event.numberValue;
            break;
        case MENU_EVENT_ADD_TO_CART:
            addToCart(currentItem);
            break;

        default:
            break;
        }
    }

    return 0;
}