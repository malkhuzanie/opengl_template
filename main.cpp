#include <GLFW/glfw3.h>
#include <cmath>
#include <ctime>

// Global variables to track window size
int windowWidth = 640;
int windowHeight = 480;

void reshape(GLFWwindow *window, int width, int height) {
  // Store new window dimensions
  windowWidth = width;
  windowHeight = height;

  // Set the viewport to cover the entire window
  glViewport(0, 0, width, height);

  // Reset projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Calculate aspect ratio
  float aspectRatio = (float)width / (float)height;

  // Use glOrtho with aspect ratio correction
  if (width >= height) {
    // Landscape or square window
    glOrtho(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0, -1.0, 1.0);
  } else {
    // Portrait window
    glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, -1.0, 1.0);
  }

  glMatrixMode(GL_MODELVIEW);
}

void drawCircle(float x, float y, float radius, int segments, bool filled) {
  if (filled) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= segments; i++) {
      float theta = i * 2.0f * M_PI / segments;
      glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
    }
    glEnd();
  } else {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
      float theta = i * 2.0f * M_PI / segments;
      glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
    }
    glEnd();
  }
}

void drawClockFace() {
  // Soft cream background
  glColor3f(0.96f, 0.95f, 0.88f);
  drawCircle(0, 0, 0.9f, 100, true);

  // Dark border
  glColor3f(0.2f, 0.2f, 0.3f);
  glLineWidth(5.0f);
  drawCircle(0, 0, 0.9f, 100, false);

  // Minute markers
  for (int i = 0; i < 60; i++) {
    float angle = i * 6.0f * M_PI / 180.0f - M_PI / 2;
    float start_radius = (i % 5 == 0) ? 0.8f : 0.85f;
    float end_radius = 0.9f;

    glLineWidth(i % 5 == 0 ? 3.0f : 1.0f);
    glColor3f(0.2f, 0.2f, 0.3f);

    glBegin(GL_LINES);
    glVertex2f(start_radius * cos(angle), start_radius * sin(angle));
    glVertex2f(end_radius * cos(angle), end_radius * sin(angle));
    glEnd();
  }
}

void drawClockHands() {
  // Get current time
  time_t now = time(nullptr);
  struct tm *timeinfo = localtime(&now);

  // Hour hand
  float hourAngle = (timeinfo->tm_hour % 12 + timeinfo->tm_min / 60.0f) *
                        30.0f * M_PI / 180.0f -
                    M_PI / 2;
  glColor3f(0.1f, 0.1f, 0.3f);
  glLineWidth(7.0f);
  glBegin(GL_LINES);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.5f * cos(hourAngle), 0.5f * sin(hourAngle));
  glEnd();

  // Minute hand
  float minuteAngle =
      (timeinfo->tm_min + timeinfo->tm_sec / 60.0f) * 6.0f * M_PI / 180.0f -
      M_PI / 2;
  glColor3f(0.3f, 0.3f, 0.5f);
  glLineWidth(5.0f);
  glBegin(GL_LINES);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.7f * cos(minuteAngle), 0.7f * sin(minuteAngle));
  glEnd();

  // Second hand
  float secondAngle = (timeinfo->tm_sec * 6.0f) * M_PI / 180.0f + M_PI / 2;
  glColor3f(0.8f, 0.2f, 0.2f);
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.75f * cos(secondAngle), 0.75f * sin(secondAngle));
  glEnd();

  // Center dot
  glColor3f(0.1f, 0.1f, 0.3f);
  drawCircle(0, 0, 0.02f, 20, true);
}

int main(void) {
  GLFWwindow *window;
  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Elegant OpenGL Clock", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glClearColor(0.7f, 0.7f, 0.8f, 0.0f);

  /* Set reshape callback to handle window resizing */
  glfwSetWindowSizeCallback(window, reshape);

  /* Initial projection setup */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    // Reset modelview matrix
    glLoadIdentity();

    // Draw clock
    drawClockFace();
    drawClockHands();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
