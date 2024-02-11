#ifndef _BPFTIME_EVENT_PROVIDER_IMPL_HPP
#define _BPFTIME_EVENT_PROVIDER_IMPL_HPP
#include "base_event_provider.hpp"
#include <set>
namespace bpftime {
struct event_provider_impl : public base_event_provider {
  friend class attach_manager;

private:
  class attach_manager *man;

  event_provider_impl(class attach_manager *man) : man(man) {}
  int instantiate_id(int id, std::set<int> &instack);

public:
  int create_bpftime_program(const void *insn, std::size_t insn_cnt,
                             const char *prog_name, int prog_type);
  int attach_bpftime_program_at(int prog_id, int attach_target_id);
  int enable_attach_link(int attach_link_id);
  int create_nginx_url_handler_attach_target();
  int destroy_object(int object_id);
  int instantiate_attach();
  int deinstantiate_attach();
};
} // namespace bpftime
#endif
