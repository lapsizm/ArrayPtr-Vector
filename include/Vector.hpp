
#ifndef INCLUDE_VECTOR_HPP_
#define INCLUDE_VECTOR_HPP_

#include "ArrayPtr.hpp"

class ProxyObject {
public:
    ProxyObject(size_t capacity) : capacity_(capacity) {}

    size_t GetCapacity() const { return capacity_; }

private:
    size_t capacity_;
};

template<typename Type>
class SimpleVector {
private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    ArrayPtr<Type> data_;

public:
// Итератор
    using Iterator = Type *;
// Константный итератор
    using ConstIterator = const Type *;

// Конструктор по умолчанию
    SimpleVector() noexcept = default;

// Конструктор с параметрами (реализовали на паре)
    SimpleVector(size_t size, const Type &value = Type{})
            : size_(size), capacity_(size * 2), data_(size_) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }

// Конструктор копирования (не реализовали на паре)
    SimpleVector(const SimpleVector<Type> &other) : size_(other.size_),
                                                    capacity_(other.capacity_), data_(size_) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

// Конструктор перемещения
    SimpleVector(SimpleVector<Type> &&rvl) {
        this->swap(rvl);
    }

// initializer_list конструктор (поймем на следущей паре)
    SimpleVector(std::initializer_list<Type> init)
            : size_(init.size()),
              capacity_(size_ * 2),
              data_(size_) {
        int count = 0;
        for (auto &element: init) {
            data_[count] = element;
            ++count;
        }
    }

// пока не вдумываемся в это
    SimpleVector(ProxyObject obj) : size_(0), capacity_(obj.GetCapacity()), data_(obj.GetCapacity()) {}

// своп
    void swap(SimpleVector &other) noexcept {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        data_.swap(other.data_);
    }

// своп
    friend void swap(SimpleVector<Type> &lhs, SimpleVector<Type> &rhs) {
        lhs.swap(rhs);
    }

// Копирующий оператор присваивания
    SimpleVector &operator=(const SimpleVector<Type> &other) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = other.data_.Get();
    }

// Оператор проверки на равенство
    friend bool operator==(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs) {
        if (lhs.size_ != rhs.size_) return false;

        for (size_t i = 0; i < lhs.size_; ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }

// Оператор проверки на НЕ равенство
    friend bool operator!=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs) {
        return !(lhs == rhs);
    }

// Уже реализовано объясню на след паре
    friend bool operator<(const SimpleVector &lhs, const SimpleVector &rhs) {
        return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
    }

    friend bool operator>(const SimpleVector &lhs, const SimpleVector &rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const SimpleVector &lhs, const SimpleVector &rhs) {
        if (lhs.size_ != rhs.size_) return false;

        for (size_t i = 0; i < lhs.size_; ++i) {
            if (lhs.data_[i] > rhs.data_[i]) return false;
        }
        return true;
    }

    friend bool operator>=(const SimpleVector &lhs, const SimpleVector &rhs) {
        return rhs <= lhs;
    }

    Type &operator[](size_t index) noexcept {
        return data_[index];
    }

// Возвращает константную ссылку на элемент с индексом index
    const Type &operator[](size_t index) const noexcept {
        return data_[index];
    }

// Метод добавления в конец
    void PushBack(const Type &item) {
        if (size_ < capacity_) {
            data_[size_] = item;
        } else {
            if (capacity_ == 0) ++capacity_;
            capacity_ *= 2;
            ArrayPtr<Type> arr(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                arr[i] = data_[i];
            }
            arr[size_] = item;
            data_.swap(arr);
        }
        ++size_;
    }

// Метод добавления в конец
    void PushBack(Type &&item) {
        if (size_ < capacity_) {
            data_[size_] = std::move(item);
        } else {
            if (capacity_ == 0) ++capacity_;
            capacity_ *= 2;
            ArrayPtr<Type> arr(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                arr[i] = std::move(data_[i]);
            }
            arr[size_] = std::move(item);
            data_.swap(arr);
        }
        ++size_;
    }

    void PopBack() noexcept {
        if (size_ > 0) {
            --size_;
        }
    }

// Метод вставки в конкретную позицию реализовали
    Iterator Insert(Iterator pos, const Type &value) {
        if (size_ < capacity_) {
            std::copy_backward(pos, end(), end() + 1);
            data_[std::distance(begin(), pos)] = value;
            ++size_;
            return pos;
        } else {
            capacity_ *= 2;
            ArrayPtr<Type> data_new(capacity_);
            Iterator it_new(data_new.Get() + size_ + 1);

            std::copy(begin(), pos, data_new.Get());

            data_new[std::distance(begin(), pos)] = value;
            Iterator new_pos(&data_new[std::distance(begin(), pos)]);

            std::copy_backward(pos, end(), it_new);
            ++size_;
            data_.swap(data_new);
            return new_pos;
        }
    }

// Метод вставки в конкретную позицию реализовали
    Iterator Insert(Iterator pos, Type &&value) {
        if (size_ < capacity_) {
            std::copy_backward(pos, end(), end() + 1);
            data_[std::distance(begin(), pos)] = std::move(value);
            ++size_;
            return pos;
        } else {
            capacity_ *= 2;
            ArrayPtr<Type> data_new(capacity_);
            Iterator it_new(data_new.Get() + size_ + 1);

            std::copy(begin(), pos, data_new.Get());

            data_new[std::distance(begin(), pos)] = std::move(value);
            Iterator new_pos(&data_new[std::distance(begin(), pos)]);

            std::copy_backward(pos, end(), it_new);
            ++size_;
            data_.swap(data_new);
            return new_pos;
        }
    }

// Удаляет элемент вектора в указанной позиции (не реализовали)
    Iterator Erase(Iterator pos) {
        if (size_ == 0) {
            return data_.Release();
        }
        std::copy(pos + 1, end(), pos);
        --size_;
        return pos;
    }

// Возвращает количество элементов в массиве
    size_t GetSize() const noexcept { return size_; }

// Возвращает вместимость массива
    size_t GetCapacity() const noexcept { return capacity_; }

    bool IsEmpty() const noexcept { return size_ == 0; }

    Type &At(size_t index) { return data_[index]; }

// Возвращает константную ссылку на элемент с индексом index
// Выбрасывает исключение std::out_of_range, если index >= size
    const Type &At(size_t index) const { return data_[index]; }

// Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept { size_ = 0; }

// Изменяет размер массива.
// При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size < capacity_) {
            size_ = new_size;
            return;
        }
        capacity_ = new_size * 2;
        SimpleVector<Type> vec(capacity_);
        std::copy(data_.Get(), data_.Get() + size_, vec.begin());
        size_ = new_size;
        data_.swap(vec.data_);
    }

    void Reserve(size_t new_capacity) {
        capacity_ = new_capacity;
        if (new_capacity < size_) {
            size_ = new_capacity;
            return;
        }
        SimpleVector<Type> vec(capacity_);
        std::copy(data_.Get(), data_.Get() + size_, vec.begin());
        data_.swap(vec.data_);
    }

    Iterator begin() noexcept {
        return Iterator(data_.Get());
    }

    Iterator end() noexcept {
        return Iterator(data_.Get() + size_);
    }

    ConstIterator begin() const noexcept {
        return ConstIterator(data_.Get());
    }

    ConstIterator end() const noexcept {
        return ConstIterator(data_.Get() + size_);

    }

    ConstIterator cbegin() const noexcept {
        return ConstIterator(data_.Get());
    }

    ConstIterator cend() const noexcept {
        return ConstIterator(data_.Get() + size_);
    }

};

ProxyObject Reserve(size_t capacity_to_reserve) {
    return ProxyObject(capacity_to_reserve);
}

#endif  // INCLUDE_VECTOR_HPP_
