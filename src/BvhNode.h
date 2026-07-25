//
// Created by Andrew Graser on 7/24/2026.
//


#pragma once

#include <algorithm>


#include "Hittable.h"



class BvHNode : public Hittable {
public:
    BvHNode(HittableList list) : BvHNode(list.Objects, 0, list.Objects.size()) {}

    BvHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
        mBounds = AABB::Empty;

        for (size_t objectIndex = start; objectIndex < end; ++objectIndex) {
            mBounds = AABB(mBounds, objects[objectIndex]->BoundingBox());
        }

        //Cut bounding box across longest axis
        //Lessen compute time
        int axis = mBounds.LongestAxis();

        auto comparator = (axis == 0) ? BoxCompareX
                : (axis == 1) ? BoxCompareY
                : BoxCompareZ;

        size_t objectSpan = end - start;

        if(objectSpan == 1) {
            mLeft = mRight = objects[start];
        } else if(objectSpan == 2) {
            mLeft = objects[start];
            mRight = objects[start + 1];
        }else {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            auto mid = start + objectSpan / 2;
            mLeft = std::make_shared<BvHNode>(objects, start, mid);
            mRight = std::make_shared<BvHNode>(objects, mid, end);
        }
    }

    bool Hit(const Ray& r, Interval r_t, HitRecord& rec) const override {
        if(!mBounds.Hit(r, r_t))
            return false;

        bool hitLeft = mLeft->Hit(r, r_t, rec);
        bool hitRight = mRight->Hit(r, Interval(r_t.Min, hitLeft ? rec.T : r_t.Max), rec);

        return hitLeft || hitRight;
    }

    AABB BoundingBox() const override {return mBounds;}


private:
    std::shared_ptr<Hittable> mLeft;
    std::shared_ptr<Hittable> mRight;
    AABB mBounds;



private:
    static bool BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axisIndex) {
        auto aInterval = a->BoundingBox().AxisInterval(axisIndex);
        auto bInterval = b->BoundingBox().AxisInterval(axisIndex);
        return aInterval.Min < bInterval.Min;
    }

    static bool BoxCompareX(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        return BoxCompare(a, b, 0);
    }

    static bool BoxCompareY(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        return BoxCompare(a, b, 1);
    }

    static bool BoxCompareZ(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        return BoxCompare(a, b, 2);
    }
};