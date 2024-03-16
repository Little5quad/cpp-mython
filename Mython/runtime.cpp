#include "runtime.h"

#include <cassert>
#include <optional>
#include <sstream>

using namespace std;

namespace runtime {

    ObjectHolder::ObjectHolder(std::shared_ptr<Object> data)
        : data_(std::move(data)) {
    }

    void ObjectHolder::AssertIsValid() const {
        assert(data_ != nullptr);
    }

    ObjectHolder ObjectHolder::Share(Object& object) {
        // Возвращаем невладеющий shared_ptr (его deleter ничего не делает)
        return ObjectHolder(std::shared_ptr<Object>(&object, [](auto* /*p*/) { /* do nothing */ }));
    }

    ObjectHolder ObjectHolder::None() {
        return ObjectHolder();
    }

    Object& ObjectHolder::operator*() const {
        AssertIsValid();
        return *Get();
    }

    Object* ObjectHolder::operator->() const {
        AssertIsValid();
        return Get();
    }

    Object* ObjectHolder::Get() const {
        return data_.get();
    }

    ObjectHolder::operator bool() const {
        return Get() != nullptr;
    }

    bool IsTrue(const ObjectHolder& object) {

        auto ptr_number = object.TryAs<Number>();
        auto ptr_string = object.TryAs<String>();
        auto ptr_bool = object.TryAs<Bool>();
        auto ptr_bool_vo = object.TryAs<ValueObject<bool>>();

        if (ptr_number) {
            return ptr_number->GetValue();
        }
        else if (ptr_string) {
            if (ptr_string->GetValue() == "") {
                return false;
            }
            else {
                return true;
            }
        }
        else if (ptr_bool) {
            return ptr_bool->GetValue();
        }
        else if (ptr_bool_vo) {
            return ptr_bool_vo->GetValue();
        }

        return false;
    }

    void ClassInstance::Print(std::ostream& os, Context& context) {

        auto ptr_method = class_.GetMethod("__str__");
        if (ptr_method) {
            Call(ptr_method->name, {}, context)->Print(os, context);
        }
        else {
            os << this;
        }
    }

    bool ClassInstance::HasMethod(const std::string& method, size_t argument_count) const {

        auto ptr_method = class_.GetMethod(method);
        if (ptr_method && ptr_method->formal_params.size() == argument_count) {
            return true;
        }

        return false;
    }

    Closure& ClassInstance::Fields() {
        return fields_;
    }

    const Closure& ClassInstance::Fields() const {
        return fields_;
    }

    ClassInstance::ClassInstance(const Class& cls) : class_(cls) {
    }

    ObjectHolder ClassInstance::Call(const std::string& method,
        const std::vector<ObjectHolder>& actual_args,
        Context& context) {

        if (!this->HasMethod(method, actual_args.size())) {
            throw std::runtime_error("No methods");
        }
        auto ptr_method = class_.GetMethod(method);
        Closure cls;
        cls["self"] = ObjectHolder::Share(*this);

        for (size_t i = 0; i < ptr_method->formal_params.size(); ++i) {
            cls[ptr_method->formal_params[i]] = actual_args[i];
        }

        return ptr_method->body->Execute(cls, context);
    }

    Class::Class(std::string /*name*/, std::vector<Method> /*methods*/, const Class* /*parent*/) {
        // Реализуйте метод самостоятельно
    }

    const Method* Class::GetMethod(const std::string& /*name*/) const {
        // Заглушка. Реализуйте метод самостоятельно
        return nullptr;
    }

    [[nodiscard]] inline const std::string& Class::GetName() const {
        // Заглушка. Реализуйте метод самостоятельно.
        throw std::runtime_error("Not implemented"s);
    }

    void Class::Print(ostream& /*os*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
    }

    void Bool::Print(std::ostream& os, [[maybe_unused]] Context& context) {
        os << (GetValue() ? "True"sv : "False"sv);
    }

    bool Equal(const ObjectHolder& /*lhs*/, const ObjectHolder& /*rhs*/, Context& /*context*/) {
        // Заглушка. Реализуйте функцию самостоятельно
        throw std::runtime_error("Cannot compare objects for equality"s);
    }

    bool Less(const ObjectHolder& /*lhs*/, const ObjectHolder& /*rhs*/, Context& /*context*/) {
        // Заглушка. Реализуйте функцию самостоятельно
        throw std::runtime_error("Cannot compare objects for less"s);
    }

    bool NotEqual(const ObjectHolder& /*lhs*/, const ObjectHolder& /*rhs*/, Context& /*context*/) {
        // Заглушка. Реализуйте функцию самостоятельно
        throw std::runtime_error("Cannot compare objects for equality"s);
    }

    bool Greater(const ObjectHolder& /*lhs*/, const ObjectHolder& /*rhs*/, Context& /*context*/) {
        // Заглушка. Реализуйте функцию самостоятельно
        throw std::runtime_error("Cannot compare objects for equality"s);
    }

    bool LessOrEqual(const ObjectHolder& /*lhs*/, const ObjectHolder& /*rhs*/, Context& /*context*/) {
        // Заглушка. Реализуйте функцию самостоятельно
        throw std::runtime_error("Cannot compare objects for equality"s);
    }

    bool GreaterOrEqual(const ObjectHolder& /*lhs*/, const ObjectHolder& /*rhs*/, Context& /*context*/) {
        // Заглушка. Реализуйте функцию самостоятельно
        throw std::runtime_error("Cannot compare objects for equality"s);
    }

}  // namespace runtime