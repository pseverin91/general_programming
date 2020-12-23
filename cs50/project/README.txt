Auction House



1. Introduction

Hello everybody! My name is Peter Severin and I am from Mannheim, Germany. Recently, I finished 
getting my PhD in Finance and now I want to extend my knowledge in the realm of computer science. 
Given my background, it is perhaps not surprising that I chose to write a program about trading. 
However, my "Auction House Program" is about trading in a computer game, namely World of Wacraft 
Classic. In this game, my brother is making significant money (or gold) by predicting demand and 
supply for certain goods in a manual way. My program is a way to automatise this tedious task 
and get trade recommendations automatically!



2. Program

The entire program is written via Spyder in Python and consists of three parts: 

1. Auction_House_Loop.py
2. Auction_House_Search.py
3. Auction_House_Analysis.py

Of these three files, "Auction_House_Loop" will have to be executed and then access the other 
two files "Auction_House_Search.py" and "Auction_House_Analysis" consecutively in an endless 
loop.


2.1. Auction_House_Loop

In this first program, Python will start an endless loop that executes the other two programs in 
an interval of roughly one hour. Some randomization in the timing should prevent the game to 
know that a bot is playing. 


2.2. Auction_House_Search

In a first step, the program will automatically login to the game. It will then browse a number of 
goods and take screenshots of the quantities and prices. After exiting the game, it will recognise 
the numbers from the screenshots and create price and quantity lists. 


2.3. Auction_House_Analysis

These information are then used to analyse price and trading behavior. At first, the lists are 
summarized for more concise information and illiquid goods are excluded from the analysis. Then, 
I start analyzing the individual goods with respect to their current prices relative to their 
previous prices. Further, I look at aggregate price behavior. At last, I compute the most 
profitable trades and make hourly recommendations based on those. 



3. Conclusion

That was it, thank you for very mnuch for your attention. This was "Auction House!




