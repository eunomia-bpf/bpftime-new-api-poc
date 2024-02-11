#include "attach_manager.hpp"
#include "nginx_module_attach_impl.hpp"
#include <cassert>
#include <memory>
#include <optional>
using namespace bpftime;

static std::optional<std::unique_ptr<bpftime::attach_manager>> manager;

extern "C" int bpftime__initialize_nginx_handler() {
  manager = std::make_unique<bpftime::attach_manager>();
  auto &man = *manager;
  man->register_attach_impl(bpftime::attach_target_type::NGINX_URL_HANDLER,
                            std::make_unique<nginx_module_attach_impl>());
  auto &provider = *man->get_event_provider();
  uint64_t insn = 0;
  int prog_id = provider.create_bpftime_program(&insn, 1, "my_prog", 0);
  assert(prog_id >= 0);

  int target_id = provider.create_nginx_url_handler_attach_target();
  assert(target_id >= 0);
  int link_id = provider.attach_bpftime_program_at(prog_id, target_id);
  assert(link_id >= 0);
  provider.enable_attach_link(link_id);
  return 0;
}
extern "C" int bpftime__instantiate_attaches() {
  return (*manager)->get_event_provider()->instantiate_attach();
}
