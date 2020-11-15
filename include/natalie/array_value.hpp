#pragma once

#include <initializer_list>
#include <utility>

#include "natalie/class_value.hpp"
#include "natalie/forward.hpp"
#include "natalie/global_env.hpp"
#include "natalie/macros.hpp"
#include "natalie/value.hpp"
#include "natalie/vector.hpp"

namespace Natalie {

struct ArrayValue : Value {
    ArrayValue(Env *env)
        : Value { Value::Type::Array, env->Array() } { }

    ArrayValue(Env *env, ClassValue *klass)
        : Value { Value::Type::Array, klass } { }

    ArrayValue(Env *env, std::initializer_list<Value *> list)
        : ArrayValue { env } {
        m_vector.set_capacity(list.size());
        for (auto &v : list) {
            m_vector.push(v);
        }
    }

    ArrayValue(ArrayValue &other)
        : Value { other.type(), other.klass() }
        , m_vector { other.m_vector } { }

    ArrayValue(Env *env, ssize_t argc, Value **args)
        : ArrayValue { env } {
        for (ssize_t i = 0; i < argc; i++) {
            push(args[i]);
        }
    }

    // Array[]
    static Value *square_new(Env *env, ssize_t argc, Value **args) {
        return new ArrayValue { env, argc, args };
    }

    Value *to_ary() { return this; }

    size_t size() const { return m_vector.size(); }

    void push(Value &val) {
        m_vector.push(&val);
    }

    void push(Value *val) {
        m_vector.push(val);
    }

    Value *pop(Env *);

    Value *&operator[](size_t index) const {
        assert(index < m_vector.size());
        return m_vector[index];
    }

    void concat(ArrayValue &other) {
        for (Value *v : other) {
            push(v);
        }
    }

    void push_splat(Env *, Value *);

    void expand_with_nil(Env *, size_t);

    void overwrite(ArrayValue &other) {
        m_vector.set_size(0);
        for (Value *v : other) {
            push(v);
        }
    }

    Value **data() {
        return m_vector.data();
    }

    void sort_in_place(Env *);

    Vector<Value *>::iterator begin() noexcept { return m_vector.begin(); }
    Vector<Value *>::iterator end() noexcept { return m_vector.end(); }

    bool is_empty() { return m_vector.is_empty(); }

    Value *initialize(Env *, Value *, Value *);
    Value *inspect(Env *);
    Value *ltlt(Env *, Value *);
    Value *add(Env *, Value *);
    Value *sub(Env *, Value *);
    Value *ref(Env *, Value *, Value *);
    Value *refeq(Env *, Value *, Value *, Value *);
    Value *any(Env *, ssize_t, Value **, Block *);
    Value *eq(Env *, Value *);
    Value *eql(Env *, Value *);
    Value *each(Env *, Block *);
    Value *each_with_index(Env *, Block *);
    Value *map(Env *, Block *);
    Value *first(Env *);
    Value *last(Env *);
    Value *include(Env *, Value *);
    Value *sort(Env *);
    Value *shift(Env *, Value *);
    Value *join(Env *, Value *);
    Value *cmp(Env *, Value *);
    Value *select(Env *, Block *);
    Value *sample(Env *);

private:
    ArrayValue(Env *env, Vector<Value *> &&vector)
        : Value { Value::Type::Array, env->Array() }
        , m_vector { std::move(vector) } { }

    Vector<Value *> m_vector {};
};

}
