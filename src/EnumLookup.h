#pragma once

#include <iostream>
#include <map>
#include <deque>
#include <initializer_list>
#include <utility> 
#include <string>

template <typename simpleType, typename EnumType, EnumType lastEnumerator>
class EnumLookup {
private:
    std::map<simpleType, EnumType> forwardMap;
    std::map<EnumType, simpleType> reverseMap;

public:
    EnumLookup(std::initializer_list<std::pair<EnumType, simpleType>> reverseItems) {
        for (const auto& item : reverseItems) {
            reverseMap.insert(item);
            const std::pair<simpleType, EnumType> ritem = { item.second, item.first };
            forwardMap.insert(ritem);
        }
    }

    EnumLookup(std::initializer_list<std::pair<simpleType, EnumType>> forwardItems) {
        for (const auto& ritem : forwardItems) {
            forwardMap.insert(ritem);
            const std::pair<EnumType, simpleType> item = { ritem.second, ritem.first };
            reverseMap.insert(item);
        }
    }

    EnumLookup(std::deque<simpleType> in_deque) {
        auto totalElements = static_cast<int>(lastEnumerator);
        if (totalElements != (in_deque.size() - 1)) {
            std::cerr << "ERROR: WRONG SIZE - EnumLookup - TotalElements: " << totalElements << ", deque size: " << in_deque.size() << std::endl;
            return;
        }
        for (int et = 0; et < totalElements; ++et) {
            simpleType c = in_deque.front();
            in_deque.pop_front();
            const std::pair<simpleType, EnumType> item = { c, static_cast<EnumType>(et) };
            const std::pair<EnumType, simpleType> ritem = { static_cast<EnumType>(et), c };
            forwardMap.insert(item);
            reverseMap.insert(ritem);
            //std::cout << ".. new deque size: " << in_deque.size() << std::endl ;
        }
    }

    simpleType toSimpleType(EnumType in) const {
        auto it = reverseMap.find(in);
        if (it != reverseMap.end()) {
            return it->second;
        }
        return simpleType();
    };

    EnumType toEnumType(simpleType in) {
        auto it = forwardMap.find(in);
        if (it != forwardMap.end()) {
            return it->second;
        }
        //throw std::invalid_argument("Invalid input for forward lookup");
        return EnumType();
    }

    // Iterators for Simple Type -> Enum Type (forwardMap)
    auto getIteratorSimpleTypeBegin() { return forwardMap.begin(); }
    auto getIteratorSimpleTypeEnd() { return forwardMap.end(); }

    auto getIteratorSimpleTypeBegin() const { return forwardMap.cbegin(); }
    auto getIteratorSimpleTypeEnd() const { return forwardMap.cend(); }

    // Iterators for Enum Type -> Simple Type (reverseMap)
    auto getIteratorEnumTypeBegin() { return reverseMap.begin(); }
    auto getIteratorEnumTypeEnd() { return reverseMap.end(); }

    auto getIteratorEnumTypeBegin() const { return reverseMap.cbegin(); }
    auto getIteratorEnumTypeEnd() const { return reverseMap.cend(); }

};
