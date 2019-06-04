//
// Created by 解贝 on 2019-05-30.
//

#include "ast.h"

#include <type_traits>

#define INDENT(d) std::string(d, ' ')

#define PREVENT_NULL(name) do {                             \
    if (this == nullptr) {                                  \
        return INDENT(d) + name + "()";                     \
    }                                                       \
} while (0);

#define TO_STRING(d, name, args...) do {              \
        PREVENT_NULL(name);                           \
        return ToStringCommon(d, name, ##args);       \
    } while (0);

// #define TOSTRING_DEFINE(name, args...) std::string \
//     name##::ToString(u32 d) {                      \
//         PREVENT_NULL(name);                        \
//         return TO_STRING(d, name, args);           \
//     }

template <typename F, typename T, typename... Args>
std::string ArgsToString(F f, T &&first, Args &&...args) {
    return f(std::move(first)) + ArgsToString(f, std::forward<Args>(args)...);
}

template <typename F>
std::string ArgsToString(F f) {
    return "";
}

template <typename F, typename T>
std::string ArgsToString(F f, T &&first) {
    return f(std::move(first));
}

template <typename T>
class ToStringHelper {
public:
    static std::string Fn(T &&t, int d) {
        return t->ToString(d+1);
    }
};

template <typename T>
class ToStringHelper<std::vector<T>> {
public:
    static std::string Fn(std::vector<T> &&t, int d) {
        auto result = std::string();
        auto fn = [d](auto t){
            return ToStringHelper<decltype(t)>::Fn(std::move(t), d+1);
        };
        for (auto &x : t) {
            result += ArgsToString(fn, d, x) + "\n";
        }
        return result;
    }
};

template <>
class ToStringHelper<std::string> {
public:
    static std::string Fn(std::string &&t, int d) {
        return INDENT(d) + t;
    }
};

template <>
class ToStringHelper<long long> {
public:
    static std::string Fn(long long t, int d) {
        return INDENT(d) + std::to_string(t) ;
    }
};

template <>
class ToStringHelper<int> {
public:
    static std::string Fn(int t, int d) {
        return INDENT(d) + std::to_string(t);
    }
};

// generic used ToString function for AstNode's derived classes
template <typename... Args>
std::string ToStringCommon(u32 d, const std::string &name, Args &&...args) {
    auto fn = [d](auto t){
        return ToStringHelper<decltype(t)>::Fn(std::move(t), d+1) + "\n";
    };
    return INDENT(d) + name + "(\n" 
        + ArgsToString(fn, args...)
        + "\n"
        + INDENT(d) + ")";
}

template <typename T>
std::string ToStringCommon(u32 d, const std::string &name, T &&t) {
    auto fn = [d](auto t){
        return ToStringHelper<decltype(t)>::Fn(std::move(t), d+1);
    };
    return INDENT(d) + name + "(" 
        + ArgsToString(fn, std::move(t)) 
        + INDENT(d) + ")" + "\n";
}

std::string ToStringCommon(u32 d, const std::string &name) {
    return INDENT(d) + name + "()" + "\n";
}

std::string Identifier::ToString(u32 d) {
    TO_STRING(d, "Id", name_)
}

std::string Operator::ToString(u32 d) {
    TO_STRING(d, "Op", op_);
}

std::string TypeId::ToString(u32 d) {
    TO_STRING(d, "TypeId", name_);
}

std::string Expr::ToString(u32 d) {
    TO_STRING(d, "Expr", left_, ops_, rights_);
}

std::string PrimeExpr::ToString(u32 d) {
    TO_STRING(d, "PrimeExpr");
}

std::string NilExpr::ToString(u32 d) {
    TO_STRING(d, "NilExpr");
}

std::string IntExpr::ToString(u32 d) {
    TO_STRING(d, "IntExpr", num_);
}

std::string UnaryExpr::ToString(u32 d) {
    TO_STRING(d, "UnaryExpr", op_, expr_);
}

std::string StrExpr::ToString(u32 d) {
    TO_STRING(d, "StrExpr", str_);
}

std::string ArrayCreate::ToString(u32 d) {
    TO_STRING(d, "ArrayCreate", type_id_, len_, init_)
}

std::string RecordCreate::ToString(u32 d) {
    TO_STRING(d, "RecordCreate", type_id_, types_, vars_)
}

std::string ObjCreate::ToString(u32 d) {
    TO_STRING(d, "ObjCreate", type_id_);
}

std::string Var::ToString(u32 d) {
    TO_STRING(d, "Var", id_, rhs_);
}

std::string VarA::ToString(u32 d) {
    TO_STRING(d, "VarA");
}

std::string BasicVar::ToString(u32 d) {
    TO_STRING(d, "BasicVar");
}

std::string FieldVar::ToString(u32 d) {
    TO_STRING(d, "FieldVar", lvar_, name_);
}

std::string ArrayElemVar::ToString(u32 d) {
    TO_STRING(d, "ArrayElemVar", lvar_, index_);
}

std::string VarExpr::ToString(u32 d) {
    TO_STRING(d, "VarExpr", lvar_);
}

std::string FnCall::ToString(u32 d) {
    TO_STRING(d, "FnCall", name_, args_);
}

std::string MethodCall::ToString(u32 d) {
    TO_STRING(d, "MethodCall", lvar_, method_, args_);
}

std::string OpExpr::ToString(u32 d) {
    TO_STRING(d, "OpExpr", op_, lhs_, rhs_);
}

std::string Exprs::ToString(u32 d) {
    TO_STRING(d, "Exprs", exprs_);
}

std::string ExprsExpr::ToString(u32 d) {
    TO_STRING(d, "ExprsExpr", exprs_);
}

std::string AssignExpr::ToString(u32 d) {
    TO_STRING(d, "AssignExpr", lval_, expr_);
}

std::string IfExpr::ToString(u32 d) {
    TO_STRING(d, "IfExpr", if_, then_, else_);
}

std::string WhileExpr::ToString(u32 d) {
    TO_STRING(d, "WhileExpr", while_, do_);
}

std::string ForExpr::ToString(u32 d) {
    TO_STRING(d, "ForExpr", id_, from_, to_, do_);
}

std::string BreakExpr::ToString(u32 d) {
    TO_STRING(d, "BreakExpr");
}

std::string LetExpr::ToString(u32 d) {
    TO_STRING(d, "LetExpr", decs_, exprs_);
}

std::string Decs::ToString(u32 d) {
    TO_STRING(d, "Decs", decs_);
}

std::string TypeDec::ToString(u32 d) {
    TO_STRING(d, "TypeDec", name_, type_);
}

std::string ClassDef::ToString(u32 d) {
    TO_STRING(d, "ClassDef", name_, parent_, fields_);
}

std::string VarDec::ToString(u32 d) {
    TO_STRING(d, "VarDec", name_, type_, var_);
}

std::string FnDec::ToString(u32 d) {
    TO_STRING(d, "FnDec", name_, args_, body_);
}

std::string PrimDec::ToString(u32 d) {
    TO_STRING(d, "PrimDec", name_, args_, ret_);
}

std::string ImportDec::ToString(u32 d) {
    TO_STRING(d, "ImportDec", import_);
}

std::string ClassField::ToString(u32 d) {
    TO_STRING(d, "ClassField");
}

std::string ClassFields::ToString(u32 d) {
    TO_STRING(d, "ClassFields", fields_);
}

std::string AttrDec::ToString(u32 d) {
    TO_STRING(d, "AttrDec", attr_);
}

std::string MethodDec::ToString(u32 d) {
    TO_STRING(d, "MethodDec", name_, args_, ret_, body_);
}

std::string Type::ToString(u32 d) {
    TO_STRING(d, "Type");
}

std::string TypeAlias::ToString(u32 d) {
    TO_STRING(d, "TypeAlias", alias_);
}

std::string RecordDef::ToString(u32 d) {
    TO_STRING(d, "RecordDef", records_);
}

std::string ArrayDef::ToString(u32 d) {
    TO_STRING(d, "ArrayDef", type_);
}

std::string ClassTypeDef::ToString(u32 d) {
    TO_STRING(d, "ClassTypeDef", parent_, fields_);
}

std::string TypeFields::ToString(u32 d) {
    TO_STRING(d, "TypeFields", names_, types_);
}