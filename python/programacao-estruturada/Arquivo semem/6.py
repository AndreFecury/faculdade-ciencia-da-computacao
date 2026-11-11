import os
import shutil



if os.path.exists("pythoncoiso"):
    shutil.rmtree("pythoncoiso")
    print("O arquivo existe")
else:
    print("O arquivo nao existe")