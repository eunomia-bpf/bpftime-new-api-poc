#include "event_provider_impl.hpp"
#include "attach_manager.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>
#include <set>
#include <variant>
using namespace bpftime;
int event_provider_impl::create_bpftime_program(const void *insn,
                                                std::size_t insn_cnt,
                                                const char *prog_name,
                                                int prog_type) {
  int id = man->allocate_id();
  man->shm.handlers.emplace(
      id,
      bpftime_program_handler{.insns = std::vector<uint64_t>(
                                 (uint64_t *)insn, (uint64_t *)insn + insn_cnt),
                             .name = prog_name,
                             .prog_type = prog_type});
  return id;
}
int event_provider_impl::attach_bpftime_program_at(int prog_id,
                                                   int attach_target_id) {
  int id = man->allocate_id();
  man->shm.handlers.emplace(id, attach_link_handler{.prog_id = prog_id,
                                                  .target_id = attach_target_id,
                                                  .enabled = false});
  return id;
}
int event_provider_impl::enable_attach_link(int attach_link_id) {
  std::get<attach_link_handler>(man->shm.handlers[attach_link_id]).enabled = true;
  return 0;
}
int event_provider_impl::create_nginx_url_handler_attach_target() {
  int id = man->allocate_id();
  man->shm.handlers.emplace(
      id, attach_target_handler{.type = attach_target_type::NGINX_URL_HANDLER});
  return id;
}
int event_provider_impl::destroy_object(int object_id) {
  man->shm.handlers.erase(object_id);
  return 0;
}
int event_provider_impl::instantiate_attach() {
  for (const auto &[k, v] : man->shm.handlers) {
    if (!std::holds_alternative<unused_handler>(v)) {
      std::set<int> instack;
      if (int err = instantiate_id(k, instack); err < 0)
        return err;
    }
  }
  return 0;
}
int event_provider_impl::deinstantiate_attach() {
  // Destroy attaches
  for (const auto &[k, v] : man->local_attach_records) {
    // man->nginx_attach_impl->detach_by_id(v);
    man->attach_impls[v.first]->detach_by_id(v.second);
  }
  man->local_attach_records.clear();
  man->local_instantiated_programs.clear();

  return 0;
}
int event_provider_impl::instantiate_id(int id, std::set<int> &instack) {
  if (instack.contains(id)) {
    assert(false && "Look dependency detected");
  }
  instack.insert(id);
  auto &obj = man->shm.handlers[id];
  if (std::holds_alternative<attach_link_handler>(obj)) {
    const auto &attach_link = std::get<attach_link_handler>(obj);

    if (attach_link.enabled && !man->local_attach_records.contains(id)) {
      std::cout << "Enable: " << id << std::endl;
      if (!man->local_instantiated_programs.contains(attach_link.prog_id)) {
        if (int err = instantiate_id(attach_link.prog_id, instack); err < 0)
          return err;
      }
      auto &prog = std::get<bpftime_program_handler>(
          man->shm.handlers[attach_link.prog_id]);

      auto &target =
          std::get<attach_target_handler>(man->shm.handlers[attach_link.target_id]);
      auto ptr = man->local_instantiated_programs[attach_link.prog_id].get();

      man->local_attach_records[id] = std::make_pair(
          target.type,
          man->attach_impls[target.type]->handle_attach_with_link(ptr));
    }
  } else if (std::holds_alternative<bpftime_program_handler>(obj)) {
    if (!man->local_instantiated_programs.contains(id)) {
      man->local_instantiated_programs[id] =
          std::make_unique<local_instantiated_bpftime_program>(
              local_instantiated_bpftime_program{
                  .name = std::get<bpftime_program_handler>(obj).name});
    }
  }
  instack.erase(id);
  return 0;
}
