#ifndef _BPFTIME_ATTACH_MANAGER_H
#define _BPFTIME_ATTACH_MANAGER_H
#include "base_event_provider.hpp"
#include "nginx_module_attach_impl.hpp"
#include <memory>
#include <unordered_map>
#include <variant>
namespace bpftime {

struct bpftime_program_object {};
struct attach_link_object {};
struct unused_object;
using objects =
    std::variant<unused_object, bpftime_program_object, attach_link_object>;

struct event_provider_impl : public base_event_provider {
public:
private:
  class attach_manager *man;
};
struct attach_manager {
  // In bpftime, this should be stored in shm, and should be a vector, not a map
  std::unordered_map<int, objects> objects;
  friend struct event_provider_impl;
  int next_id = 1;
  int allocate_id();
  std::unique_ptr<nginx_module_attach_impl> nginx_attach_impl;
  std::unique_ptr<event_provider_impl> event_provider;
};

} // namespace bpftime
#endif
