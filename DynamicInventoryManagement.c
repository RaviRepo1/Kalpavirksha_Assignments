#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PRODUCTS 100
#define MAX_NAME_LENGTH 50
#define MAX_ID 10000
#define MAX_PRICE 100000
#define MAX_QUANTITY 1000000
#define MIN_VALUE 0

typedef struct
{
    int productId;
    char productName[MAX_NAME_LENGTH];
    float productPrice;
    int productQuantity;
} ProductInformation;

/* ---------- Reusable Validation Functions ---------- */

bool isIdUnique(ProductInformation *products, int currentIndexValue, int IdToCheck)
{
    for (int index = 0; index < currentIndexValue; index++)
    {
        if (products[index].productId == IdToCheck)
            return true;
    }
    return false;
}

bool isIdValid(int productId)
{
    return (productId <= MIN_VALUE || productId > MAX_ID);
}

bool isNameValid(char productName[])
{
    for (int i = 0; productName[i] != '\0'; i++)
    {
        if (!isalpha(productName[i]) && productName[i] != '_' && productName[i] != '-')
            return true;
    }
    return false;
}
bool isPriceValid(float productPrice)
{
    return (productPrice < MIN_VALUE || productPrice > MAX_PRICE);
}

bool isQuantityValid(int productQuantity)
{
    return (productQuantity < MIN_VALUE || productQuantity > MAX_QUANTITY);
}

bool matchPartialName(const char *nameToSearch, const char *productName)
{
    int i = 0, j = 0;

    while (productName[i] != '\0')
    {
        if (tolower(productName[i]) == tolower(nameToSearch[0]))
        {
            j = 0;
            while (nameToSearch[j] != '\0' &&
                   tolower(productName[i + j]) == tolower(nameToSearch[j]))
            {
                j++;
            }
            if (nameToSearch[j] == '\0')
                return true;
        }
        i++;
    }
    return false;
}

void printProductDetails(const ProductInformation *product)
{
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           product->productId, product->productName,
           product->productPrice, product->productQuantity);
}

int inputSingleProduct(ProductInformation *product, int currentCount, int index)
{
    do
    {
        printf("\nEnter details for product %d", index + 1);

        printf("\nProduct ID: ");
        if (scanf("%d", &product[index].productId) != 1 || isIdValid(product[index].productId))
        {
            printf("Error: Invalid Product ID\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (isIdUnique(product, currentCount, product[index].productId))
        {
            printf("Error: ID already exists! Enter Unique ID\n");
            continue;
        }

        printf("Product Name: ");
        if (scanf("%s", product[index].productName) != 1 || isNameValid(product[index].productName))
        {
            printf("Error: Invalid Product Name\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        printf("Product Price: ");
        if (scanf("%f", &product[index].productPrice) != 1 || isPriceValid(product[index].productPrice))
        {
            printf("Error: Invalid Product Price\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        printf("Product Quantity: ");
        if (scanf("%d", &product[index].productQuantity) != 1 || isQuantityValid(product[index].productQuantity))
        {
            printf("Error: Invalid Product Quantity\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        break;
    } while (1);

    return 0;
}

int getInputForProductDetails(ProductInformation *product, int numberOfProducts)
{
    for (int index = 0; index < numberOfProducts; index++)
    {
        inputSingleProduct(product, index, index);
    }
    return 0;
}

bool addNewProductToInventory(ProductInformation **product, int *numberOfProducts)
{
    int newIndex = *numberOfProducts;
    ProductInformation *newProduct = realloc(*product, (newIndex + 1) * sizeof(ProductInformation));

    if (newProduct == NULL)
    {
        printf("Memory allocation failed!\n");
        return false;
    }

    *product = newProduct;
    inputSingleProduct(*product, *numberOfProducts, newIndex);
    (*numberOfProducts)++;

    printf("Product added successfully!\n");
    return true;
}

void viewAllProducts(ProductInformation *product, int numberOfProducts)
{
    if (numberOfProducts == 0)
    {
        printf("\nNo products in inventory.\n");
        return;
    }

    printf("\n========= PRODUCT LIST =========\n");
    for (int index = 0; index < numberOfProducts; index++)
    {
        printProductDetails(&product[index]);
    }
}

int updateQuantity(ProductInformation *product, int numberOfProducts)
{
    printf("\nEnter Product ID to update quantity: ");
    int searchId, newQuantity;
    bool found = false;

    if (scanf("%d", &searchId) != 1 || isIdValid(searchId))
    {
        printf("Error: Invalid Product ID\n");
        return 1;
    }

    printf("Enter new Quantity: ");
    if (scanf("%d", &newQuantity) != 1 || isQuantityValid(newQuantity))
    {
        printf("Error: Invalid Product Quantity\n");
        return 1;
    }

    for (int i = 0; i < numberOfProducts; i++)
    {
        if (product[i].productId == searchId)
        {
            product[i].productQuantity = newQuantity;
            found = true;
            break;
        }
    }

    if (!found)
        printf("No Product Found With ID: %d!\n", searchId);
    else
        printf("Quantity updated successfully!\n");
    return 0;
}

ProductInformation *searchProductById(ProductInformation *product, int numberOfProducts)
{
    printf("\nEnter Product ID to search: ");
    int searchId;
    if (scanf("%d", &searchId) != 1 || isIdValid(searchId))
    {
        printf("Error: Invalid Product ID\n");
        return NULL;
    }

    for (int i = 0; i < numberOfProducts; i++)
    {
        if (product[i].productId == searchId)
        {
            return &product[i];
        }
    }

    printf("No Product Found With ID: %d!\n", searchId);
    return NULL; 
}

int searchProductByName(ProductInformation *product, int numberOfProducts)
{
    printf("\nEnter Product Name to search: ");
    char nameToSearch[MAX_NAME_LENGTH];
    bool found = false;

    if (scanf("%s", nameToSearch) != 1 || isNameValid(nameToSearch))
    {
        printf("Error: Invalid Product Name\n");
        return 1;
    }

    for (int i = 0; i < numberOfProducts; i++)
    {
        if (matchPartialName(nameToSearch, product[i].productName))
        {
            found = true;
            printProductDetails(&product[i]);
        }
    }
    if (!found)
        printf("No Product Found With Name: %s!\n", nameToSearch);
    return 0;
}

int searchProductByPriceRange(ProductInformation *product, int numberOfProducts)
{
    printf("\nEnter Product Price Range (min max): ");
    float minPrice, maxPrice;
    bool found = false;

    if (scanf("%f %f", &minPrice, &maxPrice) != 2 ||
        isPriceValid(minPrice) || isPriceValid(maxPrice) ||
        maxPrice < minPrice)
    {
        printf("Error: Invalid Price Range\n");
        return 1;
    }

    for (int i = 0; i < numberOfProducts; i++)
    {
        if (product[i].productPrice >= minPrice && product[i].productPrice <= maxPrice)
        {
            found = true;
            printProductDetails(&product[i]);
        }
    }

    if (!found)
        printf("No Product Found Within This Price Range!\n");

    return 0;
}

int deleteProductFromInventory(ProductInformation **product, int *numberOfProducts)
{
    if (*product == NULL || *numberOfProducts <= 0)
    {
        printf("No products to delete.\n");
        return 1;
    }

    printf("\nEnter Product ID to delete: ");
    int idToDelete, deleteIndex = -1;
    bool found = false;

    if (scanf("%d", &idToDelete) != 1 || isIdValid(idToDelete))
    {
        printf("Error: Invalid Product ID\n");
        while (getchar() != '\n')
            ;
        return 1;
    }

    for (int i = 0; i < *numberOfProducts; i++)
    {
        if ((*product)[i].productId == idToDelete)
        {
            found = true;
            deleteIndex = i;
            break;
        }
    }
    if (!found)
    {
        printf("Product Not Found!\n");
        return 0;
    }

    for (int i = deleteIndex; i < *numberOfProducts - 1; i++)
    {
        (*product)[i] = (*product)[i + 1];
    }

    (*numberOfProducts)--;

    if (*numberOfProducts > 0)
    {
        ProductInformation *temp = realloc(*product, (*numberOfProducts) * sizeof(ProductInformation));
        if (temp != NULL)
            *product = temp;
        else
            printf("Unable to shrink memory, but product deleted.\n");
    }
    else
    {
        free(*product);
        *product = NULL;
    }

    printf("Product deleted successfully!\n");
    return 0;
}

void inventoryMenu(ProductInformation **product, int *numberOfProducts)
{
    int choice = 0;

    while (choice != 8)
    {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Enter Valid Choice!\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            addNewProductToInventory(product, numberOfProducts);
            break;
        case 2:
            viewAllProducts(*product, *numberOfProducts);
            break;
        case 3:
            updateQuantity(*product, *numberOfProducts);
            break;
        case 4:
        {
            ProductInformation *foundProduct = searchProductById(*product, *numberOfProducts);
            if (foundProduct != NULL)
            {
                printProductDetails(foundProduct);
            }
            break;
        }
        case 5:
            searchProductByName(*product, *numberOfProducts);
            break;
        case 6:
            searchProductByPriceRange(*product, *numberOfProducts);
            break;
        case 7:
            deleteProductFromInventory(product, numberOfProducts);
            break;
        case 8:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid Choice!\n");
        }
    }

    if (*product != NULL)
    {
        free(*product);
        *product = NULL;
    }

    printf("Memory released successfully.\n");
}

int main()
{
    int numberOfProducts;
    printf("Enter initial number of products: ");
    if (scanf("%d", &numberOfProducts) != 1 ||
        numberOfProducts <= MIN_VALUE || numberOfProducts > MAX_PRODUCTS)
    {
        printf("Error: Invalid Input for Number of Products\n");
        return 1;
    }

    ProductInformation *products = calloc(numberOfProducts, sizeof(ProductInformation));
    if (products == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    getInputForProductDetails(products, numberOfProducts);
    inventoryMenu(&products, &numberOfProducts);
    return 0;
}
