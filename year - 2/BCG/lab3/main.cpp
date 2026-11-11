#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <chrono>
#include <cmath>
#include <fstream>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <print>
#include <string>
#include <unistd.h>

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  glm::vec2 winSize(800.0, 800.0);
  SDL_Window *window;
  SDL_Renderer *renderer;
  //
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

  SDL_CreateWindowAndRenderer("window", winSize.x, winSize.y, SDL_WINDOW_OPENGL,
                              &window, &renderer);

  SDL_GetWindowSurface(window);
  SDL_UpdateWindowSurface(window);

  // SDL_GLContext glCont = SDL_GL_CreateContext(window);
  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
  glViewport(0, 0, winSize.x, winSize.y);

  GLint success = 1;
  const char *vertexShaderSource = R"(
    #version 460
    layout (location = 0) in vec3 aPos;

    void main(){
      gl_Position = vec4(aPos.xyz, 1.0);
    }
  )";
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    std::println("Vertex shader compilation error");
    char errLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, errLog);
    std::println("{}", errLog);
    return 0;
  }

  const char *fragmentShaderSource = R"(
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
        vec3 closePoint;
        if(length(lineStart - sidePoint) < length(lineStart - basePoint)){
          closePoint = sidePoint;
        }
        else {
          closePoint = basePoint;
        }
        closePoint = changeCoordSystem(closePoint, conePosition, coneSpaceMatrix());
        float red = pow(max(5.0 - length(closePoint - changeCoordSystem(lineStart, conePosition, coneSpaceMatrix())), 0.0) / 5.0, 2);
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
)";
  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    std::println("Fragment shader compilation error");
    char errLog[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, errLog);
    std::println("{}", errLog);
    return 0;
  }

  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glm::vec3 vertices[] = {
      glm::vec3(-1.0, 1.0, 0.0),
      glm::vec3(1.0, 1.0, 0.0),
      glm::vec3(1.0, -1.0, 0.0),
      glm::vec3(-1.0, -1.0, 0.0),
  };

  GLuint indices[] = {
      0, 1, 2, 2, 3, 0,
  };

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  const uint64_t delta_time = ceil(1.0 / 60.0 * 1000000.0);
  SDL_Event ev;
  bool isClosed = false;
  while (!isClosed) {
    uint64_t start_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    glClearColor(0.2f, 0.2f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    GLuint winSizeLocation = glGetUniformLocation(shaderProgram, "winSize");
    glUniform2f(winSizeLocation, winSize.x, winSize.y);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    SDL_GL_SwapWindow(window);
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EventType::SDL_EVENT_QUIT) {
        isClosed = true;
      }
    }
    uint64_t end_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    // std::println("{:.3}",
    //              static_cast<double>(end_time - start_time) / 1000000.0);
    if (delta_time > (end_time - start_time))
      usleep(delta_time - (end_time - start_time));
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  // SDL_GL_DestroyContext(glCont);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
