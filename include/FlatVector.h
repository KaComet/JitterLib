#ifndef FLAT_VECTOR_H
#define FLAT_VECTOR_H

#include <vector>
#include <cassert>

/*
Name: Flat Vector
Date: 7/22/2020 (MM/DD/YYYY)
Version: 1.01
*/

namespace flat {

    enum SetType {
        Defined,
        Redefinition,
        Discarded
    };

    template<typename E>
    class FlatVector {
    public:
        FlatVector();

        explicit FlatVector(E desiredDefaultElement);

        ~FlatVector() = default;

        E get(size_t elementNumber) const noexcept;

        E getDefault() const noexcept;

        SetType set(size_t elementNumber, E element) noexcept;

        void setCapacity(size_t desiredCapacity);

        void setDefault(E desiredDefaultElement) noexcept;

        [[nodiscard]] std::size_t capacity() const noexcept;

    private:
        std::vector<E> contents;
        std::vector<bool> contentsDefined;
        std::size_t _capacity;
        E defaultElement;
    };

    template<class E>
    FlatVector<E>::FlatVector() {
        _capacity = 0;
    }

    template<class E>
    FlatVector<E>::FlatVector(const E desiredDefaultElement) {
        _capacity = 0;
        defaultElement = desiredDefaultElement;
    }

    template<class E>
    void FlatVector<E>::setCapacity(const std::size_t desiredCapacity) {
        assert(_capacity >= 0);

        contents.resize(desiredCapacity, defaultElement);
        contentsDefined.resize(desiredCapacity, false);
        _capacity = desiredCapacity;

        assert(contentsDefined.size() == _capacity);
        assert(contents.size() == _capacity);
    }

    template<class E>
    E FlatVector<E>::getDefault() const noexcept {
        return defaultElement;
    }

    template<class E>
    void FlatVector<E>::setDefault(const E desiredDefaultElement) noexcept {
        defaultElement = desiredDefaultElement;
    }

    template<class E>
    E FlatVector<E>::get(const size_t elementNumber) const noexcept {
        if ((elementNumber >= _capacity) || (elementNumber >= contents.size()))
            return defaultElement;

        if (contentsDefined.at(elementNumber))
            return contents.at(elementNumber);
        else
            return defaultElement;
    }

    template<class E>
    SetType FlatVector<E>::set(const size_t elementNumber, const E element) noexcept {
        if ((elementNumber >= _capacity) || (elementNumber >= contents.size()))
            return SetType::Discarded;

        const bool isRedefining = contentsDefined.at(elementNumber);

        contents.at(elementNumber) = element;
        contentsDefined.at(elementNumber) = true;

        if (isRedefining)
            return SetType::Redefinition;
        else
            return SetType::Defined;
    }

    template<class E>
    std::size_t FlatVector<E>::capacity() const noexcept {
        return _capacity;
    }

}

#endif