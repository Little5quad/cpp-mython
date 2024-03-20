#include "statement.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace ast {

    using runtime::Closure;
    using runtime::Context;
    using runtime::ObjectHolder;

    namespace {
        const string ADD_METHOD = "__add__"s;
        const string INIT_METHOD = "__init__"s;
    }  // namespace

    ObjectHolder Assignment::Execute(Closure& closure, Context& context) {
        closure[var_] = move(rv_->Execute(closure, context));
        return closure.at(var_);
    }

    Assignment::Assignment(std::string var, std::unique_ptr<Statement> rv)
        : var_(move(var)), rv_(move(rv)) {
    }

    VariableValue::VariableValue(const std::string& var_name) {
        dotted_ids_.push_back(var_name);
    }

    VariableValue::VariableValue(std::vector<std::string> dotted_ids)
        : dotted_ids_(move(dotted_ids)) {
    }

    ObjectHolder VariableValue::Execute(Closure& closure, Context& /*context*/) {
        Closure* ptr_clos = &closure;
        runtime::Closure::iterator it_object;

        for (const auto& variable : dotted_ids_) {
            it_object = ptr_clos->find(variable);
            if (ptr_clos->end() == it_object) {
                throw runtime_error("Not found variable");
            }
            auto ptr_class = it_object->second.TryAs<runtime::ClassInstance>();
            if (!ptr_class) {
                return it_object->second;
            }
            ptr_clos = &ptr_class->Fields();
        }
        return it_object->second;
    }

    unique_ptr<Print> Print::Variable(const std::string& name) {
        return make_unique<Print>(make_unique<VariableValue>(name));
    }

    Print::Print(unique_ptr<Statement> argument) {
        args_.push_back(move(argument));
    }

    Print::Print(vector<unique_ptr<Statement>> args)
        : args_(move(args)) {
    }

    ObjectHolder Print::Execute(Closure& closure, Context& context) {
        auto& out = context.GetOutputStream();
        ObjectHolder object;

        for (const auto& arg : args_) {
            if (arg != args_.front()) {
                out << " ";
            }
            object = arg->Execute(closure, context);

            if (object) {
                object->Print(out, context);
            }
            else {
                out << "None";
            }
        }
        out << "\n";
        return object;
    }

    MethodCall::MethodCall(std::unique_ptr<Statement> object, std::string method,
        std::vector<std::unique_ptr<Statement>> args)
        : object_(move(object)), method_(move(method)), args_(move(args)) {
    }

    ObjectHolder MethodCall::Execute(Closure& closure, Context& context) {
        auto ptr_class = object_->Execute(closure, context).TryAs<runtime::ClassInstance>();
        std::vector<runtime::ObjectHolder> args;

        for (const auto& arg : args_) {
            args.push_back(std::move(arg->Execute(closure, context)));
        }

        return ptr_class->Call(method_, args, context);
    }


    ObjectHolder Stringify::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ObjectHolder Add::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ObjectHolder Sub::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ObjectHolder Mult::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ObjectHolder Div::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ObjectHolder Compound::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ObjectHolder Return::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ClassDefinition::ClassDefinition(ObjectHolder /*cls*/) {
        // Заглушка. Реализуйте метод самостоятельно
    }

    ObjectHolder ClassDefinition::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    FieldAssignment::FieldAssignment(VariableValue /*object*/, std::string /*field_name*/,
        std::unique_ptr<Statement> /*rv*/) {
    }

    ObjectHolder FieldAssignment::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    IfElse::IfElse(std::unique_ptr<Statement> /*condition*/, std::unique_ptr<Statement> /*if_body*/,
        std::unique_ptr<Statement> /*else_body*/) {
        // Реализуйте метод самостоятельно
    }

    ObjectHolder IfElse::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ObjectHolder Or::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ObjectHolder And::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    ObjectHolder Not::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    Comparison::Comparison(Comparator /*cmp*/, unique_ptr<Statement> lhs, unique_ptr<Statement> rhs)
        : BinaryOperation(std::move(lhs), std::move(rhs)) {
        // Реализуйте метод самостоятельно
    }

    ObjectHolder Comparison::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    NewInstance::NewInstance(const runtime::Class& /*class_*/, std::vector<std::unique_ptr<Statement>> /*args*/) {
        // Заглушка. Реализуйте метод самостоятельно
    }

    NewInstance::NewInstance(const runtime::Class& /*class_*/) {
        // Заглушка. Реализуйте метод самостоятельно
    }

    ObjectHolder NewInstance::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    MethodBody::MethodBody(std::unique_ptr<Statement>&& /*body*/) {
    }

    ObjectHolder MethodBody::Execute(Closure& /*closure*/, Context& /*context*/) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

}  // namespace ast