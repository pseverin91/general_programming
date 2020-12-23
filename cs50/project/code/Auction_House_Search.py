"Header"
# Packages
import sys, subprocess, os, csv, time, datetime
import random, pyperclip, pyautogui as gui, PIL, cv2, numpy as np

# Directories
dir = os.getcwd() + '\\'
data_orig = dir + 'data_orig\\'
data_form = dir + 'data_form\\'
data_final = dir + 'data_final\\'

# Take Time
time_start = time.time()

# Create noise variable
noise = []
for i in range(100):
    noise.append(random.uniform(0, 1))

# Breaking Conditions
#print(gui.position())
gui.PAUSE = 0.2 + noise[0]
gui.FAILSAFE = True
#time.sleep(3)
#sys.exit()





"1. Extract Screenshots from Auction House"
"1.1. Start Game"
process = subprocess.Popen('C:\Program Files (x86)\World of Warcraft\World of Warcraft Launcher.exe')
time.sleep(15 + noise[1])
gui.moveTo(780, 830, duration = 0.25 + noise[2])    # 930
gui.click(780, 830, interval = noise[3])
time.sleep(15 + noise[4])
gui.press('enter', interval = noise[5])
time.sleep(10 + noise[6])



"1.2. Browse for Goods"
# Define goods
herbs = ['peacebloom', 'silverleaf', 'earthroot', 'mageroyal', 'briarthorn', 'stranglekelp', \
         'bruiseweed', 'wild steelbloom', 'grave moss', 'kingsblood', 'liferoot', 'fadeleaf', \
         'goldthorn', 'khadgar\'s whisker', 'wintersbite', 'firebloom', 'purple lotus', \
         'ghost mushroom', 'blindweed', 'swiftthistle', 'gromsblood', 'plaguebloom', 'dreamfoil', \
         'sungrass', 'mountain silversage', 'arthas\' tears', 'golden sansam', 'icecap', \
         'black lotus', 'bloodvine']
enchanting = ['strange dust', 'soul dust', 'vision dust', 'dream dust', 'illusion dust', \
              'greater magic essence', 'greater astral essence', 'greater mystic essence', \
              'greater nether essence', 'greater eternal essence', 'large radiant shard', \
              'large brilliant shard']
cloth = ['linen cloth', 'wool cloth', 'silk cloth', 'mageweave cloth', 'runecloth', 'felcloth', 'mooncloth']
leather = ['light leather', 'medium leather', 'heavy leather', 'thick leather', 'rugged leather', \
           'black dragonscale', 'devilsaur leather', 'cured rugged hide', 'core leather']
ore = ['copper bar', 'tin bar', 'bronze bar', 'iron bar', 'mithril bar', 'thorium bar', \
       'silver bar', 'gold bar', 'enchanted thorium bar', 'dark iron bar', \
       'arcane crystal', 'arcanite bar', 'elementium bar', \
       'rough stone', 'coarse stone', 'heavy stone', 'solid stone', 'dense stone']
others = ['golden pearl', 'black pearl', 'righteous orb', 'dark rune', \
          'essence of water', 'essence of air', 'essence of earth', 'essence of fire', 'living essence', \
          'essence of undeath', 'elemental earth', 'elemental fire', 'elemental water', 'elemental air']
goods = herbs + enchanting + cloth + leather + ore + others
#goods = ['light leather', 'illusion dust', 'illusion dust2']

# Initiate item search"
gui.moveTo(960, 300, duration = 0.25 + noise[10])
gui.click(960, 300, button = 'right', interval = noise[11])
gui.moveTo(350, 700, duration = 0.25 + noise[12])
gui.click(350, 700, interval = noise[13])

# Browse through goods
runtime = cv2.imread(data_orig + 'searchtime\\ah_comparison.png')
for item in goods:

    gui.PAUSE = 0.5 + noise[random.randint(0, 99)] / 10
    gui.press('backspace')
    gui.moveTo(575, 200, duration = 0.25 + noise[15])
    gui.click(575, 200, interval = noise[16])

    pyperclip.copy('"' + item + '"')
    gui.hotkey('ctrl', 'v', duration = noise[17])
    gui.press('enter')

    # Continue searching if items not offered or found
    while True:
        image = gui.screenshot(region = (360, 400, 100, 20))
        try:
            image.save(data_orig + 'searchtime\\searchtime.png')  # Sometimes gets stuck here and throws error
            image = cv2.imread(data_orig + 'searchtime\\searchtime.png')
            comparison = cv2.matchTemplate(image, runtime, cv2.TM_CCOEFF_NORMED)
            if comparison[0][0] > 0.9:
                continue
            else:
                break
        except:
            continue

    # Take screenshots
    screenshot = gui.screenshot()
    screenshot.save(data_orig + 'screenshots\\' + item + '.png')



"1.3. Exit the Game"
gui.PAUSE = 0.2 + noise[random.randint(0, 99)]
gui.press('esc', presses = 3, interval = noise[20])
gui.moveTo(960, 640, duration = 0.25 + noise[21])
gui.click(960, 640, interval = noise[22])
time.sleep(3 + noise[23])
gui.keyDown('alt'); gui.press('f4'); gui.keyUp('alt')

del enchanting, herbs, image, leather, noise, ore, runtime
#sys.exit()





"2. Identify Prices and Quantities in Screenshots"
"2.1. Learn Numbers"
def f_numbers_extract():

    for i in range(10):
        # Learn price numbers
        screenshot = cv2.imread(data_orig + 'numbers_raw\\ah_price_' + str(i) + '.png',)
        number = screenshot[424:439, 437:445]
        cv2.imwrite(data_form + 'numbers\\price_number_' + str(i) + '.png', number)
    
        # Learn quantity numbers
        screenshot = cv2.imread(data_orig + 'numbers_raw\\ah_quantity_' + str(i) + '.png')
        number = screenshot[426:437, 494:503]
        if i == 0:
            number = screenshot[426:437, 502:511]
        if i == 9:          # To be updated
            number = screenshot[446:456, 494:503]
        cv2.imwrite(data_form + 'numbers\\quantity_number_' + str(i) + '.png', number)

#f_numbers_extract()
#sys.exit()



"2.2. Identify Numbers"
def f_numbers_detect(screenshot):
    
    # Open output image and output variables
    screenshot_marked = cv2.imread(screenshot)[400:650, 310:620]
    dictionary, price_tuples, quantity_tuples = {'0': '0'}, [], []

    # Test and mark price numbers    
    for i in range(10):
        #print('\nTesting price number ' + str(i))
        
        number = cv2.imread(data_form + 'numbers\\price_number_' + str(i) + '.png')
        image = cv2.imread(screenshot)[400:650, 310:620]        
        comparison = cv2.matchTemplate(image, number, cv2.TM_CCOEFF_NORMED)
        loc = np.where(comparison >= 0.9)
    
        for j in zip(*loc[::-1]):
            #print(j)
            dictionary[j] = i
            price_tuples.append(j)
            cv2.rectangle(screenshot_marked, j, (j[0] + number.shape[1], j[1] + number.shape[0]), (0, 0, 255))

    # Test and mark quantity numbers    
    for i in range(10):
        #print('\nTesting quantity number ' + str(i))
        
        number = cv2.imread(data_form + 'numbers\\quantity_number_' + str(i) + '.png')
        image = cv2.imread(screenshot)[400:650, 310:620]
        comparison = cv2.matchTemplate(image, number, cv2.TM_CCOEFF_NORMED)
        loc = np.where(comparison >= 0.9)
    
        for j in zip(*loc[::-1]):
            #print(j)
            dictionary[j] = i
            quantity_tuples.append(j)
            cv2.rectangle(screenshot_marked, j, (j[0] + number.shape[1], j[1] + number.shape[0]), (255, 0, 0))
    
    # Close output image and output variables
    cv2.imwrite(data_form + 'screenshots_marked\\' + item + '.png', screenshot_marked)
    return dictionary, price_tuples, quantity_tuples

#dictionary, price_tuples, quantity_tuples = f_numbers_detect(data_orig + 'cuir robuste.png')
#sys.exit()



"2.3. Creating Price and Quantity Lists"
"2.3.1. Create Price List"
def f_prices(dictionary, price_tuples):
    
    # Group tuples into observations
    prices_sorted = sorted(price_tuples, key = lambda x: x[1])
    prices = [[]]
    group = 0
    
    for i in range(len(prices_sorted)):
        #Open first group
        if i == 0:
            prices[group].append(prices_sorted[i])
        #Group if small distance
        elif prices_sorted[i][1] - prices_sorted[i-1][1] <= 5:
            prices[group].append(prices_sorted[i])
        #Create new group if large distance
        else:
            group += 1
            prices.append([])
            prices[group].append(prices_sorted[i])
    
    # Insert tuples for missing digits
    for i in range(len(prices)):
        prices[i] = sorted(prices[i], key = lambda x: x[0])
        temp = 0
        for j in range(len(prices[i])):
            #Correct for single digit silver
            if (j != len(prices[i]) - 1) and (j != 0) and \
            ((prices[i][j][0] - temp) >= 20) and \
            ((prices[i][j+1][0] - prices[i][j][0]) >= 20):
                prices[i].insert(j, '0')
                j += 2
            #Correct for single digit copper
            if (j == len(prices[i]) - 1) and ((prices[i][j][0] - temp) >= 20):
                prices[i].insert(j, '0')
                j += 1
            temp = prices[i][j][0]
    
    # Translate tuples into prices
    for i in range(len(prices)):
        for j in range(len(prices[i])):
            prices[i][j] = str(dictionary[prices[i][j]])
        try:
            prices[i] = int(''.join(prices[i])) / 100
        except:
            print('No data')
    return prices

#prices = f_prices(dictionary, price_tuples)
#sys.exit()


"2.3.2. Create Quantity List"
def f_quantities(dictionary, quantity_tuples, prices):
    
    # Group tuples into observations
    quantities_sorted = sorted(quantity_tuples, key = lambda x: x[1])
    quantities = [[]]
    group = 0
    
    for i in range(len(quantities_sorted)):
        #Open first group
        if i == 0:
            quantities[group].append(quantities_sorted[i])
        #Group if small distance
        elif quantities_sorted[i][1] - quantities_sorted[i-1][1] <= 5:
            quantities[group].append(quantities_sorted[i])
        #Create new group if large distance
        else:
            group += 1
            quantities.append([])
            quantities[group].append(quantities_sorted[i])
    
    # Split observations into stacks and sizes
    quantities_stacks = []
    quantities_size = []
    for i in range(len(quantities)):
        quantities[i] = sorted(quantities[i], key = lambda x: x[0])
        quantities_stacks.append([])
        quantities_size.append([])    
        for j in range(len(quantities[i])):
            if j == 0:
                quantities_stacks[i].append(quantities[i][j])
            elif quantities[i][j][0] - quantities[i][0][0] <= 20:
                quantities_stacks[i].append(quantities[i][j])
            else: 
                quantities_size[i].append(quantities[i][j])
       
    # Translation into quantities
    for var in [quantities_stacks, quantities_size]:
        for i in range(len(var)):
            for j in range(len(var[i])):
                var[i][j] = str(dictionary[var[i][j]])
            try:
                var[i] = int(''.join(var[i]))
            except:
                print('No data')
    quantities = list(np.multiply(quantities_stacks, quantities_size))
    quantities = quantities[len(quantities) - len(prices):]
    return quantities

#quantities = f_quantities(quantity_tuples)
#sys.exit()



"2.4. Extract Data from Screenshots"
# Define data extract function
def f_data_extract(screenshot):
    dictionary, price_tuples, quantity_tuples = f_numbers_detect(screenshot)
    prices = f_prices(dictionary, price_tuples)
    quantities = f_quantities(dictionary, quantity_tuples, prices)
    
    return prices, quantities

# Extract data from all saved screenshots
for item in goods:
    print('Current Good: ' + item)
    
    #Get prices, quantities, and time
    prices, quantities = f_data_extract(data_orig + 'screenshots\\' + item + '.png')
    time_stamp = datetime.datetime.fromtimestamp(time.time())
    week = time_stamp.isocalendar()[1]
    day = time_stamp.isoweekday()
    hour = time_stamp.hour
    
    # Create folder for each item
    if not os.path.isdir(data_final + 'extracts\\' + item):
        os.makedirs(data_final + 'extracts\\' + item)
    
    #Write to csv-file
    csv_file = open(data_final + 'extracts\\' + item + '\\' + item + ' ' + \
                    str(week) + ' ' + str(day) + ' ' + str(hour) + '.csv', 'w', newline = '')
    csv_write = csv.writer(csv_file, delimiter = '\t', lineterminator = '\n')
    csv_write.writerow(['item', 'week', 'weekday', 'hour', 'quantity', 'price'])
    
    for i in range(len(prices)):
        csv_write.writerow([item, week, day, hour, quantities[i], prices[i]])
    
    csv_file.close()

#sys.exit()





"3. Data Analysis"





print('\n\nRuntime: ' + str(round(time.time() - time_start, 4)))
#sys.exit()





