#include <stdio.h>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Source.h"
#include <vector>
#include <chrono>
#include <stdlib.h> 
#include <time.h> 

using namespace std;

int WIDTH = 1000;
int LENGTH = 1000;

struct Cell {
    int x;
    int y;
    bool alive;
};

std::vector<Cell> cellArray;
std::vector<Cell> nextgenarray;


void init() {
    for (int x = 0; x < 198; x++) {
        for (int y = 0; y < 198; y++) {
            Cell tempcell = { x, y, false };
            cellArray.push_back(tempcell);
        }
    }
    nextgenarray = cellArray;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        for (Cell i : cellArray)
            i.alive = false;
        nextgenarray = cellArray;

        for (int x = 0; x < 12000; x++) {
            cellArray[(rand() % 39200) + (rand() % 6437)].alive = true;
        }
    }

}



int counter = 0;

void drawSquare(double x1, double y1, double sidelength = 1)
{
    x1 = x1 + 0.5;
    y1 = y1 + 0.5;

    double halfside = sidelength / 2;

    glColor3d(255, 255, 255);


    glBegin(GL_POLYGON);

    glVertex2d(x1 + halfside, y1 + halfside);
    glVertex2d(x1 + halfside, y1 - halfside);
    glVertex2d(x1 - halfside, y1 - halfside);
    glVertex2d(x1 - halfside, y1 + halfside);

    glEnd();
}


void gameLogic() {
    for (int i = 0; i <= 39203; i++) {
        int alive_neighbors = 0;
        std::vector<int> neighbors{ i - 199, i - 198, i - 197, i - 1, i + 1, i + 197, i + 198, i + 199 };
        for (auto neighbor : neighbors) {
            if (neighbor >= 0 && neighbor <= 39203) {
                if (cellArray[neighbor].alive) {
                    alive_neighbors++;
                }
            }
        }

        if (alive_neighbors < 2 && cellArray[i].alive == true) {
            nextgenarray[i].alive = false;
        }
        else if (alive_neighbors == 3 || alive_neighbors == 2 && cellArray[i].alive == true) {
            nextgenarray[i].alive = true;
        }
        else if (alive_neighbors > 3 && cellArray[i].alive == true) {
            nextgenarray[i].alive = false;
        }
        else if (alive_neighbors == 3 && cellArray[i].alive == false) {
            nextgenarray[i].alive = true;
        }

    }

    cellArray = nextgenarray;
    for (auto i : nextgenarray) {
        i.alive = false;
    }

}

void draw(GLFWwindow* window) {
    glClearColor(0, 0, 0, 0); // set bg to white
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 198, 0, 198, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (auto i : cellArray) {
        if (i.alive) {
            drawSquare(i.x, i.y);
        }
    }

    // cellArray[(rand() % 39200) + (rand() % 6437)].alive = true;

    glfwSwapBuffers(window);

    glfwPollEvents();
}

int main() {
    glfwInit();
    init();

    srand(time(NULL));

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Pathfinding", NULL, NULL);

    glfwSetWindowSizeLimits(window, 1000, 1000, 1000, 1000);

    //glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    for (int x = 0; x < 12000; x++) {
        cellArray[(rand() % 39200) + (rand() % 6437)].alive = true;
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    while (!glfwWindowShouldClose(window)) {
        gameLogic();
        draw(window);
    }

    glfwTerminate();

}
