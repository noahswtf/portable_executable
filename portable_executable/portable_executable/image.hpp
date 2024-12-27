#pragma once

#include "dos_header.hpp"
#include "nt_headers.hpp"

#include "section_header.hpp"
#include "export_directory.hpp"
#include "imports_directory.hpp"
#include "relocations_directory.hpp"

namespace portable_executable
{
	class image_t
	{
	private:
		dos_header_t m_dos_header;

	public:
		dos_header_t* dos_header();

		const dos_header_t* dos_header() const;
		
		nt_headers_t* nt_headers();

		const nt_headers_t* nt_headers() const;

		section_iterator_t<section_header_t> sections()
		{
			return { this->nt_headers()->section_headers(), this->nt_headers()->num_sections() };
		}

		section_iterator_t<const section_header_t> sections() const
		{
			return { this->nt_headers()->section_headers(), this->nt_headers()->num_sections() };
		}

		section_header_t* find_section(std::string_view name);

		exports_range_t<exports_iterator_t> exports()
		{
			data_directory_t data_directory = this->nt_headers()->optional_header.data_directories.export_directory;

			if (!data_directory.present())
			{
				return { };
			}

			auto module = reinterpret_cast<std::uint8_t*>(this);

			auto export_directory = reinterpret_cast<export_directory_t*>(module + data_directory.virtual_address);

			auto names = reinterpret_cast<std::uint32_t*>(module + export_directory->address_of_names);
			auto functions = reinterpret_cast<std::uint32_t*>(module + export_directory->address_of_functions);
			auto ordinals = reinterpret_cast<std::uint16_t*>(module + export_directory->address_of_name_ordinals);

			return { module, names, functions, ordinals, export_directory->number_of_names };
		}

		exports_range_t<const exports_iterator_t> exports() const
		{
			data_directory_t data_directory = this->nt_headers()->optional_header.data_directories.export_directory;

			if (!data_directory.present())
			{
				return { };
			}

			auto module = reinterpret_cast<const std::uint8_t*>(this);

			auto export_directory = reinterpret_cast<const export_directory_t*>(module + data_directory.virtual_address);

			auto names = reinterpret_cast<const std::uint32_t*>(module + export_directory->address_of_names);
			auto functions = reinterpret_cast<const std::uint32_t*>(module + export_directory->address_of_functions);
			auto ordinals = reinterpret_cast<const std::uint16_t*>(module + export_directory->address_of_name_ordinals);

			return { module, names, functions, ordinals, export_directory->number_of_functions };
		}

		imports_range_t<imports_iterator_t> imports()
		{
			data_directory_t data_directory = this->nt_headers()->optional_header.data_directories.import_directory;

			if (!data_directory.present())
			{
				return { };
			}

			auto module = reinterpret_cast<std::uint8_t*>(this);

			return { module, data_directory.virtual_address };
		}

		imports_range_t<const imports_iterator_t> imports() const
		{
			data_directory_t data_directory = this->nt_headers()->optional_header.data_directories.import_directory;

			if (!data_directory.present())
			{
				return { };
			}

			auto module = reinterpret_cast<const std::uint8_t*>(this);

			return { module, data_directory.virtual_address };
		}

		relocations_range_t<relocations_iterator_t> relocations()
		{
			data_directory_t data_directory = this->nt_headers()->optional_header.data_directories.basereloc_directory;

			if (!data_directory.present())
			{
				return { };
			}

			auto module = reinterpret_cast<std::uint8_t*>(this);

			return { module, data_directory.virtual_address };
		}

		relocations_range_t<const relocations_iterator_t> relocations() const
		{
			data_directory_t data_directory = this->nt_headers()->optional_header.data_directories.basereloc_directory;

			if (!data_directory.present())
			{
				return { };
			}

			auto module = reinterpret_cast<const std::uint8_t*>(this);

			return { module, data_directory.virtual_address };
		}
	};
}