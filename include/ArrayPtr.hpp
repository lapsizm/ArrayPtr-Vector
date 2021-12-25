// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_ARRAYPTR_HPP_
#define INCLUDE_ARRAYPTR_HPP_

#include <algorithm>
#include <cstdlib>

template<typename Type>
class ArrayPtr {
private:
    Type *raw_ptr_ = nullptr;

public:
// Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

// Создаёт в куче массив из size элементов типа Type.
// Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(size_t size) {
        if (size != 0) {
            raw_ptr_ = new Type[size];
        }
    }

// Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type *raw_ptr) noexcept: raw_ptr_(raw_ptr) {}

// Запрещаем копирование
    ArrayPtr(const ArrayPtr &) = delete;

    ~ArrayPtr() {
        delete[] raw_ptr_;
        raw_ptr_ = nullptr;
    }

// Запрещаем присваивание
    ArrayPtr &operator=(const ArrayPtr &) = delete;

// Прекращает владением массивом в памяти, возвращает значение адреса массива
// После вызова метода указатель на массив должен стать обнулиться
// ОБЯЗАТЕЛЬНО ВСЕ ИЗУЧИТЬ ЧТО ТАКОЕ NODISCARD
    [[nodiscard]] Type *Release() noexcept {
        if (raw_ptr_ != nullptr) {
            Type* new_ptr = std::move(raw_ptr_);
            delete[] raw_ptr_;
            raw_ptr_ = nullptr;
            return new_ptr;
        }
        else{
            return raw_ptr_;
        }
    }

// Возвращает ссылку на элемент массива с индексом index
    Type &operator[](size_t index) noexcept {
        return raw_ptr_[index];
    }

// Возвращает константную ссылку на элемент массива с индексом index
    const Type &operator[](size_t index) const noexcept {
        return raw_ptr_[index];
    }

// Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        return raw_ptr_ != nullptr;
    }

// Возвращает значение сырого указателя, хранящего адрес начала массива
    Type *Get() const noexcept {
        return raw_ptr_;
    }

// SWAP <- понадобиться нам для взаимодействия с временными объектами
    void swap(ArrayPtr &other) noexcept {
        std::swap(this->raw_ptr_, other.raw_ptr_);
    }
};


#endif  // INCLUDE_ARRAYPTR_HPP_
