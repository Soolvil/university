import sys

def coords_normalise(y, x):
    if(y <= 0): 
        y += height
    if(x <= 0):
        x += width
    if(y > height):
        y -= height
    if(x > width):
        x -= width
    return (y, x)

data_path = sys.argv[1]
data = open(data_path, "r")
height, width = map(int, data.readline().split())
(cat_y, cat_x) = (int(0), int(0))
(mouse_y, mouse_x) = (int(0), int(0))
(cat_pos_known, mouse_pos_known) = (False, False)
(cat_travelled, mouse_travelled) = (int(0), int(0))
distance = int(0)
mouse_caught = False
(caught_y, caught_x) = (int(0), int(0))
print("Cat and Mouse")
print()
print("  Cat        Mouse    Distance")
print("------------------------------")
for request in data.readlines():
    if(request[0] == 'P'):
        print(f"({cat_y if cat_pos_known else '?':>2},{cat_x if cat_pos_known else '?':>2})", end = '' )
        print("     ", end = '')
        print(f"({mouse_y if mouse_pos_known else '?':>2},{mouse_x if mouse_pos_known else '?':>2})", end = '' )
        if(mouse_pos_known and cat_pos_known):
            print("    ", end = '')
            print(f"{distance:>4}", end = '')
        print()
    elif (request[0] == 'M'):
        (request_y, request_x) = map(int, request[1:].split())
        if(not mouse_pos_known):
            (mouse_y, mouse_x) = (request_y, request_x)
            mouse_pos_known = True
        else:
            mouse_y += request_y
            mouse_x += request_x
            (mouse_y, mouse_x) = coords_normalise(mouse_y, mouse_x)
            mouse_travelled += abs(request_x) + abs(request_y)
    elif (request[0] == 'C'):
        (request_y, request_x) = map(int, request[1:].split())
        if(not cat_pos_known):
            (cat_y, cat_x) = (request_y, request_x)
            cat_pos_known = True
        else:
            cat_x += request_x
            cat_y += request_y
            (cat_y, cat_x) = coords_normalise(cat_y, cat_x)
            cat_travelled += abs(request_x) + abs(request_y)
    if(cat_pos_known and mouse_pos_known):
        distance = abs(cat_y - mouse_y) + abs(cat_x - mouse_x)
        if((cat_y, cat_x) == (mouse_y, mouse_x)):
            mouse_caught = True
            (caught_y, caught_x) = (cat_y, cat_x)
            break
print("------------------------------")
print()
print("Distance   Mouse    Cat")
print(f"             {mouse_travelled:>3}    {cat_travelled:>3}")
print()
if(not mouse_caught):
    print("Mouse evaded Cat")
else:
    print(f"Mouse caught at: ({caught_y:>2},{caught_x:>2})")

