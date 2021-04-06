#ifndef FLAT_HAPPYARRAY_H
#define FLAT_HAPPYARRAY_H

#include <stdexcept>

/*
Name: Flat - Happy Array
Date: 8/2/2020 (MM/DD/YYYY)
Version: 1.02
*/

namespace flat {
    template<class T>
    class HappyArray {
    public:
        HappyArray() = delete;

        explicit HappyArray(unsigned long int size);

        ~HappyArray();

        HappyArray(const HappyArray &);

        [[nodiscard]] std::size_t size() const noexcept;

        void fill(const T &fillValue) noexcept;

        T &operator[](size_t index) const;

        HappyArray<T> &operator=(const HappyArray<T> &other);

    private:
        T *_arrayPtr;
        std::size_t _nElements;
    };

    template<class T>
    HappyArray<T>::HappyArray(const unsigned long int size) {
        _nElements = size;
        if (_nElements != 0)
            _arrayPtr = new T[_nElements];
        else
            _arrayPtr = nullptr;
    }

    template<class T>
    HappyArray<T>::HappyArray(const HappyArray &other) {
        _nElements = other._nElements;
        _arrayPtr = new T[_nElements];
        for (unsigned int i = 0; i < _nElements; i++)
            _arrayPtr[i] = other._arrayPtr[i];
    }

    template<class T>
    HappyArray<T>::~HappyArray() {
        delete[] _arrayPtr;
    }

    template<class T>
    std::size_t HappyArray<T>::size() const noexcept {
        return _nElements;
    }

    template<class T>
    void HappyArray<T>::fill(const T &fillValue) noexcept {
        for (size_t i = 0; i < _nElements; i++)
            _arrayPtr[i] = fillValue;
    }

    template<class T>
    T &HappyArray<T>::operator[](const size_t index) const {
        if ((index >= _nElements) || !_arrayPtr)
            throw std::out_of_range("");

        return _arrayPtr[index];
    }

    template<class T>
    HappyArray<T> &HappyArray<T>::operator=(const HappyArray<T> &other) {
        if (this != &other) {
            delete[] _arrayPtr;
            _nElements = other._nElements;
            _arrayPtr = new T[_nElements];
            for (unsigned int i = 0; i < _nElements; i++)
                _arrayPtr[i] = other._arrayPtr[i];
        }

        return *this;
    }
}

#endif //FLAT_HAPPYARRAY_H
