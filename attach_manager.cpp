#include "attach_manager.hpp"
#include "event_provider_impl.hpp"
#include "nginx_module_attach_impl.hpp"
#include <iostream>
#include <memory>
#include <ostream>

using namespace bpftime;
int attach_manager::allocate_id() { return next_id++; }
int local_instantiated_bpftime_program::run(const void *mem, size_t mem_size,
                                            uint64_t *ret) {
  std::cout << "Program run! name=" << name << ", url = {}" << (char *)mem
            << std::endl;
  return 0;
}
attach_manager::attach_manager() {
  nginx_attach_impl = std::make_unique<nginx_module_attach_impl>();
  event_provider =
      std::make_unique<event_provider_impl>(event_provider_impl(this));
}
