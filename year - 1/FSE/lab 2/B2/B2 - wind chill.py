import sys

data_path = "./" + sys.argv[1]
data = open(data_path, "r")
buff = data.readline()
buff = data.readline()
buff = data.readlines()
data_count = len(buff)
times = [str] * data_count
temperatures = [float] * data_count
wind_speed = [float] * data_count
for i in range(0, data_count):
    temp = (buff[i]).split()
    times[i] = (temp[0])
    temperatures[i] = (temp[1])
    wind_speed[i] = (temp[2])
wind_speed = list(map(float, wind_speed))
temperatures = list(map(float, temperatures))
wc_temperatures = [float] * data_count
wc_effect = [float] * data_count
average_temp = 0
print("Time     WC temp     WC Effect")
print("------------------------------")
for i in range(0, data_count):
    wc_temperatures[i] = round((35.74 + 0.6125 * temperatures[i] + (0.4275 * temperatures[i] - 35.75) * (pow(wind_speed[i], 0.16))), 1)
    wc_effect = round(wc_temperatures[i] - temperatures[i], 1)
    average_temp += wc_temperatures[i]
    print(f"{times[i]} {wc_temperatures[i]:>7}     {wc_effect:>9}")
print("------------------------------")
print("")
average_temp = round(average_temp / data_count, 1) 
print(f"The average adjusted temperature, based on {data_count} observations, was {average_temp}")
data.close()