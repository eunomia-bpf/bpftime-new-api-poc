#ifndef _BPFTIME_ATTACH_MANAGER_H
#define _BPFTIME_ATTACH_MANAGER_H
#include "base_attach_impl.hpp"
#include "event_provider_impl.hpp"
#include "nginx_module_attach_impl.hpp"
#include <map>
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
struct attach_link_object {
  int prog_id;
  int target_id;
  bool enabled = false;
};
struct unused_object {};
enum class attach_target_type {
  NGINX_URL_HANDLER,
};
struct attach_target {
  attach_target_type type;
};
using objects = std::variant<unused_object, bpftime_program_object,
                             attach_link_object, attach_target>;

struct bpftime_shm_client {
  std::unordered_map<int, objects> objects;
};
struct local_instantiated_bpftime_program {
  int run(const void *mem, size_t mem_size, uint64_t *ret);
};
struct attach_manager {
  // In bpftime, this should be stored in shm
  bpftime_shm_client shm;
  friend struct event_provider_impl;
  int next_id = 1;
  int allocate_id();
  std::unique_ptr<base_attach_impl> nginx_attach_impl;
  std::unique_ptr<event_provider_impl> event_provider;
  // attach link id -> local attach id
  std::map<int, int> local_attach_records;
  // prog id -> local instantiated program
  std::map<int, std::unique_ptr<local_instantiated_bpftime_program>>
      local_instantiated_programs;
};

} // namespace bpftime
#endif
