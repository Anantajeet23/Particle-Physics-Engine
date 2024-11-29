#include "lancelot/world.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

// Function to generate a random float between min and max
float getRandomReal(lancelot::real min, lancelot::real max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    float top = nearPlane * tan(fov * M_PI / 360.0);
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -right;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top, nearPlane, farPlane);
}

class Ball : public lancelot::Particle {
public:
    Ball(lancelot::real mass, lancelot::real damp, lancelot::real radius, lancelot::Vector3d pos) {
        this->setMass(mass);
        this->setRadius(radius);
        this->setPosition(pos);
        this->setVelocity(lancelot::Vector3d(getRandomReal(-2.0, 2.0), getRandomReal(-2.0, 2.0), getRandomReal(-2.0, 2.0)));
        this->setAcceleration(lancelot::Vector3d(0, 0, 0));
        this->setDamping(damp);
    }

    void initGraphics() {
        glEnable(GL_DEPTH_TEST);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        setPerspective(45.0, 1.0, 1.0, 500.0); 
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_POINT_SMOOTH);
        glPointSize(2 * this->getRadius()); 
    }

    void createSpinningParticles(lancelot::World* world, int numBalls, const lancelot::Vector3d& center, double boxSize) {
        for (int i = 0; i < numBalls; i++) {
            lancelot::Vector3d pos(
                center.x + getRandomReal(-boxSize / 2, boxSize / 2),
                center.y + getRandomReal(-boxSize / 2, boxSize / 2),
                center.z + getRandomReal(-boxSize / 2, boxSize / 2)
            );

            Ball* ball = new Ball(getRandomReal(1.0, 5.0), 0.99, getRandomReal(1.0, 2.0), pos);
            world->ParticleRegistry.push_back(ball);
        }
    }
};

int main() {
    // Create a world instance
    lancelot::World world(15000, 700);

    // Initialize ball object to create spinning particles
    Ball baseBall(1.0, 0.95, 1, lancelot::Vector3d(0, 0, 0));
    baseBall.createSpinningParticles(&world, 1000, lancelot::Vector3d(0, 0, 0), 50.0);

    // Add cube wall collision generator
    world.Detector.GenerateWallContacts();

    sf::RenderWindow window(sf::VideoMode(1200, 1200), "Spinning Cube");
    window.setFramerateLimit(60);
    sf::Clock clock;
    clock.restart();

    int count = 1;
    float cameraRotationSpeed = 0.8f; // Faster rotation

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        baseBall.initGraphics();

        sf::Time duration = clock.getElapsedTime();

        // Clear OpenGL buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Enhanced camera positioning for better visualization
        glTranslatef(0.0f, -30.0f, -300.0f);
        glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(count * cameraRotationSpeed, 0.0f, 1.0f, 0.0f);

        // Start simulation steps
        world.startFrame();
        world.simulatePhysics(duration.asSeconds());

        std::vector<lancelot::Particle*>::iterator i = world.ParticleRegistry.begin();
        for (; i != world.ParticleRegistry.end(); i++) {
            lancelot::Vector3d pos = (*i)->getPosition();

            // Draw the ball as a point
            glBegin(GL_POINTS);
            glColor3f(0.0f, 0.5f, 1.0f);
            glVertex3f(pos.x, pos.y, pos.z);
            glEnd();
        }

        window.display();
        count++;
    }

    return 0;
}