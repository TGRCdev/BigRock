#include "AABB.hpp"
#include <glm/gtx/component_wise.hpp>

namespace bigrock {
namespace data {

AABB::AABB()
{
  setNull();
}

AABB::AABB(const Vector3& center, br_real_t radius)
{
  setNull();
  extend(center, radius);
}

AABB::AABB(const Vector3& p1, const Vector3& p2)
{
  setNull();
  extend(p1);
  extend(p2);
}

AABB::AABB(const AABB& aabb)
{
  setNull();
  extend(aabb);
}

AABB::~AABB()
{
}

void AABB::extend(br_real_t val)
{
  if (!isNull())
  {
    mMin -= Vector3(val);
    mMax += Vector3(val);
  }
}

void AABB::extend(const Vector3& p)
{
  if (!isNull())
  {
    mMin = glm::min(p, mMin);
    mMax = glm::max(p, mMax);
  }
  else
  {
    mMin = p;
    mMax = p;
  }
}

void AABB::extend(const Vector3& p, br_real_t radius)
{
  Vector3 r(radius);
  if (!isNull())
  {
    mMin = glm::min(p - r, mMin);
    mMax = glm::max(p + r, mMax);
  }
  else
  {
    mMin = p - r;
    mMax = p + r;
  }
}

void AABB::extend(const AABB& aabb)
{
  if (!aabb.isNull())
  {
    extend(aabb.mMin);
    extend(aabb.mMax);
  }
}

void AABB::extendDisk(const Vector3& c, const Vector3& n, br_real_t r)
{
  if (glm::length(n) < 1.e-12) { extend(c); return; }
  Vector3 norm = glm::normalize(n);
  br_real_t x = sqrt(1 - norm.x) * r;
  br_real_t y = sqrt(1 - norm.y) * r;
  br_real_t z = sqrt(1 - norm.z) * r;
  extend(c + Vector3(x,y,z));
  extend(c - Vector3(x,y,z));
}

Vector3 AABB::getDiagonal() const
{
  if (!isNull())
    return mMax - mMin;
  else
    return Vector3(0);
}

br_real_t AABB::getLongestEdge() const
{
  return glm::compMax(getDiagonal());
}

br_real_t AABB::getShortestEdge() const
{
  return glm::compMin(getDiagonal());
}

Vector3 AABB::getCenter() const
{
  if (!isNull())
  {
    Vector3 d = getDiagonal();
    return mMin + (d * br_real_t(0.5));
  }
  else
  {
    return Vector3(0.0);
  }
}

void AABB::translate(const Vector3& v)
{
  if (!isNull())
  {
    mMin += v;
    mMax += v;
  }
}

void AABB::scale(const Vector3& s, const Vector3& o)
{
  if (!isNull())
  {
    mMin -= o;
    mMax -= o;

    mMin *= s;
    mMax *= s;

    mMin += o;
    mMax += o;
  }
}

bool AABB::overlaps(const AABB& bb) const
{
  if (isNull() || bb.isNull())
    return false;

  if( bb.mMin.x > mMax.x || bb.mMax.x < mMin.x)
    return false;
  else if( bb.mMin.y > mMax.y || bb.mMax.y < mMin.y)
    return false;
  else if( bb.mMin.z > mMax.z || bb.mMax.z < mMin.z)
    return false;

  return true;
}

AABB::INTERSECTION_TYPE AABB::intersect(const AABB& b) const
{
  if (isNull() || b.isNull())
    return OUTSIDE;

  if ((mMax.x < b.mMin.x) || (mMin.x > b.mMax.x) ||
      (mMax.y < b.mMin.y) || (mMin.y > b.mMax.y) ||
      (mMax.z < b.mMin.z) || (mMin.z > b.mMax.z)) 
  {
    return OUTSIDE;
  }

  if ((mMin.x <= b.mMin.x) && (mMax.x >= b.mMax.x) &&
      (mMin.y <= b.mMin.y) && (mMax.y >= b.mMax.y) &&
      (mMin.z <= b.mMin.z) && (mMax.z >= b.mMax.z)) 
  {
    return INSIDE;
  }

  return INTERSECT;    
}


bool AABB::isSimilarTo(const AABB& b, br_real_t diff) const
{
  if (isNull() || b.isNull()) return false;

  Vector3 acceptable_diff=( (getDiagonal()+b.getDiagonal()) / br_real_t(2.0))*diff;
  Vector3 min_diff(mMin-b.mMin);
  min_diff = Vector3(fabs(min_diff.x),fabs(min_diff.y),fabs(min_diff.z));
  if (min_diff.x > acceptable_diff.x) return false;
  if (min_diff.y > acceptable_diff.y) return false;
  if (min_diff.z > acceptable_diff.z) return false;
  Vector3 max_diff(mMax-b.mMax);
  max_diff = Vector3(fabs(max_diff.x),fabs(max_diff.y),fabs(max_diff.z));
  if (max_diff.x > acceptable_diff.x) return false;
  if (max_diff.y > acceptable_diff.y) return false;
  if (max_diff.z > acceptable_diff.z) return false;
  return true;
}

}}