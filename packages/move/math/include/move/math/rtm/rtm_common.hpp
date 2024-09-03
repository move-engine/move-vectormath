#pragma once

#define MVM_TYPE_WRAPPER(name, vector_type) \
    struct name                             \
    {                                       \
        using type = vector_type;           \
    };
