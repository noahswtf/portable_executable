#include "dos_header.hpp"

portable_executable::nt_headers_t* portable_executable::dos_header_t::nt_headers()
{
    return reinterpret_cast<nt_headers_t*>(reinterpret_cast<std::uintptr_t>(this) + this->e_lfanew);
}

const portable_executable::nt_headers_t* portable_executable::dos_header_t::nt_headers() const
{
    return reinterpret_cast<const nt_headers_t*>(reinterpret_cast<std::uintptr_t>(this) + this->e_lfanew);
}
