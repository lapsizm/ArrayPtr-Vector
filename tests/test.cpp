// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>

#include "ArrayPtr.hpp"
#include "Vector.hpp"

TEST(ArrayPtr, AllArrayTests) {
    ArrayPtr<int> arr;
    EXPECT_EQ(arr.Get(), nullptr);
    EXPECT_FALSE(bool(arr));

    ArrayPtr<int> arr2(10);
    EXPECT_EQ(*arr2.Get(), arr2[0]);
    EXPECT_TRUE(bool(arr2));
    int *ar_ = arr2.Get();      //эти массивы удалять не надо
    int *ar = arr2.Release();
    EXPECT_EQ(ar, ar_);
    EXPECT_EQ(arr2.Get(), nullptr);
    EXPECT_FALSE(bool(arr2));


    ArrayPtr<int> arr3(15);
    EXPECT_EQ(*arr3.Get(), arr3[0]);
    EXPECT_TRUE(bool(arr3));
    arr3.swap(arr2);
    EXPECT_EQ(arr3.Get(), nullptr);
    EXPECT_FALSE(bool(arr3));
    EXPECT_EQ(*arr2.Get(), arr2[0]);
    EXPECT_TRUE(bool(arr2));
}

TEST(Vector, Construct) {
    SimpleVector<int> vec;
    EXPECT_EQ(vec.GetSize(), 0);
    EXPECT_EQ(vec.begin(), nullptr);
    EXPECT_EQ(vec.GetCapacity(), 0);
    EXPECT_EQ(vec.IsEmpty(), true);

    SimpleVector<int> vec2{1, 2, 3, 4, 5};
    EXPECT_EQ(vec2.GetSize(), 5);
    EXPECT_EQ(vec2[3], 4);

    SimpleVector<char> vec3(5, 'c');
    EXPECT_EQ(vec3.GetSize(), 5);
    EXPECT_EQ(vec3[3], 'c');

    SimpleVector<char> vec4(vec3);
    EXPECT_EQ(vec3.GetSize(), 5);
    EXPECT_EQ(vec3[3], 'c');
    EXPECT_EQ(vec4.GetSize(), 5);
    EXPECT_EQ(vec4[3], 'c');

    SimpleVector<double> vec5{1.2, 1.3, 5.6, 4.7, 8.3};
    SimpleVector<double> vec6(std::move(vec5));
    EXPECT_EQ(vec6.GetSize(), 5);
    EXPECT_EQ(vec6[3], 4.7);
    EXPECT_EQ(vec5.GetSize(), 0);
    EXPECT_EQ(vec5.begin(), nullptr);

    ProxyObject pr(10);
    SimpleVector<char> vec7(pr);
    EXPECT_EQ(vec7.GetSize(), 0);
    EXPECT_EQ(vec7.GetCapacity(), 10);
}

TEST(Vector, Swap_Operators) {
    SimpleVector<int> vec1{1, 2, 3, 4, 5};
    SimpleVector<int> vec2{5, 6, 7};

    vec1.swap(vec2);
    EXPECT_EQ(vec1.GetSize(), 3);
    EXPECT_EQ(vec2.GetSize(), 5);
    EXPECT_EQ(vec1[2], 7);
    EXPECT_EQ(vec2[2], 3);

    swap(vec1, vec2);
    EXPECT_EQ(vec2.GetSize(), 3);
    EXPECT_EQ(vec1.GetSize(), 5);
    EXPECT_EQ(vec2[2], 7);
    EXPECT_EQ(vec1[2], 3);

    EXPECT_FALSE(vec1 == vec2);
    EXPECT_TRUE(vec1 != vec2);
    SimpleVector<int> vec3 = vec2;
    EXPECT_EQ(vec3.GetSize(), 3);
    EXPECT_EQ(vec3[2], 7);

    EXPECT_TRUE(vec2 <= vec3);
    EXPECT_TRUE(vec2 >= vec3);
    SimpleVector<int> vec4{1, 2, 3};
    EXPECT_TRUE(vec3 > vec4);
    EXPECT_FALSE(vec3 < vec4);
    EXPECT_TRUE(vec3 >= vec4);
    EXPECT_FALSE(vec3 <= vec4);

}
TEST(Vector, PushPopBack){
    SimpleVector<int> vec4{1, 2, 3};
    vec4.PushBack(15);
    EXPECT_EQ(vec4.GetSize(), 4);
    EXPECT_EQ(vec4[3], 15);

    vec4.PushBack(20);
    vec4.PushBack(35);
    EXPECT_EQ(vec4.GetSize(), 6);
    EXPECT_EQ(vec4[5], 35);
    EXPECT_EQ(vec4.GetCapacity(), 6);
    vec4.PushBack(77);
    EXPECT_EQ(vec4.GetCapacity(), 12);

    int a = 12;
    SimpleVector<int> vec5;
    vec5.PushBack(a);
    EXPECT_EQ(vec5.GetSize(), 1);
    EXPECT_EQ(vec5.GetCapacity(), 2);
    vec5.PushBack(a);
    EXPECT_EQ(vec5.GetSize(), 2);
    EXPECT_EQ(vec5.GetCapacity(), 2);
    EXPECT_EQ(vec5[1], 12);

    vec5.PopBack();
    EXPECT_EQ(vec5.GetSize(), 1);
}

TEST(Vector, Insert){
    SimpleVector<int> vec1{1,2,3,4};
    vec1.Insert(vec1.begin() + 2, 15);
    EXPECT_EQ(vec1.GetSize(), 5);
    EXPECT_EQ(vec1[2], 15);
    vec1.Insert(vec1.begin(), 50);
    EXPECT_EQ(vec1.GetSize(), 6);
    EXPECT_EQ(vec1[0], 50);
    vec1.Insert(vec1.end(), 100);
    EXPECT_EQ(vec1.GetSize(), 7);
    EXPECT_EQ(vec1[6], 100);

    vec1.PushBack(18);
    EXPECT_EQ(vec1.GetSize(), 8);
    EXPECT_EQ(vec1.GetCapacity(), 8);

    vec1.Insert(vec1.begin() + 5, 999);
    EXPECT_EQ(vec1[5], 999);
    EXPECT_EQ(vec1.GetSize(), 9);
    EXPECT_EQ(vec1.GetCapacity(), 16);

    SimpleVector<int> new_vec{1,2};
    int a = 999;
    new_vec.Insert(new_vec.begin(), a);
    EXPECT_EQ(new_vec[0], 999);
    EXPECT_EQ(new_vec.GetSize(), 3);
    EXPECT_EQ(new_vec.Insert(new_vec.begin() + 1, a), new_vec.begin() + 1);
    int b = 333;
    new_vec.Insert(new_vec.begin() + 1, b);
    EXPECT_EQ(new_vec[1], 333);
    EXPECT_EQ(new_vec.GetSize(), 5);
}

TEST(Vector, Erase){
    SimpleVector<int> my_vec{1,2,3,4,5,6,7};
    my_vec.Erase(my_vec.begin() + 2);
    EXPECT_EQ(my_vec[2],4);
    EXPECT_EQ(my_vec.GetSize(), 6);

    SimpleVector<int> my_vec2{1};
    my_vec2.Erase(my_vec2.begin());
    EXPECT_EQ(my_vec2.IsEmpty(), true);

    my_vec2.Erase(my_vec.begin());
    EXPECT_EQ(my_vec2.IsEmpty(), true);
    EXPECT_EQ(my_vec2.begin(), nullptr);

}

TEST(Vector, OtherMethods){
    SimpleVector<int> my_vec{1,2,3,4};
    my_vec.Resize(10);
    EXPECT_EQ(my_vec.GetSize(), 10);
    EXPECT_EQ(my_vec[3], 4);
    EXPECT_EQ(my_vec[7], 0);

    my_vec.Resize(3);
    EXPECT_EQ(my_vec.GetSize(), 3);

    my_vec.Reserve(10);
    EXPECT_EQ(my_vec.GetSize(), 3);
    EXPECT_EQ(my_vec.GetCapacity(), 10);
    EXPECT_EQ(my_vec[1], 2);
    EXPECT_EQ(my_vec[9], 0);

    my_vec.Clear();
    EXPECT_EQ(my_vec.GetSize(), 0);

}