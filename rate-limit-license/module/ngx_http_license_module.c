#include <nginx.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include <string.h>

ngx_module_t ngx_http_license_module;

typedef struct {
  ngx_uint_t   rate;
} ngx_http_license_loc_conf_t;

static ngx_int_t ngx_http_license_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_license_init(ngx_conf_t *cf);
static void *    ngx_http_license_create_loc_conf(ngx_conf_t *cf);
static char *    ngx_http_license_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static char *    ngx_conf_license_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t ngx_http_license_module_commands[] = {
  {
    ngx_string("license"),
    NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE1,
    ngx_conf_license_init,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL
  }
};

static ngx_http_module_t ngx_http_license_module_ctx = {
  NULL,                                 /* preconfiguration */
  ngx_http_license_init,                /* postconfiguration */
  NULL,                                 /* create main configuration */
  NULL,                                 /* init main configuration */
  NULL,                                 /* create server configuration */
  NULL,                                 /* merge server configuration */
  ngx_http_license_create_loc_conf,     /* create location configuration */
  ngx_http_license_merge_loc_conf       /* merge location configuration */
};

ngx_module_t ngx_http_license_module = {
  NGX_MODULE_V1,
  &ngx_http_license_module_ctx, /* module context */
  ngx_http_license_module_commands,                            /* module directives */
  NGX_HTTP_MODULE,                 /* module type */
  NULL,                            /* init master */
  NULL,                            /* init module */
  NULL,                            /* init process */
  NULL,                            /* init thread */
  NULL,                            /* exit thread */
  NULL,                            /* exit process */
  NULL,                            /* exit master */
  NGX_MODULE_V1_PADDING
};

char* ngx_conf_license_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_http_license_loc_conf_t* p = conf;
  ngx_str_t        *value;

  if (p->rate != NGX_CONF_UNSET_UINT) {
    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "License is duplicated");

    return NGX_CONF_ERROR;
  }

  value = cf->args->elts;
  p->rate = ngx_atoi(value[1].data, value[1].len);

  if ((ngx_int_t)p->rate == NGX_ERROR) {
    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value in license configuration: \"%s\"", value[1].data);

    return NGX_CONF_ERROR;
  }

  return NGX_CONF_OK;
}


static ngx_int_t ngx_http_license_handler(ngx_http_request_t *r)
{
  if (r->main->internal) {
    return NGX_DECLINED;
  }

  r->main->internal = 1;

  ngx_http_license_loc_conf_t  *conf;
  conf = ngx_http_get_module_loc_conf(r, ngx_http_license_module);

  ngx_table_elt_t *h;
  h = ngx_list_push(&r->headers_out.headers);
  h->hash = 1;

  char* str = ngx_palloc(r->pool, 20 * sizeof(char));
  sprintf(str, "Hej %lu", conf->rate);

  ngx_str_set(&h->key, "X-NGINX-Tutorial");
  ngx_str_set(&h->value, str);

  return NGX_DECLINED;
}

static ngx_int_t ngx_http_license_init(ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;

  cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

  h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
  if (h == NULL) {
    return NGX_ERROR;
  }

  *h = ngx_http_license_handler;

  return NGX_OK;
}

// Configuration
static void * ngx_http_license_create_loc_conf(ngx_conf_t *cf)
{
  ngx_http_license_loc_conf_t  *conf;
  conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_license_loc_conf_t));

  if (conf == NULL) {
    return NGX_CONF_ERROR;
  }

  conf->rate = NGX_CONF_UNSET_UINT;

  return conf;
}

static char * ngx_http_license_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
  ngx_http_license_loc_conf_t  *prev = (ngx_http_license_loc_conf_t *)parent;
  ngx_http_license_loc_conf_t  *conf = (ngx_http_license_loc_conf_t *)child;

  ngx_conf_merge_uint_value(conf->rate, prev->rate, 0);

  return NGX_CONF_OK;
}
// End configuration
