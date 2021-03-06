#include <gl/glut.h>
#include "Display.h"
#include "Vector.h"
#include "Camera.h"
#include "Object.h"
#include "Input.h"


Camera camera(65, 0.1, 100, {0, 0, 0.3});

Object player
({
    {-1, -1, 1},
    {-1, 1, 1},
    {1, 1, 1},
    {1, -1, 1},
    {-1, -1, -1},
    {-1, 1, -1},
    {1, 1, -1},
    {1, -1, -1},
},{
    {0,1,2,3},
    {3,2,6,7},
    {4,5,6,7},
    {0,1,5,4},
    {1,5,6,2},
    {0,4,7,3}
},
    {0.7, 0, 0.3}
);

Object plane
({
    {-5, 0, -5},
    {-5, 0, 5},
    {5, 0, 5},
    {5, 0, -5},
},
    {{0,1,2,3}}
);

Vector target;


double cameraDistance = 10;
double targetSpeed = 20;

double maxSpeed = 10;
double rollSpeed = 30;
double deceleration = 40;


void Initialize()
{
    player.position = Vector(0, 10, 0);
}


double speed = maxSpeed;
Vector velocity;
bool targetLock = false;

void Update(int ms)
{
    double dt = (double) ms / 1000;
    
    Vector up = Vector(0, 1, 0);
    Vector right = (camera.direction * up).Normalize();
    Vector forward = (up * right).Normalize();
    
    if (Input::roll)
        speed = rollSpeed;
    if (speed > maxSpeed)
        speed -= deceleration * dt;

    velocity = speed * (Input::move.x * right + Input::move.z * forward);

    if (velocity.Length() != 0)
        player.direction = velocity;

    if (player.position.y > 1)
        velocity.y = -20;
    else
        player.position.y = 1;

    player.position += velocity * dt;
    
    if (Input::target)
        targetLock = !targetLock;

    if (!targetLock)
        camera.direction += Input::camera.x * right + Input::camera.y * up;
    else
        camera.direction.Damp((target - player.position).Normalize(), targetSpeed, dt);
    camera.direction.Normalize();
    camera.position = player.position - cameraDistance * camera.direction;
    
    glutPostRedisplay();

    ms = 1000 / Display::fps;
    glutTimerFunc(ms, Update, ms);
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    Display::CreateWindow("P1RV", 800, 800);
    Input::BeginInput();
    Display::BeginDisplay(Initialize, Update);
}