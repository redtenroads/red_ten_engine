#include "entity.h"

Entity::Entity()
{
    registerClassName("Entity");
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

    transform.setRotation(CONST_PI / 2 - x, -y - CONST_PI / 2.0f, 0.0f);
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

const std::string Entity::getClassName()
{
    return className;
}

const std::string Entity::getClass()
{
    return *(classChierarchy.end() - 1);
}

bool Entity::is(const std::string name)
{
    return className == name;
}

bool Entity::implements(const std::string name)
{
    for (auto it = classChierarchy.begin(); it != classChierarchy.end(); it++)
        if (it->compare(name) == 0)
            return true;
    return false;
}
Layer *Entity::getCurrentLayer()
{
    return this->layer;
}

void Entity::setCurrentLayer(Layer *layer)
{
    this->layer = layer;
}

void Entity::childUpdated()
{
}

void Entity::registerClassName(std::string className)
{
    this->className = className;
    classChierarchy.push_back(className);
}