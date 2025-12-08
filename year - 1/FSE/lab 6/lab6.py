import xml.etree.ElementTree as ET
import matplotlib.pyplot as plt

class user_t:
    def __init__(self, name = "", age = 0, weight = 0, level = ""):
        self.name = name
        self.age = int(age)
        self.weight = int(weight)
        self.level = level
        
class workout_t:
    def __init__(self, user_id = 0, date = "", type = "", duration = 0, distance = 0.0, calories = 0, avg_heart = 0, intensity = ""):
        self.user = int(user_id)
        self.date = date
        self.type = type
        self.duration = int(duration)
        self.distance = float(distance)
        self.calories = int(calories)
        self.avg_heart = int(avg_heart)
        self.intensity = intensity

def load_users_data():
    try:
        user_tree = ET.parse("users.xml")
        users = {}
        for user in user_tree.getroot().findall("user"):
            users[int(user.find("user_id").text)] = (
                user_t(user.find("name").text,
                        user.find("age").text,
                        user.find("weight").text,
                        user.find("fitness_level").text))
        return users
    except FileNotFoundError:
        print("Can't load users data - file not found")
        return {}

def load_workouts_data():
    try:
        workout_tree = ET.parse("workouts.xml")
        workouts = {}
        for workout in workout_tree.getroot().findall("workout"):
            workouts[int(workout.find("workout_id").text)] = (
                workout_t(workout.find("user_id").text,
                          workout.find("date").text,
                          workout.find("type").text,
                          workout.find("duration").text,
                          workout.find("distance").text,
                          workout.find("calories").text,
                          workout.find("avg_heart_rate").text,
                          workout.find("intensity").text))
        return workouts
    except FileNotFoundError:
        print("Can't load workout data - file not found")
        return {}
# 21 / 25
def get_stats(user_data, workout_data):
    workout_count = len(workout_data)
    user_count = len(user_data)
    total_calories = sum(workout.calories for workout in workout_data)
    total_duration = sum(workout.duration for workout in workout_data) / 60
    total_distance = sum(workout.distance for workout in workout_data)
    print("ОБЩАЯ СТАТИСТИКА")
    print("===========================")
    print(f"Всего тренировок: {workout_count}")
    print(f"Всего пользователей: {user_count}")
    print(f"Сожжено калорий: {total_calories}")
    print(f"Общее время: {total_duration}")
    print(f"Пройденная дистанция: {total_distance}")

def analyze_user_activity(user_data, workout_data):
    user_workout_count = {}
    user_calories = {}
    user_time = {}
    for id, _ in user_data.items():
        user_workout_count[int(id)] = int(0)
        user_calories[int(id)] = int(0)
        user_time[int(id)] = int(0)
    for _, workout in workout_data.items():
        user_workout_count[workout.user] += 1
        user_calories[workout.user] += workout.calories
        user_time[workout.user] += workout.duration
    print(user_workout_count)
    best_users = [id for _, id in sorted([(count, id) for id, count in user_workout_count.items()], reverse=True)[:3]]
    print("ТОП-3 АКТИВНЫХ ПОЛЬЗОВАТЕЛЕЙ:")
    for i in range(0, len(best_users)):
        print(f"{i + 1}. {user_data[best_users[i]].name} ({user_data[best_users[i]].level}):")
        print(f" Тренировок: {user_workout_count[best_users[i]]}")
        print(f" Калорий: {user_calories[best_users[i]]}")
        print(f" Время: {round(user_time[best_users[i]] / 60, 1)} часов")
        print()

def analyze_workout_types(workout_data):
    workout_count = {}
    workout_avg_calories = {}
    workout_avg_duration = {}
    for _, workout in workout_data.items():
        workout_count[workout.type] = 0
        workout_avg_calories[workout.type] = 0
        workout_avg_duration[workout.type] = 0
    for _, workout in workout_data.items():
        workout_count[workout.type] += 1
        workout_avg_calories[workout.type] += workout.calories
        workout_avg_duration[workout.type] += workout.duration
    for type in workout_count:
        workout_avg_calories[type] = int(round(workout_avg_calories[type] / workout_count[type], 0)) 
        workout_avg_duration[type] = int(round(workout_avg_duration[type] / workout_count[type], 0)) 
    print("РАСПРЕДЕЛЕНИЕ ПО ТИПАМ ТРЕНИРОВОК:")
    for type in workout_count:
        print(f" {type[0].upper() + type[1:]}: {workout_count[type]} тренировок ({round((workout_count[type] / len(workout_data)) * 100, 1)}%)")
        print(f"  Средняя длительность: {workout_avg_duration[type]} мин")
        print(f"  Средние калории: {workout_avg_calories[type]} ккал")

def find_user_name(user_data, username):
    for id, user in user_data.items():
        if user.name == username:
            return id
    return 0

def find_user_workouts(workout_data, user_id):
    workouts = []
    for id, workout in workout_data.items():
        if workout.user == user_id:
            workouts.append(workout)
    return workouts

def analyze_user_workout(user_data, workout_data, user_id):
    workouts = find_user_workouts(workout_data, user_id)
    calories = sum([workout.calories for workout in workouts])
    time = round(sum([workout.duration for workout in workouts]) / 60, 1)
    distance = sum([workout.distance for workout in workouts])
    avg_calories = calories / len(workouts)
    type_count = {}
    for workout in workouts:
        if not workout in type_count:
            type_count[workout.type] = 0
        type_count[workout.type] += 1
    favourite = workouts[0].type
    for type, count in type_count.items():
        if type_count[favourite] < count:
            favourite = type
    print(f"ДЕТАЛЬНЫЙ АНАЛИЗ ДЛЯ ПОЛЬЗОВАТЕЛЯ: {user_data[user_id].name}")
    print("===========================================")
    print(f"Возраст: {user_data[user_id].age} лет, Вес: {user_data[user_id].weight} кг")
    print(f"Уровень: {user_data[user_id].level}")
    print(f"Тренировок: {len(workouts)}")
    print(f"Сожжено калорий: {calories}")
    print(f"Общее время: {time}")
    print(f"Пройдено дистанции: {distance}")
    print(f"Средние калории за тренировку:  {avg_calories}")
    print(f"Любимый тип тренировок: {favourite}")

def workouts_types_pie(workout_data): # Дописать
    type_count = {}
    for _, workout in workout_data.items():
        if not workout.type in type_count:
            type_count[workout.type] = 0
        type_count[workout.type] += 1
    types = [type for type, _ in type_count.items()]
    counts = [count for _, count in type_count.items()]
    plt.pie(counts, labels=types, startangle=90)
    plt.show()

def user_activity_histogram(user_data, workout_data): # Дописать
    workout_count = {}
    for _, workout in workout_data.items():
        if not user_data[workout.user].name in workout_count:
            workout_count[user_data[workout.user].name] = 0
        workout_count[user_data[workout.user].name] += 1
    usernames = [name for _, name in sorted([(j, i) for i, j in workout_count.items()], reverse = True)]
    counts = [count for count, _ in sorted([(j, i) for i, j in workout_count.items()], reverse = True)]
    plt.bar(range(0, len(usernames)),height=counts, tick_label=usernames)
    plt.show()

def workout_efficiency_histogram(workout_data): 
    type_calories = {}
    for _, workout in workout_data.items():
        if not workout.type in type_calories:
            type_calories[workout.type] = 0
        type_calories[workout.type] += workout.calories
    type_duration = {}
    for _, workout in workout_data.items():
        if not workout.type in type_duration:
            type_duration[workout.type] = 0
        type_duration[workout.type] += workout.duration
    for type, calories in type_calories.items():
        type_calories[type] = float((calories / type_duration[type]))
    types = [type for _, type in sorted([(j, i) for i, j in type_calories.items()], reverse=True)]
    calories = [calories for calories, _ in sorted([(j, i) for i, j in type_calories.items()], reverse=True)]
    plt.bar(range(0, len(types)), height=calories, tick_label=types, color='m')
    plt.show()

def users_comparative_diagram(user_data, workout_data):
    user_calories = {}
    for id, user in user_data.items():
        user_calories[user.name] = sum(workout.calories for workout in find_user_workouts(workout_data, id))
    usernames = [name for _, name in sorted([(j, i) for i, j in user_calories.items()], reverse = True)]
    calories = [calories for calories, _ in sorted([(j, i) for i, j in user_calories.items()], reverse = True)]
    colors = []
    for name in usernames:
        if user_data[find_user_name(user_data, name)].level == "начальный":
            colors.append('g')
        elif user_data[find_user_name(user_data, name)].level == "средний":
            colors.append('y')
        elif user_data[find_user_name(user_data, name)].level == "продвинутый":
            colors.append('r')
    print(colors)
    plt.bar(range(0, len(usernames)),height=calories, tick_label=usernames, color=colors)
    plt.show()


user_data = load_users_data()
workout_data = load_workouts_data()
analyze_user_activity(user_data, workout_data)
analyze_workout_types(workout_data)
workouts_types_pie(workout_data)
workout_efficiency_histogram(workout_data)
users_comparative_diagram(user_data, workout_data)