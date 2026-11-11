#version 460 core
out vec4 FragColor;
uniform vec2 winSize;

const vec3 coneAxis = vec3(2.0, -2.0, 0.0);
const vec3 coneRadius = vec3(0.0, 0.0, 1.0);
const vec3 conePosition = vec3(0.0, 1.0, 0.0);
const vec2 camSize = vec2(5.0, 5.0);
const vec3 camUp = vec3(0.0, 1.0, 0.0);
const vec3 camRight = vec3(0.0, 0.0, 1.0);
const vec3 camPos = vec3(-1.0, -2.5, -2.5);

mat3 coneSpaceMatrix(){
    vec3 pRadius = normalize(cross(coneAxis, coneRadius)) * length(coneRadius);
    return mat3(coneRadius, pRadius, coneAxis);
}

vec3 changeBase(in vec3 vector, in mat3 base){
    return inverse(base) * vector;
}

vec3 changeCoordSystem(in vec3 point, in vec3 startPos, in mat3 base){
    return changeBase(point - startPos, base);
}

bool coneSideIntersection(in vec3 lineStart, in vec3 lineDirection, inout vec3 intersection){
    float a = lineDirection.x * lineDirection.x + lineDirection.y * lineDirection.y - lineDirection.z * lineDirection.z;
    float b = 2.0 * (lineDirection.x * lineStart.x + lineDirection.y * lineStart.y - lineDirection.z * lineStart.z);
    float c = lineStart.x * lineStart.x + lineStart.y * lineStart.y - lineStart.z * lineStart.z;
    float d = b * b - 4.0 * a * c;
    if(d < 0.0)return false;
    if(d == 0.0){
        float t = -b / (a * 2.0);
        intersection = lineStart + lineDirection * t;
        return t >= 0.0 && intersection.z >= 0.0 && intersection.z <= 1.0;
    }
    d = sqrt(d);
    float t1 = (-b + d) / (a * 2.0);
    float t2 = (-b - d) / (a * 2.0);
    if(t1 < 0.0 && t2 < 0.0)return false;
    if(t1 >= 0.0 && t2 < 0.0){
        intersection = lineStart + lineDirection * t1;
        return intersection.z >= 0.0 && intersection.z <= 1.0;
    }
    if(t1 < 0.0 && t2 >= 0.0){
        intersection = lineStart + lineDirection * t2;
        return intersection.z >= 0.0 && intersection.z <= 1.0;
    }
    intersection = lineStart + lineDirection * min(t1, t2);
    return intersection.z >= 0.0 && intersection.z <= 1.0;
}

bool liesOnDisk(in vec2 checkPoint, in vec2 diskCenter, in float radius){
    float distance = length(checkPoint - diskCenter);
    return distance <= radius;
}

bool coneBaseIntersection(in vec3 lineStart, in vec3 lineDirection, inout vec3 intersection){
    float angle = lineDirection.z;
    if(angle == 0.0)return false;
    float t = lineStart.z / lineDirection.z;
    intersection = lineStart + lineDirection * t;
    return liesOnDisk(intersection.xy, vec2(0.0, 0.0), 1.0);
}

vec4 coneRaycast(in vec3 lineStart, in vec3 lineDirection){
    vec3 sidePoint = vec3(0.0, 0.0, 0.0);
    vec3 basePoint = vec3(0.0, 0.0, 0.0);
    bool baseIntersection = coneBaseIntersection(lineStart, lineDirection, basePoint);
    bool sideIntersection = coneSideIntersection(lineStart, lineDirection, sidePoint);
    if(!sideIntersection)return vec4(0.0, 0.0, 0.0, 0.0);
    float red = pow(max(5.0 - min(length(sidePoint - lineStart), length(basePoint - lineStart)), 0.0) / 5.0, 1.4);
    float blue = 0.0;
    float green = 0.0;
    float alpha = 1.0;
    return vec4(red, blue, green, alpha);
}

void main()
{
    const vec3 xPixelDelta = (camSize.x / winSize.x) * camRight;
    const vec3 yPixelDelta = (camSize.y / winSize.y) * camUp;
    const vec3 pixelCornerPos = camPos + gl_FragCoord.x * xPixelDelta + gl_FragCoord.y * yPixelDelta;
    vec3 pixelPos = pixelCornerPos + xPixelDelta + yPixelDelta;
    vec3 lookDirection = normalize(cross(camUp, camRight));
    pixelPos = changeCoordSystem(pixelPos, conePosition, coneSpaceMatrix());
    lookDirection = normalize(changeBase(lookDirection, coneSpaceMatrix()));
    FragColor = coneRaycast(pixelPos, lookDirection);
}  
