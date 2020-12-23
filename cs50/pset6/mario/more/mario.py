# Packages
from cs50 import get_int

# Ask for integer
size = 0
while size < 1 or size > 8:
    size = get_int('Height: ')

# Print bricks
for i in range(size):
    for j in range(2 * size + 2):
        
        #First space
        if j < size - i - 1:
            print(' ', end = '')
            
        #Triangle
        elif j < size or (j >= size + 2 and j <= size + 2 + i):
            print('#', end = '')
            
        #Middle space
        elif j >= size and j < size + 2:
            print(' ', end = '')
    
    print('')
            
        
