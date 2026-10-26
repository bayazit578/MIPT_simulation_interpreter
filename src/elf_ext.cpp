#include "elf_ext.hpp"

ElfWriter::ElfWriter() {
  elf_.create     (ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB);
  elf_.set_type   (ELFIO::ET_EXEC);
  elf_.set_machine(ELFIO::EM_NONE);
}

void ElfWriter::add_text(const std::string& code, std::size_t addr) {
  auto section = elf_.sections.add(".text");

  section->set_type (ELFIO::SHT_PROGBITS);
  section->set_flags(ELFIO::SHF_ALLOC | ELFIO::SHF_EXECINSTR);

  section->set_addr_align(sizeof(Word));
  section->set_address   (addr);
  section->set_data      (code.data(), code.size());

  const auto& segment = elf_.segments.add();
  
  segment->set_type            (ELFIO::PT_LOAD);
  segment->set_virtual_address (addr);
  segment->set_physical_address(addr);
  segment->set_flags           (ELFIO::PF_R | ELFIO::PF_X);

  segment->set_align  (kPageSize);
  segment->add_section(section, kPageSize);

  elf_.set_entry(addr);
}

void ElfWriter::save(const std::string& fname) {
  if (!elf_.save(fname)) {
    throw std::runtime_error("elfwriter: could not save file");
  }
}

void ElfReader::load_program(const std::string fname) {
  if (!elf_.load(fname)) {
    throw std::runtime_error("elfreader: can't open the file");
  }
}

ElfReader::SegmentTable ElfReader::load_segments() const {
  SegmentTable result;

  for (const auto& segment : elf_.segments) {
    if (segment->get_type() != ELFIO::PT_LOAD) {
      continue;
    }

    Segment info;

    info.addr        = segment->get_virtual_address();
    info.memory_size = segment->get_memory_size();
    info.flags       = segment->get_flags();

    auto size         = segment->get_file_size();
    const Byte* bytes = reinterpret_cast<const Byte*>(segment->get_data());

    if (size > info.memory_size || (size > 0 && !bytes)) {
      throw std::runtime_error("elfreader: invalid segment");
    }
    if (size > 0) {
      info.data.assign(bytes, bytes + size);
    }

    result.push_back(std::move(info));
  }

  return result;
}
