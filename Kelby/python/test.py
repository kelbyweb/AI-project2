
# -*- coding: utf-8 -*-
from time import time #import "time" function to test 


generatedStates = open('generatedStates.txt', 'w')
solutionRoute = open('solutionRoute.txt', 'w')
Ti=[[],[],[]] # List for initial state of each tower (3 towers) 
Tg=[[],[],[]] # List for goal state of each tower (3 towers) 
states=[[],[],[]] # states=[[towerOrigin],[last disk's value],[heuristic]] 
times=[]
temp=[] 
counterTime = 1

# making a list to each disk to know when its movement shall execute 
def makeTimes():
    for x in range(0,numberDisks): 
        times.extend([[]])
    for x in reversed(range(0,numberDisks)): 
        temp.append(x+1)
    for x in range(0,numberDisks):
        Ti[0].append(temp[x]) #sets the initial state according the number of disks EG: [[4,3,2,1],[],[]] 
    Tg[2].append(temp[x]) #sets the goal state according the number of disks EG: [[],[],[4,3,2,1]]
    for x in range(1,pow(2,numberDisks)+1): 
        for y in range(1,numberDisks+1):
            if pow(2,y)*(x)-pow(2,y-1)<pow(2,numberDisks): 
                times[y-1].append(pow(2,y)*(x)-pow(2,y-1)) #sets the expected movements for each disk.

#Returns the last disk's value from 'tower' given 
def lastDiskValue(tower):
    return Ti[tower][len(Ti[tower])-1]

#returns the expected direction movement of a given disk 
def expectedDirection(disk):
    if disk % 2 == 0 and numberDisks % 2 == 0:  #even number and even number of disk
        return -1 # To LEFT 
    elif disk % 2 != 0 and numberDisks % 2 == 0: #odd number and even number of disk
        return -2 # To RIGHT 
    elif disk % 2 == 0 and numberDisks % 2 != 0: #even number and odd number of disk 
        return -2 # To Right
    elif disk % 2 != 0 and numberDisks % 2 != 0: #Odd number and Odd number of disk
        return -1 # To LEFT 
    else:
        return -4 #Failure case

#To check if a movement from "towerOrigin" to "towerDest" may be executed 
def checkMovement(towerOrigin,towerDest):
    lastDiskOrigin=len(Ti[towerOrigin]) # origin tower last disk index
    lastDiskDest=len(Ti[towerDest]) # Dest tower last disk's index

    if lastDiskOrigin == 0: # Checking if origin tower is empty 
        return False
    else:
        if lastDiskDest == 0 : # checking if dest tower is empty
            return True
        elif lastDiskValue(towerOrigin)<lastDiskValue(towerDest):#checking if origin tower's last disk is less than dest
            return True
        else:
            return False

def printTi(fileToWrite):
    fileToWrite.write("\nTA: "+ str(Ti[0] ) + "\nTB: " + str(Ti[1] ) + "\nTC: " + str(Ti[2] )+ "\n")

#to show a possible disk's movement from 'originTower' to 'destTower'. (it doesn't affect Ti) 
def simulateMoveDish(originTower,destTower,heuris):
    Ti[originTower+destTower].append(Ti[originTower].pop()) #make movement 
    generatedStates.write("\n\t\t\tHeuristics value: : "+str(heuris))
    generatedStates.write("\n\tTA: "+ str(Ti[0] ) + "\n\tTB: " + str(Ti[1] ) + "\n\tTC: " + str(Ti[2] )+ "\n") 
    Ti[originTower].append(Ti[originTower+destTower].pop()) #returns movement

#to move a dish from 'originTower' to 'destTower' 
def moveDish(originTower,destTower):
    Ti[originTower+destTower].append(Ti[originTower].pop()) 
    printTi(solutionRoute) 
    solutionRoute.write("___________________\n")


#MAIN
numberDisks= int(input("Please, enter the number of disks: "))#ask for the number of disks to play 
makeTimes()
print ("Initial State: ",Ti)

solutionRoute.write("\t\t\t Tower of Hanoi\n\n") 
solutionRoute.write("\t\t Heuristics: Best First\n") 
solutionRoute.write("\t\t File: Solution Route\n\n") 
solutionRoute.write("Initial State\n") 
printTi(solutionRoute) 
solutionRoute.write("___________________\n")

generatedStates.write("\t\t\t Tower of Hanoi\n\n") 
generatedStates.write("\t\t Heuristics: Best First\n") 
generatedStates.write("\t\t File: Generated States\n\n")

time_i = time() 
while Ti != Tg:

    for tower in range(0,len(Ti)):
        for displ in range(1,3): #check if may displace -1 or -2 position in array, which means -1 ->1 to left ; -2 -1 right
            if checkMovement(tower,tower-displ) == True:
                states[0].append(tower) #add field 'towerOrigin'
                states[1].append(-displ) #add field 'last disk's value' with the displacement 
                states[2].append(0) #add field 'heruistic' with initial value of 0


for x in range(0,len(states[0])):
    if counterTime in times[lastDiskValue(states[0][x])-1]: #check if it's time to move disk x
        states[2][x]=states[2][x] + 1 #if yes, we add the minimum heuristic value possible .... +1 
    else:
        states[2][x]=states[2][x] + 3 #if not, we add the maxium heuristic value possible .... +3
if states[1][x] == expectedDirection(lastDiskValue(states[0][x])): 
    states[2][x]=states[2][x] + 1
else:
    states[2][x]=states[2][x] + 3

generatedStates.write("\nMovement number: "+ str(counterTime)) 
generatedStates.write("\nCurrent state of Towers:") 
printTi(generatedStates)
generatedStates.write("\nPossible Movements:")

for x in range(0,len(states[0])): 
    simulateMoveDish(states[0][x],states[1][x],states[2][x])

indexSelected=states[2].index(min(states[2]))

moveDish(states[0][indexSelected],states[1][indexSelected]) 
states = [[],[],[]]
counterTime = counterTime+1
time_f = time()

time_ex = time_f - time_i

print ("Final State",Ti)
print ('Execution time:',time_ex,'seconds')
                              
