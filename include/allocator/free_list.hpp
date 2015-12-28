#ifndef SIPLASPLAS_ALLOCATOR_FREE_LIST_HPP
#define SIPLASPLAS_ALLOCATOR_FREE_LIST_HPP

#include <cassert>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "allocator/allocator_utils.hpp"


namespace cpp
{
    class FreeList
    {
    public:
        FreeList(void* begin, void* end, std::size_t block_length, std::size_t alignment, std::size_t offset = 0)
        {    
            char* begin_ = reinterpret_cast<char*>(begin);
            char* end_ = reinterpret_cast<char*>(end);
            block_length = std::max(block_length, sizeof(void*));

            char* aligned_begin = detail::aligned_ptr(begin_ + offset, alignment);
            
            if(aligned_begin < end_)
                _next = reinterpret_cast<FreeList*>(aligned_begin);

            while(aligned_begin < end_)
            {
                char* next = detail::aligned_ptr(aligned_begin + offset + block_length, alignment);

                if(next < end_)
                {
                    detail::write_at(aligned_begin, next);
                }
                else
                {
                    detail::write_at(aligned_begin, nullptr);
                }

                aligned_begin = next;
            }
        }

        void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0)
        {
            if(_next)
            {
                void* user_ptr = _next;

                _next = _next->_next;
                return user_ptr;
            }
            else
            {
                return nullptr;
            }
        }

        void deallocate(void* pointer, std::size_t size, std::size_t offset = 0)
        {
            FreeList* node = reinterpret_cast<FreeList*>(pointer);
            node->_next = _next;
            _next = node;
        }

        std::string dump() const
        {
            std::ostringstream os;
            const FreeList* node = _next;

            os << "free list dump:" << std::endl
               << "===============" << std::endl;

            while(node)
            {
                os << node << " -> ";
                node = node->_next;
            }

            os << "(null)";

            return os.str();
        }

    private:
        FreeList* _next;
    };
}

#endif // SIPLASPLAS_ALLOCATOR_FREE_LIST_HPP
