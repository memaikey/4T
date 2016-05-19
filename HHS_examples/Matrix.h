#ifndef MATRIX_H
#define MATRIX_H

#include <stdexcept>
#include <initializer_list>
#include <array>
#include <algorithm>

template <typename Object, std::size_t NumRows, std::size_t NumCols>
class matrix {
private:
    using ArrayType = std::array<Object, NumRows * NumCols>;
    ArrayType a;
public:
    matrix() = default;
    matrix(const initializer_list<Object>& init) {
        if (init.size() > a.size()) throw std::out_of_range("To much elements in initializer list for class matrix.");
        std::copy(init.begin(), init.end(), a.begin());
    }
    matrix(const ArrayType& init) {
        std::copy(init.begin(), init.end(), a.begin());
    }
    using const_iterator = typename ArrayType::const_iterator;
    using iterator = typename ArrayType::iterator;
    using size_type = typename ArrayType::size_type;
    const Object& operator()(size_type row, size_type column) const {
        return a[row * NumCols + column];
    }
    Object& operator()(size_type row, size_type column) {
        return a[row * NumCols + column];
    }
    const Object& at(size_type row, size_type column) const {
        return a.at(row * NumCols + column);
    }
    Object& at(size_type row, size_type column) {
        return a.at(row * NumCols + column);
    }
    size_type numRows() const {
        return NumRows;
    }
    size_type numCols() const {
        return NumCols;
    }
    iterator begin() {
        return a.begin();
    }
    const_iterator begin() const {
        return a.cbegin();
    }
    const_iterator cbegin() const {
        return a.cbegin();
    }
    iterator end(){
        return a.end();
    }
    const_iterator end() const {
        return a.cend();
    }
    const_iterator cend() const {
        return a.cend();
    }
};

#endif
