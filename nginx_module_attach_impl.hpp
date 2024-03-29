#ifndef _BPFTIME_NGINX_MODULE_ATTACH_IMPL
#define _BPFTIME_NGINX_MODULE_ATTACH_IMPL
#include "base_attach_impl.hpp"
#include <functional>
namespace bpftime {
using url_handler = std::function<bool(const char *)>;
class nginx_module_attach_impl : public base_attach_impl {
public:
  int attach_url_handler(url_handler &&cb);
  int detach_by_id(int id);
  int handle_attach_with_link(struct local_instantiated_bpftime_program *prog);

private:
  int current_id = -1;
};
} // namespace bpftime

extern "C" int bpftime_nginx_module_attach__call_url_handler(const char *url);

#endif
