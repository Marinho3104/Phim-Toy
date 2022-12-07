

file = open("./token.h", 'r')

info = file.read()

file.close()

info = info.split('\n')

c = 1

for i in range (len(info)):

    if info[i] == '': continue

    cont = 0

    for _ in range (len(info[i])): 

        if cont == 2: 
            
            info[i] = info[i][:_]

            info[i] += str(c)

            c+=1

            print(info[i])
        
            break

        elif info[i][_] == ' ': cont+=1


