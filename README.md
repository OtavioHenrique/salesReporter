## SalesReporter

This project is based on read two files as input, you can find them into `input/` they are `products.csv` and `sales.csv`.

### Input Files

The `products.csv` is a simple file with a database of products of some trade.

| Information       | Format        | Comments                                                                    |
|-------------------|---------------|-----------------------------------------------------------------------------|
| Product Id        | Five digits   |                                                                             |
| Inventory Control | One character | Character 'U' for unit and 'W' for weight.                                  |
| Stock Quantity    | Real Number   | In case of unit product, decimal part will be zero. (But you can use union) |
| Product Cost      | Real Number   |                                                                             |
| Profit Margin     | Real Number   | Profit margin applied for product sale                                      |

The `sales.csv` file is a database of sales of the store

| Information   | Format      |
|---------------|-------------|
| Sale's Year   | Four digits |
| Sale's Month  | Two digits  |
| Sale's Day    | Two Digits  |
| Product Id    | Five Digits |
| Quantity Sold | Real Number |
| Sale's Price  | Real Number |

### Output File

The output file needs to have a full five step report of store sales described below:

#### Part I - Total Sales by Day

At this part you need to write the sum of all sales and the average sale's price for each day. At this part you need to order the lines by sale's date.

Ex:

```
Total Sales by Day
Day        Total   Average
02/01/2015 3844.17 53.80
03/01/2015 5441.09 72.25
04/01/2015 0.00    0.00
05/01/2015 4133.51 61.12
06/01/2015 3699.03 49.87
```

*It's important to note that to obtain the product average sale's price by day you need to sum the sale's price and the quantity of sales of the day.*

#### Part II - Total Sales by Product

At this part for each product you write the sum of its sale's price, the sum of the sold quantity, the average price of its sales, and its profitability.

*To get a product profitability you need to divide the product average sale's price by the product cost (avaible at `products.csv`), for example, if the average sale's price is U$33.90 and the product cost U$26.88 the profitability will be `profitability = (33.90 / 26.88 - 1)  * 100 = 26.1%`*

*It's important to note that to obtain the product average sale's price you need to sum the sale's price and the quantity sold.*
