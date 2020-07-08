#pragma once

#include "Symbol.hpp"

#include "ObjectType.hpp"
#include "ClassType.hpp"
#include "MethodType.hpp"

#include "PrimitiveSimpleType.hpp"
#include "PrimitiveArrayType.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace ast {

class ScopeLayer {
 public:
    ScopeLayer() = default;
    explicit ScopeLayer(ScopeLayer* parent) : parent_(parent) {
        parent_->AddChild(this);
    }

    ~ScopeLayer() {
        for (ScopeLayer* child : children_) {
            delete child;
        }
    }

    std::shared_ptr<PrimitiveSimpleType> DeclareSimpleVariable(Symbol symbol, SimpleType* simple_type) {
        if (Has(symbol)) {
            std::cout << symbol.GetName() << std::endl;
            throw std::runtime_error("Simple variable name already taken");
        }

        offsets_[symbol] = symbols_.size();
        symbols_.push_back(symbol);

        auto simple_variable_type = std::make_shared<PrimitiveSimpleType>(simple_type);
        values_[symbol] = simple_variable_type;
        return simple_variable_type;
    }

    std::shared_ptr<PrimitiveArrayType> DeclareArrayVariable(Symbol symbol, ArrayType* array_type) {
        if (Has(symbol)) {
            throw std::runtime_error("Array name already taken");
        }

        offsets_[symbol] = symbols_.size();
        symbols_.push_back(symbol);

        auto array_variable_type = std::make_shared<PrimitiveArrayType>(array_type);
        values_[symbol] = array_variable_type;
        return array_variable_type;
    }

    std::shared_ptr<MethodType> DeclareMethod(Symbol symbol, MethodDeclaration* method_declaration) {
        if (Has(symbol)) {
            throw std::runtime_error("Method name already taken");
        }

        offsets_[symbol] = symbols_.size();
        symbols_.push_back(symbol);

        auto method_type = std::make_shared<MethodType>(method_declaration);
        values_[symbol] = method_type;
        return method_type;
    }

    std::shared_ptr<ClassType> DeclareClass(Symbol symbol, ClassDeclaration* class_declaration) {
        if (Has(symbol)) {
            throw std::runtime_error("Class name already taken");
        }

        offsets_[symbol] = symbols_.size();
        symbols_.push_back(symbol);

        auto class_type = std::make_shared<ClassType>(class_declaration);
        values_[symbol] = class_type;
        return class_type;
    }

    // Shadow variable type by name
    void Put(Symbol symbol, std::shared_ptr<ObjectType> value) {
        ScopeLayer* current_layer = this;
        // go up in tree
        while (!current_layer->Has(symbol) && current_layer->parent_ != nullptr) {
            current_layer = current_layer->parent_;
        }
        // shadow variable type
        if (current_layer->Has(symbol)) {
            current_layer->values_[symbol] = std::move(value);
        } else {
            throw std::runtime_error("Variable not declared");
        }
    }

    // Get variable type by symbol
    std::shared_ptr<ObjectType> Get(Symbol symbol) {
        ScopeLayer* current_layer = this;
        // go up in tree
        while (!current_layer->Has(symbol) && current_layer->parent_ != nullptr) {
            current_layer = current_layer->parent_;
        }
        if (current_layer->Has(symbol)) {
            return current_layer->values_[symbol];
        } else {
            throw std::runtime_error("variable not declared");
        }
    }

    void PrintLayer(std::ofstream& out, size_t num_tabs = 0) const {
        for (auto& symbol : symbols_) {
            PrintTabs(out, num_tabs);
            out << symbol.GetName() << std::endl;
        }

        for (ScopeLayer* child : children_) {
            child->PrintLayer(out, num_tabs + 1);
        }
    }

    void AddChild(ScopeLayer* child) {
        children_.push_back(child);
    }

    ScopeLayer* GetParent() const {
        return parent_;
    }

    // Check access to variable in scope
    bool Has(Symbol symbol) const {
        return (values_.find(symbol) != values_.end());
    }

    ScopeLayer* GetChild(size_t index) const {
        return children_[index];
    }

 private:
    void PrintTabs(std::ofstream& out, size_t num_tabs) const {
        for (size_t i = 0; i < num_tabs; ++i) {
            out << "  ";
        }
    }

    std::unordered_map<Symbol, std::shared_ptr<ObjectType>> values_;

    std::unordered_map<Symbol, size_t> offsets_;
    std::vector<Symbol> symbols_;

    ScopeLayer* parent_ = nullptr;
    std::vector<ScopeLayer*> children_;
};

} // namespace ast