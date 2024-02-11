#include "nginx_module_attach_impl.hpp"
#include "attach_manager.hpp"
#include <cassert>
#include <optional>
using namespace bpftime;

static std::optional<url_handler> handler;

extern "C" int bpftime_nginx_module_attach__call_url_handler(const char *url) {
  assert(handler.has_value());
  return (*handler)(url);
}

int nginx_module_attach_impl::attach_url_handler(url_handler &&cb) {
  if (current_id != -1) {
    return -1;
  }
  handler = cb;
  current_id = allocate_id();
  return current_id;
}

int nginx_module_attach_impl::detach_by_id(int id) {
  if (id != current_id)
    return -1;
  handler = {};
  current_id = -1;
  return 0;
}
int nginx_module_attach_impl::handle_attach_with_link(
    struct local_instantiated_bpftime_program *prog) {
  return this->attach_url_handler([=](const char *url) -> bool {
    uint64_t ret;
    prog->run(url, 0, &ret);
    return ret;
  });
}
