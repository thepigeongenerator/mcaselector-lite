# docker buildx build . -t mcaselector-lite
# docker tag mcaselector-lite:latest ghcr.io/thepigeongenerator/mcaselector-lite:latest
# docker push ghcr.io/thepigeongenerator/mcaselector-lite:latest
FROM debian:testing-slim

# set environment variables
ENV VCPKG_DISABLE_METRICS="1"
ENV VCPKG_ROOT="/opt/vcpkg"
ENV PATH="${VCPKG_ROOT}:${PATH}"

# install the dependencies
RUN apt update && apt install -y --no-install-recommends	            \
	ca-certificates                                                     \
	git                                                                 \
	curl                                                                \
	zip                                                                 \
	unzip                                                               \
	pkg-config                                                          \
	make                                                                \
	clang                                                               \
	clang-tidy                                                          \
	gcc                                                                 \
	gcc-multilib                                                        \
	lld                                                                 \
	g++                                                                 \
	mingw-w64                                                           \
	libc6-dev                                                           \
	libglfw3-dev                                                        \
	&& rm -rf /var/lib/apt/lists/*                                      \
	&& git clone https://github.com/microsoft/vcpkg.git "${VCPKG_ROOT}" \
	&& "${VCPKG_ROOT}/bootstrap-vcpkg.sh"                               \
	&& vcpkg install glfw3:x64-mingw-dynamic

CMD ["bash"]
