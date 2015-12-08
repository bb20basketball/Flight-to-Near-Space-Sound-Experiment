import xlsxwriter
name = raw_input("enter in a name for the file ")
workbook = xlsxwriter.Workbook('%s.xlsx'%name)
worksheet = workbook.add_worksheet()
fob = open("DATALOG.TXT","r")
cool = fob.read()
cool = cool.split("\n")
newlist=[]
for i in cool:
    i = i.split(" ; ")
    newlist.append(i)

another=[]
time = []
for b in newlist:
    for i in b:
        i = i.split(" : ")
        if len(i)==1:
            time.append(i)
        else:
            another.append(i)
final = []
second = []
ok = []
for i in another:
    i=i[0].split(" / ")
    final.append(i[0])
    second.append(i[1])
    ok.append((i[2]))
counter=2
worksheet.write("A1","Response")
worksheet.write("B2","Altitude")
for i in range(len(final)):
    worksheet.write("A%s"%counter,float(final[i]))
    worksheet.write("B%s"%counter,float(second[i]))
    worksheet.write("C%s"%counter,float(ok[i]))
    worksheet.write("D%s"%counter,float(another[i][1]))
    worksheet.write("E%s"%counter,str(time[i][0]))
    counter+=1


workbook.close()
fob.close()
