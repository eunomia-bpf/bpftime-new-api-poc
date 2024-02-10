#include "attach_manager.hpp"
#include <vector>

using namespace bpftime;
int attach_manager::allocate_id() { return next_id++; }
int event_provider_impl::create_bpftime_program(const void *insn,
                                                std::size_t insn_cnt,
                                                const char *prog_name,
                                                int prog_type) {
  int id = man->allocate_id();
  man->shm.objects.emplace(
      id,
      bpftime_program_object{.insns = std::vector<uint64_t>(
                                 (uint64_t *)insn, (uint64_t *)insn + insn_cnt),
                             .name = prog_name,
                             .prog_type = prog_type});
  return id;
}
int event_provider_impl::attach_bpftime_program_at(int prog_id,
                                                   int attach_target_id) {}
int event_provider_impl::enable_attach_link(int attach_link_id) {}
int event_provider_impl::create_nginx_url_handler_attach_target(
    const char *url) {}
int event_provider_impl::destroy_object(int object_id) {}
int event_provider_impl::instantiate_attach() {}
int event_provider_impl::deinstantiate_attach() {}
