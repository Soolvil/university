import sys
import math

data_path = "./" + str(sys.argv[1])
data = open(data_path, "r")
print("Pupkevich Dmitry")
print("Simple Map Distance Computations")
print("")
dist_count, map_scale = map(float, data.readline().split())
dist_count = int(dist_count) 
map_scale = math.ceil((map_scale) * 100) / 100
print(f"Map Scale Factor: {map_scale:>7} miles per inch")
distances = data.read().split("\n")
distances = distances[:-1]
distances = list(map(float, distances))
for dist in distances:
    dist = math.ceil((dist) * 10) / 10
print("      Map       Mileage")
print("      Measure   Distance")
print("============================================================")
total_dist = float(0)
for i in range(1, dist_count + 1):
    real_dist = distances[i - 1]
    real_dist = math.ceil((real_dist * map_scale) * 10) / 10
    total_dist += real_dist
    print(f"# {i:<4} {distances[i - 1]:<9} {real_dist}")
print("============================================================")
print(f"Total Distance: {total_dist:>6} miles")
data.close()