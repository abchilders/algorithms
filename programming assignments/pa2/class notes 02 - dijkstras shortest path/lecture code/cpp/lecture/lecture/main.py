#python tends to be more readable and less verbose, but also comes with weird syntax
#also python documentation is better than c++ documentation lol

# python has built in CSV parser
# formatting is important in python -- indentation is important! 
# can use visual studio code debugger on this as well 

#for CSV parsing:
import csv 
from graph import graph #???

# function definition
# python has no return types or variable types in definition
def process_csv(file_name):
	data = [] #data is an empty array
	with open(file_name, 'r') as some_file: # open file_name in read mode
		csv_file = csv.reader(some_file, 
								delimiter=',' #delimiter is comma
								quotechar='"' #escape character -- if you have a comma inside a cell, encase it in quotes so that it doesn't act as a delimiter
								) 
		for row in csv_file:
			data.append(row) #append row into data array
		return data

# example of getting data from CSV
result = process_csv("data.csv")

g = Graph();
g.add_vertex("a")
g.add_vertex("b")
g.add_vertex("c")
g.connect_vertex("a", "b", 3, True)
g.connect_vertex("a", "c", 15)
g.connect_vertex("b", "c", 7, True)
distances = g.compute_shortest_path(a, b) #???

print("done")