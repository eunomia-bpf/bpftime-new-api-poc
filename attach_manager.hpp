#ifndef _BPFTIME_ATTACH_MANAGER_H
#define _BPFTIME_ATTACH_MANAGER_H
#include "base_event_provider.hpp"
#include "nginx_module_attach_impl.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
namespace bpftime {

struct bpftime_program_object {
    std::vector<uint64_t> insns;
    std::string name;
    int prog_type;
};
struct attach_link_object {};
struct unused_object;
using objects =
    std::variant<unused_object, bpftime_program_object, attach_link_object>;

struct event_provider_impl : public base_event_provider {
public:
private:
  class attach_manager *man;
  event_provider_impl(class attach_manager *man) : man(man) {}
  int create_bpftime_program(const void *insn, std::size_t insn_cnt,
                             const char *prog_name, int prog_type);
  int attach_bpftime_program_at(int prog_id, int attach_target_id);
  int enable_attach_link(int attach_link_id);
  int create_nginx_url_handler_attach_target(const char *url);
  int destroy_object(int object_id);
  int instantiate_attach();
  int deinstantiate_attach();
};

struct bpftime_shm_client {
  std::unordered_map<int, objects> objects;
};

struct attach_manager {
  // In bpftime, this should be stored in shm
  bpftime_shm_client shm;
  friend struct event_provider_impl;
  int next_id = 1;
  int allocate_id();
  std::unique_ptr<nginx_module_attach_impl> nginx_attach_impl;
  std::unique_ptr<event_provider_impl> event_provider;
};

} // namespace bpftime
#endif
