import os
os.system('cls')

from random import shuffle

set1 = {False, True, True, False}
set2 = {1, 2, 3}
set3 = {1, "automação", False}

list1 = list(set1)
list2 = list(set2)
list3 = list(set3)

shuffle(list1)
shuffle(list2)
shuffle(list3)

print(list1)
print(list2)
print(list3)

