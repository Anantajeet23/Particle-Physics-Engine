#include "lancelot/particle.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

// Function to generate a random float between min and max
float getRandomReal(lancelot::real min, lancelot::real max) {
    // Generate a random float between 0.0 and 1.0, then scale to [min, max]
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int getRandomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

lancelot::Vector3d getRandomVector(const lancelot::Vector3d &min, const lancelot::Vector3d &max) {
    return lancelot::Vector3d(getRandomReal(min.x,max.x),getRandomReal(min.y,max.y),getRandomReal(min.z,max.z));

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

lancelot::Vector3d minvel(-2, -1, -2);
lancelot::Vector3d maxvel(2, 1, 2);


class Myfireworks : public lancelot::Particle {
    private:
    protected:
    public:

    Myfireworks createParent(const lancelot::real x,const lancelot::real y, const lancelot::real z){
        Myfireworks parent;
        parent.age = 0.5;
        parent.setMass(1);
        parent.setRadius(5.0);
        lancelot::Vector3d pos(x,y,z);
        parent.setPosition(pos);
        lancelot::Vector3d vel(1,8,3);
        parent.setVelocity(vel);
        lancelot::Vector3d acc(0,-0.002,0);
        parent.setAcceleration(acc);
        parent.setDamping(0.95);
        return parent;
    }


    
    

    lancelot::real age;


    int burst(Myfireworks* fireworks, int* counts, int arrnum){
        int num = getRandomInt(1,8);
        if ((counts[arrnum] + num >= 500)){
            return 1;
        }
        for (int i = 0; i < num; i++)
        {
            Myfireworks temp;
            temp.age = getRandomReal(100,300);
            temp.setMass(1);
            temp.setRadius(5.0);
            lancelot::Vector3d pos = this->getPosition(); 
            temp.setPosition(pos);
            temp.setVelocity(getRandomVector(minvel,maxvel));
            lancelot::Vector3d acc(0,-0.002,0);
            temp.setAcceleration(acc);
            temp.setDamping(0.95);

            fireworks[counts[arrnum] + i] = temp;

        }
        return counts[arrnum] + num;
    }

    /**
     * Updates the firework by the given duration of time. Returns true
     * if the firework has reached the end of its life and needs to be
     * removed.
     */
    int update(lancelot::real duration,Myfireworks* fireworks, int* counts, int arrnum)
    {
        // Update our physical state
        Particle::integrate(duration);

        // We work backwards from our age to zero.
        age -= duration;
        if(age <= 0){
            return burst(fireworks,counts,arrnum);
        }
        else{
            return counts[arrnum];
        }
    }

    void initGraphics() {
    // Set up OpenGL settings for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    setPerspective(45.0, 1.0, 1.0, 500.0); // Simple perspective projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_POINT_SMOOTH);
    glPointSize(this->getRadius()); // Set point size for particles
    }

    void display(sf::RenderWindow& window) {
    // Clear OpenGL buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Translate to a better position for viewing the fireworks
    glTranslatef(0.0f, 0.0f, -200.0f);

    // Draw the firework particle as a point
            glBegin(GL_POINTS);
            glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for firework
            lancelot::Vector3d position = this->getPosition();
            glVertex3f(position.x,position.y,position.z);
            glEnd();

    window.display();
    }


};

int main(){

    Myfireworks base;
    base.setRadius(3);

    int counts[5] = {};
    int* firstcount = &counts[0];

    Myfireworks fireworks[10][500] = {};
    

    for (int i = 0; i < 10; i++)
    {
        if(i < 5){
        fireworks[i][0] = base.createParent(-5*i,-50,0);
        }
        else{
            fireworks[i][0] = base.createParent(5*i,-50,0);
        }
        counts[i] += 1;

    }


    sf::RenderWindow window(sf::VideoMode(1200, 1200), "Lancelot Firework Demo");
    window.setFramerateLimit(40);
    sf::Clock clock;
    clock.restart();


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        base.initGraphics();
        //parent.display(window);

        sf::Time duration = clock.getElapsedTime();

        // Clear OpenGL buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
    
        // Translate to a better position for viewing the fireworks
        glTranslatef(0.0f, 0.0f, -200.0f);

        for (int j = 0; j < 10; j++){
            Myfireworks* first = &fireworks[j][0];
            for (int i = 0; i < 500; i++){
                if(counts[j] > 0){
                    Myfireworks* curr = &first[i];
                    if (curr->age != 0){
                        counts[j] = curr->update(duration.asSeconds(),first,firstcount,j);

                        lancelot::Vector3d pos = curr->getPosition();
                         // Draw the firework particle as a point
                        if(pos.y > -25){
                        glBegin(GL_POINTS);

                        int random = getRandomInt(1,100);

                        if(random > 75){ // randomizies the color
                            glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for firework
                        }
                        else if (random > 50)
                        {
                            glColor3f(1.0f, 0.0f, 0.0f);
                        }
                        else if (random > 25)
                        {
                            glColor3f(0.0f, 1.0f, 0.0f);
                        }
                        else if (random > 0)
                        {
                            glColor3f(0.0f, 0.0f, 1.0f);
                        }
                        glVertex3f(pos.x,pos.y,pos.z);
                        glEnd();
                        }

                    }
                }
            } 
        }
        window.display();
    }

    return 0;
}

