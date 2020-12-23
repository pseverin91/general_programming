"Header"
# Packages
import sys, os, time, datetime
import numpy as np, pandas as pd, matplotlib.pyplot as plt

# Directories
dir = os.getcwd()
data_orig = dir + '\\data_orig\\'
data_form = dir + '\\data_form\\'
data_final = dir + '\\data_final\\'





"3. Market Price Analysis"
"3.1. Data Preparation"
"3.1.1. Summarize Data"
for item in os.listdir(data_final + 'extracts\\'):
    
    print('\n' + item.center(25, '*'))
    df = pd.DataFrame()
    
    for excerpt in os.listdir(data_final + 'extracts\\' + item):
        
        print(excerpt)
        
        #Load data excerpt
        df_temp = pd.read_csv(data_final + 'extracts\\' + item + '\\' + excerpt, sep = '\t')

        #Adjust variables
        try:
            df_temp.loc[(df_temp['quantity'] == '[]') | (df_temp['price'] == '[]'), ['quantity', 'price']] = 0
        except:
            pass
        df_temp['volume'] = df_temp['quantity'] * df_temp['price']
        
        #Drop outliers
        if df_temp['price'].min() != 0:
            df_temp = df_temp[df_temp['price'] / df_temp['price'].min() <= 1.5]     # Only reasonable prices

        #Collapse dataset
        statistics = {'week': 'mean', 'weekday': 'mean', 'hour': 'mean', \
                      'quantity': 'sum', 'price': ['min', 'max'], 'volume': 'sum'}
        df_temp = df_temp.groupby('item').agg(statistics)
        df_temp.columns = ['week', 'weekday', 'hour', 'quantity_total', 'price_min', 'price_max', 'volume']

        #Compute variables
        df_temp['volume'] = round(df_temp['volume'], 2)
        df_temp['price_avg'] = round(df_temp['volume'] / df_temp['quantity_total'], 2)
        df_temp['spread'] = df_temp['price_max'] - df_temp['price_min']

        #Append datasets
        df = pd.concat([df, df_temp])

    #Sort and save dataset
    df = df.sort_values(['week', 'weekday', 'hour'])
    df.to_csv(data_final + '\\summary\\summary ' + item + '.csv', sep = '\t')


"3.1.2. Determine liquid Goods"
# Set liquidity threshold
threshold_volume = 2000
threshold_liquidity = 0.8
goods_liquid = []
for item in os.listdir(data_final + 'extracts\\'):
    
    print('\nAnalyze item for liquidity: ' + item)
    
    #Load data summary
    df = pd.read_csv(data_final + 'summary\\summary ' + item + '.csv', sep = '\t')
    
    #Compute liquid times
    df['liquid'] = 0
    df.loc[df['volume'] >= threshold_volume, 'liquid'] = 1
    liquid = df['liquid'].mean()
    print('Liquidity Factor: ' + str(round(liquid, 2)))
    
    #Select mostly liquid items
    if liquid >= threshold_liquidity:
        goods_liquid.append(item)

del threshold_volume, threshold_liquidity, liquid



"3.2. Analysis"
"3.2.1. Individual Analysis"
# Compute summary statistics for each good
df = pd.DataFrame()
for item in goods_liquid:
    
    print(item)
    
    #Load data summary
    df_temp = pd.read_csv(data_final + 'summary\\summary ' + item + '.csv', sep = '\t')

    #Generate item summary statistics
    df_temp = df_temp.groupby('item').mean()
    df_temp = df_temp.drop(columns = ['week', 'weekday', 'hour'])
    
    df = pd.concat([df, df_temp])

#Finalize dataset
for col in df.columns:
    df[col] = round(df[col], 2)
df = df[['price_min', 'price_avg', 'price_max', 'quantity_total']]

df.to_csv(data_final + 'summary.csv', sep = '\t')

# Analyze data history for most profitable trades
for item in goods_liquid:
    
    print('\n' + item.center(25, '*'))
    
    #Load data summary
    df_temp = pd.read_csv(data_final + 'summary\\summary ' + item + '.csv', sep = '\t')
    
    #Compute trade values
    purchase_price = float(df_temp[df_temp['price_avg'] == df_temp['price_avg'].min()]['price_avg'].iloc[0])
    purchase_quantity = int(df_temp[df_temp['price_avg'] == df_temp['price_avg'].min()]['quantity_total'].iloc[0])
    sales_price = float(df_temp[df_temp['price_min'] == df_temp['price_min'].max()]['price_avg'].iloc[0])
    sales_grossprofit = round((sales_price - purchase_price) * purchase_quantity / 100, 2)
    sales_netprofit = round((sales_price * 0.95 - purchase_price) * purchase_quantity / 100, 2)
    
    print('Purchase quantity: ' + str(purchase_quantity))
    print('Purchase price: ' + str(round(purchase_price / 100, 2)) + 'g')
    print('Sales price; ' + str(round(sales_price / 100, 2)) + 'g')
    print('Gross profit: ' + str(sales_grossprofit) + 'g')
    print('Net profit: ' + str(sales_netprofit) + 'g')

# sys.exit()


"3.2.3. Aggregate Analysis"
# Hourly movements
df = pd.DataFrame()
for item in goods_liquid:
    
    print(item)
    
    #Load data summary
    df_temp = pd.read_csv(data_final + 'summary\\summary ' + item + '.csv', sep = '\t')

    #Generate normalized variables
    df_temp['price_min'] = df_temp['price_min'] / df_temp['price_min'].mean()
    df_temp['price_avg'] = df_temp['price_avg'] / df_temp['price_avg'].mean()
    df_temp['volume'] = df_temp['volume'] / df_temp['volume'].mean()
    df_temp['spread'] = df_temp['spread'] / df_temp['spread'].mean()
    
    #Create complete dataset
    df = pd.concat([df, df_temp])

#Collapse data over hours
print('Minimum, Maximum, and STD of Price (min): ' + str((df['price_min'].min(), df['price_min'].max(), df['price_min'].std())))
print('Minimum, Maximum, and STD of Price (avg): ' + str((df['price_avg'].min(), df['price_avg'].max(), df['price_avg'].std())))
print('Minimum, Maximum, and STD of Volume: ' + str((df['volume'].min(), df['volume'].max(), df['volume'].std())))
print('Minimum, Maximum, and STD of Spread: ' + str((df['spread'].min(), df['spread'].max(), df['spread'].std())))
df = df.groupby('hour').mean()
df['hour'] = df.index

#Price plot
plt.plot(df['hour'], df['price_min'])
plt.plot(df['hour'], df['price_avg'])
plt.title('Market Prices over Time')
plt.xlabel('Time (Hour)')
plt.ylabel('Price')
plt.legend(['Minimum Price', 'Average Price'])
plt.savefig(data_final + 'prices.png')
plt.show()

#Liquidity plot
plt.plot(df['hour'], df['volume'])
plt.plot(df['hour'], df['spread'])
plt.title('Market Liquidity over Time')
plt.xlabel('Time (Hour)')
plt.ylabel('Liquidity')
plt.legend(['Volume', 'Spread'])
plt.savefig(data_final + 'liquidity.png')
plt.show()


"3.3. Purchase Recommendations"
# Combine summary data and recent trade data
df1 = pd.read_csv(data_final + 'summary.csv', sep = '\t')
df2 = pd.DataFrame()

for item in goods_liquid:
    
    print(item)
    
    #Load data summary
    df_temp = pd.read_csv(data_final + 'summary\\summary ' + item + '.csv', sep = '\t')

    df2 = pd.concat([df2, df_temp.loc[[len(df_temp) - 1]]])

df2 = df2[['item', 'price_min', 'price_avg', 'price_max', 'quantity_total']]
df = pd.merge(df1, df2, on = 'item', suffixes = ('_sum', '_current'))

# Identify attractive trades
df['margin_min'] = round(df['price_min_current'] / df['price_min_sum'], 2)
df['margin_avg'] = round(df['price_avg_current'] / df['price_min_sum'], 2)
df['price'] = round(df['price_avg_current'], 2)
df['quantity'] = df['quantity_total_current']
df['profit'] = round(df['price'] * (0.95 - df['margin_avg']) * df['quantity'] / 100, 2)
df['price level'] = round(df['margin_min'].mean(), 2)
df = df[['item', 'price level', 'margin_min', 'margin_avg', 'price', 'quantity', 'profit']]
df = df.sort_values(['profit'], ascending = False)

df = df[((df['margin_min'] <= 0.8) | (df['profit'] >= 5)) & (df['margin_min'] != 0)]

print('\nNumber of Deals: ' + str(len(df)))
print('General price level: ' + str(round(df['price level'].mean(), 2)))
print('Average price margin: ' + str(round(df['margin_min'].mean(), 2)))
print('Average profit in gold: ' + str(round(df['profit'].mean(), 2)))

# Save suggested trades
time_stamp = datetime.datetime.fromtimestamp(time.time())
week = time_stamp.isocalendar()[1]
day = time_stamp.isoweekday()
hour = time_stamp.hour

df.to_csv(data_final + 'suggested trades.csv', sep = '\t')
df.to_csv(data_final + 'suggestions\\suggested trades' + ' ' + \
                    str(week) + ' ' + str(day) + ' ' + str(hour) + '.csv', sep = '\t')


