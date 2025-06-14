# docker buildx build . -t ghcr.io/thepigeongenerator/mcaselector-lite:latest
# docker push ghcr.io/thepigeongenerator/mcaselector-lite:latest
FROM debian:testing-slim
LABEL org.opencontainers.image.source="https://github.com/thepigeongenerator/mcaselector-lite"
LABEL org.opencontainers.image.source-path="/.github/Dockerfile"

# install the dependencies
RUN \
	echo 'APT::Install-Recommends "false";' > /etc/apt/apt.conf.d/no-recommends; \
	echo 'APT::Install-Suggests "false";' >> /etc/apt/apt.conf.d/no-recommends;  \
	dpkg --add-architecture i386;                                                \
	apt-get update -qq;                                                          \
	apt-get install -y                                                           \
		ca-certificates                                                         \
		binutils                                                                \
		git curl                                                                \
		zip unzip                                                               \
		wine wine64                                                             \
		make cmake pkg-config                                                   \
		gcc gcc-multilib g++ mingw-w64                                          \
		libc6-dev                                                               \
		libxinerama-dev libxcursor-dev libxi-dev libxrandr-dev libglu1-mesa-dev \
		libglfw3                                                                \
		nodejs;                                                                 \
	apt-get clean;                                                                     \
	rm -rf /var/lib/apt/lists/* /usr/share/doc/* /usr/share/man/* /usr/share/locale/*; \
	rm -rf /usr/lib/node_modules/npm/docs /usr/lib/node_modules/npm/html;              \
	rm -rf /usr/share/wine/fonts /usr/share/wine/mono;                                 \
	find /usr -name "*.py" -delete;                                                    \
	find /usr/lib/wine -name "*.a" -delete;                                            \
	find /usr/lib/wine -name "*.la" -delete;                                           \
	find /usr/lib/wine -name "*.def" -delete;                                          \
	strip /usr/bin/wine* || true;                                                      \
    rm -rf /var/cache/* /var/log/*;

CMD ["bash"]
