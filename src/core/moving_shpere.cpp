#include "moving_sphere.h"

namespace Asuka {

    point3 MovingSphere::center(double time) const {
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
    }

    bool MovingSphere::hit(const Ray& ray, double t_min, double t_max) const {
        vec3 oc = ray.origin() - center(ray.time());
        double a = ray.direction().length_squared();
        double half_b = dot(oc, ray.direction());
        double c = oc.length_squared() - radius * radius;
        double discriminant = half_b * half_b - a * c;
        return (discriminant > 0);
    }


    bool MovingSphere::hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min, double t_max) const {
        vec3 oc = ray.origin() - center(ray.time());
        double a = ray.direction().length_squared();
        double half_b = dot(oc, ray.direction());
        double c = oc.length_squared() - radius * radius;
        double discriminant = half_b * half_b - a * c;

        if (discriminant < 0) return false;

        double sqrtd = sqrt(discriminant);
        double root = (-half_b - sqrtd) / a;

        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root) return false;
        }

        hit_point.t = root;
        hit_point.p = ray.at(hit_point.t);
        vec3 outward_normal = (hit_point.p - center(ray.time())) / radius;
        hit_point.set_face_normal(ray, outward_normal);
        hit_point.material = material;

        return true;
    }

    bool MovingSphere::bounding_box(double _time0, double _time1, AABB& output_box) const {
        AABB box0(center(_time0) - vec3(radius), center(_time0) + vec3(radius));
        AABB box1(center(_time1) - vec3(radius), center(_time1) + vec3(radius));
        output_box = merge_box(box0, box1);
        return true;
    }


}