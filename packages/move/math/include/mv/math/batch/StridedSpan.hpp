#pragma once

#include <cassert>
#include <cstddef>
#include <span>
#include <type_traits>

namespace mv::math
{
    template <typename T>
    class StridedSpan
    {
    public:
        using Element = T;
        using Value = std::remove_const_t<T>;
        using Pointer = T*;
        using Reference = T&;

        constexpr StridedSpan() noexcept = default;

        constexpr StridedSpan(Pointer first,
                              std::size_t count,
                              std::size_t strideBytes) noexcept :
            First_(reinterpret_cast<BytePointer>(first)),
            Count_(count),
            StrideBytes_(strideBytes)
        {
            assert(count == 0U || first != nullptr);
            assert(count == 0U || strideBytes >= sizeof(Value));
        }

        [[nodiscard]] constexpr std::size_t Size() const noexcept
        {
            return Count_;
        }

        [[nodiscard]] constexpr bool Empty() const noexcept
        {
            return Count_ == 0U;
        }

        [[nodiscard]] constexpr std::size_t StrideBytes() const noexcept
        {
            return StrideBytes_;
        }

        [[nodiscard]] Reference operator[](std::size_t index) const noexcept
        {
            assert(index < Count_);
            return *reinterpret_cast<Pointer>(First_ + index * StrideBytes_);
        }

    private:
        using Byte =
            std::conditional_t<std::is_const_v<T>, const std::byte, std::byte>;
        using BytePointer = Byte*;

        BytePointer First_{};
        std::size_t Count_{};
        std::size_t StrideBytes_{};
    };

    template <typename Record, typename Field>
    [[nodiscard]] StridedSpan<Field> MakeFieldView(
        std::span<Record> records, Field Record::* member) noexcept
    {
        if (records.empty())
        {
            return {};
        }

        return StridedSpan<Field>(&(records.front().*member), records.size(),
                                  sizeof(Record));
    }

    template <typename Record, typename Field>
    [[nodiscard]] StridedSpan<const Field> MakeFieldView(
        std::span<const Record> records, Field Record::* member) noexcept
    {
        if (records.empty())
        {
            return {};
        }

        return StridedSpan<const Field>(&(records.front().*member),
                                        records.size(), sizeof(Record));
    }
}  // namespace mv::math
