
from datetime import datetime

now = datetime.now()


def calcplus (number1, number2):
    return number1 + number2
def calcminus (number1, number2):
    return number1 - number2
def calcmulti (number1, number2):
    return number1 * number2
def calcdivide (number1, number2):
    return number1 / number2

def clock ():
    return datetime.now().strftime("%H:%M")



if __name__ == "__main__":
    print(datetime.now().strftime("%H:%M"))