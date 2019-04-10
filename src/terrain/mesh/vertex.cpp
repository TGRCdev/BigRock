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

bool Vertex::operator==(const Vertex &other) const
{
    return (position == other.position) && (normal == other.normal) && (tx == other.tx) && (ty == other.ty);
}

bool Vertex::operator!=(const Vertex &other) const
{
    return (position != other.position) || (normal != other.normal) || (tx != other.tx) || (ty != other.ty);
}

}}}
