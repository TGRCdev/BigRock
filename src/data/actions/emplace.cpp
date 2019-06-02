#include "emplace.hpp"

namespace bigrock {
namespace data {
namespace actions {

Emplace::Emplace(const unsigned char material)
{
    this->data.emplace.material = material;
}

void Emplace::update(const Tool &t, const Vector3 position, Point &v, ToolQueryResult *res) const
{
    float val = t.value(position);
    if(val > -1)
    {
        if(val > v.isovalue)
            v.isovalue = val;
        if(val > 0)
            v.material = this->data.emplace.material;
    }
}

}}}