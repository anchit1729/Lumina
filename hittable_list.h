//
// Created by Anchit Mishra on 2023-06-07.
//

#ifndef LUMINA_HITTABLE_LIST_H
#define LUMINA_HITTABLE_LIST_H

#include <hittable.h>
#include <aabb.h>

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable   {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object)  { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object)   {
        objects.push_back(object);
        bbox = aabb(bbox, object -> bounding_box());
    }

    virtual bool hit(const ray& r, interval t_interval, hit_record& hit_rec) const override;
    aabb bounding_box() const override { return bbox; }
    std::vector<shared_ptr<hittable>> objects;
    aabb bbox;
};

bool hittable_list::hit(const ray& r, interval t_interval, hit_record& hit_rec) const    {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_interval.max;
    interval closest_so_far_interval = interval(t_interval.min, closest_so_far);

    for (const auto& object: objects)   {
        if (object->hit(r, closest_so_far_interval, temp_rec))    {
            hit_anything = true;
            closest_so_far = temp_rec.root;
            closest_so_far_interval.max = closest_so_far;
            hit_rec = temp_rec;
        }
    }

    return hit_anything;
}

//aabb hittable_list::get_bounding_box() const {
//    return bounding_box;
//}

#endif //LUMINA_HITTABLE_LIST_H
