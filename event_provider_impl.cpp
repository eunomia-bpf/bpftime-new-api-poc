#include "event_provider_impl.hpp"
#include "attach_manager.hpp"
#include "nginx_module_attach_impl.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <variant>
using namespace bpftime;
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
                                                   int attach_target_id) {
  int id = man->allocate_id();
  man->shm.objects.emplace(id, attach_link_object{.prog_id = prog_id,
                                                  .target_id = attach_target_id,
                                                  .enabled = false});
  return id;
}
int event_provider_impl::enable_attach_link(int attach_link_id) {
  std::get<attach_link_object>(man->shm.objects[attach_link_id]).enabled = true;
  return 0;
}
int event_provider_impl::create_nginx_url_handler_attach_target() {
  int id = man->allocate_id();
  man->shm.objects.emplace(
      id, attach_target{.type = attach_target_type::NGINX_URL_HANDLER});
  return id;
}
int event_provider_impl::destroy_object(int object_id) {
  man->shm.objects.erase(object_id);
  return 0;
}
int event_provider_impl::instantiate_attach() {
  for (const auto &[k, v] : man->shm.objects) {
    if (std::holds_alternative<attach_link_object>(v)) {
    }
  }
  return 0;
}
int event_provider_impl::deinstantiate_attach() {}
int event_provider_impl::instantiate_id(int id, std::set<int> &instack) {
  if (instack.contains(id)) {
    assert(false && "Look dependency detected");
  }
  instack.insert(id);
  auto &obj = man->shm.objects[id];
  if (std::holds_alternative<attach_link_object>(obj)) {
    const auto &attach_link = std::get<attach_link_object>(obj);

    if (attach_link.enabled) {
      std::cout << "Enable: " << id << std::endl;
      if (!man->local_instantiated_programs.contains(attach_link.prog_id)) {
        if (int err = instantiate_id(attach_link.prog_id, instack); err < 0)
          return err;
      }
      auto &prog = std::get<bpftime_program_object>(
          man->shm.objects[attach_link.prog_id]);

      auto &target =
          std::get<attach_target>(man->shm.objects[attach_link.target_id]);
      if (target.type == attach_target_type::NGINX_URL_HANDLER) {
        if (auto p = dynamic_cast<nginx_module_attach_impl *>(
                man->nginx_attach_impl.get());
            p) {
          auto ptr =
              man->local_instantiated_programs[attach_link.prog_id].get();
          p->attach_url_handler([&](const char *url) -> bool {
            uint64_t ret;
            ptr->run(url, 0, &ret);
            return ret;
          });
        }
      }
    }
  }
}
