#ifndef _BPFTIME_ATTACH_MANAGER_H
#define _BPFTIME_ATTACH_MANAGER_H
#include "base_attach_impl.hpp"
#include "base_event_provider.hpp"
#include "event_provider_impl.hpp"
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
namespace bpftime {

struct bpftime_program_handler {
  std::vector<uint64_t> insns;
  std::string name;
  int prog_type;
};
struct attach_link_handler {
  int prog_id;
  int target_id;
  bool enabled = false;
};
struct unused_handler {};
// In the real bpftime, we may use an integer to distinguish attach target types, not an enumeration
enum class attach_target_type {
  NGINX_URL_HANDLER,
};
struct attach_target_handler {
  attach_target_type type;
};
using handlers = std::variant<unused_handler, bpftime_program_handler,
                             attach_link_handler, attach_target_handler>;

struct bpftime_shm_client {
  std::unordered_map<int, handlers> objects;
};
struct local_instantiated_bpftime_program {
  std::string name;
  int run(const void *mem, size_t mem_size, uint64_t *ret);
};
struct attach_manager {
private:
  // In bpftime, this should be stored in shm
  bpftime_shm_client shm;
  friend struct event_provider_impl;
  int next_id = 1;
  int allocate_id();
  // std::unique_ptr<base_attach_impl> nginx_attach_impl;
  std::map<attach_target_type, std::unique_ptr<base_attach_impl>> attach_impls;
  std::unique_ptr<base_event_provider> event_provider;
  // attach link id -> (attach_target_type, local attach id)
  std::map<int, std::pair<attach_target_type, int>> local_attach_records;
  // prog id -> local instantiated program
  std::map<int, std::unique_ptr<local_instantiated_bpftime_program>>
      local_instantiated_programs;

public:
  attach_manager();
  base_event_provider *get_event_provider() const {
    return event_provider.get();
  }
  void register_attach_impl(attach_target_type ty,
                            std::unique_ptr<base_attach_impl> &&impl) {
    attach_impls.emplace(ty, std::move(impl));
  }
};

} // namespace bpftime
#endif
