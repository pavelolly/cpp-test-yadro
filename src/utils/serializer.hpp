#pragma once

#include <iostream>
#include <memory>

/*
    This file should be included after all necessary
    Dump(std::ostream&, <whatever>)
    Load(std::istream&, <whatever>)
    functions are decalred
*/

class Serializer {
public:
    Serializer() = default;

    template <typename T>
    Serializer(const T& obj)
        : serialazable_(std::make_unique<Serializable<T>>(obj))
    {}

    void Dump(std::ostream &os) const {
        if (serialazable_) {
            serialazable_->Dump(os);
        }
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