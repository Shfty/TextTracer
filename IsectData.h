#ifndef ISECTDATA_H
#define ISECTDATA_H

class WorldObject;

struct IsectData
{
    glm::vec4 Colour;

    glm::vec3 Entry;
    glm::vec3 Exit;

    float Distance;
    bool Portal = false;

    WorldObject* Object;
};

#endif // ISECTDATA_H
