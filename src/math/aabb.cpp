#include "aabb.hpp"

namespace bigrock
{
    namespace math
    {
        // Parts of AABB are taken from the Godot Engine
        // https://godotengine.org
        /*************************************************************************/
        /* Copyright (c) 2007-2019 Juan Linietsky, Ariel Manzur.                 */
        /* Copyright (c) 2014-2019 Godot Engine contributors (cf. AUTHORS.md)    */
        /*                                                                       */
        /* Permission is hereby granted, free of charge, to any person obtaining */
        /* a copy of this software and associated documentation files (the       */
        /* "Software"), to deal in the Software without restriction, including   */
        /* without limitation the rights to use, copy, modify, merge, publish,   */
        /* distribute, sublicense, and/or sell copies of the Software, and to    */
        /* permit persons to whom the Software is furnished to do so, subject to */
        /* the following conditions:                                             */
        /*                                                                       */
        /* The above copyright notice and this permission notice shall be        */
        /* included in all copies or substantial portions of the Software.       */
        /*                                                                       */
        /* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
        /* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
        /* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
        /* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
        /* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
        /* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
        /* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
        /*************************************************************************/

        AABB::AABB(const Vector3 &position, const Vector3 &size)
        {
            this->position = position;
            this->size = size;
        }

        bool AABB::has_no_area() const
        {
            return (size.x <= 0 || size.y <= 0 || size.z <= 0);
        }

        bool AABB::has_no_surface() const
        {
            return (size.x <= 0 && size.y <= 0 && size.z <= 0);
        }

        bool AABB::intersects(const AABB &other) const
        {
            if(position.x >= (other.position.x + other.size.x))
                return false;
            if((position.x + size.x) <= other.position.x)
                return false;
            if(position.y >= (other.position.y + other.size.y))
                return false;
            if((position.y + size.y) <= other.position.y)
                return false;
            if(position.z >= (other.position.z + other.size.z))
                return false;
            if((position.z + size.z) <= other.position.z)
                return false;
            
            return true;
        }

        bool AABB::encloses(const AABB &other) const
        {
            Vector3 src_min = position;
            Vector3 src_max = position + size;
            Vector3 dst_min = other.position;
            Vector3 dst_max = other.position + other.size;

            return (
                    (src_min.x <= dst_min.x) &&
                    (src_max.x > dst_max.x) &&
                    (src_min.y <= dst_min.y) &&
                    (src_max.y > dst_max.y) &&
                    (src_min.z <= dst_min.z) &&
                    (src_max.z > dst_max.z));
        }

        bool AABB::contains_point(const Vector3 &point) const
        {
            if (point.x < position.x)
                return false;
            if (point.y < position.y)
                return false;
            if (point.z < position.z)
                return false;
            if (point.x > position.x + size.x)
                return false;
            if (point.y > position.y + size.y)
                return false;
            if (point.z > position.z + size.z)
                return false;

            return true;
        }
    }
}