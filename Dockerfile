FROM louisjenkinscs/atlas-hp

WORKDIR /usr/home/atlas/compiler-plugin
RUN ./build_plugin

WORKDIR /usr/home/atlas/runtime
RUN mkdir build-all
WORKDIR /usr/home/atlas/runtime/build-all
RUN cmake ..
RUN make

WORKDIR /usr/home
RUN mkdir pm-wrapper
COPY . .
RUN make

ENTRYPOINT ["tail", "-f", "/dev/null"]
