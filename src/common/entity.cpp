#include "entity.h"

Entity::Entity()
{
    registerName("Entity");
}

Entity::~Entity()
{
}

void Entity::lookAt(Vector3 v)
{
    Vector3 position = transform.getPosition();
    Vector3 dif = glm::normalize(Vector3(v.x - position.x, v.y - position.y, v.z - position.z));

    float y = atan2f(dif.z, dif.x);

    float len = sqrtf(dif.x * dif.x + dif.z * dif.z);
    float x = atan2(len, dif.y);

    transform.setRotation(-M_PI / 2.0f + x, -y - M_PI / 2.0f, 0.0f);
}

void Entity::lookAt(float x, float y, float z)
{
    lookAt(Vector3(x, y, z));
}

bool Entity::isVisible()
{
    return bIsVisible;
}

void Entity::setVisible(bool state)
{
    bIsVisible = state;
}

const std::string Entity::getName()
{
    return className;
}

const std::string Entity::getClass()
{
    return *(classChierarchy.end() - 1);
}

bool Entity::is(std::string name)
{
    return className == name;
}

bool Entity::implements(std::string name)
{
    for (auto it = classChierarchy.begin(); it != classChierarchy.end(); it++)
        if (*it == name)
            return true;
    return false;
}

void Entity::setCurrentLayer(Layer *layer)
{
    this->layer = layer;
}

void Entity::childUpdated()
{
}

void Entity::registerName(std::string name)
{
    className = name;
    classChierarchy.push_back(className);
}