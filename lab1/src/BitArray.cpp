#include <algorithm>
#include "BitArray.h"

BitArray::BitArray() : bitArray(), size(0) {}
BitArray::BitArray(int num_bits, unsigned long value) :
bitArray((num_bits + sizeof(arrayType) - 1) / sizeof(arrayType)), size(num_bits) {
    for (int i = 0; i < num_bits; ++i) {
        bitArray[i / sizeof(arrayType)] |= ((value >> i) & 1) << (i % sizeof(arrayType));
    }
}
BitArray::BitArray(const BitArray& b) {
    size = b.size;
    bitArray = b.bitArray;
}

void BitArray::swap(BitArray& b) {
    std::swap(size, b.size);
    std::swap(bitArray, b.bitArray);
}

BitArray& BitArray::operator=(const BitArray& b) {
    if (this != &b) {
        size = b.size;
        bitArray = b.bitArray;
    }
    return *this;
}

void BitArray::resize(int num_bits, bool value) {
    if(num_bits < 0){
        throw std::invalid_argument("Number of bits cannot be negative");
    }
    int oldSize = size;
    size = num_bits;
    bitArray.resize((num_bits + sizeof(arrayType) - 1) / sizeof(arrayType));
  
    if ((oldSize < size) && value) {
        for (int i = oldSize; i < size; ++i) {
            bitArray[i / sizeof(arrayType)] |= (1u << (i % sizeof(arrayType)));
        }
    }
}

void BitArray::clear() {
    bitArray.clear();
    size = 0;
}

void BitArray::push_back(bool bit) {
    resize(size + 1, bit);
}

BitArray& BitArray::operator&=(const BitArray& b) {
    if (size != b.size) {
        throw std::invalid_argument("Size mismatch");
    }
    for (int i = 0; i < size; i++) {
        if (operator[](i) && !b.operator[](i))
            reset(i);
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    if (size != b.size) {
        throw std::invalid_argument("Size mismatch");
    }
    for (int i = 0; i < size; i++) {
        if (!operator[](i) && b.operator[](i))
            set(i);
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    if (size != b.size) {
        throw std::invalid_argument("Size mismatch");
    }
    for (int i = 0; i < size; i++) {
        if (!operator[](i) && b.operator[](i))
            set(i);
        else if (operator[](i) && b.operator[](i))
            reset(i);
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    if (n == 0) {
        return *this;
    }
    for (int i = 0; i < size; i++) {
        set(i, (i + n >= size ? 0 : (*this)[i + n]));
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n == 0) {
        return *this;
    }
    for (int i = size - 1; i >= 0; i--) {
        set(i, (i - n < 0 ? 0 : (*this)[i - n]));
    }
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    BitArray result(*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

BitArray& BitArray::set(int n, bool val) {
    if(n < 0) {
        throw std::invalid_argument("Index cannot be negative");
    }
    if (n >= size) {
        resize(n + 1);
    }
    int byte = n / sizeof(arrayType);
    int bit = n % sizeof(arrayType);
        
    if (val) {
        bitArray[byte] |= (1 << bit);
    }
    else {
        bitArray[byte] &= ~(1 << bit);
    }
    return *this;
}

BitArray& BitArray::set() {
    for (arrayType& number : bitArray) {
        number = ~0u;
    }
    return *this;
}

BitArray& BitArray::reset(int n) {
    set(n, false);
    return *this;
}

BitArray& BitArray::reset() {
    bitArray.assign(bitArray.size(), 0);
    return *this;
}

bool BitArray::any() const {
    for (const arrayType& number : bitArray) {
        if (number != 0) {
            return true;
        }
    }
    return false;
}

bool BitArray::none() const {
    return !any();
}

BitArray BitArray::operator~() const {
    BitArray result(*this);
    for (arrayType& number : result.bitArray) {
        number = ~number;
    }
    return result;
}

int BitArray::count() const {
    int result = 0;
    for (const arrayType& number : bitArray) {
        int current = number;
        while (current >= 1) {
            result += current % 2;
            current /= 2;
        }
    }
    return result;
}

bool BitArray::operator[](int i) const {
    if(i < 0){
        throw std::invalid_argument("Index cannot be negative");
    }
    if (i >= size || size == 0) {
        throw std::out_of_range("Index out of range");
    }
    int byte = i / sizeof(arrayType);
    int bit = i % sizeof(arrayType);  
    return (bitArray[byte] >> bit) & 1;
}

size_t BitArray::Size() const {
    return size;
}

bool BitArray::empty() const {
    return size == 0;
}

std::string BitArray::to_string() const {
    std::string result;
    for (const arrayType& number : bitArray) {
        for (int i = 0; i < (int)sizeof(arrayType); i++) {
            result += ((number >> i) & 1) ? '1' : '0';
        }
    }
    result.resize(size);
    reverse(result.begin(), result.end()); 
    return result;
}

bool operator==(const BitArray& a, const BitArray& b) {
    return (a.size == b.size) && (a.bitArray == b.bitArray);
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !operator==(a, b);
}

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result &= b2;
    return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result |= b2;
    return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result ^= b2;
    return result;
}
