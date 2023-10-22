import csv
from progress.bar import Bar

files =[
    'sbdb_query_results(Inc-0-1).csv',
    'sbdb_query_results(Inc-1-3).csv',
    'sbdb_query_results(Inc-5-7).csv',
    'sbdb_query_results(Inc-0-1).csv',
    'sbdb_query_results(Inc-3-5).csv',
    'sbdb_query_results(Inc-7-10).csv'
]

#Combine a list of csvs into a single file
def combineData(files, output='asteroids.csv', path='data/'):
    bar = Bar('Combining asteroid csv files together', max=795697)
    with open(output, "w+") as outputFile:
        fieldNames = ["spkid","full_name","neo","pha","e","a","q","i","om","w","ma","ad","n","per"]
        writer = csv.DictWriter(outputFile, delimiter=',', fieldnames=fieldNames)
        writer.writeheader()
        for filename in files:
            with open(path + filename, 'r') as csvFile:
                reader = csv.DictReader(csvFile, delimiter=',')
                for row in reader:
                    bar.next()
                    writer.writerow(row)

combineData(files)