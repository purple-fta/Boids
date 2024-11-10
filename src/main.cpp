#include <SFML/Graphics.hpp>

#include "vec.hpp"

#include <vector>
#include <random>
#include <iostream>
#include <list>


class Boid {
    Vec2 direction;
    float speed;

public:
    Vec2 position;
    Boid(float x, float y, int direction) {
        position.x = x;
        position.y = y;
        this->direction.x = cos(direction*M_PI/180);
        this->direction.y = sin(direction*M_PI/180);
    }

    Vec2 getDirection() {
        return direction;
    }

    void setDirection(float x, float y) {
        direction.x = x;
        direction.y = y;
        direction = direction.normalize();
    }

    void addDirection(float x, float y, float k) {
        direction.x += x * k;
        direction.y += y * k;  
        direction = direction.normalize();
    }

    float getDirectionAngle() {
        // std::cout << direction.x << ", " << direction.y << std::endl;
        if (direction.x == 0) return 90 * direction.y;
        else return atan2(direction.y, direction.x) * 180 / M_PI;
    }

    bool operator==(const Boid& otherBoid) { 
        return (position == otherBoid.position && direction == otherBoid.direction); 
    }
    
    bool operator!=(const Boid& otherBoid) {
        return operator==(otherBoid); 
    }
};

class World {
private:
    int width, height;

    float getDistance(const Boid& boid1, const Boid& boid2) {
        return sqrt(pow(boid1.position.x - boid2.position.x, 2) +
                    pow(boid1.position.y - boid2.position.y, 2)); 
    }

public:
    int neighborRadius = 50;

    std::vector<Boid> boids;
    int boidsCount;

    World(int boidsCount, int width, int height) {
        this->boidsCount = boidsCount;
        this->width = width;
        this->height = height;
        
        boids.reserve(boidsCount);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> randX(0, width);
        std::uniform_int_distribution<> randY(0, height);
        std::uniform_int_distribution<> randDirection(0, 360);
        for (int i = 0; i < boidsCount; i++)
        {
            boids.emplace_back(randX(gen), randY(gen), randDirection(gen));
        }
        
    }

    void moveBoids() {
        for (Boid& boid : boids) {
            boid.position += boid.getDirection()*0.2;
            if (boid.position.x > width) boid.position.x = 0;
            if (boid.position.x < 0) boid.position.x = width;
            if (boid.position.y > height) boid.position.y = 0;
            if (boid.position.y < 0) boid.position.y = height;
        }
    }

    void alignment(float neighborRadius) {
        Vec2 avgVelocity;
        Vec2 resVelocity;
        int neighborCount = 0;

        for (Boid& boid : boids)
        {
            avgVelocity.x = 0;
            avgVelocity.y = 0;
            neighborCount = 0;

            for (Boid& otherBoid : boids)
            {
                float distance = getDistance(boid, otherBoid);

                if (distance < neighborRadius && &boid != &otherBoid) {
                    avgVelocity += otherBoid.getDirection();
                    neighborCount++;
                }
            }
            if (neighborCount > 0)  {
                avgVelocity.normalize();
                boid.addDirection(avgVelocity.x, avgVelocity.y, 0.0005);

            }
        }
    }

    void cohesion(float neighborRadius) {
        Vec2 centerOfMass(0, 0);
        Vec2 resVelocity;
        int neighborCount = 0;

        for (Boid& boid : boids) {
            centerOfMass.x = 0;
            centerOfMass.y = 0;
            neighborCount = 0;

            for (const Boid& otherBoid : boids) {
                if (getDistance(boid, otherBoid) < neighborRadius && &boid != &otherBoid) {
                    centerOfMass += otherBoid.position;
                    neighborCount++;
                }
            }
            if (neighborCount > 0) {
                centerOfMass = centerOfMass * (1.0f / neighborCount);
                resVelocity.x = centerOfMass.x - boid.position.x;
                resVelocity.y = centerOfMass.y - boid.position.y;
                resVelocity = resVelocity.normalize();

                boid.addDirection(resVelocity.x, resVelocity.y, 0.0005);
            }
        }
    }

    void separation(float radius) {
        Vec2 avoid(0, 0);
        int neighborCount = 0;

        for (Boid& boid : boids) {
            avoid.x = 0;
            avoid.y = 0;
            neighborCount = 0;

            for (const Boid& otherBoid : boids) {
                if (getDistance(boid, otherBoid) < radius && &boid != &otherBoid) {
                    avoid.x = avoid.x + (boid.position.x - otherBoid.position.x);
                    avoid.y = avoid.y + (boid.position.y - otherBoid.position.y);
                    neighborCount++;
                }
            }
            if (neighborCount > 0) {
                avoid = avoid * (1.0f / neighborCount);
                avoid = avoid.normalize();
                boid.addDirection(avoid.x, avoid.y, 0.005);
            }
        }
    }

    void updateBoids() {
        alignment(50);
        cohesion(50);
        separation(20);
        moveBoids();
    }
};


sf::ConvexShape getBoidShape() {
    sf::ConvexShape boidShape(3);
    boidShape.setPoint(0, sf::Vector2f(50.f, 0.f));
    boidShape.setPoint(1, sf::Vector2f(-25.f, 25.f));
    boidShape.setPoint(2, sf::Vector2f(-25.f, -25.f));
    boidShape.setFillColor(sf::Color(139, 233, 253));
    boidShape.setScale(0.2, 0.2);

    return boidShape;
}


void DrawBoids(World world, sf::ConvexShape shape, sf::RenderWindow& window) {
    float directionAngle;
    for (int i = 0; i < world.boidsCount; i++)
    {  
        shape.setPosition(world.boids[i].position.x, world.boids[i].position.y);
        directionAngle = world.boids[i].getDirectionAngle();
        shape.setRotation(directionAngle);
        window.draw(shape);
    }
}


int main()
{
    // Создаём окно
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Boids");

    sf::ConvexShape boidShape = getBoidShape();   

    World world(80, 1280, 720); 

    sf::Clock deltaClock;

    // Основной цикл программы
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
           if (event.type == sf::Event::Closed)
                window.close();
        }

        // Очищаем экран
        window.clear();

        world.updateBoids();
        DrawBoids(world, boidShape, window);

        // Отображаем нарисованное
        window.display();
    }

    return 0;
}
