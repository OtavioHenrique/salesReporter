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
void writeTotalStatistic(FILE *reportFile, Sale *sales, Product *products, int numberOfSales, int numberOfProducts);
float totalSales(Sale *sales, int numberOfSales);
float totalProductSold(Sale *sales, int numberOfSales);
int totalDaysOfPeriod(Sale *sales, int numberOfSales);
float averageProfitability(Sale *sales, Product product, int numberOfSales);
float productSoldQuantity(Sale *sales, Product product, int numberOfSales);

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

float totalSales(Sale *sales, int numberOfSales) {
    float totalSales = 0.0;

    for(int i = 0; i < numberOfSales; i++) {
        totalSales += sales[i].price;
    }

    return totalSales;
}

float totalProductSold(Sale *sales, int numberOfSales) {
    float totalSold = 0.0;

    for(int i = 0; i < numberOfSales; i++) {
        totalSold += sales[i].quantity;
    }

    return totalSold;
}

float productRevenue(Sale *sales, Product product, int numberOfSales) {
    float totalSales = 0.0;

    for(int i = 0; i < numberOfSales; i++) {
        if(sales[i].product == product.productID) {
            totalSales += sales[i].price;
        }
    }

    return totalSales;
}

int totalDaysOfPeriod(Sale *sales, int numberOfSales) {
    int totalDays = 0;
    
    for(int i = 0; i < numberOfSales; i++) {
        if(sales[i].day != sales[i+1].day) {
            totalDays++;
        }
    }

    return totalDays;
}

float averageProfitability(Sale *sales, Product product, int numberOfSales) {
    float totalSales = 0.0;
    int productSold = 0;

    for(int i =0; i < numberOfSales; i++) {
        if(sales[i].product == product.productID) {
            productSold++;
            totalSales += sales[i].price;
        }
    } 

    float averagePrice = totalSales / productSold;
    float profitability = (averagePrice / product.cost - 1) * 100;

    return profitability;
}

float productSoldQuantity(Sale *sales, Product product, int numberOfSales) {
    float totalQuantity = 0.0;

    for(int i =0; i < numberOfSales; i++) {
        if(sales[i].product == product.productID) {
            totalQuantity += sales[i].quantity;
        }
    }

    return totalQuantity;
}

float weightedAverageProfitability(Sale *sales, Product *products, int numberOfSales, int numberOfProducts) {
    float sum = 0.0;

    for(int i = 0; i < numberOfProducts; i++) {
        float avgProf = averageProfitability(sales, products[i], numberOfSales);
        float soldQty = productSoldQuantity(sales, products[i], numberOfSales);

        sum += avgProf * soldQty;
    }

    int productSold = totalProductSold(sales, numberOfSales);
    return sum/productSold;
}

typedef struct profitability {
    int product;
    float profitability;
} Profitability;

void orderProfitability(Profitability *profitabilityByProducts, int numberOfProducts) {
    for(int i = 0; i < numberOfProducts -1; i++) {
        for(int j = 0; j < numberOfProducts - i - 1; j++) {
            if(profitabilityByProducts[j].profitability < profitabilityByProducts[j+1].profitability) {
                Profitability temp = profitabilityByProducts[j];
                profitabilityByProducts[j] = profitabilityByProducts[j+1];
                profitabilityByProducts[j+1] = temp;
            }
        }
    }
}

void printProfitability(Profitability *profitabilities, int numberOfProducts) {
    for(int i = 0; i < numberOfProducts; i++) {
        printf("Product: %d profitability: %.2f\n", profitabilities[i].product, profitabilities[i].profitability);
    }
}

Profitability *profitabilityByProducts(Sale *sales, Product *products, int numberOfSales, int numberOfProducts) {
    Profitability *profitabilityByProduct = malloc(sizeof(Profitability) * numberOfProducts);

    for(int i = 0; i < numberOfProducts; i++) {
        float avgProf = averageProfitability(sales, products[i], numberOfSales);
        profitabilityByProduct[i].product = products[i].productID;
        profitabilityByProduct[i].profitability = avgProf;
    }

    orderProfitability(profitabilityByProduct, numberOfProducts);

    return profitabilityByProduct;
}

void writeProfitabilityByProduct(FILE *reportFile, Sale *sales, Product *products, int numberOfSales, int numberOfProducts) {
    Profitability *profitabilityProducts = profitabilityByProducts(sales, products, numberOfSales, numberOfProducts);
    float profitabilityAverage = weightedAverageProfitability(sales, products, numberOfSales, numberOfProducts);

    fprintf(reportFile, "\n\nMore profitable products");
    fprintf(reportFile, "\n\nAverage profitability = %.2f \n\n", profitabilityAverage);

    for(int i = 0; i < numberOfProducts; i++){
        if(profitabilityProducts[i].profitability >= profitabilityAverage) {
            fprintf(reportFile, "%d %.2f \n", profitabilityProducts[i].product, profitabilityProducts[i].profitability);
        }
    }
}

void writeTotalStatistic(FILE *reportFile, Sale *sales, Product *products, int numberOfSales, int numberOfProducts) {
    float totalSale = totalSales(sales, numberOfSales);
    float totalDays = totalDaysOfPeriod(sales, numberOfSales);
    int productSold = totalProductSold(sales, numberOfSales);
    float profitabilityAverage = weightedAverageProfitability(sales, products, numberOfSales, numberOfProducts);

    fprintf(reportFile, "\n\nTotal and Statistic of Period\n\n");
    
    fprintf(reportFile, "Total Sales %.2f \n", totalSale);
    fprintf(reportFile, "Quantity of products sold %d \n", productSold);
    fprintf(reportFile, "Average by weekday %.2f \n", totalSale/totalDays);
    fprintf(reportFile, "Average by product %.2f \n", totalSale/productSold);
    fprintf(reportFile, "Average profitability %.2f% \n", profitabilityAverage);
}

void writeTotalSaleDay(FILE *reportFile, Sale *sales, int numberOfSales) {
    float total = 0;
    float dayTotal = 0.0;
    int daySales = 0;

     
    fprintf(reportFile, "Total Day Sales\n\n");
    fprintf(reportFile, "Day            Total     Average\n\n");

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

typedef struct productContribution {
    int product;
    float totalSales;
    float contribution;
} ProductContribution;

void orderContributions(ProductContribution *contributionByProduct, int numberOfProducts) {
    for(int i = 0; i < numberOfProducts -1; i++) {
        for(int j = 0; j < numberOfProducts - i - 1; j++) {
            if(contributionByProduct[j].contribution < contributionByProduct[j+1].contribution) {
                ProductContribution temp = contributionByProduct[j];
                contributionByProduct[j] = contributionByProduct[j+1];
                contributionByProduct[j+1] = temp;
            }
        }
    }
}

ProductContribution *productsContributions(Sale *sales, Product *products, int numberOfSales, int numberOfProducts) {
    ProductContribution *contributionByProduct = malloc(sizeof(ProductContribution) * numberOfProducts);
    float totalSale = totalSales(sales, numberOfSales);

    for(int i = 0; i < numberOfProducts; i++) {
        float productSales = productRevenue(sales, products[i], numberOfSales);
        contributionByProduct[i].product = products[i].productID;
        contributionByProduct[i].totalSales = productSales;
        contributionByProduct[i].contribution = (productSales / totalSale) * 100;
    }

    orderContributions(contributionByProduct, numberOfProducts);

    return contributionByProduct;
}

void writeContributionByProcut(FILE *reportFile, Sale *sales, Product *products, int numberOfSales, int numberOfProducts) {
    ProductContribution *contributionsByProduct = productsContributions(sales, products, numberOfSales, numberOfProducts);

    fprintf(reportFile, "\n\nContribution by products\n");
    fprintf(reportFile, "\nProduct      Total Sales      Contribution\n");

    for(int i = 0; i < numberOfProducts; i++) {
        fprintf(reportFile, "%d         %.2f          %.2f \n", contributionsByProduct[i].product, 
                contributionsByProduct[i].totalSales, contributionsByProduct[i].contribution);
    }
}

void outputReport(Sale *sales, Product *products, int numberOfProducts, int numberOfSales) {
    FILE *reportFile = fopen("final_report.txt", "w+");

    writeTotalSaleDay(reportFile, sales, numberOfSales);
    writeTotalSaleProduct(reportFile, sales, products, numberOfSales, numberOfProducts);
    writeTotalStatistic(reportFile, sales, products, numberOfSales, numberOfProducts);
    writeProfitabilityByProduct(reportFile, sales, products, numberOfSales, numberOfProducts);
    writeContributionByProcut(reportFile, sales, products, numberOfSales, numberOfProducts);
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
