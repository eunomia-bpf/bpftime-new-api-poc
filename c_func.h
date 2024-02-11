#ifndef _C_FUNC_H
#define _C_FUNC_H

extern "C" int bpftime_nginx_module_attach__call_url_handler(const char *url);
extern "C" int bpftime__instantiate_attaches();
extern "C" int bpftime__initialize_nginx_handler();

#endif
