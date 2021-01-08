//
//  Game.cpp
//  AsteroidsTestBed
//
//  Created by Chris Greening on 04/01/2021.
//

#include "Game.hpp"
#include "box2d/box2d.h"
#include "GameObject.hpp"
#include <set>

static const b2Vec2 shipPoints[] = {
    b2Vec2(0.2877956623691139, 0.5),
    b2Vec2(4.728071212635484e-8, -0.49999999999999994),
    b2Vec2(-0.287795662369115, 0.5)};
static const int shipPointsCount = 3;

static const b2Vec2 asteroid1Points[] = {
    b2Vec2(0.057291626835217886, -0.4479166903502308),
    b2Vec2(0.1848958112645577, -0.24479167247990513),
    b2Vec2(0.4505208489429552, -0.255208337854741),
    b2Vec2(0.4088541736640834, -0.015624992894930764),
    b2Vec2(0.5, 0.19010418012323715),
    b2Vec2(0.22916671403379496, 0.23437501743971545),
    b2Vec2(0.0546875026913141, 0.4479166903502308),
    b2Vec2(-0.10156249515563455, 0.27604168927370515),
    b2Vec2(-0.4999999999999999, 0.12239584657459872),
    b2Vec2(-0.3124999956938973, -0.1380208325797654),
    b2Vec2(-0.23697915536314731, -0.38802083602464743)};
static const int asteroid1PointsCount = 11;

static const b2Vec2 asteroid2Points[] = {
    b2Vec2(-0.35082873126030634, -0.12154699997812933),
    b2Vec2(-0.0966850749587742, -0.4889503034580926),
    b2Vec2(0.08839780072522405, -0.33977902010144123),
    b2Vec2(0.3149171206617623, -0.430939252270621),
    b2Vec2(0.39226520109252255, -0.1712707476981352),
    b2Vec2(0.5, -0.011049742219900347),
    b2Vec2(0.3895027787841655, 0.2403314552308802),
    b2Vec2(-0.124309385744092, 0.4889503034580926),
    b2Vec2(-0.5, 0.25138121754909754)};
static const int asteroid2PointsCount = 9;

static const b2Vec2 asteroid3Points[] = {
    b2Vec2(-0.0821428593025872, -0.2839285783802025),
    b2Vec2(0.21428571293588303, -0.40535714683914514),
    b2Vec2(0.5000000000000001, -0.0875000089763781),
    b2Vec2(0.2214285476715408, 0.40535714683914514),
    b2Vec2(-0.3642857148256468, 0.3839285953880768),
    b2Vec2(-0.5, -0.05892857554555684),
    b2Vec2(-0.23928572899887535, -0.3660714140607422)};
static const int asteroid3PointsCount = 7;

void Game::createWorld()
{
    // no gravity in our world
    b2Vec2 gravity(0.0f, 0.0f);
    world = new b2World(gravity);
    // create the objects for our game
    // place the ship at the center of the world
    ship = new GameObject(SHIP, world, shipPoints, shipPointsCount, b2Vec2(0, 0), 0, b2Vec2(0, 0), 0, 5);
    objects.push_back(ship);
    // place the asteroids
    objects.push_back(new GameObject(ASTEROID, world, asteroid1Points, asteroid1PointsCount, b2Vec2(25, 25), 0, b2Vec2(10, 10), 0, 10));
    objects.push_back(new GameObject(ASTEROID, world, asteroid2Points, asteroid2PointsCount, b2Vec2(-25, -25), 0, b2Vec2(-10, 10), 0, 10));
    objects.push_back(new GameObject(ASTEROID, world, asteroid3Points, asteroid3PointsCount, b2Vec2(-25, 0), 0, b2Vec2(-10, -10), 0, 10));

    objects.push_back(new GameObject(ASTEROID, world, asteroid1Points, asteroid1PointsCount, b2Vec2(0, 25), 0, b2Vec2(10, 10), 0, 10));
    objects.push_back(new GameObject(ASTEROID, world, asteroid2Points, asteroid2PointsCount, b2Vec2(-25, 0), 0, b2Vec2(-10, 10), 0, 10));
    objects.push_back(new GameObject(ASTEROID, world, asteroid3Points, asteroid3PointsCount, b2Vec2(-25, 5), 0, b2Vec2(-10, -10), 0, 10));

    world->SetContactListener(this);
}

void Game::stepWorld(float elapsedTime)
{
    world->Step(elapsedTime, 6, 2);
    for (auto object : this->objects)
    {
        auto position = object->getPosition();
        if (position.x < -50)
        {
            position.x = 50;
        }
        else if (position.x > 50)
        {
            position.x = -50;
        }
        if (position.y < -50)
        {
            position.y = 50;
        }
        else if (position.y > 50)
        {
            position.y = -50;
        }
        object->setPosition(position);
    }
    // age bullets so that they dissappear
    for (auto bullet : bullets)
    {
        bullet->increaseAge(elapsedTime);
        if (bullet->getAge() > 2.0)
        {
            deadBullets.insert(bullet);
        }
    }
    // remove any dead bullets
    for (auto bullet : deadBullets)
    {
        objects.remove(bullet);
        bullets.remove(bullet);
        delete bullet;
    }
    // remove any hit asteroids
    for (auto asteroid : hitAsteroids)
    {
        // add any child asteroids
        if (asteroid->getAge() < 2)
        {
            {
                auto position = asteroid->getPosition();
                auto scale = 10.0 / (asteroid->getAge() + 2);
                auto linearVelocity = asteroid->getLinearVelocity();
                auto angle = atan2(linearVelocity.y, linearVelocity.x) + M_PI_2;
                // printf("Angle %f %f = %f\n", linearVelocity.y, linearVelocity.x, angle);
                auto newAsteroid = new GameObject(
                    ASTEROID,
                    world,
                    asteroid1Points,
                    asteroid1PointsCount,
                    b2Vec2(position.x + scale * cos(angle),
                           position.y + scale * sin(angle)),
                    0,
                    b2Vec2(10 * cos(angle), 10 * sin(angle)),
                    0,
                    scale);
                newAsteroid->setAge(asteroid->getAge() + 1);
                objects.push_back(newAsteroid);
            }
            {
                auto position = asteroid->getPosition();
                auto scale = 10 / (asteroid->getAge() + 2);
                auto linearVelocity = asteroid->getLinearVelocity();
                auto angle = atan2(linearVelocity.y, linearVelocity.x) - M_PI_2;
                // printf("Angle %f %f = %f\n", linearVelocity.y, linearVelocity.x, angle);
                auto newAsteroid = new GameObject(
                    ASTEROID,
                    world,
                    asteroid2Points,
                    asteroid2PointsCount,
                    b2Vec2(position.x + scale * cos(angle),
                           position.y + scale * sin(angle)),
                    0,
                    b2Vec2(10 * cos(angle), 10 * sin(angle)),
                    0,
                    scale);
                newAsteroid->setAge(asteroid->getAge() + 1);
                objects.push_back(newAsteroid);
            }
        }
        // and remove the dead asteroid
        objects.remove(asteroid);
        delete asteroid;
    }
    deadBullets.clear();
    hitAsteroids.clear();

    // render the world if required
    if (needs_render && render_buffer)
    {
        render();
        needs_render = false;
    }
}

void Game::BeginContact(b2Contact *contact)
{
    GameObject *objA = reinterpret_cast<GameObject *>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    GameObject *objB = reinterpret_cast<GameObject *>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
    if (objA->getObjectType() == BULLET && objB->getObjectType() == ASTEROID)
    {
        deadBullets.insert(objA);
        hitAsteroids.insert(objB);
    }
    if (objA->getObjectType() == ASTEROID && objB->getObjectType() == BULLET)
    {
        deadBullets.insert(objB);
        hitAsteroids.insert(objA);
    }
}

void Game::EndContact(b2Contact *contact)
{
    B2_NOT_USED(contact);
}

void Game::spinLeft()
{
    ship->setAngle(ship->getAngle() + 10 * M_PI / 180.0f);
}

void Game::spinRight()
{
    ship->setAngle(ship->getAngle() - 10 * M_PI / 180.0f);
}

void Game::thrust()
{
    ship->thrust(10);
}

void Game::fire()
{
    if (bullets.size() > 5)
    {
        return;
    }

    GameObject *bullet = new GameObject(BULLET, world, shipPoints, shipPointsCount, ship->getPosition(), M_PI + ship->getAngle(), ship->getLinearVelocity() - 40 * b2Vec2(cos(M_PI_2 + ship->getAngle()), sin(M_PI_2 + ship->getAngle())), 0, 1.5);

    objects.push_back(bullet);
    bullets.push_back(bullet);
}

#define MAX_MOVE_LENGTH 2
#define MAX_LINE_LENGTH 1

void Game::renderSegment(bool laser, float maxLineLength, b2Vec2 start, const b2Vec2 &end)
{
    auto vector = end - start;
    auto length = vector.Length();
    auto invLength = 1.0f / length;
    auto step = invLength * vector;
    // start of the segment
    render_buffer->push_back({.x = uint16_t(std::max(std::min(255.0f, 128 + 2.0f * start.x), 0.0f)),
                              .y = uint16_t(std::max(std::min(255.0f, 128 + 2.0f * start.y), 0.0f)),
                              .laser = laser});
    // move in steps
    while (length > maxLineLength)
    {
        length -= maxLineLength;
        start += maxLineLength * step;
        render_buffer->push_back({.x = uint16_t(std::max(std::min(255.0f, 128.0f + 2.0f * start.x), 0.0f)),
                                  .y = uint16_t(std::max(std::min(255.0f, 128.0f + 2.0f * start.y), 0.0f)),
                                  .laser = laser});
    }
    // do the remaining line
    if (length > 0)
    {
        start += length * step;
        render_buffer->push_back({.x = uint16_t(std::max(std::min(255.0f, 128.0f + 2.0f * start.x), 0.0f)),
                                  .y = uint16_t(std::max(std::min(255.0f, 128.0f + 2.0f * start.y), 0.0f)),
                                  .laser = laser});
    }
}

GameObject *removeNearest(b2Vec2 search_point, std::list<GameObject *> &objects)
{
    GameObject *nearest_object = NULL;
    float nearest_distance = FLT_MAX;
    for (auto object : objects)
    {
        auto distance = (object->getPosition() - search_point).LengthSquared();
        if (distance < nearest_distance)
        {
            nearest_distance = distance;
            nearest_object = object;
        }
    }
    objects.remove(nearest_object);
    return nearest_object;
}

void Game::render()
{
    render_buffer->clear();
    // assume we start from 0,0 - we might optimise this later to start from the last drawn location
    b2Vec2 cur(0, 0);
    std::list<GameObject *> objects_to_draw(objects);
    // while we still have objects to draw
    while (objects_to_draw.size() > 0)
    {
        // get the nearest object to the current search_point
        auto object = removeNearest(cur, objects_to_draw);
        auto numPoints = object->getNumPoints();
        auto points = object->getPoints();
        auto position = object->getPosition();
        auto angle = object->getAngle();
        auto c = cos(angle);
        auto s = sin(angle);
        // move to the start of the object
        renderSegment(false, MAX_MOVE_LENGTH, cur, points[0] + position);
        auto start = b2Vec2(points[0].x * c - points[0].y * s, points[0].x * s + points[0].y * c) + position;
        cur = start;
        for (int i = 0; i < numPoints; i++)
        {
            // draw each line segment
            auto p = b2Vec2(points[i].x * c - points[i].y * s, points[i].x * s + points[i].y * c) + position;
            renderSegment(true, MAX_LINE_LENGTH, cur, p);
            cur = p;
        }
        // close the line
        renderSegment(true, MAX_LINE_LENGTH, cur, start);
        cur = start;
    }
    // move back to 0,0 ready for the next draw pass
    renderSegment(false, MAX_MOVE_LENGTH, cur, b2Vec2(0, 0));
}
