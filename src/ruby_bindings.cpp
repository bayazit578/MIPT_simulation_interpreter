#include <elfio/elfio.hpp>
#include <rice/rice.hpp>
#include <rice/stl.hpp>

#include "elf_ext.hpp"

extern "C" void Init_bindings() {
  Rice::define_class<ElfWriter>("ElfWriter")
    .define_constructor(Rice::Constructor<ElfWriter>())
    .define_method("add_text", &ElfWriter::add_text)
    .define_method("save", &ElfWriter::save);
}
