"Run File every Hour"
import os, datetime, time, random

hour = datetime.datetime.now().hour
minute = datetime.datetime.now().minute

while True:
    # Execute file at given time
    if hour == datetime.datetime.now().hour and \
        minute == datetime.datetime.now().minute:
        print('\n\nExecute File at ' + str(hour) + ':0' + str(minute) + '\n')
        exec(open(os.getcwd() + '\\Auction_House_Search.py').read())
        exec(open(os.getcwd() + '\\Auction_House_Analysis.py').read())
    
    # One hour and some minutes later
    hour = datetime.datetime.now().hour + 1
    minute = random.randint(0, 5)
    
    # Reset hour
    if hour == 24:
        hour = 0
    
    # Sleep until timestamp
    print('\n\nNext Execution at ' + str(hour) + ':0' + str(minute))
    while hour > datetime.datetime.now().hour or \
        minute > datetime.datetime.now().minute or \
        (hour == 0 and minute > datetime.datetime.now().minute):
        time.sleep(random.uniform(0, 10))