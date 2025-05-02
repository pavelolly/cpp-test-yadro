#pragma once

#include <iostream>
#include <memory>


class Serializer {
public:
    template <typename T>
    Serializer(const T& obj) : serialazable_(std::make_unique<Serializable<T>>(obj)) {}

    void Dump(std::ostream &os) const {
        serialazable_->Dump(os);
    }
private:
    struct ISerializable {
        virtual void Dump(std::ostream &os) const = 0;
    };

    template <typename T>
    class Serializable : public ISerializable {
    public:
        Serializable(const T& obj) : obj_(obj) {}

        void Dump(std::ostream &os) const override {
            ::Dump(os, obj_);
        }
    private:
        const T &obj_;
    };

    std::unique_ptr<ISerializable> serialazable_;
};