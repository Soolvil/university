import sys
import math

cur_test = int(sys.argv[1])
inmap_path = "./inmap" + str(cur_test) + ".dat" 
inmap = open(inmap_path, "r")
print("Pupkevich Dmitry")
print("Simple Map Distance Computations")
print("")
dist_count, map_scale = map(float, inmap.readline().split())
dist_count = int(dist_count) 
map_scale = math.ceil((map_scale) * 100) / 100
print(f"Map Scale Factor: {map_scale:>7} miles per inch")
distances = inmap.read().split("\n")
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