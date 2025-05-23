# docker buildx build . -t ghcr.io/thepigeongenerator/mcaselector-lite:latest
# docker push ghcr.io/thepigeongenerator/mcaselector-lite:latest
FROM debian:testing-slim
LABEL org.opencontainers.image.source="https://github.com/thepigeongenerator/mcaselector-lite"
LABEL org.opencontainers.image.source-path="/.github/Dockerfile"

# install the dependencies
RUN apt update && apt install -y --no-install-recommends \
	ca-certificates                                          \
	git curl                                                 \
	zip unzip                                                \
	make cmake pkg-config                                    \
	clang-tidy                                               \
	gcc gcc-multilib g++ mingw-w64                           \
	libc6-dev                                                \
	libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev \
	libglfw3                                                 \
	nodejs                                                   \
	&& rm -rf /var/lib/apt/lists/*;

CMD ["bash"]
