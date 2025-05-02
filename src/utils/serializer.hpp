#pragma once

#include <iostream>
#include <memory>

/*
    This file should be included after all necessary
    Dump(std::ostream&, <whatever>) functions are decalred
*/

class Serializer {
public:
    template <typename T>
    Serializer(const T& obj)
        : serialazable_(std::make_unique<Serializable<T>>(obj))
    {}

    void Dump(std::ostream &os) const {
        serialazable_->Dump(os);
    }
private:
    struct ISerializable {
        virtual void Dump(std::ostream &os) const = 0;
        virtual ~ISerializable() = default;
    };

    template <typename T>
    class Serializable : public ISerializable {
    public:
        Serializable(const T& obj)
            : obj_(obj)
        {}

        void Dump(std::ostream &os) const override {
            ::Dump(os, obj_);
        }
    private:
        const T &obj_;
    };

    std::unique_ptr<ISerializable> serialazable_;
};

template <typename T>
    requires requires(const T &t) {
        Dump(std::declval<std::ostream&>(), t);
    }
std::ostream &operator <<(std::ostream &os, const T& t) {
    Dump(os, t);
    return os;
}