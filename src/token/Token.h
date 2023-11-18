//
// Created by thierrycd on 2023-11-16.
//

#pragma once

#include <optional>
#include <string>
#include <utility>


enum class TokenType
{
    kill,
    int_lit,
    endl,
    variable,
    ident,
    assign
};

struct Token
{
    TokenType type;
    std::optional<std::string> val{};

    explicit Token(TokenType type_, std::optional<std::string> val_ = {}) : type(type_), val(std::move(val_)){};
};
