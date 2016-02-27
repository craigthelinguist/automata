

import random

BOARD_WIDTH = 45
BOARD_HEIGHT = 35

def random_bool():
    x = random.random()
    return True if x > 0.5 else False

def random_board():
    random.seed()
    s = str(BOARD_HEIGHT) + " " + str(BOARD_WIDTH) + "\n"
    for row in range(BOARD_HEIGHT):
        for col in range(BOARD_WIDTH):
            if random_bool():
                s += "1"
            else:
                s += "0"
        s += "\n"
    return s

for i in range(10):
    with open("rand_" + str(i) + ".txt", "w") as f:
        f.write(random_board())

print("done")
    
    
