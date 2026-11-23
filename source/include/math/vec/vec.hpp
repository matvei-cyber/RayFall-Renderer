#pragma once


struct vec2
{
    float x, y;


    vec2 ();
    vec2 (const float x_val, const float y_val);


    vec2 add (const vec2& other) const;
    void add_rw (const vec2& other);
    vec2 sub (const vec2& other) const;
    void sub_rw (const vec2& other);
    vec2 operator- () const;
    vec2 mul (const float num) const;
    void mul_rw (const float num);
    vec2 div (const float num) const;
    void div_rw (const float num);
    vec2 operator* (const vec2& other) const;
    void operator*= (const vec2& other);
    float dot (const vec2& other) const;
    float length () const;
    void normalize ();
    vec2 normalized () const;
    vec2 right_normal () const;
    vec2 left_normal () const;
};


struct vec3
{
    float x, y, z;


    vec3 ();
    vec3 (const float x_val, const float y_val, const float z_val);


    vec3 add (const vec3& other) const;
    void add_rw (const vec3& other);
    vec3 sub (const vec3& other) const;
    void sub_rw (const vec3& other);
    vec3 operator- () const;
    vec3 mul (const float num) const;
    void mul_rw (const float num);
    vec3 div (const float num) const;
    void div_rw (const float num);
    vec3 operator* (const vec3& other) const;
    void operator*= (const vec3& other);
    vec3 cross (const vec3& other) const;
    float dot (const vec3& other) const;
    float length () const;
    void normalize ();
    vec3 normalized () const;
    vec3 right_normal () const;
    vec3 left_normal () const;
};
