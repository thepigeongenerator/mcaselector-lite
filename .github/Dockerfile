# docker buildx build . -t ghcr.io/thepigeongenerator/mcaselector-lite:latest
# docker push ghcr.io/thepigeongenerator/mcaselector-lite:latest
FROM debian:testing-slim
LABEL org.opencontainers.image.source="https://github.com/thepigeongenerator/mcaselector-lite"
LABEL org.opencontainers.image.source-path="/.github/Dockerfile"

# set environment variables
ENV VCPKG_DISABLE_METRICS="1"
ENV VCPKG_ROOT="/opt/vcpkg"
ENV PATH="${VCPKG_ROOT}:${PATH}"

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

# install packages / VCPKG
RUN git clone https://github.com/microsoft/vcpkg.git "${VCPKG_ROOT}" \
	&& "${VCPKG_ROOT}/bootstrap-vcpkg.sh" \
	&& vcpkg install glfw3:x64-linux-dynamic glfw3:x64-mingw-static;

CMD ["bash"]
