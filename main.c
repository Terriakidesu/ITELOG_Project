#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 10
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

struct CartItem
{
    char *name;
    char *addon;
    char *size;

    int quantity;
};

int cartSize = 0;
struct CartItem cart[MAX_CART_SIZE];

char *getProductFullName(struct CartItem cartItem)
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

void addToCart(struct CartItem cartItem)
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
        struct CartItem item = cart[i];

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
        struct CartItem item = cart[i];

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
 *             MENU             *
 *==============================*/

void showMenuName(const char *menuName)
{
    printf("+-------------------------------------------------------------+\n");
    printf("| %-11s                                         |\n", menuName);
    printf("+-------------------------------------------------------------+\n");
}

void showMenuItems(const char *menuName, const char *menuItems[], unsigned int menuItemCount)
{

    showMenuName(menuName);

    for (int i = 0; i < menuItemCount; i++)
    {
        printf("| %2d. %-25s %-29s |\n", i + 1, menuItems[i], "");
    }

    printf("+-------------------------------------------------------------+\n");
}

/*==============================*
 *          NAVIGATION          *
 *==============================*/

// TODO : Naigation System

/*==============================*
 *        CHOICE HANDLER        *
 *==============================*/

int handleChoice(const char *choice)
{

    char buffer[25];
    strcpy(buffer, choice);
    buffer[strcspn(choice, "\n")] = '\0';

    if (strcmp(buffer, "quit") == 0)
    {
        return 0;
    }

    return 1;
}

/*==============================*
 *             MAIN             *
 *==============================*/

int main()
{

    int running = 1;

    while (running == 1)
    {
        char inp[25];

        printf("Enter Input: ");
        fgets(inp, 25, stdin);

        running = handleChoice(inp);
    }

    return 0;
}