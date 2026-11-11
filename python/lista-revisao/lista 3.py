import os
os.system("cls")

hokages = ["Hashirama","Tobirama","Hiruzen", "Minato", "Tsunade"]
sensei = ["Assuma", "Kurenai", "Gai", "Kakashi", "Iruka"]

print(hokages)
print(hokages[0:3])
print(sensei)
print(sensei[0:3])

hokages.append(sensei)
print(hokages)

hokages.insert(5, "jiraya")
print(hokages)

hokages.pop()
print(hokages)

hokages.remove("Hiruzen")
print(hokages)

hokages.clear()
print(hokages)

hokages.index("Tsunade")
print(hokages)

hokages.count("Tobirama")
print(hokages)

hokages.sort()
print(hokages)

hokages.reverse()
print(hokages)

hokages.copy()
print(hokages)

