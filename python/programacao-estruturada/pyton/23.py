import os
os.system('cls')

hokages = {"Hashirama" , "Tobirama" , "Hiruzen" , "Minato" , "Tsunade", "Hashirama", "Minato"}
print(set(hokages))

hokages.add("Kakashi")              # Add.
print(set(hokages))

hokages.add("Naruto")               # Add.
print(set(hokages))

hokages.remove("Hiruzen")           # Remove
print(set(hokages))

