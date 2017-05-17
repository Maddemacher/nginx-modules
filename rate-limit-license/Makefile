LOCAL = $(CURDIR)
NAME = nginx
VERSION ?= 1.11.9
NGINX_URL = http://nginx.org/download/nginx-${VERSION}.tar.gz
NGINX_PATH = deps/nginx-${VERSION}
NGX_DEVEL_VERSION = 0.3.0
NGX_DEVEL_PATH = deps/ngx_devel_kit-${NGX_DEVEL_VERSION}
NGX_DEVEL_URL = https://github.com/simpl/ngx_devel_kit/archive/v${NGX_DEVEL_VERSION}.tar.gz

all: build

${NGINX_PATH}:
	mkdir -pv deps
	curl -L ${NGINX_URL} -o ${NGINX_PATH}.tar.gz
	tar -C deps -xzf ${NGINX_PATH}.tar.gz

${NGX_DEVEL_PATH}:
	mkdir -pv deps
	curl -L ${NGX_DEVEL_URL} -o ${NGX_DEVEL_PATH}.tar.gz
	tar -C deps -xzf ${NGX_DEVEL_PATH}.tar.gz

deps: ${NGINX_PATH} ${NGX_DEVEL_PATH} ${LUA_NGINX_PATH}

configure: $(NGINX_PATH)/Makefile

$(NGINX_PATH)/Makefile: deps
	cd ${NGINX_PATH} && ./configure ${RPM_BUILD_PARAMS} \
	--add-module=../../${NGX_DEVEL_PATH} \
  --add-module=../../module

build: $(NGINX_PATH)/Makefile
	cd ${NGINX_PATH} && $(MAKE)


clean:
	$(MAKE) -C ${LOCAL}/${NGINX_PATH} clean || true
