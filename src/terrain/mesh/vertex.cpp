#include "vertex.hpp"

namespace bigrock {
namespace terrain {
namespace mesh {

Vertex::Vertex(const math::Vector3 &position, const math::Vector3 &normal, const float &tx, const float &ty)
{
    this->position = position;
    this->normal = normal;
    this->tx = tx;
    this->ty = ty;
}

Vertex Vertex::interpolate(const Vertex &other, const br_real &t) const
{
    if(t <= 0.0)
        return *this;
    else if(t >= 1.0)
        return other;
    else
    {
        Vertex ret;
        ret.position = this->position.lerp(other.position, t);
        ret.normal = this->normal.lerp(other.normal, t).normalized();
        return ret;
    }
}

bool Vertex::operator==(const Vertex &other) const
{
    return (position == other.position) && (normal == other.normal) && (tx == other.tx) && (ty == other.ty);
}

bool Vertex::operator!=(const Vertex &other) const
{
    return (position != other.position) || (normal != other.normal) || (tx != other.tx) || (ty != other.ty);
}

}}}
