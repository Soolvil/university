#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <type_traits>

constexpr uint64_t factorial(const uint64_t n) {
  uint64_t res = 1;
  for (uint64_t i = 2; i <= n; i++) res *= i;
  return res;
}

constexpr uint64_t binomial_coeff(const uint64_t n, const uint64_t m) {
  return factorial(n) / (factorial(n - m) * factorial(m));
}

template <typename T>
constexpr T binpow(const T num, uint64_t power) {
  T res = 1;
  T mult = num;
  for (uint64_t i = 0; i < 64 && (1 << i) <= power; ++i) {
    if ((1 << i) & power) res *= mult;
    mult *= mult;
  }
  return res;
}

template <typename pointT>
constexpr pointT Bezier(const double t, pointT point) {
  return point;
}

template <typename pointT, uint64_t size>
constexpr pointT Bezier(const double t,
                        const std::array<pointT, size>& points) {
  pointT res = pointT();
  for (uint64_t i = 0; i < points.size(); ++i) {
    res += points[i] * binpow(t, i) * binpow(1 - t, points.size() - i - 1) *
           binomial_coeff(points.size() - 1, i);
  }
  return res;
}

template <typename pointT>
constexpr pointT Bezier4Dt(const double t,
                           const std::array<pointT, 4>& points) {
  return ((-1 + 2 * t - t * t) * points[0] +
          (1 - 4 * t + 3 * t * t) * points[1] +
          (2 * t - 3 * t * t) * points[2] + t * t * points[3]) *
         3;
}

template <typename pointT>
constexpr pointT Bezier4Dt2(const double t,
                            const std::array<pointT, 4>& points) {
  return 3 * ((2 - 2 * t) * points[0] + (-4 + 6 * t) * points[1] +
              (2 - 6 * t) * points[2] + 2 * t * points[3]);
}

template <typename pointT, uint64_t size>
constexpr std::array<pointT, (size - 1) * 2> getControlPoints(
    const std::array<pointT, size>& points) {
  const uint64_t eq_count = (size - 1) * 2;
  Eigen::Matrix<double, eq_count, eq_count> A(
      Eigen::Matrix<double, eq_count, eq_count>::Constant(0));
  Eigen::Matrix<double, eq_count, pointT().size()> B(
      Eigen::Matrix<double, eq_count, pointT().size()>::Constant(0));
  A(0, 0) = -2;
  A(0, 1) = 1;
  B.row(0) = -points[0];
  A(1, eq_count - 2) = 1;
  A(1, eq_count - 1) = -2;
  B.row(1) = -points[size - 1];
  for (uint64_t i = 1; i < size - 1; i++) {
    A(i * 2, (i - 1) * 2 + 1) = 1;
    A(i * 2, i * 2 + 0) = 1;
    B.row(i * 2) = (2 * points[i]);
    A(i * 2 + 1, (i - 1) * 2 + 0) = 1;
    A(i * 2 + 1, (i - 1) * 2 + 1) = -2;
    A(i * 2 + 1, i * 2 + 0) = 2;
    A(i * 2 + 1, i * 2 + 1) = -1;
    B.row(i * 2 + 1) = pointT::Constant(0);
  }
  Eigen::Matrix<double, eq_count, pointT().size()> R;
  R = A.lu().solve(B);
  std::array<pointT, eq_count> res;
  for (uint64_t i = 0; i < eq_count; i++) res[i] = R.row(i);
  return res;
}

template <typename pointT, uint64_t size>
constexpr std::array<std::array<pointT, 4>, size - 1> getBezierSpline(
    const std::array<pointT, size>& points) {
  std::array<pointT, (size - 1) * 2> control = getControlPoints(points);
  std::array<std::array<pointT, 4>, size - 1> spline;
  for (uint64_t i = 0; i < size - 1; i++) {
    spline[i][0] = points[i];
    spline[i][1] = control[i * 2 + 0];
    spline[i][2] = control[i * 2 + 1];
    spline[i][3] = points[i + 1];
  }
  return spline;
}

template <typename pointT, uint64_t size>
pointT calcSpline(const double u,
                  const std::array<std::array<pointT, 4>, size>& spline) {
  return Bezier(u - trunc(u), spline[trunc(u)]);
}

template <typename pointT, uint64_t size, uint64_t rsize>
std::array<pointT, rsize> getDrawingPoints(
    const double h, const std::array<std::array<pointT, 4>, size>& spline) {
  std::array<pointT, rsize> res;
  for (uint64_t i = 0; i < rsize; i++) res[i] = calcSpline(h * i, spline);
  ;
  return res;
}

template <uint64_t size>
void DrawSpline(SDL_Renderer* renderer, const SDL_FRect& area,
                const std::array<std::array<Eigen::Vector2d, 4>, size>& spline,
                uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  if (size == 0) return;
  const uint64_t n = 1000;
  const double scale = 10;
  std::array<Eigen::Vector2d, n> drawPoints =
      getDrawingPoints<Eigen::Vector2d, size, n>(static_cast<double>(size) / n,
                                                 spline);
  for (auto& point : drawPoints) point /= scale;
  SDL_FPoint rectPoints[n];
  for (uint64_t i = 0; i < n; i++) {
    rectPoints[i].x = area.x + static_cast<float>(drawPoints[i](0)) * area.w;
    rectPoints[i].y =
        area.y + static_cast<float>(1.0 - drawPoints[i](1)) * area.h;
  }
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderLines(renderer, rectPoints, n);
}

int main() {
  Eigen::Vector3d A(2, 2, 5);
  Eigen::Vector3d B(6, 9, 7);
  Eigen::Vector3d C(5, 4, 3);
  Eigen::Vector3d D(1, 2, 9);
  std::array<std::array<Eigen::Vector3d, 4>, 3> splineXYZ =
      getBezierSpline(std::array<Eigen::Vector3d, 4>{A, B, C, D});

  std::array<std::array<Eigen::Vector2d, 4>, 3> splineXY;
  std::array<std::array<Eigen::Vector2d, 4>, 3> splineXZ;
  std::array<std::array<Eigen::Vector2d, 4>, 3> splineYZ;

  for (uint64_t i = 0; i < 3; i++) {
    for (uint64_t j = 0; j < 4; j++) {
      splineXY[i][j](0) = splineXYZ[i][j](0);
      splineXY[i][j](1) = splineXYZ[i][j](1);
      splineXZ[i][j](0) = splineXYZ[i][j](0);
      splineXZ[i][j](1) = splineXYZ[i][j](2);
      splineYZ[i][j](0) = splineXYZ[i][j](1);
      splineYZ[i][j](1) = splineXYZ[i][j](2);
    }
  }
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_CreateWindowAndRenderer("window", 800, 800, 0, &window, &renderer);

  SDL_GetWindowSurface(window);
  SDL_UpdateWindowSurface(window);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
  for (uint i = 40; i < 800; i += 40) {
    SDL_RenderLine(renderer, i, 0, i, 800);
    SDL_RenderLine(renderer, 0, i, 800, i);
  }
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderLine(renderer, 0, 400, 800, 400);
  SDL_RenderLine(renderer, 400, 0, 400, 800);
  DrawSpline(renderer, SDL_FRect{0, 0, 400, 400}, splineXY, 255, 0, 0, 255);
  DrawSpline(renderer, SDL_FRect{400, 0, 400, 400}, splineXZ, 0, 255, 0, 255);
  DrawSpline(renderer, SDL_FRect{0, 400, 400, 400}, splineYZ, 0, 0, 255, 255);
  SDL_RenderPresent(renderer);

  SDL_Event ev;
  bool isClosed = false;
  while (!isClosed) {
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EventType::SDL_EVENT_QUIT) {
        isClosed = true;
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
