#include <stdio.h>
#include <stdlib.h>

typedef struct sale {
    int year;
    int month;
    int day;
    int product;
    float quantity;
    float price;
} Sale;

typedef struct product {
    int productID;
    char type;
    float quantity;
    float cost;
    float profit;
} Product;

FILE* openFile(char fileName[]);
void populateProducts(FILE *file,int numberLines, Product *products);
void populateSales(FILE *file,int numberLines, Sale *sales);
void printStructsProduct(Product* products, int numberOfProducts);
void printStructsSale(Sale* sales, int numberOfSale);
int countLines(FILE *fp);
void outputReport(Sale *sales, Product *products, int numberOfProducts, int numberOfSales);
void writeTotalSaleDay(FILE *reportFile, Sale *sales, int numberOfSales);
void writeTotalSaleProduct(FILE *reportFile, Sale *sales, Product *products, int numberOfSales, int numberOfProducts);

int main() {
    FILE *productFile = openFile("produtos.txt");
    FILE *saleFile = openFile("vendas.txt");

    int numberOfProducts = countLines(productFile);
    int numberOfSales = countLines(saleFile);
    Product *products = malloc(sizeof(Product) * numberOfProducts);
    Sale *sales = malloc(sizeof(Sale) * numberOfSales);
    populateProducts(productFile, numberOfProducts, products);
    populateSales(saleFile, numberOfSales, sales);

    printf("%d products detected\n", numberOfProducts);
    printf("%d sales detected\n", numberOfSales);

    /*printf("\n\nProducts Detected: \n\n");*/
    /*printStructsProduct(products, numberOfProducts);*/

    /*printf("\n\nSales Detected: \n\n");*/
    /*printStructsSale(sales, numberOfSales);*/

    outputReport(sales, products, numberOfProducts, numberOfSales);

    free(sales);
    free(products);
}

void writeTotalSaleProduct(FILE *reportFile, Sale *sales, Product *products, int numberOfSales, int numberOfProducts) {
    int productSales = 0;
    float totalSales = 0.0;
    float totalQuantity = 0.0;

    fprintf(reportFile, "\n\nTotal Sales by Product\n\n");
    fprintf(reportFile, "Product     Total Sold      Total Quantity   Average/Price      Profitability\n\n");

    for(int i = 0; i < numberOfProducts; i++) {
        for(int j = 0; j < numberOfSales; j++) {
            if(products[i].productID == sales[j].product) {
                totalSales += sales[j].price;
                totalQuantity += sales[j].quantity;
                productSales++;
            }
        }

        float averagePrice = totalSales / productSales;
        float profitability = (averagePrice / products[i].cost - 1) * 100;

        fprintf(reportFile, "%d        %.2f          %d          %.2f             %.1f\n", 
                products[i].productID, totalSales, totalQuantity, averagePrice, profitability);

        productSales = 0;
        totalSales = 0.0;
        totalQuantity = 0.0;
    }
}

void writeTotalSaleDay(FILE *reportFile, Sale *sales, int numberOfSales) {
    float total = 0;
    float dayTotal = 0.0;
    int daySales = 0;

     
    fprintf(reportFile, "Total Day Sales\n\n");
    fprintf(reportFile, "Day            Total            Average\n\n");

    for(int i = 0; i < numberOfSales; i++) {
        dayTotal += sales[i].price;
        daySales++;
        
        if (sales[i+1].day != sales[i].day) {
            total += dayTotal;
            fprintf(reportFile, "%d/%d/%d      %.2f     %.2f\n", sales[i].day, sales[i].month,
                sales[i].year, dayTotal, dayTotal/daySales);

            daySales = 0;
            dayTotal = 0.0;
        }
    }
}

void outputReport(Sale *sales, Product *products, int numberOfProducts, int numberOfSales) {
    FILE *reportFile = fopen("final_report.txt", "w+");

    writeTotalSaleDay(reportFile, sales, numberOfSales);
    writeTotalSaleProduct(reportFile, sales, products, numberOfSales, numberOfProducts);
}

FILE* openFile(char fileName[]) {
    FILE *file;

    if ((file = fopen(fileName,"r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }

    return file;
}

void populateSales(FILE *file, int numberOfSales, Sale *sales) {
    char str1[50];
    char str2[50];
    char str3[50];
    char str4[50];
    char str5[50];
    char str6[50];

    for(int i = 0; i < numberOfSales; i++) {
        fscanf(file, " %[^;];%[^;];%[^;];%[^;];%[^;];%[^;];", str1, str2, str3, str4, str5, str6);
        sales[i].year = atoi(str1);
        sales[i].month = atoi(str2);
        sales[i].day = atoi(str3);
        sales[i].product = atoi(str4);
        sales[i].quantity = atof(str5);
        sales[i].price = atof(str6);
    }
}

void populateProducts(FILE *file, int numberOfProducts, Product *products) {
    char str1[50];
    char str2[50];
    char str3[50];
    char str4[50];
    char str5[50];

    for(int i = 0; i < numberOfProducts; i++) {
        fscanf(file, " %[^;];%[^;];%[^;];%[^;];%[^;];", str1, str2, str3, str4, str5);
        products[i].productID = atoi(str1);
        products[i].type = str2[0];
        products[i].quantity = atof(str3);
        products[i].cost = atof(str4);
        products[i].profit = atof(str5);
    }
}

void printStructsProduct(Product *products, int numberOfProducts) {
    for(int i=0; i<numberOfProducts; i++) {
        printf("%d | ", products[i].productID);
        printf("%c | ", products[i].type);
        printf("%f | ", products[i].quantity);
        printf("%f | ", products[i].cost);
        printf("%f | \n", products[i].profit);
    }
}

void printStructsSale(Sale *sales, int numberOfSales) {
    for(int i=0; i<numberOfSales; i++) {
        printf("%d | ", sales[i].year);
        printf("%d | ", sales[i].month);
        printf("%d | ", sales[i].day);
        printf("%d | ", sales[i].product);
        printf("%f | ", sales[i].quantity);
        printf("%f | \n", sales[i].price);
    }
}

int countLines(FILE *fp) {
    int count = 0;
    char c;

    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n')
            count += 1;

    rewind(fp);
    return count;
}
